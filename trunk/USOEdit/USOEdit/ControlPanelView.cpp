/****************************************************************************/
/*                                                                          */
/*      �ļ���:    ControlPanelView.cpp                                     */
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
/*      �ļ���:    ControlPanelView.cpp                                     */
/*      ��������:  2009��09��11��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
// ControlPanelView.cpp : ʵ���ļ�
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


// CControlPanelView ���

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


// CControlPanelView ��Ϣ�������

void CControlPanelView::OnKillFocus(CWnd* pNewWnd)
{
	CFormView::OnKillFocus(pNewWnd);

	// TODO: �ڴ˴������Ϣ����������
}
