/****************************************************************************/
/*                                                                          */
/*      �ļ���:    USOEdit.cpp                                              */
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
/*      �ļ���:    USOEdit.cpp                                              */
/*      ��������:  2010��02��09��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
// USOEdit.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "USOEdit.h"




// CUSOEditApp

BEGIN_MESSAGE_MAP(CUSOEditApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	// �����ļ��ı�׼�ĵ�����
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)	
	ON_COMMAND(ID_SET_TEXTURE_PATH, OnSetTexturePath)
	ON_COMMAND(ID_SET_FX_PATH, OnSetFxPath)	
	ON_COMMAND(ID_LOAD_BLZ_DBC, &CUSOEditApp::OnLoadBlzDbc)
	ON_COMMAND(ID_SET_MPQ_FILE, &CUSOEditApp::OnSetMpqFile)
END_MESSAGE_MAP()


// CUSOEditApp ����

CUSOEditApp::CUSOEditApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
	m_CameraSetting.CameraMode=CAMERA_MODE_SCENE;
	m_CameraSetting.CameraProjectMode=CAMERA_PROJECT_MODE_PERSPECTIVE;
	m_CameraSetting.CameraNearFace=1.0f;
	m_CameraSetting.CameraFarFace=2000.0f;
	m_CameraSetting.CameraMoveStep=1.0f;
	m_CameraSetting.CameraRotateStep=0.001f;
	m_ViewportSetting.ViewportWidth=800;
	m_ViewportSetting.ViewportHeight=600;
}


// Ψһ��һ�� CUSOEditApp ����

CUSOEditApp theApp;

CUSOEditApp * CUSOEditApp::GetInstance()
{
	return &theApp;
}

// CUSOEditApp ��ʼ��



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

	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControls()�����򣬽��޷��������ڡ�
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
	SetRegistryKey(_T("Universal System Object Editor"));
	LoadStdProfileSettings(4);  // ���ر�׼ INI �ļ�ѡ��(���� MRU)
	// ע��Ӧ�ó�����ĵ�ģ�塣�ĵ�ģ��
	// �������ĵ�����ܴ��ں���ͼ֮�������
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(IDR_USOTYPE,
		RUNTIME_CLASS(CUSOEditDoc),
		RUNTIME_CLASS(CChildFrame), // �Զ��� MDI �ӿ��
		NULL);
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);
	// ������ MDI ��ܴ���
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;
	// �������к�׺ʱ�ŵ��� DragAcceptFiles
	//  �� MDI Ӧ�ó����У���Ӧ������ m_pMainWnd ֮����������
	// ������/��
	m_pMainWnd->DragAcceptFiles();
	// ���á�DDE ִ�С�
	EnableShellOpen();
	RegisterShellFileTypes(FALSE);
	// ������׼������DDE�����ļ�������������
	
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	// ��������������ָ����������
	// �� /RegServer��/Register��/Unregserver �� /Unregister ����Ӧ�ó����򷵻� FALSE��
	if(cmdInfo.m_nShellCommand!=CCommandLineInfo::FileNew)
	{
		if (!ProcessShellCommand(cmdInfo))
			return FALSE;
	}

	LoadSetting();	
	
	// �������ѳ�ʼ���������ʾ����������и���
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
void CUSOEditApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// CUSOEditApp ��Ϣ�������


BOOL CUSOEditApp::OnIdle(LONG lCount)
{
	// TODO: �ڴ����ר�ô����/����û���	
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
	// TODO: �ڴ����ר�ô����/����û���	
	return CWinApp::OnDDECommand(lpszCommand);
}



void CUSOEditApp::OnSetTexturePath()
{
	// TODO: �ڴ���������������
	CDlgPathListSetting dlg;

	dlg.m_PathList=m_TexturePathList;
	dlg.m_Title=_T("��������·��");
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
	// TODO: �ڴ���������������
	CDlgPathListSetting dlg;

	dlg.m_PathList=m_FXPathList;
	dlg.m_Title=_T("����FX·��");
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
	// TODO: �ڴ���������������
	BROWSEINFO bi;
	TCHAR FilePath[MAX_PATH];
	LPITEMIDLIST pidl;

	ZeroMemory(&bi,sizeof(bi));

	//bi.pszDisplayName=FilePath;
	bi.lpszTitle=_T("��ѡ��Ŀ��Ŀ¼");
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
		AfxGetMainWnd()->MessageBox(_T("װ������ʧ�ܣ�"));
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
		AfxGetMainWnd()->MessageBox(_T("��������ʧ�ܣ�"));
	}
	
}




void CUSOEditApp::OnSetMpqFile()
{
	// TODO: �ڴ���������������
	CDlgPathListSetting Dlg;
	Dlg.m_WorkMode=CDlgPathListSetting::WORK_MODE_FILE_LIST;
	Dlg.m_Title=_T("MPQ�ļ�");
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
