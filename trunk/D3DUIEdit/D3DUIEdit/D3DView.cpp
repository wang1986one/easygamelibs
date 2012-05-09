/****************************************************************************/
/*                                                                          */
/*      文件名:    D3DView.cpp                                              */
/*      创建日期:  2011年10月07日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
// D3DView.cpp : 实现文件
//
#include "stdafx.h"
#include "D3DUIEditDoc.h"
#include "D3DView.h"


// CD3DView

IMPLEMENT_DYNCREATE(CD3DView, CScrollView)

CD3DView::CD3DView()
{
	m_pViewPort=NULL;
	SetScrollSizes(MM_TEXT,CSize(800,600));

}

CD3DView::~CD3DView()
{
	SAFE_DELETE(m_pViewPort);
}

BEGIN_MESSAGE_MAP(CD3DView, CScrollView)
//	ON_WM_TIMER()
END_MESSAGE_MAP()


// CD3DView 绘图

void CD3DView::OnDraw(CDC* pDC)
{
//	CDocument* pDoc = GetDocument();
	// TODO: 在此添加绘制代码
}


// CD3DView 诊断

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


// CD3DView 消息处理程序

//void CD3DView::OnTimer(UINT nIDEvent){
//	// TODO: 在此添加消息处理程序代码和/或调用默认值
//	
//	CScrollView::OnTimer(nIDEvent);
//}

void CD3DView::OnInitialUpdate()
{
	SetScrollSizes(MM_TEXT,CSize(800,600));
	CScrollView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类

	
	
}



//BOOL CD3DView::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
//{
//	// TODO: 在此添加专用代码和/或调用基类
//	
//	return CScrollView::OnWndMsg(message, wParam, lParam, pResult);
//}

BOOL CD3DView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	// TODO: 在此添加专用代码和/或调用基类

	if(!CScrollView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext))
		return false;
	m_pViewPort=new CD3DViewPort();
	m_pViewPort->Create(this);	
	return true;
}
