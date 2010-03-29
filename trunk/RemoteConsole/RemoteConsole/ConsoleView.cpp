/****************************************************************************/
/*                                                                          */
/*      �ļ���:    ConsoleView.cpp                                          */
/*      ��������:  2009��02��09��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
// ConsoleView.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RemoteConsole.h"
#include "ConsoleView.h"


// CConsoleView

IMPLEMENT_DYNCREATE(CConsoleView, CFormView)

CConsoleView::CConsoleView()
	: CFormView(CConsoleView::IDD)
{

}

CConsoleView::~CConsoleView()
{
}

void CConsoleView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_LOG, m_edLog);
	DDX_Control(pDX, IDC_LIST_LINKS, m_lvLinks);
}

BEGIN_MESSAGE_MAP(CConsoleView, CFormView)
	ON_MESSAGE(WM_USER_LOG_MSG, OnConsoleMsg)
	ON_WM_SIZE()
//	ON_WM_CLOSE()
ON_WM_TIMER()
END_MESSAGE_MAP()


// CConsoleView ���

#ifdef _DEBUG
void CConsoleView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CConsoleView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CConsoleView ��Ϣ�������

void CConsoleView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	m_lvLinks.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_lvLinks.InsertColumn(0,"ID",LVCFMT_LEFT,80);
	m_lvLinks.InsertColumn(1,"��ַ",LVCFMT_LEFT,150);
	m_lvLinks.InsertColumn(2,"״̬",LVCFMT_LEFT,50);
	

	CControlPanel::GetInstance()->SetHwnd(GetSafeHwnd());

	m_ItemSorter.SetParent(this);

	m_ItemSorter.SetSortType(0,CDialogItemSorter::SORT_HORIZONTAL|CDialogItemSorter::PARAM_ALIGN_SPACE,5);

	m_ItemSorter.AddDialogItem(0,1,
		CDialogItemSorter::SORT_HORIZONTAL|		
		CDialogItemSorter::RESIZE_FIT_PARENT_WIDTH|
		CDialogItemSorter::RESIZE_FIT_PARENT_HEIGHT|
		CDialogItemSorter::ALIGN_CENTER,
		0,IDC_EDIT_LOG,CRect(0,0,0,0));
	m_ItemSorter.AddDialogItem(0,2,
		CDialogItemSorter::SORT_HORIZONTAL|				
		CDialogItemSorter::RESIZE_FIT_PARENT_HEIGHT|
		CDialogItemSorter::ALIGN_CENTER,
		0,IDC_LIST_LINKS,CRect(0,0,300,0));

	m_ItemSorter.DoSort();

	SetScrollSizes(MM_TEXT,CSize(0,0));

	SetTimer(1001,5000,NULL);

	// TODO: �ڴ����ר�ô����/����û���
}

LRESULT CConsoleView::OnConsoleMsg(WPARAM wParam, LPARAM lParam)
{	

	PANEL_MSG * pMsg=(PANEL_MSG *)lParam;

	if(IsWindowVisible())
	{
		switch(pMsg->MsgType)
		{
		case CON_MSG_LOG:
			PrintLogMsg(pMsg->Msg);
			break;
		}
	}

	CControlPanel::GetInstance()->ReleaseMsg(pMsg->ID);


	return 0;
}

void CConsoleView::PrintLogMsg(LPCTSTR szMsg)
{
	int s1,s2;
	int sm1,sm2;
	int SelLine;
	int EndLine;

	m_edLog.SetRedraw(false);

	m_edLog.GetSel(sm1,sm2);
	m_edLog.SetSel(0,-1);
	m_edLog.GetSel(s1,s2);
	SelLine=m_edLog.LineFromChar(sm1);
	EndLine=m_edLog.GetLineCount()-1;
	if(s2>MAX_LOG_MSG_COUNT)
	{
		m_edLog.SetSel(0,-1);
		m_edLog.Clear();
		s2=0;
	}
	m_edLog.SetSel(s2,s2);

	m_edLog.ReplaceSel(szMsg);


	m_edLog.SetSel(0,-1);
	m_edLog.GetSel(s1,s2);
	m_edLog.SetSel(s2,s2);	

	m_edLog.ReplaceSel("\r\n");

	m_edLog.SetRedraw(true);	


	if(SelLine==EndLine)
		m_edLog.LineScroll(m_edLog.GetLineCount());
	else
		m_edLog.SetSel(sm1,sm2);

}
void CConsoleView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	m_ItemSorter.DoSort();
}

//void CConsoleView::OnClose()
//{
//	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
//
//	CFormView::OnClose();
//}

void CConsoleView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if(nIDEvent=1001)
	{
		FillList();
	}

	CFormView::OnTimer(nIDEvent);
}


void CConsoleView::FillList()
{
	m_lvLinks.DeleteAllItems();

	for(UINT i=0;i<CRemoteConsoleApp::GetInstance()->GetNetLinkManager()->GetConnectionCount();i++)
	{
		CEasyNetLinkConnection * pLink=CRemoteConsoleApp::GetInstance()->GetNetLinkManager()->GetConnectionByIndex(i);
		if(pLink)
		{
			CClassifiedID ID=pLink->GetID();
			int Item=m_lvLinks.InsertItem(i,ID.ToStr());
			CEasyString Address;
			Address.Format("%s:%u",pLink->GetRemoteAddress().GetIPString(),pLink->GetRemoteAddress().GetPort());
			m_lvLinks.SetItemText(Item,1,Address);			
		}
	}
}