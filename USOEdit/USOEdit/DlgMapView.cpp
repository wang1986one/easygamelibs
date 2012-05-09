/****************************************************************************/
/*                                                                          */
/*      文件名:    DlgMapView.cpp                                           */
/*      创建日期:  2011年10月07日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
// DlgMapView.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgMapView.h"




// CDlgMapView 对话框

IMPLEMENT_DYNAMIC(CDlgMapView, CDialog)

CDlgMapView::CDlgMapView(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMapView::IDD, pParent)
{
	m_MapID=0xffffffff;
	m_hbmpMap=NULL;
	m_MapWidth=0;
	m_MapHeight=0;
	m_MapShowX=0;
	m_MapShowY=0;
	m_IsInDrag=false;
	m_DragStartX=0;
	m_DragStartY=0;
	m_ViewWidth=512;
	m_ViewHeight=512;
	m_SelectedMapX=0;
	m_SelectedMapY=0;
	
	m_pLightInfos=NULL;
}

CDlgMapView::~CDlgMapView()
{
	if(m_hbmpMap)
	{
		DeleteObject(m_hbmpMap);
		m_hbmpMap=NULL;
	}
}

void CDlgMapView::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgMapView, CDialog)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CDlgMapView 消息处理程序

BOOL CDlgMapView::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_MapDC.DeleteDC();
	m_MapDC.CreateCompatibleDC(GetDC());
	m_MapDC.SelectObject(m_hbmpMap);

	CRect WndRect,ClientRect;
	GetWindowRect(&WndRect);
	GetClientRect(&ClientRect);
	WndRect.right=WndRect.right-ClientRect.Width()+m_ViewWidth;
	WndRect.bottom=WndRect.bottom-ClientRect.Height()+m_ViewHeight;
	MoveWindow(&WndRect);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgMapView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialog::OnPaint()

	UINT Width=m_ViewWidth;
	UINT Height=m_ViewHeight;
	if(Width>m_MapWidth)
		Width=m_MapWidth;
	if(Height>m_MapHeight)
		Height=m_MapHeight;


	BOOL Ret=dc.BitBlt(0,0,Width,Height,&m_MapDC,m_MapShowX,m_MapShowY,SRCCOPY);

	D3DCOLOR GlobalDiffuse=0xFFFFFFFF;
	D3DCOLOR GlobalAmbient=0xFFFFFFFF;

	if(m_pLightInfos)
	{
		for(UINT i=0;i<m_pLightInfos->GetCount();i++)
		{
			CBLZWOWDatabase::BLZ_DB_LIGHT_INFO * pInfo=(*m_pLightInfos)[i];
			if(pInfo->IsGlobal)
			{
				pInfo->GetColor(DBC_LICI_LIGHT_DIFFUSE,43200,GlobalDiffuse);
				pInfo->GetColor(DBC_LICI_LIGHT_AMBIENT,43200,GlobalAmbient);
			}
			else
			{
				float x=(pInfo->Pos.x/BLZ_ADT_MAP_AREA_SIZE-m_StartBlockX)*64;
				float y=((-pInfo->Pos.z/BLZ_ADT_MAP_AREA_SIZE)-m_StartBlockY)*64;

				float r1=pInfo->FallOffStart*64/BLZ_ADT_MAP_AREA_SIZE;
				float r2=pInfo->FallOffEnd*64/BLZ_ADT_MAP_AREA_SIZE;
				
				

				x-=m_MapShowX;
				y-=m_MapShowY;
				int x1=x-r1;
				int y1=y-r1;
				int x2=x+r1;
				int y2=y+r1;

				CString ID;
				ID.Format(_T("%u"),pInfo->ID);

				dc.TextOut(x,y,ID);
			
				D3DCOLOR Color=GlobalDiffuse;
				pInfo->GetColor(DBC_LICI_LIGHT_DIFFUSE,43200,Color);
				Color&=0xFFFFFF;
				m_Pen.CreatePen(PS_SOLID,1,Color);
				CPen * pOldPen=dc.SelectObject(&m_Pen);	

				dc.Arc(x1,y1,x2,y2,x1,y1,x1,y1);

				dc.SelectObject(pOldPen);
				m_Pen.DeleteObject();


				x1=x-r2;
				y1=y-r2;
				x2=x+r2;
				y2=y+r2;

				Color=GlobalAmbient;
				pInfo->GetColor(DBC_LICI_LIGHT_AMBIENT,43200,Color);
				Color&=0xFFFFFF;
				m_Pen.CreatePen(PS_SOLID,1,Color);
				pOldPen=dc.SelectObject(&m_Pen);	

				dc.Arc(x1,y1,x2,y2,x1,y1,x1,y1);

				dc.SelectObject(pOldPen);
				m_Pen.DeleteObject();
				
			}
		}
	}
	
}


void CDlgMapView::SetMap(UINT MapID,HBITMAP hbmpMap,UINT Width,UINT Height,UINT StartBlockX,UINT StartBlockY)
{
	if(m_hbmpMap)
	{
		DeleteObject(m_hbmpMap);
	}
	m_MapID=MapID;
	m_hbmpMap=hbmpMap;
	m_MapWidth=Width;
	m_MapHeight=Height;	
	m_StartBlockX=StartBlockX;
	m_StartBlockY=StartBlockY;
	m_MapShowX=0;
	m_MapShowY=0;
	m_ViewWidth=1024;
	m_ViewHeight=768;
	if(m_ViewWidth>m_MapWidth)
		m_ViewWidth=m_MapWidth;
	if(m_ViewHeight>m_MapHeight)
		m_ViewHeight=m_MapHeight;
	
	m_pLightInfos=CBLZWOWDatabase::GetInstance()->GetMapLightInfos(MapID);

}
BOOL CDlgMapView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE;
	//return CDialog::OnEraseBkgnd(pDC);
}

BOOL CDlgMapView::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	switch(pMsg->message)
	{
	case WM_LBUTTONDOWN:
		m_IsInDrag=true;
		m_DragStartX=LOWORD(pMsg->lParam);
		m_DragStartY=HIWORD(pMsg->lParam);
		break;
	case WM_LBUTTONUP:
		m_IsInDrag=false;
		break;
	case WM_MOUSEMOVE:
		if(m_IsInDrag)
		{
			int MouseX=LOWORD(pMsg->lParam);
			int MouseY=HIWORD(pMsg->lParam);
			m_MapShowX-=MouseX-m_DragStartX;
			m_MapShowY-=MouseY-m_DragStartY;
			m_DragStartX=MouseX;
			m_DragStartY=MouseY;
			if(m_MapShowX<0)
				m_MapShowX=0;
			if(m_MapShowY<0)
				m_MapShowY=0;
			
			if(m_MapShowX+m_ViewWidth>m_MapWidth)
				m_MapShowX=m_MapWidth-m_ViewWidth;
			if(m_MapShowY+m_ViewHeight>m_MapHeight)
				m_MapShowY=m_MapHeight-m_ViewHeight;
			Invalidate();

		}
		break;
	case WM_LBUTTONDBLCLK:
		{
			m_SelectedMapX=m_MapShowX+LOWORD(pMsg->lParam);
			m_SelectedMapY=m_MapShowY+HIWORD(pMsg->lParam);
			OnOK();
		}
		break;
	}

	return CDialog::PreTranslateMessage(pMsg);
}
