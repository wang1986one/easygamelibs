/****************************************************************************/
/*                                                                          */
/*      �ļ���:    FileLogPrinter.cpp                                       */
/*      ��������:  2009��09��11��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#include "StdAfx.h"

CFileLogPrinter::CFileLogPrinter()
{
	m_Flag=0;
	m_pFileAccessor=NULL;
	m_FileOpenMode=0;
	m_LogLevel=0;
	
}

CFileLogPrinter::CFileLogPrinter(LPCTSTR FileName,DWORD Flag)
{
	Create(FileName,Flag);
}

bool CFileLogPrinter::Create(LPCTSTR FileName,DWORD Flag)
{
	CEasyString LogFileName;

	m_pFileAccessor=CFileSystemManager::GetInstance()->CreateFileAccessor(FILE_CHANNEL_NORMAL1);
	if(m_pFileAccessor==NULL)
		return false;

#ifdef _DEBUG
	m_LogLevel=ILogPrinter::LOG_LEVEL_DEBUG|ILogPrinter::LOG_LEVEL_NORMAL;
#else
	m_LogLevel=ILogPrinter::LOG_LEVEL_NORMAL;
#endif

	m_Flag=Flag;
	m_LogFileName=FileName;
	if(m_Flag&FILE_LOG_SPLIT_BY_DAY)
	{
		m_RecentLogTime.FetchLocalTime();
		LogFileName.Format("%s.%u-%02u-%02u.log",
			(LPCTSTR)m_LogFileName,
			m_RecentLogTime.Year(),
			m_RecentLogTime.Month(),
			m_RecentLogTime.Day());
	}
	else
	{
		LogFileName.Format("%s.log",
			(LPCTSTR)m_LogFileName);
	}
	if(m_Flag&FILE_LOG_APPEND)
		m_FileOpenMode=IFileAccessor::modeOpenAlways|IFileAccessor::modeAppend;
	else
		m_FileOpenMode=IFileAccessor::modeCreateAlways;
	m_FileOpenMode|=IFileAccessor::modeWrite|IFileAccessor::shareShareRead;
	if(m_Flag&FILE_LOG_SAFE_WRITE)
		m_FileOpenMode|=IFileAccessor::osWriteThrough;
	return m_pFileAccessor->Open(LogFileName,m_FileOpenMode)==TRUE;
}

CFileLogPrinter::~CFileLogPrinter(void)
{
	SAFE_RELEASE(m_pFileAccessor);
}

void CFileLogPrinter::PrintLogVL(int Level,DWORD Color,LPCTSTR Format,va_list vl)
{
	CAutoLock Lock(m_EasyCriticalSection);

	if((m_LogLevel&Level)==0)
		return;

	if(m_pFileAccessor==NULL)
		return;

	CEasyTime CurTime;
	CurTime.FetchLocalTime();
	if(m_Flag&FILE_LOG_SPLIT_BY_DAY)
	{
		if(!CEasyTime::IsSameDate(m_RecentLogTime,CurTime))
		{
			CEasyString LogFileName;

			m_pFileAccessor->Close();
			m_RecentLogTime=CurTime;
			LogFileName.Format("%s.%u-%02u-%02u.log",
				(LPCTSTR)m_LogFileName,
				m_RecentLogTime.Year(),
				m_RecentLogTime.Month(),
				m_RecentLogTime.Day());
			m_pFileAccessor->Open(LogFileName,m_FileOpenMode);
			if(m_Flag&FILE_LOG_APPEND)
				m_pFileAccessor->Seek(0,IFileAccessor::seekEnd);
		}
	}
	CurTime.Format(m_MsgBuff,40960,"[%m-%d %H:%M:%S]:");
	//sprintf_s(m_MsgBuff,40960,"[%02d-%02d %02d:%02d:%02d]:",
	//	SystemTime.wMonth,SystemTime.wDay,
	//	SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond);
	m_pFileAccessor->Write(m_MsgBuff,strlen(m_MsgBuff));
	vsprintf_s( m_MsgBuff,40960, Format, vl );
	m_pFileAccessor->Write(m_MsgBuff,strlen(m_MsgBuff));
	m_pFileAccessor->Write("\r\n",2);

}
