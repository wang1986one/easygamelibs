/****************************************************************************/
/*                                                                          */
/*      �ļ���:    ServerLogPrinter.cpp                                     */
/*      ��������:  2009��07��06��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#include "StdAfx.h"

CServerLogPrinter::CServerLogPrinter(IBaseServer * pServer,UINT Mode,int Level,LPCTSTR FileName,int FileLogQueueLen)
{
	m_pServer=pServer;
	SetLogMode(Mode,Level,FileName,FileLogQueueLen);
}

CServerLogPrinter::~CServerLogPrinter(void)
{
}

void CServerLogPrinter::SetLogMode(UINT Mode,int Level,LPCTSTR FileName,int FileLogBufferLen)
{
	m_LogOutputMode=Mode;
	
	if(m_LogOutputMode==0)
		return;

	if(m_LogOutputMode&LOM_FILE)
	{
		ResetLog(Level,FileName,FileLogBufferLen);
	}
	else
	{
		CloseLog();
	}
}


void CServerLogPrinter::PrintLog(int Level,DWORD Color,LPCTSTR Format,va_list vl)
{
	try
	{
		if((Level&m_LogLevel)==0)
		{
			return;
		}
		char MsgBuff[5000];

		CEasyTime CurTime;
		CurTime.FetchLocalTime();


		sprintf_s(MsgBuff,5000,"[%02d-%02d][%02d:%02d:%02d]:",
			CurTime.Month(),CurTime.Day(),
			CurTime.Hour(),CurTime.Minute(),CurTime.Second());


		vsprintf_s(MsgBuff+17,4096-17,Format, vl );
		MsgBuff[4095]=0;


		if((m_LogOutputMode&LOM_CONSOLE)&&m_pServer)
		{
			m_pServer->PrintConsoleLog(MsgBuff+7);
		}

		strncat_s(MsgBuff,5000,"\r\n",4096);

		if(m_LogOutputMode&LOM_VS)
		{
#ifdef WIN32
			OutputDebugString(MsgBuff+7);
#else
			printf("%s",MsgBuff+7);
#endif
		}

		if(m_LogOutputMode&LOM_FILE)
		{
			m_FileLogWorkThread.PushLog(MsgBuff);
		}

	}
	catch(...)
	{
		PrintImportantLog(0,"Log[%s]��������쳣[%s]",(LPCTSTR)m_LogFileName,Format);
	}
}

