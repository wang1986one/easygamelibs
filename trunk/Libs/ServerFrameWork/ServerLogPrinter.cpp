#include "StdAfx.h"

CServerLogPrinter::CServerLogPrinter(IBaseServer * pServer,UINT Mode,int Level,LPCTSTR FileName,int FileLogQueueLen)
{
	m_pServer=pServer;
	SetLogMode(Mode,Level,FileName,FileLogQueueLen);
}

CServerLogPrinter::~CServerLogPrinter(void)
{
	m_FileLogWorkThread.SafeTerminate();
}

void CServerLogPrinter::SetLogMode(UINT Mode,int Level,LPCTSTR FileName,int FileLogBufferLen)
{
	m_LogOutputMode=Mode;
	m_LogLevel=Level;
	m_LogFileName=FileName;
	
	if(m_LogOutputMode==0)
		return;

	CAutoLock Lock(m_EasyCriticalSection);


	if(m_LogOutputMode&LOM_FILE)
	{
		m_FileLogWorkThread.Init(FileName,FileLogBufferLen);
		if(!m_FileLogWorkThread.IsWorking())
			m_FileLogWorkThread.Start();
	}
	else
	{
		m_FileLogWorkThread.SafeTerminate();
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
			//m_FileLogWorkThread.WriteLogDirect(MsgBuff);
		}

	}
	catch(...)
	{
		PrintImportantLog(0,"Log[%s]输出发生异常[%s]",(LPCTSTR)m_LogFileName,Format);
	}
}

void CServerLogPrinter::OutputLogString(int Level,DWORD Color,LPCTSTR szLog)
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


		sprintf_s(MsgBuff,5000,"[%02d-%02d][%02d:%02d:%02d]:%s",
			CurTime.Month(),CurTime.Day(),
			CurTime.Hour(),CurTime.Minute(),CurTime.Second(),
			szLog);

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
		PrintImportantLog(0,"Log[%s]输出发生异常[%s]",(LPCTSTR)m_LogFileName,szLog);
	}
}
