/****************************************************************************/
/*                                                                          */
/*      文件名:    AsyncFileLogWorkThread.cpp                                    */
/*      创建日期:  2010年02月09日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#include "StdAfx.h"

#define LOG_BUFF_FULL_ALARM_TIME	(60*1000)

IMPLEMENT_CLASS_INFO_STATIC(CAsyncFileLogWorkThread,CEasyThread);

CAsyncFileLogWorkThread::CAsyncFileLogWorkThread()
{
	m_RecentLogTime.FetchLocalTime();
	m_pLogFile=NULL;
}

BOOL CAsyncFileLogWorkThread::Init(LPCTSTR FileName,int LogBufferLen)
{

	if(LogBufferLen)
	{
		if(!m_LogDataBuffer.Create(LogBufferLen))
		{
			PrintImportantLog(0,"无法创建Log缓冲%s",(LPCTSTR)FileName);
		}
	}
	

	m_LogDataBuffer.SetLockMode(false,true);

	if(FileName)
	{
		if(FileName[0])
		{
			m_LogFileName=FileName;			
		}
	}


	CEasyString LogFileName;
	LogFileName.Format("%s.%u-%02u-%02u.log",
		(LPCTSTR)m_LogFileName,
		m_RecentLogTime.Year(),
		m_RecentLogTime.Month(),
		m_RecentLogTime.Day());


	int FileOpenMode=IFileAccessor::modeAppend|IFileAccessor::modeWrite|
		IFileAccessor::shareShareRead|IFileAccessor::osWriteThrough;
	CreateDir(LogFileName);

	SAFE_RELEASE(m_pLogFile);
	
	m_pLogFile=CFileSystemManager::GetInstance()->CreateFileAccessor(FILE_CHANNEL_NORMAL1);
	if(m_pLogFile==NULL)
	{
		PrintImportantLog(0,"无法创建文件存取器");
		return FALSE;
	}

	if(!m_pLogFile->Open(LogFileName,FileOpenMode))
	{
		PrintImportantLog(0,"无法打开Log文件%s",(LPCTSTR)LogFileName);
		return FALSE;
	}
	m_pLogFile->Seek(0,IFileAccessor::seekEnd);
	return TRUE;
}



CAsyncFileLogWorkThread::~CAsyncFileLogWorkThread(void)
{
	SAFE_RELEASE(m_pLogFile);
}


BOOL CAsyncFileLogWorkThread::OnStart()
{
	return TRUE;
}

BOOL CAsyncFileLogWorkThread::OnRun()
{
	int DataSize=m_LogDataBuffer.GetSmoothUsedSize();
	if(DataSize)
	{
		m_pLogFile->Write(m_LogDataBuffer.GetUsedBuffer(),DataSize);
		m_LogDataBuffer.PopFront((LPVOID)NULL,DataSize);		
	}
	else
	{
		CEasyTime CurTime;
		CurTime.FetchLocalTime();

		if(m_RecentLogTime.Year()!=CurTime.Year()||
			m_RecentLogTime.Month()!=CurTime.Month()||
			m_RecentLogTime.Day()!=CurTime.Day())
		{
			m_RecentLogTime=CurTime;
			Init(m_LogFileName,0);
		}

		DoSleep(1);
	}

	return TRUE;
}

void CAsyncFileLogWorkThread::OnTerminate()
{
	int DataSize=m_LogDataBuffer.GetSmoothUsedSize();
	while(DataSize)
	{
		m_pLogFile->Write(m_LogDataBuffer.GetUsedBuffer(),DataSize);
		m_LogDataBuffer.PopFront((LPVOID)NULL,DataSize);
		DataSize=m_LogDataBuffer.GetSmoothUsedSize();
	}
	m_pLogFile->Close();
}

BOOL CAsyncFileLogWorkThread::PushLog(LPCTSTR LogData)
{
	
	int DataLen=(int)strlen(LogData);
	m_LogDataBuffer.PushBack((LPVOID)LogData,DataLen);

	return TRUE;
}


void CAsyncFileLogWorkThread::CreateDir(LPCTSTR FilePath)
{
	CEasyString FileDir=GetPathDirectory(FilePath);
	if(!CreateDirEx(FileDir))
	{
		PrintImportantLog(0,"创建目录%s失败",(LPCTSTR)FileDir);
	}
}

