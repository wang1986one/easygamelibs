/****************************************************************************/
/*                                                                          */
/*      �ļ���:    ServerAppLinux.cpp                                       */
/*      ��������:  2009��09��11��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
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
