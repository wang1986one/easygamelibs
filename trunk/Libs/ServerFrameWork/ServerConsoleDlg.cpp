/****************************************************************************/
/*                                                                          */
/*      �ļ���:    ServerConsoleDlg.cpp                                     */
/*      ��������:  2010��02��09��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
// SuperStarServerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ServerConsoleDlg.h"


//#ifdef _DEBUG
//#define new DEBUG_NEW
//#endif


#define PANEL_TIMER_ID		87832
#define LOG_MSG_FETCH_TIMER	84759

#define LOG_MSG_FETCH_TIME	200

#define LOG_MSG_FETCH_COUNT	50

// CServerConsoleDlg �Ի���



CServerConsoleDlg::CServerConsoleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CServerConsoleDlg::IDD, pParent)
	, m_CycleTime(_T("0"))
	, m_TCPRecv(_T(""))
	, m_TCPSend(_T(""))
	, m_UDPRecv(_T(""))
	, m_UCPSend(_T(""))
{	
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON_TRAY);
}

CServerConsoleDlg::~CServerConsoleDlg()
{
	UnregisterTrayIcon();
}

void CServerConsoleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_LOG, m_edMsgWnd);
	DDX_Text(pDX, IDC_ST_CYCLE_TIME, m_CycleTime);
	DDX_Text(pDX, IDC_ST_TCP_RECV, m_TCPRecv);
	DDX_Text(pDX, IDC_ST_TCP_SEND, m_TCPSend);
	DDX_Text(pDX, IDC_ST_UDP_RECV, m_UDPRecv);
	DDX_Text(pDX, IDC_ST_UDP_SEND, m_UCPSend);
	DDX_Text(pDX, IDC_EDIT_COMMAND, m_Command);
}

BEGIN_MESSAGE_MAP(CServerConsoleDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_TRAY_ICON_NOTIFY, OnTryIconNotify)
	//}}AFX_MSG_MA
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_EXEC_COMMAND, OnBnClickedExecCommand)
	ON_BN_CLICKED(IDC_CLOSE_SERVER, OnBnClickedCloseServer)
	ON_BN_CLICKED(IDC_SHOW_SERVER_STATUS, &CServerConsoleDlg::OnBnClickedShowServerStatus)
END_MESSAGE_MAP()


// CServerConsoleDlg ��Ϣ�������

BOOL CServerConsoleDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��\������...\���˵�����ӵ�ϵͳ�˵��С�


	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	//��ʼ���汾��Ϣ
	ShowVersion();
	
	m_ServerStatus.Create(SERVER_STATUS_BLOCK_SIZE);
	m_DlgServerStatus.Create(m_DlgServerStatus.IDD,this);

	CControlPanel::GetInstance()->SetHwnd(GetSafeHwnd());
	

	SetTimer(PANEL_TIMER_ID,SERVER_INFO_COUNT_TIME,NULL);
	SetTimer(LOG_MSG_FETCH_TIMER,LOG_MSG_FETCH_TIME,NULL);
	
	return TRUE;  // ���������˿ؼ��Ľ��㣬���򷵻� TRUE
}

void CServerConsoleDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	CDialog::OnSysCommand(nID, lParam);
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CServerConsoleDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
HCURSOR CServerConsoleDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CServerConsoleDlg::OnOK()
{
}

void CServerConsoleDlg::OnCancel()
{
	//int nRetCode = MessageBox("����Ҫ�ر�?", "����", MB_YESNO);
	//if(nRetCode == IDYES)
	{		
		CDialog::OnCancel();
	}	
}


void CServerConsoleDlg::ShowVersion()
{
	CString Caption;
	Caption.Format("[%s]�汾:%u.%u.%u.%u  ����ʱ��:%s",
		(LPCTSTR)g_ProgramName,
		g_ProgramVersion[3],
		g_ProgramVersion[2],
		g_ProgramVersion[1],
		g_ProgramVersion[0],
		(LPCTSTR)(CTime::GetCurrentTime().Format("%m-%d %H:%M")));

	SetWindowText(Caption);	
}

int CServerConsoleDlg::FetchConsoleMsg(int ProcessLimit)
{
	int ProcessCount=0;
	PANEL_MSG * pMsg=CControlPanel::GetInstance()->GetMsg();
	while(pMsg)
	{
		if(IsWindowVisible())
			OnConsoleMsg(pMsg);

		CControlPanel::GetInstance()->ReleaseMsg(pMsg->ID);

		ProcessCount++;
		if(ProcessCount>ProcessLimit)
			break;
		pMsg=CControlPanel::GetInstance()->GetMsg();
	}

	return ProcessCount;
}

void CServerConsoleDlg::OnConsoleMsg(PANEL_MSG * pMsg)
{
	switch(pMsg->MsgType)
	{
	case CON_MSG_LOG:
		OnLogMsg(pMsg->Msg);
		break;
	}
}

void CServerConsoleDlg::OnLogMsg(LPCTSTR szLogMsg)
{

	int s1,s2;
	int sm1,sm2;
	int SelLine;
	int EndLine;

	m_edMsgWnd.SetRedraw(false);

	m_edMsgWnd.GetSel(sm1,sm2);
	m_edMsgWnd.SetSel(0,-1);
	m_edMsgWnd.GetSel(s1,s2);
	SelLine=m_edMsgWnd.LineFromChar(sm1);
	EndLine=m_edMsgWnd.GetLineCount()-1;
	if(s2>MAX_CONTROL_PANEL_MSG_LEN)
	{
		m_edMsgWnd.SetSel(0,-1);
		m_edMsgWnd.Clear();
		s2=0;
	}
	m_edMsgWnd.SetSel(s2,s2);
	//m_edMsgWnd.ReplaceSel(CTime::GetCurrentTime().Format("%H:%M:%S :"));


	//m_edMsgWnd.SetSel(0,-1);
	//m_edMsgWnd.GetSel(s1,s2);
	//m_edMsgWnd.SetSel(s2,s2);
	m_edMsgWnd.ReplaceSel(szLogMsg);


	m_edMsgWnd.SetSel(0,-1);
	m_edMsgWnd.GetSel(s1,s2);
	m_edMsgWnd.SetSel(s2,s2);	

	m_edMsgWnd.ReplaceSel("\r\n");



	m_edMsgWnd.SetRedraw(true);	


	if(SelLine==EndLine)
		m_edMsgWnd.LineScroll(m_edMsgWnd.GetLineCount());
	else
		m_edMsgWnd.SetSel(sm1,sm2);
}

void CServerConsoleDlg::OnTimer(UINT nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if(nIDEvent==PANEL_TIMER_ID&&IsWindowVisible())
	{
		UINT Size=CControlPanel::GetInstance()->GetServerStatus((LPVOID)m_ServerStatus.GetBuffer(),m_ServerStatus.GetBufferSize());
		CSmartStruct StatusPacket(m_ServerStatus.GetBuffer(),Size,false);
		UpdateData(true);		
		static char buff[128];
		sprintf_s(buff,128,"%.2f",
			(float)StatusPacket.GetMember(SC_SST_SS_CYCLE_TIME));
		m_CycleTime=buff;
		m_TCPRecv=FormatNumberWordsFloat(StatusPacket.GetMember(SC_SST_SS_TCP_RECV_FLOW),true);
		m_TCPSend=FormatNumberWordsFloat(StatusPacket.GetMember(SC_SST_SS_TCP_SEND_FLOW),true);
		m_UDPRecv=FormatNumberWordsFloat(StatusPacket.GetMember(SC_SST_SS_UDP_RECV_FLOW),true);
		m_UCPSend=FormatNumberWordsFloat(StatusPacket.GetMember(SC_SST_SS_UDP_SEND_FLOW),true);

		UpdateData(false);

		m_DlgServerStatus.FlushStatus(StatusPacket);
	}
	if(nIDEvent==LOG_MSG_FETCH_TIMER)
	{
		FetchConsoleMsg(LOG_MSG_FETCH_COUNT);
	}

	CDialog::OnTimer(nIDEvent);
}

void CServerConsoleDlg::OnBnClickedExecCommand()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	m_Command.Trim();
	if(!m_Command.IsEmpty())
		OnCommand(m_Command);
	m_Command.Empty();
	UpdateData(false);
}

void CServerConsoleDlg::OnCommand(LPCTSTR szCommand)
{
	CControlPanel::GetInstance()->PushCommand(szCommand);
}

void CServerConsoleDlg::RegisterTrayIcon(LPCTSTR szTip)
{

	NOTIFYICONDATA Nid;

	Nid.cbSize = sizeof(NOTIFYICONDATA);
	Nid.hWnd = GetSafeHwnd();
	Nid.uID = 0;
	Nid.hIcon = m_hIcon;

	Nid.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE;
	Nid.uCallbackMessage = WM_TRAY_ICON_NOTIFY;

	strcpy_s(Nid.szTip, sizeof(Nid.szTip),szTip);
	Shell_NotifyIcon(NIM_ADD, &Nid);

}

void CServerConsoleDlg::UnregisterTrayIcon()
{

	NOTIFYICONDATA Nid;

	Nid.cbSize = sizeof(NOTIFYICONDATA);
	Nid.hWnd = GetSafeHwnd();
	Nid.uID = 0;
	Nid.hIcon = m_hIcon;

	Nid.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE;
	Nid.uCallbackMessage = WM_TRAY_ICON_NOTIFY;

	Shell_NotifyIcon(NIM_DELETE, &Nid);

}

LRESULT CServerConsoleDlg::OnTryIconNotify(WPARAM wParam, LPARAM lParam)
{
	

	switch(lParam)
	{
	case WM_LBUTTONDBLCLK:
		ShowWindow(SW_SHOW);
		break;
	}

	

	return 0;


}
void CServerConsoleDlg::OnBnClickedCloseServer()
{
	

	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(MessageBox("�Ƿ�Ҫ�رշ�����","��ʾ",MB_YESNO)==IDYES)
	{		
		PostMessage(WM_QUIT);
	}
	
}

void CServerConsoleDlg::OnBnClickedShowServerStatus()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_DlgServerStatus.ShowWindow(SW_SHOW);
}
