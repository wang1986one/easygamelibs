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


CServerApp	* m_gAppInstance=NULL;

int main()//int argc, char *argv[])
{
	m_gAppInstance->Run();
	return 0;
}


CServerApp::CServerApp(void)
{
	m_WantExist=FALSE;
	m_pServer=NULL;
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
	char szCmd[2048];
	szCmd[0]=0;
	if(OnStartUp())
	{
		while(!m_WantExist)
		{
			DoSleep(1);
			gets(szCmd);
			if(_stricmp(szCmd,"quit")==0)
			{
				m_WantExist=true;
			}
		}
		OnShutDown();
	}
	return 0;
}
