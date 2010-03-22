// RemoteConsole.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "RemoteConsole.h"
#include "MainFrm.h"

#include "ChildFrm.h"
#include "RemoteConsoleDoc.h"
#include "RemoteConsoleView.h"
#include ".\remoteconsole.h"




// CRemoteConsoleApp

BEGIN_MESSAGE_MAP(CRemoteConsoleApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	// �����ļ��ı�׼�ĵ�����
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	
END_MESSAGE_MAP()


// CRemoteConsoleApp ����

CRemoteConsoleApp::CRemoteConsoleApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CRemoteConsoleApp ����

CRemoteConsoleApp theApp;

// CRemoteConsoleApp ��ʼ��

CRemoteConsoleApp * CRemoteConsoleApp::GetInstance()
{
	return &theApp;
}

BOOL CRemoteConsoleApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControls()�����򣬽��޷��������ڡ�

	CExceptionParser::GetInstance()->Init(EXCEPTION_SET_DEFAULT_HANDLER);

	//char * p=NULL;

	//*p=0;

	InitCommonControls();

	CWinApp::InitInstance();

	// ��ʼ�� OLE ��
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));
	LoadStdProfileSettings(4);  // ���ر�׼ INI �ļ�ѡ��(���� MRU)
	// ע��Ӧ�ó�����ĵ�ģ�塣�ĵ�ģ��
	// �������ĵ�����ܴ��ں���ͼ֮�������
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(IDR_RemoteConsoleTYPE,
		RUNTIME_CLASS(CRemoteConsoleDoc),
		RUNTIME_CLASS(CChildFrame), // �Զ��� MDI �ӿ��
		RUNTIME_CLASS(CRemoteConsoleView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	
	pDocTemplate= new CMultiDocTemplate(IDR_SYSTEM_CONSOLE,
		NULL,
		RUNTIME_CLASS(CSystemConsoleFrame), // �Զ��� MDI �ӿ��
		RUNTIME_CLASS(CConsoleView));
	AddDocTemplate(pDocTemplate);

	// ������ MDI ��ܴ���
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;
	// �������к�׺ʱ�ŵ��� DragAcceptFiles
	//  �� MDI Ӧ�ó����У���Ӧ������ m_pMainWnd ֮����������
	// ������׼������DDE�����ļ�������������

	m_Server.StartUp();
	m_NetLinkManager.Init(&m_Server);

	
	

	g_ServiceWorkMode=SWM_APPLICATION;

	CConsoleLogPrinter * pLog=new CConsoleLogPrinter();

	CLogManager::GetInstance()->AddChannel(SERVER_LOG_CHANNEL,pLog);
	CLogManager::GetInstance()->AddChannel(LOG_NET_CHANNEL,pLog);	
	SAFE_RELEASE(pLog);

	//CCommandLineInfo cmdInfo;
	//ParseCommandLine(cmdInfo);
	//// ��������������ָ����������
	//// �� /RegServer��/Register��/Unregserver �� /Unregister ����Ӧ�ó����򷵻� FALSE��
	//if (!ProcessShellCommand(cmdInfo))
	//	return FALSE;

	// �������ѳ�ʼ���������ʾ����������и���
	pMainFrame->ShowWindow(SW_SHOWMAXIMIZED);
	pMainFrame->UpdateWindow();


	POSITION Pos=CRemoteConsoleApp::GetInstance()->GetFirstDocTemplatePosition();
	CDocTemplate* pTemplate=CRemoteConsoleApp::GetInstance()->GetNextDocTemplate(Pos);
	pTemplate=CRemoteConsoleApp::GetInstance()->GetNextDocTemplate(Pos);
	if(pTemplate)
	{
		CFrameWnd * pFrame=pTemplate->CreateNewFrame(NULL,NULL);
		pFrame->SetTitle("ϵͳ��Ϣ");
		pTemplate->InitialUpdateFrame(pFrame,NULL);
	}
	

	
	return TRUE;
}



// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// �������жԻ����Ӧ�ó�������
void CRemoteConsoleApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// CRemoteConsoleApp ��Ϣ�������


BOOL CRemoteConsoleApp::OnIdle(LONG lCount)
{
	// TODO: �ڴ����ר�ô����/����û���
	m_NetLinkManager.Update();
	return TRUE;
	//return CWinApp::OnIdle(lCount);
}

int CRemoteConsoleApp::ExitInstance()
{
	// TODO: �ڴ����ר�ô����/����û���
	m_NetLinkManager.Destory();
	m_Server.ShutDown();
	return CWinApp::ExitInstance();
}


void CRemoteConsoleApp::OnFileNew()
{
	CDlgConnect Dlg;

	if(Dlg.DoModal()==IDOK)
	{
		CIPAddress Address(Dlg.m_Address,Dlg.m_Port);

		m_NetLinkManager.AddConnection(1,Address);
	}
}
void CRemoteConsoleApp::OnFileOpen()
{
}