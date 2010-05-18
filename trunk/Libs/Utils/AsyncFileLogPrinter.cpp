/****************************************************************************/
/*                                                                          */
/*      �ļ���:    AsyncFileLogPrinter.cpp                                     */
/*      ��������:  2009��07��06��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#include "StdAfx.h"

CAsyncFileLogPrinter::CAsyncFileLogPrinter()
{
	m_LogLevel=0;
}

CAsyncFileLogPrinter::CAsyncFileLogPrinter(int Level,LPCTSTR FileName,int FileLogQueueLen)
{
	ResetLog(Level,FileName,FileLogQueueLen);
}

CAsyncFileLogPrinter::~CAsyncFileLogPrinter(void)
{
	m_FileLogWorkThread.SafeTerminate();
}

void CAsyncFileLogPrinter::ResetLog(int Level,LPCTSTR FileName,int FileLogBufferLen)
{
	m_LogLevel=Level;
	m_LogFileName=FileName;
	
	CAutoLock Lock(m_EasyCriticalSection);
	
	m_FileLogWorkThread.Init(FileName,FileLogBufferLen);
	if(!m_FileLogWorkThread.IsWorking())
		m_FileLogWorkThread.Start();
	
}

void CAsyncFileLogPrinter::CloseLog()
{
	m_FileLogWorkThread.SafeTerminate();
}


void CAsyncFileLogPrinter::PrintLog(int Level,DWORD Color,LPCTSTR Format,va_list vl)
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
		strncat_s(MsgBuff,5000,"\r\n",4096);

		m_FileLogWorkThread.PushLog(MsgBuff);
			

	}
	catch(...)
	{
		PrintImportantLog(0,"Log[%s]��������쳣[%s]",(LPCTSTR)m_LogFileName,Format);
	}
}


