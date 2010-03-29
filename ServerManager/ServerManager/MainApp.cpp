/****************************************************************************/
/*                                                                          */
/*      文件名:    MainApp.cpp                                              */
/*      创建日期:  2010年02月09日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
// DBProxy.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

LPCTSTR g_ProgramName="ServerManager";
LPCTSTR g_ServiceName="ServerManager";
LPCTSTR g_ServiceDesc="ServerManager";



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
	if(!CMainThread::GetInstance()->StartUp(MAX_EVENT_OBJECT,WORK_THREAD_NUM))
		return FALSE;
	return TRUE;
}
void CMainApp::OnShutDown()
{
	CMainThread::GetInstance()->ShutDown(MAX_SERVER_TERMINATE_WAIT_TIME);
	CLogManager::GetInstance()->ReleaseInstance();
}

//int main()
//{
//    //theApp.Run();
//    getchar();
//    return 0;
//}
