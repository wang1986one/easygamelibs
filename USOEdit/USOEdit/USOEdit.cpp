/****************************************************************************/
/*                                                                          */
/*      文件名:    USOEdit.cpp                                              */
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
/*      文件名:    USOEdit.cpp                                              */
/*      创建日期:  2010年02月09日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
// USOEdit.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "USOEdit.h"




// CUSOEditApp

BEGIN_MESSAGE_MAP(CUSOEditApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	// 基于文件的标准文档命令
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)	
	ON_COMMAND(ID_SET_TEXTURE_PATH, OnSetTexturePath)
	ON_COMMAND(ID_SET_FX_PATH, OnSetFxPath)	
	ON_COMMAND(ID_LOAD_BLZ_DBC, &CUSOEditApp::OnLoadBlzDbc)
	ON_COMMAND(ID_SET_MPQ_FILE, &CUSOEditApp::OnSetMpqFile)
END_MESSAGE_MAP()


// CUSOEditApp 构造

CUSOEditApp::CUSOEditApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
	m_CameraSetting.CameraMode=CAMERA_MODE_SCENE;
	m_CameraSetting.CameraProjectMode=CAMERA_PROJECT_MODE_PERSPECTIVE;
	m_CameraSetting.CameraNearFace=1.0f;
	m_CameraSetting.CameraFarFace=2000.0f;
	m_CameraSetting.CameraMoveStep=1.0f;
	m_CameraSetting.CameraRotateStep=0.001f;
	m_ViewportSetting.ViewportWidth=800;
	m_ViewportSetting.ViewportHeight=600;
}


// 唯一的一个 CUSOEditApp 对象

CUSOEditApp theApp;

CUSOEditApp * CUSOEditApp::GetInstance()
{
	return &theApp;
}

// CUSOEditApp 初始化



BOOL CUSOEditApp::InitInstance()
{		
	CExceptionParser::GetInstance()->Init(EXCEPTION_SET_DEFAULT_HANDLER|EXCEPTION_USE_API_HOOK);
	CFileSystemManager::GetInstance()->SetFileObjectCreator(FILE_CHANNEL_NORMAL3,&m_MPQFileSystem);
	CBLZWOWDatabase::SetFileChannel(FILE_CHANNEL_NORMAL3);
	CD3DWOWM2Model::SetFileChannel(FILE_CHANNEL_NORMAL3);
	CD3DTexture::SetFileChannel(FILE_CHANNEL_NORMAL3);
	CD3DWOWWMOModel::SetFileChannel(FILE_CHANNEL_NORMAL3);
	CD3DWOWADTModel::SetFileChannel(FILE_CHANNEL_NORMAL3);
	CBLZWDTFile::SetFileChannel(FILE_CHANNEL_NORMAL3);
	CD3DWOWWDLModel::SetFileChannel(FILE_CHANNEL_NORMAL3);

	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControls()。否则，将无法创建窗口。
	InitCommonControls();

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
	SetRegistryKey(_T("Universal System Object Editor"));
	LoadStdProfileSettings(4);  // 加载标准 INI 文件选项(包括 MRU)
	// 注册应用程序的文档模板。文档模板
	// 将用作文档、框架窗口和视图之间的连接
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(IDR_USOTYPE,
		RUNTIME_CLASS(CUSOEditDoc),
		RUNTIME_CLASS(CChildFrame), // 自定义 MDI 子框架
		NULL);
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);
	// 创建主 MDI 框架窗口
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;
	// 仅当具有后缀时才调用 DragAcceptFiles
	//  在 MDI 应用程序中，这应在设置 m_pMainWnd 之后立即发生
	// 启用拖/放
	m_pMainWnd->DragAcceptFiles();
	// 启用“DDE 执行”
	EnableShellOpen();
	RegisterShellFileTypes(FALSE);
	// 分析标准外壳命令、DDE、打开文件操作的命令行
	
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	// 调度在命令行中指定的命令。如果
	// 用 /RegServer、/Register、/Unregserver 或 /Unregister 启动应用程序，则返回 FALSE。
	if(cmdInfo.m_nShellCommand!=CCommandLineInfo::FileNew)
	{
		if (!ProcessShellCommand(cmdInfo))
			return FALSE;
	}

	LoadSetting();	
	
	// 主窗口已初始化，因此显示它并对其进行更新
	pMainFrame->ShowWindow(SW_SHOWMAXIMIZED);
	pMainFrame->UpdateWindow();

	m_DlgFileViewer.Create(m_DlgFileViewer.IDD,AfxGetMainWnd());

	CVSOutputLogPrinter * pLog=new CVSOutputLogPrinter();

	CLogManager::GetInstance()->AddChannel(LOG_D3D_ERROR_CHANNEL,pLog);

	SAFE_RELEASE(pLog);	
	
	return TRUE;
}

int CUSOEditApp::ExitInstance()
{
	SaveSetting();
	CLogManager::ReleaseInstance();
	CPerformanceStatistician::ReleaseInstance();
	return CWinApp::ExitInstance();
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
void CUSOEditApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// CUSOEditApp 消息处理程序


BOOL CUSOEditApp::OnIdle(LONG lCount)
{
	// TODO: 在此添加专用代码和/或调用基类	
	if(m_D3DViewPortList.GetCount())
	{
		CWinApp::OnIdle(lCount);
		for(int i=0;i<m_D3DViewPortList.GetCount();i++)
		{
			m_D3DViewPortList[i]->Update();
		}		
		return true;
	}
	return CWinApp::OnIdle(lCount);;
}

BOOL CUSOEditApp::OnDDECommand(LPTSTR lpszCommand)
{
	// TODO: 在此添加专用代码和/或调用基类	
	return CWinApp::OnDDECommand(lpszCommand);
}



void CUSOEditApp::OnSetTexturePath()
{
	// TODO: 在此添加命令处理程序代码
	CDlgPathListSetting dlg;

	dlg.m_PathList=m_TexturePathList;
	dlg.m_Title=_T("设置纹理路径");
	if(dlg.DoModal()==IDOK)
	{
		m_TexturePathList=dlg.m_PathList;
		CD3DTexture::ClearPath();
		CD3DTexture::AddPathList(m_TexturePathList);
		SaveSetting();
	}
}

void CUSOEditApp::OnSetFxPath()
{
	// TODO: 在此添加命令处理程序代码
	CDlgPathListSetting dlg;

	dlg.m_PathList=m_FXPathList;
	dlg.m_Title=_T("设置FX路径");
	if(dlg.DoModal()==IDOK)
	{
		m_FXPathList=dlg.m_PathList;
		CD3DFX::ClearPath();
		CD3DFX::AddPathList(m_FXPathList);
		SaveSetting();
	}	
}



void CUSOEditApp::OnLoadBlzDbc()
{
	// TODO: 在此添加命令处理程序代码
	BROWSEINFO bi;
	TCHAR FilePath[MAX_PATH];
	LPITEMIDLIST pidl;

	ZeroMemory(&bi,sizeof(bi));

	//bi.pszDisplayName=FilePath;
	bi.lpszTitle=_T("请选择目标目录");
	bi.ulFlags=BIF_RETURNONLYFSDIRS|BIF_USENEWUI|BIF_SHAREABLE;
	pidl=::SHBrowseForFolder(&bi);
	if(pidl!=NULL)
	{
		::SHGetPathFromIDList(pidl,FilePath);
		IMalloc *pMalloc=NULL;
		::SHGetMalloc(&pMalloc);
		pMalloc->Free(pidl);
		pMalloc->Release();
		pMalloc=NULL;	

		CEasyString ErrorMsg;

		if(CBLZWOWDatabase::GetInstance()->LoadDBCs(FilePath,ErrorMsg))
		{
			m_BLZDBCPath=FilePath;
			SaveSetting();
		}
		else
		{
			AfxGetMainWnd()->MessageBox(ErrorMsg);
		}
		
	}	
}

void CUSOEditApp::LoadSetting()
{
	pug::xml_parser Xml;
	if(Xml.parse_file(MakeModuleFullPath(NULL,SETTING_FILE_NAME),pug::parse_trim_attribute))
	{
		xml_node Setting=Xml.document();
		if(Setting.moveto_child(_T("Setting")))
		{
			xml_node TexturePathList=Setting;
			if(TexturePathList.moveto_child(_T("TexturePathList")))
			{
				m_TexturePathList.Clear();
				for(UINT i=0;i<TexturePathList.children();i++)
				{
					xml_node TexturePath=TexturePathList.child(i);
					if(_tcsicmp(TexturePath.name(),_T("TexturePath"))==0)
					{
						if(TexturePath.has_attribute(_T("Path")))
						{
							CEasyString Path=TexturePath.attribute(_T("Path")).getvalue();
							m_TexturePathList.Add(Path);
						}
					}
					
				}
			}
			xml_node FXPathList=Setting;
			if(FXPathList.moveto_child(_T("FXPathList")))
			{
				m_FXPathList.Clear();
				for(UINT i=0;i<FXPathList.children();i++)
				{
					xml_node FXPath=FXPathList.child(i);
					if(_tcsicmp(FXPath.name(),_T("FXPath"))==0)
					{
						if(FXPath.has_attribute(_T("Path")))
						{
							CEasyString Path=FXPath.attribute(_T("Path")).getvalue();
							m_FXPathList.Add(Path);
						}
					}

				}
			}
			xml_node MPQFileList=Setting;
			if(MPQFileList.moveto_child(_T("MPQFileList")))
			{
				m_MPQFileList.Clear();
				m_MPQFileSystem.ClearAllMPQ();
				for(UINT i=0;i<MPQFileList.children();i++)
				{
					xml_node MPQFile=MPQFileList.child(i);
					if(_tcsicmp(MPQFile.name(),_T("MPQFile"))==0)
					{
						if(MPQFile.has_attribute(_T("File")))
						{
							CEasyString File=MPQFile.attribute(_T("File")).getvalue();
							m_MPQFileList.Add(File);
							m_MPQFileSystem.AddMPQ(File);
						}
					}

				}
			}
			xml_node BLZDBCPath=Setting;
			if(BLZDBCPath.moveto_child(_T("BLZDBCPath")))
			{
				if(BLZDBCPath.has_attribute(_T("Path")))
				{
					m_BLZDBCPath=BLZDBCPath.attribute(_T("Path")).getvalue();
					if(!m_BLZDBCPath.IsEmpty())
					{
						CEasyString ErrorMsg;
						if(!CBLZWOWDatabase::GetInstance()->LoadDBCs(m_BLZDBCPath,ErrorMsg))
						{
							AfxGetMainWnd()->MessageBox(ErrorMsg);
						}
					}
				}
			}		
			xml_node Camera=Setting;
			if(Camera.moveto_child(_T("Camera")))
			{
				if(Camera.has_attribute(_T("CameraMode")))
				{
					m_CameraSetting.CameraMode=Camera.attribute(_T("CameraMode"));					
				}
				if(Camera.has_attribute(_T("ProjectMode")))
				{
					m_CameraSetting.CameraProjectMode=Camera.attribute(_T("ProjectMode"));					
				}				
				if(Camera.has_attribute(_T("NearFace")))
				{
					m_CameraSetting.CameraNearFace=(double)Camera.attribute(_T("NearFace"));					
				}
				if(Camera.has_attribute(_T("FarFace")))
				{
					m_CameraSetting.CameraFarFace=(double)Camera.attribute(_T("FarFace"));					
				}
				if(Camera.has_attribute(_T("MoveStep")))
				{
					m_CameraSetting.CameraMoveStep=(double)Camera.attribute(_T("MoveStep"));					
				}
				if(Camera.has_attribute(_T("RotateStep")))
				{
					m_CameraSetting.CameraRotateStep=(double)Camera.attribute(_T("RotateStep"));					
				}
			}	
			xml_node Viewport=Setting;
			if(Viewport.moveto_child(_T("Viewport")))
			{
				if(Viewport.has_attribute(_T("Width")))
				{
					m_ViewportSetting.ViewportWidth=Viewport.attribute(_T("Width"));					
				}
				if(Viewport.has_attribute(_T("Height")))
				{
					m_ViewportSetting.ViewportHeight=Viewport.attribute(_T("Height"));					
				}
			}
		}
	}
	else
	{
		AfxGetMainWnd()->MessageBox(_T("装载配置失败！"));
	}
}
void CUSOEditApp::SaveSetting()
{
	
	pug::xml_parser Xml;

	Xml.create();
	xml_node Doc;
	Doc = Xml.document();
	xml_node pi = Doc.append_child(node_pi);
	pi.name(_T("xml"));
	pi.attribute(_T("version")) = _T("1.0");
	//pi.attribute(_T("encoding")) = _T("gb2312");

	xml_node Setting=Doc.append_child(node_element,_T("Setting"));

	xml_node TexturePathList=Setting.append_child(node_element,_T("TexturePathList"));
	CFilePathList& TexturePaths=CD3DTexture::GetFilePathList();
	void * Pos=TexturePaths.GetFirstObjectPos();
	while(Pos)
	{
		CEasyString Key;
		TexturePaths.GetNextObject(Pos,Key);
		xml_node TexturePath=TexturePathList.append_child(node_element,_T("TexturePath"));
		TexturePath.append_attribute(_T("Path"),(LPCTSTR)Key);
	}

	xml_node FXPathList=Setting.append_child(node_element,_T("FXPathList"));

	CFilePathList& FXPaths=CD3DFX::GetFilePathList();
	Pos=FXPaths.GetFirstObjectPos();
	while(Pos)
	{
		CEasyString Key;
		FXPaths.GetNextObject(Pos,Key);
		xml_node FXPath=FXPathList.append_child(node_element,_T("FXPath"));
		FXPath.append_attribute(_T("Path"),(LPCTSTR)Key);
	}		

	xml_node MPQFileList=Setting.append_child(node_element,_T("MPQFileList"));

	for(size_t i=0;i<m_MPQFileList.GetCount();i++)
	{
		xml_node MPQFile=MPQFileList.append_child(node_element,_T("MPQFile"));
		MPQFile.append_attribute(_T("File"),(LPCTSTR)m_MPQFileList[i]);
	}	

	xml_node BLZDBCPath=Setting.append_child(node_element,_T("BLZDBCPath"));
	BLZDBCPath.append_attribute(_T("Path"),m_BLZDBCPath);

	xml_node Camera=Setting.append_child(node_element,_T("Camera"));
	Camera.append_attribute(_T("CameraMode"),(long)m_CameraSetting.CameraMode);
	Camera.append_attribute(_T("ProjectMod"),(long)m_CameraSetting.CameraProjectMode);
	Camera.append_attribute(_T("NearFace"),m_CameraSetting.CameraNearFace);
	Camera.append_attribute(_T("FarFace"),m_CameraSetting.CameraFarFace);
	Camera.append_attribute(_T("MoveStep"),m_CameraSetting.CameraMoveStep);
	Camera.append_attribute(_T("RotateStep"),m_CameraSetting.CameraRotateStep);

	xml_node Viewport=Setting.append_child(node_element,_T("Viewport"));
	Viewport.append_attribute(_T("Width"),(long)m_ViewportSetting.ViewportWidth);
	Viewport.append_attribute(_T("Height"),(long)m_ViewportSetting.ViewportHeight);

	if(!Xml.document().SaveToFile(0,SETTING_FILE_NAME))
	{
		AfxGetMainWnd()->MessageBox(_T("保存配置失败！"));
	}
	
}




void CUSOEditApp::OnSetMpqFile()
{
	// TODO: 在此添加命令处理程序代码
	CDlgPathListSetting Dlg;
	Dlg.m_WorkMode=CDlgPathListSetting::WORK_MODE_FILE_LIST;
	Dlg.m_Title=_T("MPQ文件");
	Dlg.m_PathList=m_MPQFileList;
	if(Dlg.DoModal()==IDOK)
	{
		m_MPQFileList=Dlg.m_PathList;
		m_MPQFileSystem.ClearAllMPQ();
		for(size_t i=0;i<m_MPQFileList.GetCount();i++)
		{
			m_MPQFileSystem.AddMPQ(m_MPQFileList[i]);
		}
	}
}
