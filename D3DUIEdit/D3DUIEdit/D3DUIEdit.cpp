/****************************************************************************/
/*                                                                          */
/*      文件名:    D3DUIEdit.cpp                                            */
/*      创建日期:  2011年10月07日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
// D3DUIEdit.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "D3DUIEdit.h"
#include "MainFrm.h"

#include "ChildFrm.h"
#include "D3DUIEditDoc.h"
//#include "D3DUIEditView.h"
#include ".\d3duiedit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CD3DUIEditApp

BEGIN_MESSAGE_MAP(CD3DUIEditApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	// 基于文件的标准文档命令
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	//ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// 标准打印设置命令
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
	ON_COMMAND(ID_FILE_OPEN, &CD3DUIEditApp::OnFileOpen)
END_MESSAGE_MAP()


// CD3DUIEditApp 构造

CD3DUIEditApp::CD3DUIEditApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
	m_pD3DViewPort=NULL;
}


// 唯一的一个 CD3DUIEditApp 对象

CD3DUIEditApp theApp;

// CD3DUIEditApp 初始化

BOOL CD3DUIEditApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControls()。否则，将无法创建窗口。
	//_CrtSetBreakAlloc(1046);

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
	SetRegistryKey(_T("D3D UI Edit"));
	LoadStdProfileSettings(4);  // 加载标准 INI 文件选项(包括 MRU)
	

	CheckSystemFile();

	CD3DTexture::AddPath(GetModulePath(NULL));


	// 注册应用程序的文档模板。文档模板
	// 将用作文档、框架窗口和视图之间的连接
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(IDR_D3DUITYPE,
		RUNTIME_CLASS(CD3DUIEditDoc),
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
	// 主窗口已初始化，因此显示它并对其进行更新
	pMainFrame->ShowWindow(SW_MAXIMIZE);
	pMainFrame->UpdateWindow();
	return TRUE;
}

int CD3DUIEditApp::ExitInstance()
{
	// TODO: 在此添加专用代码和/或调用基类	
	CLogManager::ReleaseInstance();
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
void CD3DUIEditApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// CD3DUIEditApp 消息处理程序


BOOL CD3DUIEditApp::OnIdle(LONG lCount)
{
	CWinApp::OnIdle(lCount);
	
	if(m_pD3DViewPort)
	{
		m_pD3DViewPort->Update();		
		return TRUE;
	}
	else
	{
		return FALSE;
	}

}

void CD3DUIEditApp::OnFileOpen()
{
	// TODO: 在此添加命令处理程序代码
	CFileDialog Dlg(true,_T("*.UI"),_T("*.UI"),0,_T("UI Script Files(*.UI)|*.UI|Universal System Object Files (*.USO)|*.USO|All Files(*.*)|*.*||"));
	if(Dlg.DoModal()==IDOK)
	{
		OpenDocumentFile(Dlg.GetPathName());
	}
}


void CD3DUIEditApp::CheckSystemFile()
{
	CEasyString DefaultTextureFile=MakeModuleFullPath(NULL,DEFAULT_UI_TEXTURE);
	if(!PathFileExists(DefaultTextureFile))
	{
		CResourceFileAccessor ResourceFile;
		CEasyString ResourceName;
		ResourceName.Format(_T("Module=%I64u;Type=Texture;ID=%u"),(INT64)NULL,IDT_DEFAULT);
		if(ResourceFile.Open(ResourceName,0))
		{
			CWinFileAccessor TextureFile;

			if(TextureFile.Open(DefaultTextureFile,IFileAccessor::modeCreateAlways|IFileAccessor::modeWrite))
			{
				UINT Size=ResourceFile.GetSize();
				BYTE * pBuffer=new BYTE[Size];
				ResourceFile.Read(pBuffer,Size);
				TextureFile.Write(pBuffer,Size);
				delete[] pBuffer;
				ResourceFile.Close();
				TextureFile.Close();
			}
		}		
	}
}
