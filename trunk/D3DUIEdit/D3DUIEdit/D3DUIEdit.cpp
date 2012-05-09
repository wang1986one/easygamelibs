/****************************************************************************/
/*                                                                          */
/*      �ļ���:    D3DUIEdit.cpp                                            */
/*      ��������:  2011��10��07��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
// D3DUIEdit.cpp : ����Ӧ�ó��������Ϊ��
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
	// �����ļ��ı�׼�ĵ�����
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	//ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// ��׼��ӡ��������
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
	ON_COMMAND(ID_FILE_OPEN, &CD3DUIEditApp::OnFileOpen)
END_MESSAGE_MAP()


// CD3DUIEditApp ����

CD3DUIEditApp::CD3DUIEditApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
	m_pD3DViewPort=NULL;
}


// Ψһ��һ�� CD3DUIEditApp ����

CD3DUIEditApp theApp;

// CD3DUIEditApp ��ʼ��

BOOL CD3DUIEditApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControls()�����򣬽��޷��������ڡ�
	//_CrtSetBreakAlloc(1046);

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
	SetRegistryKey(_T("D3D UI Edit"));
	LoadStdProfileSettings(4);  // ���ر�׼ INI �ļ�ѡ��(���� MRU)
	

	CheckSystemFile();

	CD3DTexture::AddPath(GetModulePath(NULL));


	// ע��Ӧ�ó�����ĵ�ģ�塣�ĵ�ģ��
	// �������ĵ�����ܴ��ں���ͼ֮�������
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(IDR_D3DUITYPE,
		RUNTIME_CLASS(CD3DUIEditDoc),
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
	// �������ѳ�ʼ���������ʾ����������и���
	pMainFrame->ShowWindow(SW_MAXIMIZE);
	pMainFrame->UpdateWindow();
	return TRUE;
}

int CD3DUIEditApp::ExitInstance()
{
	// TODO: �ڴ����ר�ô����/����û���	
	CLogManager::ReleaseInstance();
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
void CD3DUIEditApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// CD3DUIEditApp ��Ϣ�������


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
	// TODO: �ڴ���������������
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
