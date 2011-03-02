/****************************************************************************/
/*                                                                          */
/*      �ļ���:    ServerAppWin.cpp                                         */
/*      ��������:  2010��02��09��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#include "StdAfx.h"

//BEGIN_MESSAGE_MAP(CServerApp, CWinApp)
//	
//END_MESSAGE_MAP()


CServerApp::CServerApp(void)
	:CNTService(g_ServiceName,g_ServiceDesc)
{
	m_pServer=NULL;
}

CServerApp::~CServerApp(void)
{
}

BOOL CServerApp::InitInstance()
{
	return TRUE;
}

 int CServerApp::ExitInstance()
 {
	 return 0;
 }

 BOOL CServerApp::OnIdle(LONG lCount)
 {
	 // TODO: �ڴ����ר�ô����/����û���

	 CWinApp::OnIdle(lCount);

	 if(m_pServer)
	 {
		if(m_pServer->IsServerTerminated())
		{
			m_pMainWnd->PostMessage(WM_QUIT);
		}
	 }
	 else
	 {
		 m_pMainWnd->PostMessage(WM_QUIT);
	 }

	 Sleep(1);
	 return TRUE;
 }

 int CServerApp::Run()
 {
	 RegisterService(__argc, __argv);
	 return 0;
 }

void CServerApp::Run(DWORD argc, LPTSTR * argv)
{
	ReportStatus(SERVICE_START_PENDING);


	//CExceptionParser::GetInstance()->Init(0);
	

	GetProgamVersion();

	InitCommonControls();
	AfxEnableControlContainer();

	CServerConsoleDlg * pConsoleDlg=new CServerConsoleDlg();
	m_pMainWnd=pConsoleDlg;

	//if(g_ServiceWorkMode==SWM_APPLICATION)
	{
		pConsoleDlg->Create(CServerConsoleDlg::IDD);
		pConsoleDlg->RegisterTrayIcon(g_ServiceName);
		
	}

	ReportStatus(SERVICE_RUNNING);

	if(OnStartUp())
	{
		pConsoleDlg->SetServer(m_pServer);
		CWinApp::Run();
	}

	OnShutDown();
	
	SAFE_DELETE(m_pMainWnd);
	ReportStatus(SERVICE_STOPPED);	

}

void CServerApp::Stop()
{
	ReportStatus(SERVICE_STOP_PENDING, 11000);
	if(m_pServer)
		m_pServer->QueryShowDown();
}

BOOL CServerApp::OnStartUp()
{
	return TRUE;
}
void CServerApp::OnShutDown()
{
}

void CServerApp::GetProgamVersion()
{
	CString strGSPathName;
	GetModuleFileName(NULL,strGSPathName.GetBuffer(255),255);
	strGSPathName.ReleaseBuffer();
	DWORD dwHandle = -1;
	DWORD dwSize = GetFileVersionInfoSize(strGSPathName.GetBuffer(0), &dwHandle );
	if(dwSize)
	{
		LPVOID lpData = new BYTE[dwSize];
		GetFileVersionInfo( strGSPathName.GetBuffer(0), dwHandle, dwSize, lpData );	

		UINT uiLen=0;
		LPVOID pInfo=NULL;
		VerQueryValue(
			lpData, 
			TEXT( "\\" ), 
			&pInfo,
			&uiLen );

		if(pInfo&&uiLen>=sizeof(VS_FIXEDFILEINFO))
		{
			VS_FIXEDFILEINFO * pFileInfo=(VS_FIXEDFILEINFO *)pInfo;
			g_ProgramVersion[3]=HIWORD(pFileInfo->dwProductVersionMS);
			g_ProgramVersion[2]=LOWORD(pFileInfo->dwProductVersionMS);
			g_ProgramVersion[1]=HIWORD(pFileInfo->dwProductVersionLS);
			g_ProgramVersion[0]=LOWORD(pFileInfo->dwProductVersionLS);			
		}


		delete [] lpData;
	}
	
}