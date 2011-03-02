/****************************************************************************/
/*                                                                          */
/*      文件名:    ServerAppLinux.cpp                                       */
/*      创建日期:  2009年09月11日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#include "StdAfx.h"
#include "ServerAppWin.h"

CServerApp	* m_gAppInstance=NULL;

int main()//int argc, char *argv[])
{
	m_gAppInstance->Run();
	return 0;
}


CServerApp::CServerApp(void)
{
	m_WantExist=FALSE;
	m_gAppInstance=this;
}

CServerApp::~CServerApp(void)
{
}



BOOL CServerApp::OnStartUp()
{
	return TRUE;
}
void CServerApp::OnShutDown()
{
}

int CServerApp::Run()
{
	if(OnStartUp())
	{
		while(!m_WantExist)
		{
			DoSleep(1);
		}
		OnShutDown();
	}
	return 0;
}
