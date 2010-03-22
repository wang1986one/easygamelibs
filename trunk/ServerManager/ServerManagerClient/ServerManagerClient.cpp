// ServerManagerClient.cpp : ����Ӧ�ó��������Ϊ��
//
#include "stdafx.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CServerManagerClientApp

BEGIN_MESSAGE_MAP(CServerManagerClientApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CServerManagerClientApp::OnAppAbout)	
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
	ON_COMMAND(ID_SERVER_MANAGE, &CServerManagerClientApp::OnServerManage)
END_MESSAGE_MAP()


// CServerManagerClientApp ����

CServerManagerClientApp::CServerManagerClientApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CServerManagerClientApp ����

CServerManagerClientApp theApp;


CServerManagerClientApp * CServerManagerClientApp::GetInstance()
{
	return &theApp;
}


// CServerManagerClientApp ��ʼ��

BOOL CServerManagerClientApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

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
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CServerManagerClientDoc),
		RUNTIME_CLASS(CMainFrame),       // �� SDI ��ܴ���
		RUNTIME_CLASS(CServerManagerClientView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	LoadConfig();

	m_Server.StartUp();

	// ������׼������DDE�����ļ�������������
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);


	// ��������������ָ����������
	// �� /RegServer��/Register��/Unregserver �� /Unregister ����Ӧ�ó����򷵻� FALSE��
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// Ψһ��һ�������ѳ�ʼ���������ʾ����������и���
	m_pMainWnd->ShowWindow(SW_SHOWMAXIMIZED);
	m_pMainWnd->UpdateWindow();
	// �������к�׺ʱ�ŵ��� DragAcceptFiles
	//  �� SDI Ӧ�ó����У���Ӧ�� ProcessShellCommand ֮����

	CheckServerConnections();
	return TRUE;
}


int CServerManagerClientApp::ExitInstance()
{
	// TODO: �ڴ����ר�ô����/����û���
	for(size_t i=0;i<m_ConnectionList.GetCount();i++)
	{
		SAFE_RELEASE(m_ConnectionList[i]);
	}
	m_ConnectionList.Clear();

	m_Server.ShutDown();

	CLogManager::GetInstance()->ReleaseInstance();

	return CWinApp::ExitInstance();
}

BOOL CServerManagerClientApp::OnIdle(LONG lCount)
{
	// TODO: �ڴ����ר�ô����/����û���

	CWinApp::OnIdle(lCount);

	int ProcessCount=0;

	if(m_ConnectionList.GetCount())
	{
		for(size_t i=0;i<m_ConnectionList.GetCount();i++)
		{
			ProcessCount+=m_ConnectionList[i]->Update();
		}	
		if(ProcessCount==0)
		{
			Sleep(1);
		}
		return TRUE;		
	}
	return FALSE;
	
}

void CServerManagerClientApp::OnServerManage()
{
	// TODO: �ڴ���������������
	CDlgServerManage Dlg;

	CheckServerConnections();
	Dlg.SetServerList(m_ServerList);
	if(Dlg.DoModal()==IDOK)
	{
		m_ServerList=Dlg.GetServerList();
		SaveConfig();
		CheckServerConnections();
	}
}

void CServerManagerClientApp::CheckServerConnections()
{
	for(UINT i=0;i<m_ServerList.GetCount();i++)
	{
		m_ServerList[i].ServerStatus=SCS_NO_CONNECTION;
		for(UINT j=0;j<m_ConnectionList.GetCount();j++)
		{
			if(m_ServerList[i].ServerAddress==m_ConnectionList[j]->GetServerAddress()&&
				m_ServerList[i].ServerPort==m_ConnectionList[j]->GetServerPort())
			{
				if(m_ConnectionList[j]->IsConnected())
				{
					m_ServerList[i].ServerStatus=SCS_CONNECTING;
				}
				else
				{
					m_ServerList[i].ServerStatus=SCS_DISCONNECTED;
				}
				break;
			}
		}
		if(m_ServerList[i].ServerStatus==SCS_NO_CONNECTION)
		{
			CMainFrame * pMainFrame=(CMainFrame *)AfxGetMainWnd();
			if(pMainFrame)
			{
				CServerManagerClientView * pView=(CServerManagerClientView *)pMainFrame->GetActiveView();
				if(pView)
				{
					CServerConnection * pConnection=new CServerConnection(pView,m_ServerList[i].ServerAddress,m_ServerList[i].ServerPort);
					pConnection->SetServer(&m_Server);
					m_ConnectionList.Add(pConnection);
				}
			}
			
		}
	}

	for(int i=m_ConnectionList.GetCount()-1;i>=0;i--)
	{
		bool WantDelete=true;
		for(UINT j=0;j<m_ServerList.GetCount();j++)
		{
			if(m_ServerList[j].ServerAddress==m_ConnectionList[i]->GetServerAddress()&&
				m_ServerList[j].ServerPort==m_ConnectionList[i]->GetServerPort())
			{
				WantDelete=false;
				break;
			}
		}
		if(WantDelete)
		{
			SAFE_RELEASE(m_ConnectionList[i]);
			m_ConnectionList.Delete(i);
		}
	}
}

void CServerManagerClientApp::LoadConfig()
{
	pug::xml_parser Xml;
	if(Xml.parse_file(MakeModuleFullPath(NULL,SETTING_FILE_NAME),pug::parse_trim_attribute))
	{
		xml_node Setting=Xml.document();
		if(Setting.moveto_child("Setting"))
		{
			xml_node ServerList=Setting;
			if(ServerList.moveto_child("ServerList"))
			{
				m_ServerList.Clear();
				for(UINT i=0;i<ServerList.children();i++)
				{
					xml_node Server=ServerList.child(i);
					SERVER_INFO ServerInfo;
					ServerInfo.ServerAddress=Server.attribute("Address").getvalue().c_str();
					ServerInfo.ServerPort=(long)Server.attribute("Port");
					ServerInfo.ServerStatus=SCS_NO_CONNECTION;
					m_ServerList.Add(ServerInfo);
				}
			}			
		}
	}
	else
	{
		AfxGetMainWnd()->MessageBox("װ������ʧ�ܣ�");
	}
}

void CServerManagerClientApp::SaveConfig()
{
	std::ofstream os;	
	os.open(MakeModuleFullPath(NULL,SETTING_FILE_NAME), std::ios::trunc);	
	if (os.is_open())
	{
		pug::xml_parser Xml;

		Xml.create();
		xml_node Doc;
		Doc = Xml.document();
		xml_node pi = Doc.append_child(node_pi);
		pi.name(_T("xml"));
		pi.attribute(_T("version")) = _T("1.0");
		pi.attribute(_T("encoding")) = _T("gb2312");

		xml_node Setting=Doc.append_child(node_element,"Setting");

		xml_node ServerList=Setting.append_child(node_element,"ServerList");

		for(UINT i=0;i<m_ServerList.GetCount();i++)
		{
			xml_node Server=ServerList.append_child(node_element,"Server");
			Server.append_attribute("Address",(LPCTSTR)m_ServerList[i].ServerAddress);
			Server.append_attribute("Port",(long)m_ServerList[i].ServerPort);
		}		

		os << Xml.document();
		os.close();				
	}
	else
	{
		AfxGetMainWnd()->MessageBox("��������ʧ�ܣ�");
	}
}

CServerConnection * CServerManagerClientApp::GetServerConnection(LPCTSTR ServerAddress)
{
	for(size_t i=0;i<m_ConnectionList.GetCount();i++)
	{
		if(strcmp(m_ConnectionList[i]->GetServerAddress(),ServerAddress)==0)
		{
			return m_ConnectionList[i];
		}
	}	
	return NULL;
}

void CServerManagerClientApp::StartupService(LPCTSTR ServerAddress,UINT ServiceIndex)
{
	for(size_t i=0;i<m_ConnectionList.GetCount();i++)
	{
		if(strcmp(m_ConnectionList[i]->GetServerAddress(),ServerAddress)==0)
		{
			m_ConnectionList[i]->QueryStartupService(ServiceIndex);
		}
	}	
}

void CServerManagerClientApp::ShutdownService(LPCTSTR ServerAddress,UINT ServiceIndex)
{
	for(size_t i=0;i<m_ConnectionList.GetCount();i++)
	{
		if(strcmp(m_ConnectionList[i]->GetServerAddress(),ServerAddress)==0)
		{
			m_ConnectionList[i]->QueryShutDownService(ServiceIndex);
		}
	}	
}

void CServerManagerClientApp::BrowseWorkDir(LPCTSTR ServerAddress,UINT ServiceIndex,LPCTSTR Dir)
{
	for(size_t i=0;i<m_ConnectionList.GetCount();i++)
	{
		if(strcmp(m_ConnectionList[i]->GetServerAddress(),ServerAddress)==0)
		{
			m_ConnectionList[i]->QueryBrowseWorkDir(ServiceIndex,Dir);
		}
	}	
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
void CServerManagerClientApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// CServerManagerClientApp ��Ϣ�������

