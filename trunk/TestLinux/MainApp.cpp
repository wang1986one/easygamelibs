// DBProxy.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "MainApp.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

LPCTSTR g_ProgramName="TestServer";
LPCTSTR g_ServiceName="TestServer";
LPCTSTR g_ServiceDesc="TestServer";



// CMainApp ����

CMainApp::CMainApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CMainApp ����

CMainApp theApp;


BOOL CMainApp::OnStartUp()
{
	if(!CMainThread::GetInstance()->StartUp())
		return FALSE;
	m_pServer=CMainThread::GetInstance();
	return TRUE;
}
void CMainApp::OnShutDown()
{
	CMainThread::GetInstance()->ShutDown(MAX_SERVER_TERMINATE_WAIT_TIME);
	m_pServer=NULL;
}

//int main()
//{
//    //theApp.Run();
//    getchar();
//    return 0;
//}
