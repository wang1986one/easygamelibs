/****************************************************************************/
/*                                                                          */
/*      �ļ���:    D3DViewBox.cpp                                           */
/*      ��������:  2011��10��07��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
// D3DViewBox.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "D3DUIEdit.h"
#include "D3DViewBox.h"
#include ".\d3dviewbox.h"

#define UPDATE_TIME				10
#define UPDATE_TIMER_ID			10001
// CD3DViewBox

IMPLEMENT_DYNCREATE(CD3DViewBox, CScrollView)

CD3DViewBox::CD3DViewBox()
{
	SetScrollSizes(MM_TEXT,CSize(800,600));
	m_pD3DDevice=NULL;
	m_pRender=NULL;
	m_pObjectCreator=NULL;
}

CD3DViewBox::~CD3DViewBox()
{
	SAFE_RELEASE(m_pGUI);
	SAFE_RELEASE(m_pObjectCreator);
	SAFE_RELEASE(m_pRender);
	SAFE_RELEASE(m_pD3DDevice);	
}


BEGIN_MESSAGE_MAP(CD3DViewBox, CScrollView)
	ON_WM_MOUSEACTIVATE()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CD3DViewBox ��ͼ

void CD3DViewBox::OnInitialUpdate()
{
	SetScrollSizes(MM_TEXT,CSize(800,600));
	CScrollView::OnInitialUpdate();
	m_ContentWnd.Create(NULL,NULL,WS_CHILD|WS_VISIBLE,CRect(0,0,800,600),this,10388);
	
	m_pD3DDevice=new CD3DDevice();

	D3DDEVICE_PARAMS D3DParams;

	D3DParams.hFocusWindow=m_ContentWnd.GetSafeHwnd();

	m_pD3DDevice->Create(D3DParams);


	m_pRender=new CD3DUIRender();

	m_pRender->Create(m_pD3DDevice);

	m_pObjectCreator=new CD3DGUIObjectCreator(m_pRender);

	m_pGUI=new CD3DGUI(m_pObjectCreator,GetParent()->GetSafeHwnd());

	SetTimer(UPDATE_TIMER_ID,UPDATE_TIME,NULL);
}

void CD3DViewBox::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: �ڴ���ӻ��ƴ���
}


// CD3DViewBox ���

#ifdef _DEBUG
void CD3DViewBox::AssertValid() const
{
	CScrollView::AssertValid();
}

void CD3DViewBox::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif //_DEBUG


// CD3DViewBox ��Ϣ�������


int CD3DViewBox::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	return MA_NOACTIVATE;
	//return CScrollView::OnMouseActivate(pDesktopWnd, nHitTest, message);
}

void CD3DViewBox::OnTimer(UINT nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	if(nIDEvent==UPDATE_TIMER_ID)
	{		
		m_pD3DDevice->GetTextureManager()->Update(0.0f);
		m_pGUI->Update();
		m_pRender->Update(0.0f);		
		m_pD3DDevice->StartRender(D3DCOLOR_XRGB(64,128,64));
		m_pRender->Render();
		m_pD3DDevice->EndRender();		
	}
	CScrollView::OnTimer(nIDEvent);
}

void CD3DViewBox::SetContentSize(int Width,int Height)
{
	SetScrollSizes(MM_TEXT,CSize(Width,Height));
	ScrollToPosition(CPoint(0,0));
	m_ContentWnd.MoveWindow(0,0,Width,Height);
	m_pD3DDevice->GetFXManager()->Reset();
	m_pRender->Reset();
	m_pD3DDevice->Reset();
	m_pRender->Restore();
	m_pD3DDevice->GetFXManager()->Restore();
}

BOOL CD3DViewBox::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if(pMsg->message==WM_MOUSEWHEEL)
	{		
		POINT point;
		point.x=LOWORD(pMsg->lParam);
		point.y=HIWORD(pMsg->lParam);
		ScreenToClient(&point);
		pMsg->lParam=point.y<<16|point.x;
	}

	if(m_pGUI)
	{
		if(m_pGUI->HandleMessage(pMsg->message, pMsg->wParam, pMsg->lParam ))
			return true;
	}

	return CScrollView::PreTranslateMessage(pMsg);
}
