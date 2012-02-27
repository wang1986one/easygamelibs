// DBProxy.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "MainApp.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

LPCTSTR g_ProgramName="TestServer";
LPCTSTR g_ServiceName="TestServer";
LPCTSTR g_ServiceDesc="TestServer";



// CMainApp 构造

CMainApp::CMainApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CMainApp 对象

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
