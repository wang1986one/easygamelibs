/****************************************************************************/
/*                                                                          */
/*      文件名:    ControlPanelView.cpp                                     */
/*      创建日期:  2011年10月07日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
/****************************************************************************/
/*                                                                          */
/*      文件名:    ControlPanelView.cpp                                     */
/*      创建日期:  2009年09月11日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
// ControlPanelView.cpp : 实现文件
//

#include "stdafx.h"
#include "ControlPanelView.h"



// CControlPanelView

IMPLEMENT_DYNCREATE(CControlPanelView, CFormView)

CControlPanelView::CControlPanelView()
	: CFormView(CControlPanelView::IDD)
{
}

CControlPanelView::~CControlPanelView()
{
}

void CControlPanelView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CControlPanelView, CFormView)
	ON_WM_KILLFOCUS()
END_MESSAGE_MAP()


// CControlPanelView 诊断

#ifdef _DEBUG
void CControlPanelView::AssertValid() const
{
	CFormView::AssertValid();
}

void CControlPanelView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG


// CControlPanelView 消息处理程序

void CControlPanelView::OnKillFocus(CWnd* pNewWnd)
{
	CFormView::OnKillFocus(pNewWnd);

	// TODO: 在此处添加消息处理程序代码
}
