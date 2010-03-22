// ServerManagerClient.cpp : 定义应用程序的类行为。
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


// CServerManagerClientApp 构造

CServerManagerClientApp::CServerManagerClientApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CServerManagerClientApp 对象

CServerManagerClientApp theApp;


CServerManagerClientApp * CServerManagerClientApp::GetInstance()
{
	return &theApp;
}


// CServerManagerClientApp 初始化

BOOL CServerManagerClientApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	// 初始化 OLE 库
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));
	LoadStdProfileSettings(4);  // 加载标准 INI 文件选项(包括 MRU)
	// 注册应用程序的文档模板。文档模板
	// 将用作文档、框架窗口和视图之间的连接
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CServerManagerClientDoc),
		RUNTIME_CLASS(CMainFrame),       // 主 SDI 框架窗口
		RUNTIME_CLASS(CServerManagerClientView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	LoadConfig();

	m_Server.StartUp();

	// 分析标准外壳命令、DDE、打开文件操作的命令行
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);


	// 调度在命令行中指定的命令。如果
	// 用 /RegServer、/Register、/Unregserver 或 /Unregister 启动应用程序，则返回 FALSE。
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// 唯一的一个窗口已初始化，因此显示它并对其进行更新
	m_pMainWnd->ShowWindow(SW_SHOWMAXIMIZED);
	m_pMainWnd->UpdateWindow();
	// 仅当具有后缀时才调用 DragAcceptFiles
	//  在 SDI 应用程序中，这应在 ProcessShellCommand 之后发生

	CheckServerConnections();
	return TRUE;
}


int CServerManagerClientApp::ExitInstance()
{
	// TODO: 在此添加专用代码和/或调用基类
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
	// TODO: 在此添加专用代码和/或调用基类

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
	// TODO: 在此添加命令处理程序代码
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
		AfxGetMainWnd()->MessageBox("装载配置失败！");
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
		AfxGetMainWnd()->MessageBox("保存配置失败！");
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

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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

// 用于运行对话框的应用程序命令
void CServerManagerClientApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// CServerManagerClientApp 消息处理程序

