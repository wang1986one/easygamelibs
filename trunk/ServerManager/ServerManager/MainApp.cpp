/****************************************************************************/
/*                                                                          */
/*      �ļ���:    MainApp.cpp                                              */
/*      ��������:  2010��02��09��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
// DBProxy.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

LPCTSTR g_ProgramName="ServerManager";
LPCTSTR g_ServiceName="ServerManager";
LPCTSTR g_ServiceDesc="ServerManager";



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
