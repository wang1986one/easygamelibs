/****************************************************************************/
/*                                                                          */
/*      文件名:    RemoteConsoleView.cpp                                    */
/*      创建日期:  2009年02月09日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
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
	DDX_Control(pDX, IDC_LIST_SERVER_STATUS, m_lvServerStatus);
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
		5,IDC_LIST_SERVER_STATUS,CRect(0,0,500,0));


	m_ItemSorter.AddDialogItem(1,11,
		CDialogItemSorter::SORT_HORIZONTAL|		
		CDialogItemSorter::RESIZE_FIT_PARENT_WIDTH|
		CDialogItemSorter::RESIZE_FIT_PARENT_HEIGHT,
		0,IDC_EDIT_LOG,CRect(0,0,0,0));

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

	

	m_ItemSorter.DoSort();

	

	SetTimer(SERVER_STATUS_QUERY_TIMER_ID,SERVER_STATUS_QUERY_TIME,NULL);

	SetScrollSizes(MM_TEXT,CSize(0,0));

	m_lvServerStatus.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);

	m_lvServerStatus.InsertColumn(0,"项",LVCFMT_LEFT,150);
	m_lvServerStatus.InsertColumn(1,"值",LVCFMT_LEFT,100);
	
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

	PrintFileLog("%s",szMsg);
}

void CRemoteConsoleView::PrintFileLog(LPCTSTR szFormat,...)
{
	va_list	vl;
	va_start(vl,szFormat);
	m_pFileLog->PrintLogVL(ILogPrinter::LOG_LEVEL_NORMAL,0,szFormat,vl);
	va_end( vl);
}

void CRemoteConsoleView::OnTimer(UINT nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(nIDEvent==SERVER_STATUS_QUERY_TIMER_ID)
	{
		if(m_pNetLink)
		{
			if(m_ServerStatusFormats.GetObjectCount()==0)
			{
				m_pNetLink->SendMsg(SC_MSG_GET_SERVER_STATUS_FORMAT_INFO);
			}
			m_pNetLink->SendMsg(SC_MSG_QUERY_ALL_SERVER_STATUS);
		}
	}
	CFormView::OnTimer(nIDEvent);
}


void CRemoteConsoleView::SetServerStatus(CSmartStruct& ServerStatus)
{
	m_lvServerStatus.DeleteAllItems();
	LPVOID Pos=ServerStatus.GetFirstMemberPosition();
	while(Pos)
	{
		WORD MemberID;
		CSmartValue Value=ServerStatus.GetNextMember(Pos,MemberID);
		CEasyString MemberIDStr("未知属性");
		int FormatType=SSFT_DEFAULT;
		SERVER_STATUS_FORMAT_INFO * pFormatInfo=m_ServerStatusFormats.Find(MemberID);
		if(pFormatInfo)
		{
			MemberIDStr=pFormatInfo->szName;
			FormatType=pFormatInfo->FormatType;
		}
		CEasyString ValueStr;
		switch(Value.GetType())
		{
		case CSmartValue::VT_CHAR:
			ValueStr.Format("%d",(char)Value);
			break;
		case CSmartValue::VT_UCHAR:
			ValueStr.Format("%u",(BYTE)Value);
			break;
		case CSmartValue::VT_SHORT:
			if(FormatType==SSFT_FLOW)
				ValueStr=FormatNumberWords((short)Value,true);
			else
				ValueStr.Format("%d",(short)Value);
			break;
		case CSmartValue::VT_USHORT:
			if(FormatType==SSFT_FLOW)
				ValueStr=FormatNumberWords((WORD)Value,true);
			else
				ValueStr.Format("%u",(WORD)Value);
			break;
		case CSmartValue::VT_INT:
			if(FormatType==SSFT_FLOW)
				ValueStr=FormatNumberWords((int)Value,true);
			else
				ValueStr.Format("%d",(int)Value);
			break;
		case CSmartValue::VT_UINT:
			if(FormatType==SSFT_FLOW)
				ValueStr=FormatNumberWords((UINT)Value,true);
			else
				ValueStr.Format("%u",(UINT)Value);
			break;
		case CSmartValue::VT_BIGINT:
			if(FormatType==SSFT_FLOW)
				ValueStr=FormatNumberWords((INT64)Value,true);
			else
				ValueStr.Format("%lld",(INT64)Value);
			break;
		case CSmartValue::VT_UBIGINT:
			if(FormatType==SSFT_FLOW)
				ValueStr=FormatNumberWords((UINT64)Value,true);
			else
				ValueStr.Format("%llu",(UINT64)Value);
			break;
		case CSmartValue::VT_FLOAT:
			if(FormatType==SSFT_FLOW)
				ValueStr=FormatNumberWordsFloat((float)Value,true);
			else
				ValueStr.Format("%g",(float)Value);
			break;
		case CSmartValue::VT_DOUBLE:
			if(FormatType==SSFT_FLOW)
				ValueStr=FormatNumberWordsFloat((double)Value,true);
			else
				ValueStr.Format("%g",(double)Value);
			break;
		case CSmartValue::VT_STRING:
			ValueStr=(LPCTSTR)Value;
			break;
		case CSmartValue::VT_USTRING:
			ValueStr=(LPCWSTR)Value;
			break;
		default:
			ValueStr="未知格式数据";
		}
		int Item=m_lvServerStatus.InsertItem(m_lvServerStatus.GetItemCount(),MemberIDStr);
		m_lvServerStatus.SetItemText(Item,1,ValueStr);
	}
	
}

void CRemoteConsoleView::SetServerStatusFormats(CSmartStruct& ServerStatusFormats)
{
	m_ServerStatusFormats.Clear();
	void * Pos=ServerStatusFormats.GetFirstMemberPosition();
	while(Pos)
	{
		WORD MemberID;
		CSmartStruct FormatInfo=ServerStatusFormats.GetNextMember(Pos,MemberID);
		if(MemberID==SC_SST_SSFIL_SERVER_STATUS_FORMAT_INFO)
		{
			WORD StatusID=FormatInfo.GetMember(SC_SST_SSFI_STATUS_ID);
			SERVER_STATUS_FORMAT_INFO * pInfo=m_ServerStatusFormats.New(StatusID);
			if(pInfo)
			{
				pInfo->FormatType=FormatInfo.GetMember(SC_SST_SSFI_FORMAT_TYPE);
				strncpy_0(pInfo->szName,MAX_SERVER_STATUS_NAME_LEN,
					FormatInfo.GetMember(SC_SST_SSFI_NAME),MAX_SERVER_STATUS_NAME_LEN);
			}
		}
	}
}	