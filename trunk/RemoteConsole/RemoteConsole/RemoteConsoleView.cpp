// RemoteConsoleView.cpp : CRemoteConsoleView 类的实现
//

#include "stdafx.h"
#include "RemoteConsole.h"

#include "RemoteConsoleDoc.h"
#include "RemoteConsoleView.h"
#include ".\remoteconsoleview.h"




// CRemoteConsoleView

IMPLEMENT_DYNCREATE(CRemoteConsoleView, CFormView)

BEGIN_MESSAGE_MAP(CRemoteConsoleView, CFormView)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_SEND_COMMAND, OnBnClickedButtonSendCommand)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CRemoteConsoleView 构造/析构

CRemoteConsoleView::CRemoteConsoleView()
	: CFormView(CRemoteConsoleView::IDD)
	, m_STClientCount(0)
	, m_STCycleTime(0)
	, m_STTCPRecvFlow(0)
	, m_STTCPSendFlow(0)
	, m_STUDPRecvFlow(0)
	, m_STUDPSendFlow(0)
{
	// TODO: 在此处添加构造代码
	m_pNetLink=NULL;
	m_pFileLog=NULL;
}

CRemoteConsoleView::~CRemoteConsoleView()
{
	if(m_pNetLink)
		m_pNetLink->ForceDisconnect();
	SAFE_RELEASE(m_pFileLog);
}

void CRemoteConsoleView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_COMMAND, m_edCommand);
	DDX_Control(pDX, IDC_EDIT_LOG, m_edLog);
	DDX_Text(pDX, IDC_ST_CLIENT_COUNT, m_STClientCount);
	DDX_Text(pDX, IDC_ST_CYCLE_TIME, m_STCycleTime);
	DDX_Text(pDX, IDC_ST_TCP_RECV, m_STTCPRecvFlow);
	DDX_Text(pDX, IDC_ST_TCP_SEND, m_STTCPSendFlow);
	DDX_Text(pDX, IDC_ST_UDP_RECV, m_STUDPRecvFlow);
	DDX_Text(pDX, IDC_ST_UDP_SEND, m_STUDPSendFlow);
}

BOOL CRemoteConsoleView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改 CREATESTRUCT cs 来修改窗口类或
	// 样式

	return CFormView::PreCreateWindow(cs);
}

void CRemoteConsoleView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	ResizeParentToFit();

	m_ItemSorter.SetParent(this);

	m_ItemSorter.SetSortType(0,CDialogItemSorter::SORT_HORIZONTAL|CDialogItemSorter::PARAM_ALIGN_SPACE,5);

	m_ItemSorter.AddDialogItem(0,1,
		CDialogItemSorter::SORT_VERTICAL|
		CDialogItemSorter::RESIZE_FIT_PARENT_WIDTH|
		CDialogItemSorter::RESIZE_FIT_PARENT_HEIGHT|
		CDialogItemSorter::PARAM_ALIGN_SPACE,
		5,0,CRect(0,0,0,0));
	m_ItemSorter.AddDialogItem(0,2,
		CDialogItemSorter::SORT_VERTICAL|		
		CDialogItemSorter::RESIZE_FIT_PARENT_HEIGHT|
		CDialogItemSorter::ALIGN_TOP|
		CDialogItemSorter::PARAM_ALIGN_SPACE,
		5,0,CRect(0,0,200,0));


	m_ItemSorter.AddDialogItem(1,11,
		CDialogItemSorter::SORT_HORIZONTAL|		
		CDialogItemSorter::RESIZE_FIT_PARENT_WIDTH|
		CDialogItemSorter::RESIZE_FIT_PARENT_HEIGHT,
		0,IDC_EDIT_LOG,CRect(0,0,0,00));

	m_ItemSorter.AddDialogItem(1,12,
		CDialogItemSorter::SORT_HORIZONTAL|		
		CDialogItemSorter::RESIZE_FIT_PARENT_WIDTH,		
		0,0,CRect(0,0,0,30));

	m_ItemSorter.AddDialogItem(12,121,
		CDialogItemSorter::SORT_HORIZONTAL|		
		CDialogItemSorter::RESIZE_FIT_PARENT_WIDTH|		
		CDialogItemSorter::ALIGN_LEFT,
		0,IDC_EDIT_COMMAND,CRect(0,0,0,0));

	m_ItemSorter.AddDialogItem(12,122,
		CDialogItemSorter::SORT_HORIZONTAL|				
		CDialogItemSorter::ALIGN_CENTER,
		0,IDC_BUTTON_SEND_COMMAND,CRect(0,0,0,0));



	m_ItemSorter.AddDialogItem(2,21,
		CDialogItemSorter::SORT_HORIZONTAL|		
		CDialogItemSorter::RESIZE_FIT_PARENT_WIDTH|		
		CDialogItemSorter::PARAM_ALIGN_SPACE,
		5,0,CRect(0,0,0,20));
	m_ItemSorter.AddDialogItem(2,22,
		CDialogItemSorter::SORT_HORIZONTAL|		
		CDialogItemSorter::RESIZE_FIT_PARENT_WIDTH|
		CDialogItemSorter::PARAM_ALIGN_SPACE,
		5,0,CRect(0,0,0,20));
	m_ItemSorter.AddDialogItem(2,23,
		CDialogItemSorter::SORT_HORIZONTAL|		
		CDialogItemSorter::RESIZE_FIT_PARENT_WIDTH|
		CDialogItemSorter::PARAM_ALIGN_SPACE,
		5,0,CRect(0,0,0,20));
	m_ItemSorter.AddDialogItem(2,24,
		CDialogItemSorter::SORT_HORIZONTAL|		
		CDialogItemSorter::RESIZE_FIT_PARENT_WIDTH|
		CDialogItemSorter::PARAM_ALIGN_SPACE,
		5,0,CRect(0,0,0,20));
	m_ItemSorter.AddDialogItem(2,25,
		CDialogItemSorter::SORT_HORIZONTAL|		
		CDialogItemSorter::RESIZE_FIT_PARENT_WIDTH|
		CDialogItemSorter::PARAM_ALIGN_SPACE,
		5,0,CRect(0,0,0,20));
	m_ItemSorter.AddDialogItem(2,26,
		CDialogItemSorter::SORT_HORIZONTAL|		
		CDialogItemSorter::RESIZE_FIT_PARENT_WIDTH|
		CDialogItemSorter::PARAM_ALIGN_SPACE,
		5,0,CRect(0,0,0,20));

	m_ItemSorter.AddDialogItem(21,211,
		CDialogItemSorter::SORT_HORIZONTAL|		
		CDialogItemSorter::RESIZE_FIT_PARENT_WIDTH|
		CDialogItemSorter::RESIZE_FIT_PARENT_HEIGHT|
		CDialogItemSorter::ALIGN_LEFT,
		0,0,CRect(0,0,0,0));
	m_ItemSorter.AddDialogItem(21,212,
		CDialogItemSorter::SORT_HORIZONTAL|		
		CDialogItemSorter::RESIZE_FIT_PARENT_WIDTH|
		CDialogItemSorter::RESIZE_FIT_PARENT_HEIGHT|
		CDialogItemSorter::ALIGN_LEFT,
		0,0,CRect(0,0,0,0));

	m_ItemSorter.AddDialogItem(211,2111,
		0,
		0,IDC_ST_CLIENT_COUNT_TITLE,CRect(0,0,0,0));

	m_ItemSorter.AddDialogItem(212,2121,
		0,
		0,IDC_ST_CLIENT_COUNT,CRect(0,0,0,0));

	m_ItemSorter.AddDialogItem(22,221,
		CDialogItemSorter::SORT_HORIZONTAL|		
		CDialogItemSorter::RESIZE_FIT_PARENT_WIDTH|
		CDialogItemSorter::RESIZE_FIT_PARENT_HEIGHT|
		CDialogItemSorter::ALIGN_LEFT,
		0,0,CRect(0,0,0,0));
	m_ItemSorter.AddDialogItem(22,222,
		CDialogItemSorter::SORT_HORIZONTAL|		
		CDialogItemSorter::RESIZE_FIT_PARENT_WIDTH|
		CDialogItemSorter::RESIZE_FIT_PARENT_HEIGHT|
		CDialogItemSorter::ALIGN_LEFT,
		0,0,CRect(0,0,0,0));

	m_ItemSorter.AddDialogItem(221,2211,
		0,
		0,IDC_ST_CYCLE_TIME_TITLE,CRect(0,0,0,0));

	m_ItemSorter.AddDialogItem(222,2221,
		0,
		0,IDC_ST_CYCLE_TIME,CRect(0,0,0,0));

	m_ItemSorter.AddDialogItem(23,231,
		CDialogItemSorter::SORT_HORIZONTAL|		
		CDialogItemSorter::RESIZE_FIT_PARENT_WIDTH|
		CDialogItemSorter::RESIZE_FIT_PARENT_HEIGHT|
		CDialogItemSorter::ALIGN_LEFT,
		0,0,CRect(0,0,0,0));
	m_ItemSorter.AddDialogItem(23,232,
		CDialogItemSorter::SORT_HORIZONTAL|		
		CDialogItemSorter::RESIZE_FIT_PARENT_WIDTH|
		CDialogItemSorter::RESIZE_FIT_PARENT_HEIGHT|
		CDialogItemSorter::ALIGN_LEFT,
		0,0,CRect(0,0,0,0));

	m_ItemSorter.AddDialogItem(231,2311,
		0,
		0,IDC_ST_TCP_RECV_TITLE,CRect(0,0,0,0));

	m_ItemSorter.AddDialogItem(232,2321,
		0,
		0,IDC_ST_TCP_RECV,CRect(0,0,0,0));

	m_ItemSorter.AddDialogItem(24,241,
		CDialogItemSorter::SORT_HORIZONTAL|		
		CDialogItemSorter::RESIZE_FIT_PARENT_WIDTH|
		CDialogItemSorter::RESIZE_FIT_PARENT_HEIGHT|
		CDialogItemSorter::ALIGN_LEFT,
		0,0,CRect(0,0,0,0));
	m_ItemSorter.AddDialogItem(24,242,
		CDialogItemSorter::SORT_HORIZONTAL|		
		CDialogItemSorter::RESIZE_FIT_PARENT_WIDTH|
		CDialogItemSorter::RESIZE_FIT_PARENT_HEIGHT|
		CDialogItemSorter::ALIGN_LEFT,
		0,0,CRect(0,0,0,0));

	m_ItemSorter.AddDialogItem(241,2411,
		0,
		0,IDC_ST_TCP_SEND_TITLE,CRect(0,0,0,0));

	m_ItemSorter.AddDialogItem(242,2421,
		0,
		0,IDC_ST_TCP_SEND,CRect(0,0,0,0));

	m_ItemSorter.AddDialogItem(25,251,
		CDialogItemSorter::SORT_HORIZONTAL|		
		CDialogItemSorter::RESIZE_FIT_PARENT_WIDTH|
		CDialogItemSorter::RESIZE_FIT_PARENT_HEIGHT|
		CDialogItemSorter::ALIGN_LEFT,
		0,0,CRect(0,0,0,0));
	m_ItemSorter.AddDialogItem(25,252,
		CDialogItemSorter::SORT_HORIZONTAL|		
		CDialogItemSorter::RESIZE_FIT_PARENT_WIDTH|
		CDialogItemSorter::RESIZE_FIT_PARENT_HEIGHT|
		CDialogItemSorter::ALIGN_LEFT,
		0,0,CRect(0,0,0,0));

	m_ItemSorter.AddDialogItem(251,2511,
		0,
		0,IDC_ST_UDP_RECV_TITLE,CRect(0,0,0,0));

	m_ItemSorter.AddDialogItem(252,2521,
		0,
		0,IDC_ST_UDP_RECV,CRect(0,0,0,0));

	m_ItemSorter.AddDialogItem(26,261,
		CDialogItemSorter::SORT_HORIZONTAL|		
		CDialogItemSorter::RESIZE_FIT_PARENT_WIDTH|
		CDialogItemSorter::RESIZE_FIT_PARENT_HEIGHT|
		CDialogItemSorter::ALIGN_LEFT,
		0,0,CRect(0,0,0,0));
	m_ItemSorter.AddDialogItem(26,262,
		CDialogItemSorter::SORT_HORIZONTAL|		
		CDialogItemSorter::RESIZE_FIT_PARENT_WIDTH|
		CDialogItemSorter::RESIZE_FIT_PARENT_HEIGHT|
		CDialogItemSorter::ALIGN_LEFT,
		0,0,CRect(0,0,0,0));

	m_ItemSorter.AddDialogItem(261,2611,
		0,
		0,IDC_ST_UDP_SEND_TITLE,CRect(0,0,0,0));

	m_ItemSorter.AddDialogItem(262,2621,
		0,
		0,IDC_ST_UDP_SEND,CRect(0,0,0,0));

	m_ItemSorter.DoSort();

	

	SetTimer(SERVER_STATUS_QUERY_TIMER_ID,SERVER_STATUS_QUERY_TIME,NULL);

	SetScrollSizes(MM_TEXT,CSize(0,0));

	
	
}


// CRemoteConsoleView 诊断

#ifdef _DEBUG
void CRemoteConsoleView::AssertValid() const
{
	CFormView::AssertValid();
}

void CRemoteConsoleView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CRemoteConsoleDoc* CRemoteConsoleView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRemoteConsoleDoc)));
	return (CRemoteConsoleDoc*)m_pDocument;
}
#endif //_DEBUG


// CRemoteConsoleView 消息处理程序

void CRemoteConsoleView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	m_ItemSorter.DoSort();
}

void CRemoteConsoleView::OnBnClickedButtonSendCommand()
{
	// TODO: 在此添加控件通知处理程序代码
	CString Command;
	m_edCommand.GetWindowText(Command);
	m_edCommand.SetWindowText("");
	if(m_pNetLink&&(!Command.IsEmpty()))
	{
		CSmartStruct CommandInfo(1024);
		CommandInfo.AddMember(SC_SST_EC_COMMAND_STR,(LPCTSTR)Command);

		m_pNetLink->SendMsg(SC_MSG_EXEC_COMMAND,
			CommandInfo.GetData(),CommandInfo.GetDataLen());
	}
}

void CRemoteConsoleView::OnEditCut()
{
	// TODO: 在此添加命令处理程序代码
	if(GetFocus()==&m_edCommand)
		m_edCommand.Cut();
	else
		m_edLog.Cut();
}

void CRemoteConsoleView::OnEditCopy()
{
	// TODO: 在此添加命令处理程序代码
	if(GetFocus()==&m_edCommand)
		m_edCommand.Copy();
	else
		m_edLog.Copy();
}

void CRemoteConsoleView::OnEditPaste()
{
	// TODO: 在此添加命令处理程序代码
	if(GetFocus()==&m_edCommand)
		m_edCommand.Paste();
	else
		m_edLog.Paste();
}

void CRemoteConsoleView::OnEditUndo()
{
	// TODO: 在此添加命令处理程序代码
	if(GetFocus()==&m_edCommand)
		m_edCommand.Undo();
	else
		m_edLog.Undo();
}


void CRemoteConsoleView::PrintLogMsg(LPCTSTR szMsg)
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

	m_pFileLog->OutputLogString(ILogPrinter::LOG_LEVEL_NORMAL,0,szMsg);

}



void CRemoteConsoleView::OnTimer(UINT nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(nIDEvent==SERVER_STATUS_QUERY_TIMER_ID)
	{
		if(m_pNetLink)
		{
			CSmartStruct QueryInfo(1024);


			QueryInfo.AddMember(SC_SST_QSS_STATUS_ID,SC_SST_SS_CLIENT_COUNT);
			QueryInfo.AddMember(SC_SST_QSS_STATUS_ID,SC_SST_SS_CYCLE_TIME);
			QueryInfo.AddMember(SC_SST_QSS_STATUS_ID,SC_SST_SS_TCP_RECV_FLOW);
			QueryInfo.AddMember(SC_SST_QSS_STATUS_ID,SC_SST_SS_TCP_SEND_FLOW);
			QueryInfo.AddMember(SC_SST_QSS_STATUS_ID,SC_SST_SS_UDP_RECV_FLOW);
			QueryInfo.AddMember(SC_SST_QSS_STATUS_ID,SC_SST_SS_UDP_SEND_FLOW);			
			
			m_pNetLink->SendMsg(SC_MSG_QUERY_SERVER_STATUS,
				QueryInfo.GetData(),QueryInfo.GetDataLen());
		}
	}
	CFormView::OnTimer(nIDEvent);
}


void CRemoteConsoleView::SetServerStatus(CSmartStruct& ServerStatus)
{
	m_STClientCount=ServerStatus.GetMember(SC_SST_SS_CLIENT_COUNT);
	m_STCycleTime=ServerStatus.GetMember(SC_SST_SS_CYCLE_TIME);
	m_STTCPRecvFlow=ServerStatus.GetMember(SC_SST_SS_TCP_RECV_FLOW);
	m_STTCPSendFlow=ServerStatus.GetMember(SC_SST_SS_TCP_SEND_FLOW);
	m_STUDPRecvFlow=ServerStatus.GetMember(SC_SST_SS_UDP_RECV_FLOW);
	m_STUDPSendFlow=ServerStatus.GetMember(SC_SST_SS_UDP_SEND_FLOW);
	UpdateData(false);
}