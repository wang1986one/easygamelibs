/****************************************************************************/
/*                                                                          */
/*      文件名:    D3DViewPort.cpp                                          */
/*      创建日期:  2011年10月07日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
// D3DViewPort.cpp : 实现文件
//

#include "stdafx.h"
#include "D3DUIEditDoc.h"
#include "D3DViewPort.h"
#include ".\d3dviewport.h"
#include "MainFrm.h"
#include "D3DView.h"
#include <sstream>
#include "D3DUIEdit.h"

#define UPDATE_TIME				30
#define FRAME_CACULATE_TIME		1000

#define UPDATE_TIMER_ID			10001
#define FRAME_CACULATE_TIME_ID	10002

// CD3DViewPort

enum SELECT_STATUS
{
	SST_NONE,
	SST_READY,
	SST_SELECTING,
};


LPCSTR DEFAULT_FX_DOT_LINE=
"texture PrimaryTex < string name = \"test.jpg\"; >;"
"texture SecondaryTex < string name = \"test1.jpg\"; >;"
"technique tec0"
"{"
"    pass p0"
"    {"
"		MultiSampleAntialias = FALSE;"
"		Lighting=false;"
"		zenable = false;"
"		zwriteenable = false;"
"		CullMode = ccw;"
"		fogenable = false;"
"		Texture[0] = <PrimaryTex>;"
"		AlphaTestEnable = false;"
"		AlphaBlendEnable = true;"
"		SrcBlend = InvDestColor;"
"		DestBlend = InvDestColor;"
"		Texture[0] = <PrimaryTex>;"
"     	ColorOp[0] = Modulate;"
"       ColorArg1[0] = Texture;"
"       ColorArg2[0] = Diffuse;"      	
"       AlphaOp[0] = selectarg1;"
"       AlphaArg1[0] = Texture;"
"       AlphaArg2[0] = diffuse;"
"		ColorOp[1] = disable;"
"		AlphaOp[1] = disable;"
"		AddressU[0] = wrap;"
"		AddressV[0] = wrap;"      
"		MinFilter[0] = none;"
"       MagFilter[0] = none;"
"       MipFilter[0] = none;"
"    }"
"}";

IMPLEMENT_DYNAMIC(CD3DViewPort, CWnd)
CD3DViewPort::CD3DViewPort()
{
	m_pD3DDevice=NULL;
	m_pRender=NULL;
	m_pObjectCreator=NULL;
	m_pGUI=NULL;

	m_SelectStatus=SST_NONE;
	m_SelectStartX=0;
	m_SelectStartY=0;

	ZeroMemory(m_SelectRect,sizeof(m_SelectRect));
	m_pDotLineTexture=NULL;
	m_pSizeBarTexture=NULL;
	
}

CD3DViewPort::~CD3DViewPort()
{
	((CD3DUIEditApp *)AfxGetApp())->SetD3DViewPort(NULL);	
	SAFE_RELEASE(m_pDotLineTexture);
	SAFE_RELEASE(m_pSizeBarTexture);
	for(int i=0;i<4;i++)
		SAFE_RELEASE(m_SelectRect[i]);

	SAFE_RELEASE(m_pGUI);
	SAFE_RELEASE(m_pObjectCreator);
	SAFE_RELEASE(m_pRender);
	SAFE_RELEASE(m_pD3DDevice);	
}


BEGIN_MESSAGE_MAP(CD3DViewPort, CWnd)
	ON_WM_TIMER()
	ON_WM_SIZE()
	ON_MESSAGE(WM_D3DGUI_WND_LOADED,OnWndLoaded)	
END_MESSAGE_MAP()



// CD3DViewPort 消息处理程序

BOOL CD3DViewPort::Create(CWnd * pParent)
{
	if(!CWnd::Create(NULL,_T("ViewPort"),WS_CHILD|WS_VISIBLE,CRect(0,0,800,600),pParent,100101))
		return FALSE;

	CD3DTexture::EnableSaveOrgInfo(true);

	m_pD3DDevice=new CD3DDevice();

	D3DDEVICE_PARAMS D3DParams;

	D3DParams.hFocusWindow=GetSafeHwnd();
	D3DParams.PresentParams.PresentationInterval=D3DPRESENT_INTERVAL_ONE;

	m_pD3DDevice->Create(D3DParams);

	m_pRender=new CD3DUIRender();

	m_pRender->Create(m_pD3DDevice);

	m_pObjectCreator=new CD3DGUIObjectCreator(m_pRender);

	m_pGUI=new CD3DGUI(m_pObjectCreator,GetSafeHwnd());

	CResourceFileAccessor ResourceFile;
	CEasyString ResourceName;
	ResourceName.Format(_T("Module=%I64u;Type=Texture;ID=%u"),(INT64)NULL,IDT_DOT_LINE);
	if(!ResourceFile.Open(ResourceName,0))
	{
		AfxMessageBox(_T("加载系统纹理资源失败"));
		return FALSE;
	}

	m_pDotLineTexture=new CD3DGUITexture(m_pD3DDevice->GetTextureManager());

	if(!m_pDotLineTexture->LoadTexture(&ResourceFile))
	{
		AfxMessageBox(_T("加载系统纹理失败"));
		return FALSE;
	}


	ResourceName.Format(_T("Module=%I64u;Type=TEXTURE;ID=%u"),(INT64)NULL,IDT_SIZE_BAR);
	if(!ResourceFile.Open(ResourceName,0))
	{
		AfxMessageBox(_T("加载系统纹理资源失败"));
		return FALSE;
	}

	m_pSizeBarTexture=new CD3DGUITexture(m_pD3DDevice->GetTextureManager());

	if(!m_pSizeBarTexture->LoadTexture(&ResourceFile))
	{
		AfxMessageBox(_T("加载系统纹理失败"));
		return FALSE;
	}

	m_pSizeBarTexture->SetName(_T("[UI]\\SYS_SIZE_BAR"));
	m_pD3DDevice->GetTextureManager()->AddTexture(m_pSizeBarTexture,_T("[UI]\\SYS_SIZE_BAR"));

	//m_pDotLineTexture=m_pObjectCreator->CreateTextureFromFile("DotLine.png");
	for(int i=0;i<4;i++)
	{
		m_SelectRect[i]=m_pObjectCreator->CreateWndRect(NULL);
		m_SelectRect[i]->SetTexture(m_pDotLineTexture);
		m_SelectRect[i]->SetFXFromMemory(_T("DotLine"),(LPVOID)DEFAULT_FX_DOT_LINE,(int)strlen(DEFAULT_FX_DOT_LINE));				
		m_SelectRect[i]->SetVisible(false);
	}

	m_FrameCount=0;
	m_CreateCount=0;

	
	
	//SetTimer(UPDATE_TIMER_ID,UPDATE_TIME,NULL);
	SetTimer(FRAME_CACULATE_TIME_ID,FRAME_CACULATE_TIME,NULL);

	((CD3DUIEditApp *)AfxGetApp())->SetD3DViewPort(this);

	return TRUE;
}
void CD3DViewPort::OnTimer(UINT nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(nIDEvent==UPDATE_TIMER_ID)
	{
		//Update();
	}
	else if(nIDEvent==FRAME_CACULATE_TIME_ID)
	{
		CString FrameRate;
		FrameRate.Format(_T("%d,%d"),m_FrameCount,FRAME_CACULATE_TIME/1000);
		((CMainFrame *)AfxGetMainWnd())->GetStatusBar()->SetPaneText(0,FrameRate);
		m_FrameCount=0;
	}
	CWnd::OnTimer(nIDEvent);
}

BOOL CD3DViewPort::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: 在此添加专用代码和/或调用基类
	
	return CWnd::OnWndMsg(message, wParam, lParam, pResult);
}

void CD3DViewPort::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码

	if(m_pD3DDevice)
	{
		RECT rect;

		GetClientRect(&rect);
		m_pD3DDevice->SetViewPort(rect);
	}
}

BOOL CD3DViewPort::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类

	

	return CWnd::PreTranslateMessage(pMsg);
}

CD3DUIEditDoc * CD3DViewPort::GetDocument()
{
	return ((CD3DView *)GetParent())->GetDocument();
}

void CD3DViewPort::OnSelected(CRect& SelectRect)
{
	for(int i=0;i<4;i++)
	{
		m_SelectRect[i]->SetVisible(false);
	}

	CD3DWnd * pGUIObject=NULL;
	CString Temp;
	
	
	if(GetDocument()->GetCurNewObject()==GET_CLASS_INFO(CD3DWnd))
	{
		CD3DWnd * pWnd;
		WIN_BORDERS borders;

		pWnd=m_pGUI->CreateWnd(SelectRect);
		pGUIObject=pWnd;

		borders[RECT_TOP]=38;
		borders[RECT_BOTTOM]=19;
		borders[RECT_LEFT]=10;
		borders[RECT_RIGHT]=25;
		borders[RECT_TOP_LEFT]=65;
		borders[RECT_TOP_RIGHT]=82;
		borders[RECT_BOTTOM_LEFT]=65;
		borders[RECT_BOTTOM_RIGHT]=82;
	
		Temp.Format(_T("Window%d"),m_CreateCount);
		pWnd->SetName(Temp);
		pWnd->SetBorders(borders);
		pWnd->SetTexture(DEFAULT_UI_TEXTURE,CRect(103,193,256,256));				
		pWnd->EnableResize(true);			
		pWnd->SetText(_T("窗口"));			
	}
	else if(GetDocument()->GetCurNewObject()==GET_CLASS_INFO(CD3DButton))
	{
		WIN_BORDERS borders;
		CD3DButton * pBtn=m_pGUI->CreateButton(SelectRect);
		pGUIObject=pBtn;
		
		borders[RECT_TOP]=4;
		borders[RECT_BOTTOM]=4;
		borders[RECT_LEFT]=4;
		borders[RECT_RIGHT]=4;
		borders[RECT_TOP_LEFT]=4;
		borders[RECT_TOP_RIGHT]=4;
		borders[RECT_BOTTOM_LEFT]=4;
		borders[RECT_BOTTOM_RIGHT]=4;

		Temp.Format(_T("Button%d"),m_CreateCount);
		pBtn->SetName(Temp);
		pBtn->SetBorders(borders);
		pBtn->SetTexture(BUTTON_STATUS_NORMAL,	DEFAULT_UI_TEXTURE,CRect(133,62,201,88),FLOAT_POINT(0,0));
		pBtn->SetTexture(BUTTON_STATUS_ACTIVE,	DEFAULT_UI_TEXTURE,CRect(133,62,201,88),FLOAT_POINT(0,0));
		pBtn->SetTexture(BUTTON_STATUS_PRESS,	DEFAULT_UI_TEXTURE,CRect(133,32,201,58),FLOAT_POINT(2,2));
		pBtn->SetTexture(BUTTON_STATUS_DISBALE,	DEFAULT_UI_TEXTURE,CRect(133,2,201,29),FLOAT_POINT(0,0));
		pBtn->SetText(_T("按钮"));	

	}
	else if(GetDocument()->GetCurNewObject()==GET_CLASS_INFO(CD3DEdit))
	{
		WIN_BORDERS borders;

		borders[RECT_TOP]=2;
		borders[RECT_BOTTOM]=2;
		borders[RECT_LEFT]=2;
		borders[RECT_RIGHT]=2;
		borders[RECT_TOP_LEFT]=2;
		borders[RECT_TOP_RIGHT]=2;
		borders[RECT_BOTTOM_LEFT]=2;
		borders[RECT_BOTTOM_RIGHT]=2;

		CD3DEdit * pEdit=m_pGUI->CreateEdit(SelectRect);
		pGUIObject=pEdit;

		Temp.Format(_T("Edit%d"),m_CreateCount);
		pEdit->SetName(Temp);
		pEdit->SetBorders(borders);
		pEdit->SetTexture(DEFAULT_UI_TEXTURE,CRect(174,123,203,152));				
		pEdit->SetText(_T("编辑框"));


		borders[RECT_TOP]=1;
		borders[RECT_BOTTOM]=1;
		borders[RECT_LEFT]=1;
		borders[RECT_RIGHT]=1;
		borders[RECT_TOP_LEFT]=1;
		borders[RECT_TOP_RIGHT]=1;
		borders[RECT_BOTTOM_LEFT]=1;
		borders[RECT_BOTTOM_RIGHT]=1;

		CD3DScrollBar * pScrollBar=pEdit->GetScrollBar();			
		pScrollBar->SetBorders(borders);
		pScrollBar->SetTexture(DEFAULT_UI_TEXTURE,CRect(174,92,203,121));	

		borders[RECT_TOP]=2;
		borders[RECT_BOTTOM]=2;
		borders[RECT_LEFT]=2;
		borders[RECT_RIGHT]=2;
		borders[RECT_TOP_LEFT]=2;
		borders[RECT_TOP_RIGHT]=2;
		borders[RECT_BOTTOM_LEFT]=2;
		borders[RECT_BOTTOM_RIGHT]=2;

		pScrollBar->GetDecButton()->SetBorders(borders);
		pScrollBar->GetDecButton()->SetTexture(BUTTON_STATUS_NORMAL,DEFAULT_UI_TEXTURE,CRect(101,42,114,55),FLOAT_POINT(0,0));
		pScrollBar->GetDecButton()->SetTexture(BUTTON_STATUS_ACTIVE,DEFAULT_UI_TEXTURE,CRect(101,42,114,55),FLOAT_POINT(0,0));
		pScrollBar->GetDecButton()->SetTexture(BUTTON_STATUS_PRESS,DEFAULT_UI_TEXTURE,CRect(101,55,114,68),FLOAT_POINT(0,0));
		pScrollBar->GetDecButton()->SetTexture(BUTTON_STATUS_DISBALE,DEFAULT_UI_TEXTURE,CRect(101,42,114,55),FLOAT_POINT(0,0));


		pScrollBar->GetIncButton()->SetBorders(borders);
		pScrollBar->GetIncButton()->SetTexture(BUTTON_STATUS_NORMAL,DEFAULT_UI_TEXTURE,CRect(115,42,128,55),FLOAT_POINT(0,0));
		pScrollBar->GetIncButton()->SetTexture(BUTTON_STATUS_ACTIVE,DEFAULT_UI_TEXTURE,CRect(115,42,128,55),FLOAT_POINT(0,0));
		pScrollBar->GetIncButton()->SetTexture(BUTTON_STATUS_PRESS,DEFAULT_UI_TEXTURE,CRect(115,55,128,68),FLOAT_POINT(0,0));
		pScrollBar->GetIncButton()->SetTexture(BUTTON_STATUS_DISBALE,DEFAULT_UI_TEXTURE,CRect(115,42,128,55),FLOAT_POINT(0,0));

		pScrollBar->GetTrackButton()->SetBorders(borders);
		pScrollBar->GetTrackButton()->SetTexture(BUTTON_STATUS_NORMAL,DEFAULT_UI_TEXTURE,CRect(103,84,116,97),FLOAT_POINT(0,0));
		pScrollBar->GetTrackButton()->SetTexture(BUTTON_STATUS_ACTIVE,DEFAULT_UI_TEXTURE,CRect(103,84,116,97),FLOAT_POINT(0,0));
		pScrollBar->GetTrackButton()->SetTexture(BUTTON_STATUS_PRESS,DEFAULT_UI_TEXTURE,CRect(103,84,116,97),FLOAT_POINT(0,0));
		pScrollBar->GetTrackButton()->SetTexture(BUTTON_STATUS_DISBALE,DEFAULT_UI_TEXTURE,CRect(103,84,116,97),FLOAT_POINT(0,0));

		CD3DWnd * pIMEReadingWnd=m_pGUI->CreateWnd(CEasyRect(0,0,100,100));

		borders[RECT_TOP]=0;
		borders[RECT_BOTTOM]=0;
		borders[RECT_LEFT]=0;
		borders[RECT_RIGHT]=0;
		borders[RECT_TOP_LEFT]=0;
		borders[RECT_TOP_RIGHT]=0;
		borders[RECT_BOTTOM_LEFT]=0;
		borders[RECT_BOTTOM_RIGHT]=0;
		
		pIMEReadingWnd->SetBorders(borders);
		pIMEReadingWnd->SetTexture(DEFAULT_UI_TEXTURE,CRect(130,230,148,238));	

		pEdit->SetIMEReadingWnd(pIMEReadingWnd);

		CD3DWnd * pIMECandidateWnd=m_pGUI->CreateWnd(CEasyRect(0,0,100,100));

		borders[RECT_TOP]=2;
		borders[RECT_BOTTOM]=2;
		borders[RECT_LEFT]=2;
		borders[RECT_RIGHT]=2;
		borders[RECT_TOP_LEFT]=2;
		borders[RECT_TOP_RIGHT]=2;
		borders[RECT_BOTTOM_LEFT]=2;
		borders[RECT_BOTTOM_RIGHT]=2;

		pIMECandidateWnd->SetBorders(borders);
		pIMECandidateWnd->SetTexture(DEFAULT_UI_TEXTURE,CRect(174,123,203,152));	

		pEdit->SetIMECandidateWnd(pIMECandidateWnd);
		

	}
	else if(GetDocument()->GetCurNewObject()==GET_CLASS_INFO(CD3DSimpleList))
	{
		WIN_BORDERS borders;
		CD3DSimpleList * pList=m_pGUI->CreateSimpleList(SelectRect);
		pGUIObject=pList;

		borders[RECT_TOP]=2;
		borders[RECT_BOTTOM]=2;
		borders[RECT_LEFT]=2;
		borders[RECT_RIGHT]=2;
		borders[RECT_TOP_LEFT]=2;
		borders[RECT_TOP_RIGHT]=2;
		borders[RECT_BOTTOM_LEFT]=2;
		borders[RECT_BOTTOM_RIGHT]=2;

		Temp.Format(_T("SimpleList%d"),m_CreateCount);
		pList->SetName(Temp);
		pList->SetBorders(borders);
		pList->SetTexture(DEFAULT_UI_TEXTURE,CRect(174,123,203,152));						
		pList->AddItem(_T("选项1"));
		pList->AddItem(_T("选项2"));
		pList->AddItem(_T("选项3"));
		pList->AddItem(_T("选项4"));
		pList->AddItem(_T("选项5"));			

		borders[RECT_TOP]=1;
		borders[RECT_BOTTOM]=1;
		borders[RECT_LEFT]=1;
		borders[RECT_RIGHT]=1;
		borders[RECT_TOP_LEFT]=1;
		borders[RECT_TOP_RIGHT]=1;
		borders[RECT_BOTTOM_LEFT]=1;
		borders[RECT_BOTTOM_RIGHT]=1;

		CD3DScrollBar * pScrollBar=pList->GetScrollBar();			
		pScrollBar->SetBorders(borders);
		pScrollBar->SetTexture(DEFAULT_UI_TEXTURE,CRect(174,92,203,121));	

		borders[RECT_TOP]=2;
		borders[RECT_BOTTOM]=2;
		borders[RECT_LEFT]=2;
		borders[RECT_RIGHT]=2;
		borders[RECT_TOP_LEFT]=2;
		borders[RECT_TOP_RIGHT]=2;
		borders[RECT_BOTTOM_LEFT]=2;
		borders[RECT_BOTTOM_RIGHT]=2;

		pScrollBar->GetDecButton()->SetBorders(borders);
		pScrollBar->GetDecButton()->SetTexture(BUTTON_STATUS_NORMAL,DEFAULT_UI_TEXTURE,CRect(101,42,114,55),FLOAT_POINT(0,0));
		pScrollBar->GetDecButton()->SetTexture(BUTTON_STATUS_ACTIVE,DEFAULT_UI_TEXTURE,CRect(101,42,114,55),FLOAT_POINT(0,0));
		pScrollBar->GetDecButton()->SetTexture(BUTTON_STATUS_PRESS,DEFAULT_UI_TEXTURE,CRect(101,55,114,68),FLOAT_POINT(0,0));
		pScrollBar->GetDecButton()->SetTexture(BUTTON_STATUS_DISBALE,DEFAULT_UI_TEXTURE,CRect(101,42,114,55),FLOAT_POINT(0,0));


		pScrollBar->GetIncButton()->SetBorders(borders);
		pScrollBar->GetIncButton()->SetTexture(BUTTON_STATUS_NORMAL,DEFAULT_UI_TEXTURE,CRect(115,42,128,55),FLOAT_POINT(0,0));
		pScrollBar->GetIncButton()->SetTexture(BUTTON_STATUS_ACTIVE,DEFAULT_UI_TEXTURE,CRect(115,42,128,55),FLOAT_POINT(0,0));
		pScrollBar->GetIncButton()->SetTexture(BUTTON_STATUS_PRESS,DEFAULT_UI_TEXTURE,CRect(115,55,128,68),FLOAT_POINT(0,0));
		pScrollBar->GetIncButton()->SetTexture(BUTTON_STATUS_DISBALE,DEFAULT_UI_TEXTURE,CRect(115,42,128,55),FLOAT_POINT(0,0));

		pScrollBar->GetTrackButton()->SetBorders(borders);
		pScrollBar->GetTrackButton()->SetTexture(BUTTON_STATUS_NORMAL,DEFAULT_UI_TEXTURE,CRect(103,84,116,97),FLOAT_POINT(0,0));
		pScrollBar->GetTrackButton()->SetTexture(BUTTON_STATUS_ACTIVE,DEFAULT_UI_TEXTURE,CRect(103,84,116,97),FLOAT_POINT(0,0));
		pScrollBar->GetTrackButton()->SetTexture(BUTTON_STATUS_PRESS,DEFAULT_UI_TEXTURE,CRect(103,84,116,97),FLOAT_POINT(0,0));
		pScrollBar->GetTrackButton()->SetTexture(BUTTON_STATUS_DISBALE,DEFAULT_UI_TEXTURE,CRect(103,84,116,97),FLOAT_POINT(0,0));


	}
	else if(GetDocument()->GetCurNewObject()==GET_CLASS_INFO(CD3DCombo))
	{
		WIN_BORDERS borders;
		CD3DCombo * pCombo=m_pGUI->CreateCombo(SelectRect);
		pGUIObject=pCombo;

		borders[RECT_TOP]=2;
		borders[RECT_BOTTOM]=2;
		borders[RECT_LEFT]=2;
		borders[RECT_RIGHT]=2;
		borders[RECT_TOP_LEFT]=2;
		borders[RECT_TOP_RIGHT]=2;
		borders[RECT_BOTTOM_LEFT]=2;
		borders[RECT_BOTTOM_RIGHT]=2;

		Temp.Format(_T("Combo%d"),m_CreateCount);
		pCombo->SetName(Temp);
		pCombo->SetBorders(borders);
		pCombo->SetTexture(DEFAULT_UI_TEXTURE,CRect(174,123,203,152));	

		pCombo->GetExpandButton()->SetBorders(borders);

		pCombo->GetExpandButton()->SetTexture(BUTTON_STATUS_NORMAL,DEFAULT_UI_TEXTURE,CRect(115,42,128,55),FLOAT_POINT(0,0));
		pCombo->GetExpandButton()->SetTexture(BUTTON_STATUS_ACTIVE,DEFAULT_UI_TEXTURE,CRect(115,42,128,55),FLOAT_POINT(0,0));
		pCombo->GetExpandButton()->SetTexture(BUTTON_STATUS_PRESS,DEFAULT_UI_TEXTURE,CRect(115,55,128,68),FLOAT_POINT(0,0));
		pCombo->GetExpandButton()->SetTexture(BUTTON_STATUS_DISBALE,DEFAULT_UI_TEXTURE,CRect(115,42,128,55),FLOAT_POINT(0,0));
					

		pCombo->SetComboListHeight(200);
		pCombo->GetComboList()->SetBorders(borders);
		pCombo->GetComboList()->SetTexture(DEFAULT_UI_TEXTURE,CRect(174,123,203,152));	
		pCombo->GetComboList()->AddItem(_T("选项1"));
		pCombo->GetComboList()->AddItem(_T("选项2"));
		pCombo->GetComboList()->AddItem(_T("选项3"));
		pCombo->GetComboList()->AddItem(_T("选项4"));
		pCombo->GetComboList()->AddItem(_T("选项5"));

		borders[RECT_TOP]=1;
		borders[RECT_BOTTOM]=1;
		borders[RECT_LEFT]=1;
		borders[RECT_RIGHT]=1;
		borders[RECT_TOP_LEFT]=1;
		borders[RECT_TOP_RIGHT]=1;
		borders[RECT_BOTTOM_LEFT]=1;
		borders[RECT_BOTTOM_RIGHT]=1;

		CD3DScrollBar * pScrollBar=pCombo->GetComboList()->GetScrollBar();			
		pScrollBar->SetBorders(borders);
		pScrollBar->SetTexture(DEFAULT_UI_TEXTURE,CRect(174,92,203,121));	

		borders[RECT_TOP]=2;
		borders[RECT_BOTTOM]=2;
		borders[RECT_LEFT]=2;
		borders[RECT_RIGHT]=2;
		borders[RECT_TOP_LEFT]=2;
		borders[RECT_TOP_RIGHT]=2;
		borders[RECT_BOTTOM_LEFT]=2;
		borders[RECT_BOTTOM_RIGHT]=2;

		pScrollBar->GetDecButton()->SetBorders(borders);
		pScrollBar->GetDecButton()->SetTexture(BUTTON_STATUS_NORMAL,DEFAULT_UI_TEXTURE,CRect(101,42,114,55),FLOAT_POINT(0,0));
		pScrollBar->GetDecButton()->SetTexture(BUTTON_STATUS_ACTIVE,DEFAULT_UI_TEXTURE,CRect(101,42,114,55),FLOAT_POINT(0,0));
		pScrollBar->GetDecButton()->SetTexture(BUTTON_STATUS_PRESS,DEFAULT_UI_TEXTURE,CRect(101,55,114,68),FLOAT_POINT(0,0));
		pScrollBar->GetDecButton()->SetTexture(BUTTON_STATUS_DISBALE,DEFAULT_UI_TEXTURE,CRect(101,42,114,55),FLOAT_POINT(0,0));


		pScrollBar->GetIncButton()->SetBorders(borders);
		pScrollBar->GetIncButton()->SetTexture(BUTTON_STATUS_NORMAL,DEFAULT_UI_TEXTURE,CRect(115,42,128,55),FLOAT_POINT(0,0));
		pScrollBar->GetIncButton()->SetTexture(BUTTON_STATUS_ACTIVE,DEFAULT_UI_TEXTURE,CRect(115,42,128,55),FLOAT_POINT(0,0));
		pScrollBar->GetIncButton()->SetTexture(BUTTON_STATUS_PRESS,DEFAULT_UI_TEXTURE,CRect(115,55,128,68),FLOAT_POINT(0,0));
		pScrollBar->GetIncButton()->SetTexture(BUTTON_STATUS_DISBALE,DEFAULT_UI_TEXTURE,CRect(115,42,128,55),FLOAT_POINT(0,0));

		pScrollBar->GetTrackButton()->SetBorders(borders);
		pScrollBar->GetTrackButton()->SetTexture(BUTTON_STATUS_NORMAL,DEFAULT_UI_TEXTURE,CRect(103,84,116,97),FLOAT_POINT(0,0));
		pScrollBar->GetTrackButton()->SetTexture(BUTTON_STATUS_ACTIVE,DEFAULT_UI_TEXTURE,CRect(103,84,116,97),FLOAT_POINT(0,0));
		pScrollBar->GetTrackButton()->SetTexture(BUTTON_STATUS_PRESS,DEFAULT_UI_TEXTURE,CRect(103,84,116,97),FLOAT_POINT(0,0));
		pScrollBar->GetTrackButton()->SetTexture(BUTTON_STATUS_DISBALE,DEFAULT_UI_TEXTURE,CRect(103,84,116,97),FLOAT_POINT(0,0));

		
	}
	else if(GetDocument()->GetCurNewObject()==GET_CLASS_INFO(CD3DScrollBar))
	{
		WIN_BORDERS borders;

		borders[RECT_TOP]=2;
		borders[RECT_BOTTOM]=2;
		borders[RECT_LEFT]=2;
		borders[RECT_RIGHT]=2;
		borders[RECT_TOP_LEFT]=2;
		borders[RECT_TOP_RIGHT]=2;
		borders[RECT_BOTTOM_LEFT]=2;
		borders[RECT_BOTTOM_RIGHT]=2;


		CD3DScrollBar * pScrollBar=m_pGUI->CreateScrollBar(SelectRect);		
		pGUIObject=pScrollBar;

		Temp.Format(_T("ScrollBar%d"),m_CreateCount);
		pScrollBar->SetName(Temp);
		pScrollBar->SetBorders(borders);
		pScrollBar->SetTexture(DEFAULT_UI_TEXTURE,CRect(174,92,203,121));	


		borders[RECT_TOP]=2;
		borders[RECT_BOTTOM]=2;
		borders[RECT_LEFT]=2;
		borders[RECT_RIGHT]=2;
		borders[RECT_TOP_LEFT]=2;
		borders[RECT_TOP_RIGHT]=2;
		borders[RECT_BOTTOM_LEFT]=2;
		borders[RECT_BOTTOM_RIGHT]=2;
	

		pScrollBar->GetDecButton()->SetBorders(borders);
		pScrollBar->GetDecButton()->SetTexture(BUTTON_STATUS_NORMAL,DEFAULT_UI_TEXTURE,CRect(101,42,114,55),FLOAT_POINT(0,0));
		pScrollBar->GetDecButton()->SetTexture(BUTTON_STATUS_ACTIVE,DEFAULT_UI_TEXTURE,CRect(101,42,114,55),FLOAT_POINT(0,0));
		pScrollBar->GetDecButton()->SetTexture(BUTTON_STATUS_PRESS,DEFAULT_UI_TEXTURE,CRect(101,55,114,68),FLOAT_POINT(0,0));
		pScrollBar->GetDecButton()->SetTexture(BUTTON_STATUS_DISBALE,DEFAULT_UI_TEXTURE,CRect(101,42,114,55),FLOAT_POINT(0,0));


		pScrollBar->GetIncButton()->SetBorders(borders);
		pScrollBar->GetIncButton()->SetTexture(BUTTON_STATUS_NORMAL,DEFAULT_UI_TEXTURE,CRect(115,42,128,55),FLOAT_POINT(0,0));
		pScrollBar->GetIncButton()->SetTexture(BUTTON_STATUS_ACTIVE,DEFAULT_UI_TEXTURE,CRect(115,42,128,55),FLOAT_POINT(0,0));
		pScrollBar->GetIncButton()->SetTexture(BUTTON_STATUS_PRESS,DEFAULT_UI_TEXTURE,CRect(115,55,128,68),FLOAT_POINT(0,0));
		pScrollBar->GetIncButton()->SetTexture(BUTTON_STATUS_DISBALE,DEFAULT_UI_TEXTURE,CRect(115,42,128,55),FLOAT_POINT(0,0));

		pScrollBar->GetTrackButton()->SetBorders(borders);
		pScrollBar->GetTrackButton()->SetTexture(BUTTON_STATUS_NORMAL,DEFAULT_UI_TEXTURE,CRect(103,84,116,97),FLOAT_POINT(0,0));
		pScrollBar->GetTrackButton()->SetTexture(BUTTON_STATUS_ACTIVE,DEFAULT_UI_TEXTURE,CRect(103,84,116,97),FLOAT_POINT(0,0));
		pScrollBar->GetTrackButton()->SetTexture(BUTTON_STATUS_PRESS,DEFAULT_UI_TEXTURE,CRect(103,84,116,97),FLOAT_POINT(0,0));
		pScrollBar->GetTrackButton()->SetTexture(BUTTON_STATUS_DISBALE,DEFAULT_UI_TEXTURE,CRect(103,84,116,97),FLOAT_POINT(0,0));


	}
	else if(GetDocument()->GetCurNewObject()==GET_CLASS_INFO(CD3DProgressBar))
	{
		WIN_BORDERS borders;

		borders[RECT_TOP]=2;
		borders[RECT_BOTTOM]=2;
		borders[RECT_LEFT]=2;
		borders[RECT_RIGHT]=2;
		borders[RECT_TOP_LEFT]=2;
		borders[RECT_TOP_RIGHT]=2;
		borders[RECT_BOTTOM_LEFT]=2;
		borders[RECT_BOTTOM_RIGHT]=2;


		CD3DProgressBar * pProgressBar=m_pGUI->CreateProgressBar(SelectRect);		
		pGUIObject=pProgressBar;

		Temp.Format(_T("ProgressBar%d"),m_CreateCount);
		pProgressBar->SetName(Temp);
		pProgressBar->SetBorders(borders);
		pProgressBar->SetTexture(DEFAULT_UI_TEXTURE,CRect(174,92,203,121));	


		borders[RECT_TOP]=2;
		borders[RECT_BOTTOM]=2;
		borders[RECT_LEFT]=2;
		borders[RECT_RIGHT]=2;
		borders[RECT_TOP_LEFT]=2;
		borders[RECT_TOP_RIGHT]=2;
		borders[RECT_BOTTOM_LEFT]=2;
		borders[RECT_BOTTOM_RIGHT]=2;


		pProgressBar->GetBarWnd()->SetBorders(borders);
		pProgressBar->GetBarWnd()->SetTexture(DEFAULT_UI_TEXTURE,CRect(103,84,116,97));	

	}
	else
	{
		SelectWnds(SelectRect);
		return;
	}
	
	AddGUIObjectRecursion(pGUIObject,true);
	GetDocument()->SetCurNewObject(GET_CLASS_INFO(CNameObject));
	m_CreateCount++;
	return;
}

void CD3DViewPort::OnSelecting(CRect& SelectRect)
{
	FLOAT_RECT Rect;
	for(int i=0;i<4;i++)
	{
		m_SelectRect[i]->SetVisible(true);
	}
	Rect=SelectRect;
	Rect.bottom=Rect.top+1;
	m_SelectRect[0]->SetRect(&(Rect));
	Rect.right=(Rect.right-Rect.left)/2;
	Rect.left=0;
	Rect.bottom=(Rect.bottom-Rect.top)/2;
	Rect.top=0;
	m_SelectRect[0]->SetUV(&Rect);

	Rect=SelectRect;
	Rect.top=Rect.bottom-1;
	m_SelectRect[1]->SetRect(&(Rect));
	Rect.right=(Rect.right-Rect.left)/2;
	Rect.left=0;
	Rect.bottom=(Rect.bottom-Rect.top)/2;
	Rect.top=0;
	m_SelectRect[1]->SetUV(&Rect);

	Rect=SelectRect;
	Rect.right=Rect.left+1;
	m_SelectRect[2]->SetRect(&(Rect));
	Rect.right=(Rect.right-Rect.left)/2;
	Rect.left=0;
	Rect.bottom=(Rect.bottom-Rect.top)/2;
	Rect.top=0;
	m_SelectRect[2]->SetUV(&Rect);

	Rect=SelectRect;
	Rect.left=Rect.right-1;
	m_SelectRect[3]->SetRect(&(Rect));
	Rect.right=(Rect.right-Rect.left)/2;
	Rect.left=0;
	Rect.bottom=(Rect.bottom-Rect.top)/2;
	Rect.top=0;
	m_SelectRect[3]->SetUV(&Rect);
}

void CD3DViewPort::SelectWnds(CRect& SelectRect)
{
	LPVOID Pos;
	std::vector<CD3DWnd *> WndList;

	m_pSizeBarManager.ClearSelectedObject();
	m_pSizeBarManager.SetControlPress(true);
	
	Pos=m_pGUI->GetFirstWndPos();
	while(Pos)
	{
		CD3DWnd * pWnd=m_pGUI->GetNextWnd(Pos);
		if(pWnd->IsKindOf(GET_CLASS_INFO(CD3DSizeBar)))
		{
			CRect Rect=pWnd->GetWndRect();
			if(SelectRect.PtInRect(CPoint(Rect.left,Rect.top))||
				SelectRect.PtInRect(CPoint(Rect.left,Rect.bottom))||
				SelectRect.PtInRect(CPoint(Rect.right,Rect.top))||
				SelectRect.PtInRect(CPoint(Rect.right,Rect.bottom)))
			{
				WndList.push_back(pWnd);				
			}
		}
	}

	for(int i=(int)WndList.size()-1;i>=0;i--)
	{
		for(int j=0;j<(int)WndList.size();j++)
		{
			if(WndList[j]->IsChildRecursion(WndList[i]))
			{
				WndList.erase(WndList.begin()+i);
				break;
			}
		}
	}
	
	for(int i=0;i<(int)WndList.size();i++)
	{
		WndList[i]->SetFocus();
	}
	
	m_pSizeBarManager.SetControlPress(false);
}

CD3DSizeBar * CD3DViewPort::CreateSizeBar(CRect Rect)
{
	CD3DSizeBar * pSizeBar=new CD3DSizeBar(m_pGUI);
	pSizeBar->SetManager(&m_pSizeBarManager);
	m_pGUI->AddWnd(pSizeBar);
	m_pGUI->AddRootWnd(pSizeBar);

	WIN_BORDERS borders;

	borders[RECT_TOP]=6;
	borders[RECT_BOTTOM]=6;
	borders[RECT_LEFT]=6;
	borders[RECT_RIGHT]=6;
	borders[RECT_TOP_LEFT]=6;
	borders[RECT_TOP_RIGHT]=6;
	borders[RECT_BOTTOM_LEFT]=6;
	borders[RECT_BOTTOM_RIGHT]=6;

	Rect.top-=6;
	Rect.left-=6;
	Rect.bottom+=6;
	Rect.right+=6;
	pSizeBar->SetRect(Rect);
	pSizeBar->SetName(_T("SizeBar"));
	pSizeBar->SetBorders(borders);
	pSizeBar->SetTexture(_T("[UI]\\SYS_SIZE_BAR"),CRect(0,0,32,32));
	pSizeBar->EnableResize(true);
	pSizeBar->EnableDrag(true);	
	return pSizeBar;
}

void CD3DViewPort::AddGUIObject(CD3DWnd * pGUIObject,bool AutoBindParent)
{
	if(pGUIObject)
	{
		if(pGUIObject->IsInternal())
		{
			GetDocument()->AddGUIObject(pGUIObject);
		}
		else
		{
			CRect Rect;
			CRect OrgRect;
			LPVOID Pos;
			CD3DWnd * pParent;

			
			OrgRect=pGUIObject->GetWndRect();
			Rect=pGUIObject->GetRect();
			CD3DSizeBar * pSizeBar=CreateSizeBar(Rect);		
			pSizeBar->SetParent(pGUIObject->GetParent());
			pGUIObject->SetParent(pSizeBar);

			if(AutoBindParent)
			{
				Pos=m_pGUI->GetFirstWndPosAtPos(OrgRect.left,OrgRect.top);
				while(Pos)
				{
					pParent=m_pGUI->GetNextWndAtPos(Pos,OrgRect.left,OrgRect.top);
					if(pParent&&pParent!=pGUIObject&&(!pParent->IsKindOf(GET_CLASS_INFO(CD3DSizeBar))))
					{
						Rect=pSizeBar->GetWndRect();
						pSizeBar->SetParent(pParent);
						pParent->ScreenToClient(&Rect);
						pSizeBar->SetRect(Rect);
						break;
					}			
				}
			}
			Rect=OrgRect;
			pSizeBar->ScreenToClient(&Rect);
			pGUIObject->SetRect(Rect);
			pSizeBar->SetVisible(false);
			pSizeBar->SetName(CString(pSizeBar->GetName())+_T("_")+pGUIObject->GetName());
			GetDocument()->AddGUIObject(pSizeBar);
		}
	}
}

void CD3DViewPort::AddGUIObjectRecursion(CD3DWnd * pGUIObject,bool AutoBindParent)
{
	std::vector<CD3DWnd *> ChildList;
	AddGUIObject(pGUIObject,AutoBindParent);
	for(int i=0;i<pGUIObject->GetChildWndCount();i++)
	{
		ChildList.push_back(pGUIObject->GetChildWnd(i));
	}
	for(int i=0;i<(int)ChildList.size();i++)
	{
		if(!ChildList[i]->IsKindOf(GET_CLASS_INFO(CD3DSizeBar)))
		{
			AddGUIObjectRecursion(ChildList[i],false);
		}
	}
}

LRESULT CD3DViewPort::OnWndLoaded(WPARAM wParam,LPARAM lParam)
{
	CD3DWnd * pWnd=(CD3DWnd *)lParam;	
	AddGUIObject(pWnd,false);
	return true;
}
void CD3DViewPort::Cut()
{
	// TODO: 在此添加命令处理程序代码
	Copy();
	m_pSizeBarManager.DeleteAllSelectedObject();
}

void CD3DViewPort::Copy()
{
	// TODO: 在此添加命令处理程序代码
	if(m_pSizeBarManager.GetSelectedObjectCount()<=0)
		return;

	pug::xml_parser Xml;

	Xml.create();
	xml_node Doc;
	Doc = Xml.document();
	xml_node pi = Doc.append_child(node_pi);
	pi.name(_T("xml"));
	pi.attribute(_T("version")) = _T("1.0");
#ifdef UNICODE
	pi.attribute(_T("encoding")) = _T("ucs-2");
#else
	pi.attribute(_T("encoding")) = _T("gb2312");
#endif
	xml_node GUI=Doc.append_child(node_element,_T("D3DGUI"));

	CD3DWnd * pParent;
	for(int i=0;i<m_pSizeBarManager.GetSelectedObjectCount();i++)
	{
		pParent=m_pSizeBarManager.GetSelectedObject(i)->GetParent();
		m_pSizeBarManager.GetSelectedObject(i)->SaveToXml(&GUI);
	}

	if(pParent)
		GUI.attribute(_T("Parent"))=pParent->GetName();

#ifdef UNICODE
	std::wstringstream os;		
#else
	std::stringstream os;
#endif
	
	os << Xml.document();
	os<<'\0';
	//os.freeze();

	CEasyString Script=os.str().c_str();

	Script.Replace(_T("\n"),_T("\r\n"));

	HGLOBAL hClip = GlobalAlloc(GMEM_ZEROINIT|GMEM_MOVEABLE|GMEM_DDESHARE,(Script.GetLength()+1)*sizeof(TCHAR));
	if(!hClip)
		return;

	TCHAR *Buff = (TCHAR*)GlobalLock(hClip);
	if(!Buff)
	{
		GlobalUnlock(hClip);
		GlobalFree(hClip);
		return;
	}
	_tcscpy_s(Buff,Script.GetLength()+1,(LPCTSTR)Script);		

	GlobalUnlock(hClip);

	if(OpenClipboard())
	{
		EmptyClipboard();
		SetClipboardData( CF_TEXT,hClip);
		CloseClipboard();
	}
	else
		GlobalFree(hClip);
	
}

void CD3DViewPort::Paste()
{
	// TODO: 在此添加命令处理程序代码
	if(OpenClipboard())
	{
		HGLOBAL hClip=NULL;

		if( hClip=GetClipboardData(CF_TEXT) )
		{
			CString Text;
			Text = (LPCTSTR)GlobalLock(hClip);				
			if(Text.GetLength())
			{				
				pug::xml_parser Xml;
				Xml.parse(Text.GetBuffer(),pug::parse_trim_attribute);
				xml_node GUI=Xml.document();
				if(!GUI.moveto_child(_T("D3DGUI")))
					return;
				CD3DWnd * pParent=NULL;
				if(GUI.has_attribute(_T("Parent")))
					pParent=m_pGUI->GetWndByName(GUI.attribute(_T("Parent")).getvalue());
				m_pGUI->LoadFromXML(&GUI,pParent);
			}
			GlobalUnlock(hClip);				
		}
		CloseClipboard();		
	}
}

void CD3DViewPort::AlignLeft()
{
	if(m_pSizeBarManager.GetSelectedObjectCount()<=1)
		return;

	CRect TemplRect=m_pSizeBarManager.GetSelectedObject(m_pSizeBarManager.GetSelectedObjectCount()-1)->GetRect();
	for(int i=0;i<m_pSizeBarManager.GetSelectedObjectCount()-1;i++)
	{
		CRect Rect=	m_pSizeBarManager.GetSelectedObject(i)->GetRect();
		Rect.right=Rect.right-Rect.left+TemplRect.left;
		Rect.left=TemplRect.left;
		m_pSizeBarManager.GetSelectedObject(i)->SetRect(Rect);
		m_pSizeBarManager.GetSelectedObject(i)->UpdateSizeToChild();
	}

}

void CD3DViewPort::AlignRight()
{
	if(m_pSizeBarManager.GetSelectedObjectCount()<=1)
		return;

	CRect TemplRect=m_pSizeBarManager.GetSelectedObject(m_pSizeBarManager.GetSelectedObjectCount()-1)->GetRect();
	for(int i=0;i<m_pSizeBarManager.GetSelectedObjectCount()-1;i++)
	{
		CRect Rect=	m_pSizeBarManager.GetSelectedObject(i)->GetRect();
		Rect.left=TemplRect.right-(Rect.right-Rect.left);
		Rect.right=TemplRect.right;
		m_pSizeBarManager.GetSelectedObject(i)->SetRect(Rect);
		m_pSizeBarManager.GetSelectedObject(i)->UpdateSizeToChild();
	}
}

void CD3DViewPort::AlignTop()
{
	if(m_pSizeBarManager.GetSelectedObjectCount()<=1)
		return;

	CRect TemplRect=m_pSizeBarManager.GetSelectedObject(m_pSizeBarManager.GetSelectedObjectCount()-1)->GetRect();
	for(int i=0;i<m_pSizeBarManager.GetSelectedObjectCount()-1;i++)
	{
		CRect Rect=	m_pSizeBarManager.GetSelectedObject(i)->GetRect();
		Rect.bottom=Rect.bottom-Rect.top+TemplRect.top;
		Rect.top=TemplRect.top;
		m_pSizeBarManager.GetSelectedObject(i)->SetRect(Rect);
		m_pSizeBarManager.GetSelectedObject(i)->UpdateSizeToChild();
	}
}

void CD3DViewPort::AlignBottom()
{
	if(m_pSizeBarManager.GetSelectedObjectCount()<=1)
		return;

	CRect TemplRect=m_pSizeBarManager.GetSelectedObject(m_pSizeBarManager.GetSelectedObjectCount()-1)->GetRect();
	for(int i=0;i<m_pSizeBarManager.GetSelectedObjectCount()-1;i++)
	{
		CRect Rect=	m_pSizeBarManager.GetSelectedObject(i)->GetRect();
		Rect.top=TemplRect.bottom-(Rect.bottom-Rect.top);
		Rect.bottom=TemplRect.bottom;
		m_pSizeBarManager.GetSelectedObject(i)->SetRect(Rect);
		m_pSizeBarManager.GetSelectedObject(i)->UpdateSizeToChild();
	}
}

void CD3DViewPort::VCenter()
{
	if(m_pSizeBarManager.GetSelectedObjectCount()<=0)
		return;

	int Height,Top,Bottom,dy;

	CD3DWnd * pParent=m_pSizeBarManager.GetSelectedObject(m_pSizeBarManager.GetSelectedObjectCount()-1)->GetParent();
	if(pParent)
	{
		Height=pParent->GetClientRect().Height();
	}
	else
	{
		Height=600;
	}
	Top=Height;
	Bottom=0;
	for(int i=0;i<m_pSizeBarManager.GetSelectedObjectCount();i++)
	{
		CRect Rect=	m_pSizeBarManager.GetSelectedObject(i)->GetRect();
		if(Rect.top<Top)
			Top=Rect.top;
		if(Rect.bottom>Bottom)
			Bottom=Rect.bottom;
	}
	dy=(Height-(Bottom-Top))/2;
	dy=dy-Top;
	for(int i=0;i<m_pSizeBarManager.GetSelectedObjectCount();i++)
	{
		CRect Rect=	m_pSizeBarManager.GetSelectedObject(i)->GetRect();
		Rect.OffsetRect(0,dy);
		m_pSizeBarManager.GetSelectedObject(i)->SetRect(Rect);
		m_pSizeBarManager.GetSelectedObject(i)->UpdateSizeToChild();
	}
}

void CD3DViewPort::HCenter()
{
	if(m_pSizeBarManager.GetSelectedObjectCount()<=0)
		return;

	int Width,Left,Right,dx;

	CD3DWnd * pParent=m_pSizeBarManager.GetSelectedObject(m_pSizeBarManager.GetSelectedObjectCount()-1)->GetParent();
	if(pParent)
	{
		Width=pParent->GetClientRect().Width();
	}
	else
	{
		Width=800;
	}
	Left=Width;
	Right=0;
	for(int i=0;i<m_pSizeBarManager.GetSelectedObjectCount();i++)
	{
		CRect Rect=	m_pSizeBarManager.GetSelectedObject(i)->GetRect();
		if(Rect.left<Left)
			Left=Rect.left;
		if(Rect.right>Right)
			Right=Rect.right;
	}
	dx=(Width-(Right-Left))/2;
	dx=dx-Left;
	for(int i=0;i<m_pSizeBarManager.GetSelectedObjectCount();i++)
	{
		CRect Rect=	m_pSizeBarManager.GetSelectedObject(i)->GetRect();
		Rect.OffsetRect(dx,0);
		m_pSizeBarManager.GetSelectedObject(i)->SetRect(Rect);
		m_pSizeBarManager.GetSelectedObject(i)->UpdateSizeToChild();
	}
}

void CD3DViewPort::HArrange()
{
	if(m_pSizeBarManager.GetSelectedObjectCount()<=1)
		return;

	int Width=0,Left=99999,Right=-99999,Space;
	CArray<CD3DSizeBar *,CD3DSizeBar *> WndList;
	for(int i=0;i<m_pSizeBarManager.GetSelectedObjectCount();i++)
	{
		CRect Rect=	m_pSizeBarManager.GetSelectedObject(i)->GetRect();
		if(Rect.left<Left)
			Left=Rect.left;
		if(Rect.right>Right)
			Right=Rect.right;
		Width+=Rect.Width();
		int j=0;
		for(;j<WndList.GetCount();j++)
		{
			CRect rc=WndList[j]->GetRect();
			if(rc.left>Rect.left)
				break;
		}
		WndList.InsertAt(j,m_pSizeBarManager.GetSelectedObject(i));
	}
	Space=Right-Left-Width;
	Space=Space/(m_pSizeBarManager.GetSelectedObjectCount()-1);

	for(int i=1;i<WndList.GetCount();i++)
	{
		CRect Rect=	WndList[i]->GetRect();
		Width=Rect.Width();
		Rect.left=WndList[i-1]->GetRect().right+Space;
		Rect.right=Rect.left+Width;
		WndList[i]->SetRect(Rect);
		WndList[i]->UpdateSizeToChild();
	}

}
void CD3DViewPort::VArrange()
{
	if(m_pSizeBarManager.GetSelectedObjectCount()<=1)
		return;

	int Height=0,Top=99999,Bottom=-99999,Space;
	CArray<CD3DSizeBar *,CD3DSizeBar *> WndList;
	for(int i=0;i<m_pSizeBarManager.GetSelectedObjectCount();i++)
	{
		CRect Rect=	m_pSizeBarManager.GetSelectedObject(i)->GetRect();
		if(Rect.top<Top)
			Top=Rect.top;
		if(Rect.bottom>Bottom)
			Bottom=Rect.bottom;
		Height+=Rect.Height();
		int j=0;
		for(;j<WndList.GetCount();j++)
		{
			CRect rc=WndList[j]->GetRect();
			if(rc.top>Rect.top)
				break;
		}
		WndList.InsertAt(j,m_pSizeBarManager.GetSelectedObject(i));
	}
	Space=Bottom-Top-Height;
	Space=Space/(m_pSizeBarManager.GetSelectedObjectCount()-1);

	for(int i=1;i<WndList.GetCount();i++)
	{
		CRect Rect=	WndList[i]->GetRect();
		Height=Rect.Height();
		Rect.top=WndList[i-1]->GetRect().bottom+Space;
		Rect.bottom=Rect.top+Height;
		WndList[i]->SetRect(Rect);
		WndList[i]->UpdateSizeToChild();
	}
}

void CD3DViewPort::SameWidth()
{
	if(m_pSizeBarManager.GetSelectedObjectCount()<=1)
		return;

	int Width=m_pSizeBarManager.GetSelectedObject(m_pSizeBarManager.GetSelectedObjectCount()-1)->GetRect().Width();
	for(int i=0;i<m_pSizeBarManager.GetSelectedObjectCount()-1;i++)
	{
		CRect Rect=	m_pSizeBarManager.GetSelectedObject(i)->GetRect();
		Rect.right=Rect.left+Width;		
		m_pSizeBarManager.GetSelectedObject(i)->SetRect(Rect);
		m_pSizeBarManager.GetSelectedObject(i)->UpdateSizeToChild();
	}
}
void CD3DViewPort::SameHeight()
{
	if(m_pSizeBarManager.GetSelectedObjectCount()<=1)
		return;

	int Height=m_pSizeBarManager.GetSelectedObject(m_pSizeBarManager.GetSelectedObjectCount()-1)->GetRect().Height();
	for(int i=0;i<m_pSizeBarManager.GetSelectedObjectCount()-1;i++)
	{
		CRect Rect=	m_pSizeBarManager.GetSelectedObject(i)->GetRect();
		Rect.bottom=Rect.top+Height;
		m_pSizeBarManager.GetSelectedObject(i)->SetRect(Rect);
		m_pSizeBarManager.GetSelectedObject(i)->UpdateSizeToChild();
	}
}
void CD3DViewPort::SameSize()
{
	if(m_pSizeBarManager.GetSelectedObjectCount()<=1)
		return;

	int Width=m_pSizeBarManager.GetSelectedObject(m_pSizeBarManager.GetSelectedObjectCount()-1)->GetRect().Width();
	int Height=m_pSizeBarManager.GetSelectedObject(m_pSizeBarManager.GetSelectedObjectCount()-1)->GetRect().Height();
	for(int i=0;i<m_pSizeBarManager.GetSelectedObjectCount()-1;i++)
	{
		CRect Rect=	m_pSizeBarManager.GetSelectedObject(i)->GetRect();
		Rect.right=Rect.left+Width;	
		Rect.bottom=Rect.top+Height;
		m_pSizeBarManager.GetSelectedObject(i)->SetRect(Rect);
		m_pSizeBarManager.GetSelectedObject(i)->UpdateSizeToChild();
	}	
}


void CD3DViewPort::SetParent()
{
	CArray<CD3DSizeBar *,CD3DSizeBar *> WndList;

	for(int i=0;i<m_pSizeBarManager.GetSelectedObjectCount();i++)
	{
		WndList.Add(m_pSizeBarManager.GetSelectedObject(i));
	}

	for(int i=0;i<WndList.GetCount();i++)
	{
		CD3DWnd * pParent=NULL;
		CRect Rect=	WndList[i]->GetWndRect();
		LPVOID Pos=m_pGUI->GetFirstWndPosAtPos(Rect.left,Rect.top);
		if(Pos)
		{		
			pParent=m_pGUI->GetNextWndAtPos(Pos,Rect.left,Rect.top);
		}
		if(pParent)
		{		
			if(pParent->IsKindOf(GET_CLASS_INFO(CD3DSizeBar)))
				pParent=((CD3DSizeBar *)pParent)->GetChild();
		}		
		WndList[i]->SetParent(pParent);
		if(pParent)
		{
			pParent->ScreenToClient(&Rect);
		}
		WndList[i]->SetRect(Rect);
	}	
}
LRESULT CD3DViewPort::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	
	switch(message)
	{		
	case WM_KEYDOWN:
		{
			switch(wParam)
			{			
			case VK_CONTROL:

				m_pSizeBarManager.SetControlPress(true);
				break;			
			}

		}
		break;
	case WM_KEYUP:
		{			
			switch(wParam)
			{
			case VK_CONTROL:				
				m_pSizeBarManager.SetControlPress(false);
				break;
			}

		}
		break;
	}

	if(message==WM_MOUSEWHEEL)
	{		
		POINT point;
		point.x=LOWORD(lParam);
		point.y=HIWORD(lParam);
		ScreenToClient(&point);
		lParam=point.y<<16|point.x;
	}

	if(m_pGUI)
	{
		if(m_pGUI->HandleMessage(message, wParam, lParam ))
			return true;
	}

	switch(message)
	{	
	case WM_LBUTTONDOWN:
		SetFocus();
		if(m_SelectStatus==SST_NONE)
		{
			m_SelectStatus=SST_READY;
			m_SelectStartX=LOWORD(lParam);
			m_SelectStartY=HIWORD(lParam);
			for(int i=0;i<4;i++)
			{
				m_SelectRect[i]->TopTo(NULL);
			}
		}

		break;
	case WM_LBUTTONUP:
		if(m_SelectStatus==SST_SELECTING)
		{		
			CRect Rect;			
			if(m_SelectStartX<LOWORD(lParam))
			{
				Rect.left=m_SelectStartX;				
				Rect.right=LOWORD(lParam);				
			}
			else
			{
				Rect.right=m_SelectStartX;
				Rect.left=LOWORD(lParam);
			}
			if(m_SelectStartY<HIWORD(lParam))
			{
				Rect.top=m_SelectStartY;
				Rect.bottom=HIWORD(lParam);
			}
			else
			{				
				Rect.bottom=m_SelectStartY;				
				Rect.top=HIWORD(lParam);
			}
			if(Rect.Width()||Rect.Height())
				OnSelected(Rect);			
		}
		m_SelectStatus=SST_NONE;
		break;
	case WM_RBUTTONUP:
		GetDocument()->SetCurNewObject(GET_CLASS_INFO(CNameObject));
		break;
	case WM_MOUSEMOVE:
		if(m_SelectStatus==SST_READY||m_SelectStatus==SST_SELECTING)
		{
			CRect Rect;
			m_SelectStatus=SST_SELECTING;			
			if(m_SelectStartX<LOWORD(lParam))
			{
				Rect.left=m_SelectStartX;				
				Rect.right=LOWORD(lParam);				
			}
			else
			{
				Rect.right=m_SelectStartX;
				Rect.left=LOWORD(lParam);
			}
			if(m_SelectStartY<HIWORD(lParam))
			{
				Rect.top=m_SelectStartY;
				Rect.bottom=HIWORD(lParam);
			}
			else
			{				
				Rect.bottom=m_SelectStartY;				
				Rect.top=HIWORD(lParam);
			}
			if(Rect.Width()||Rect.Height())
				OnSelecting(Rect);			
		}
		break;


	}	

	return CWnd::WindowProc(message, wParam, lParam);
}

void CD3DViewPort::Update()
{
	float CurTime=CEasyTimer::GetTime()/1000.0f;
	m_pD3DDevice->GetTextureManager()->Update(CurTime);
	m_pGUI->Update();
	m_pRender->Update(CurTime);		
	m_pD3DDevice->StartRender(D3DCOLOR_XRGB(64,128,64));
	m_pRender->Render();
	m_pD3DDevice->EndRender();
	m_FrameCount++;
	Sleep(1);
}