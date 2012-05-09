/****************************************************************************/
/*                                                                          */
/*      �ļ���:    D3DView.cpp                                              */
/*      ��������:  2011��10��07��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
/****************************************************************************/
/*                                                                          */
/*      �ļ���:    D3DView.cpp                                              */
/*      ��������:  2009��09��11��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
// D3DView.cpp : ʵ���ļ�
//
#include "stdafx.h"
#include "D3DView.h"



// CD3DView

IMPLEMENT_DYNCREATE(CD3DView, CScrollView)

CD3DView::CD3DView()
{
	m_pViewPort=NULL;
	SetScrollSizes(MM_TEXT,CSize(
		CUSOEditApp::GetInstance()->GetViewportSetting().ViewportWidth,
		CUSOEditApp::GetInstance()->GetViewportSetting().ViewportHeight));

}

CD3DView::~CD3DView()
{
	SAFE_DELETE(m_pViewPort);
}

BEGIN_MESSAGE_MAP(CD3DView, CScrollView)
//	ON_WM_TIMER()
ON_WM_KILLFOCUS()
END_MESSAGE_MAP()


// CD3DView ��ͼ

void CD3DView::OnDraw(CDC* pDC)
{
//	CDocument* pDoc = GetDocument();
	// TODO: �ڴ���ӻ��ƴ���
}


// CD3DView ���

#ifdef _DEBUG
void CD3DView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CD3DView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif //_DEBUG


// CD3DView ��Ϣ�������

//void CD3DView::OnTimer(UINT nIDEvent){
//	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
//	
//	CScrollView::OnTimer(nIDEvent);
//}

void CD3DView::OnInitialUpdate()
{
	SetScrollSizes(MM_TEXT,CSize(
		CUSOEditApp::GetInstance()->GetViewportSetting().ViewportWidth,
		CUSOEditApp::GetInstance()->GetViewportSetting().ViewportHeight));
	CScrollView::OnInitialUpdate();

	// TODO: �ڴ����ר�ô����/����û���

	
	
}



//BOOL CD3DView::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
//{
//	// TODO: �ڴ����ר�ô����/����û���
//	
//	return CScrollView::OnWndMsg(message, wParam, lParam, pResult);
//}

BOOL CD3DView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	// TODO: �ڴ����ר�ô����/����û���

	m_pViewPort=new CD3DViewPort();

	if(!CScrollView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext))
		return false;
	
	if(!m_pViewPort->Create(this))
		return false;
	return true;
}

BOOL CD3DView::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if(pMsg->message==WM_MOUSEWHEEL)
	{	
		m_pViewPort->PreTranslateMessage(pMsg);
		
	}
	return CScrollView::PreTranslateMessage(pMsg);
}


void CD3DView::OnKillFocus(CWnd* pNewWnd)
{
	CScrollView::OnKillFocus(pNewWnd);

	// TODO: �ڴ˴������Ϣ����������
}
