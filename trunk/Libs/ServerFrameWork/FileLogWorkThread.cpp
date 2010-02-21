#include "StdAfx.h"

#define LOG_BUFF_FULL_ALARM_TIME	(60*1000)

IMPLEMENT_CLASS_INFO_STATIC(CFileLogWorkThread,CEasyThread);

CFileLogWorkThread::CFileLogWorkThread()
{
	m_RecentLogTime.FetchLocalTime();
	m_pLogFile=NULL;
}

BOOL CFileLogWorkThread::Init(LPCTSTR FileName,int LogBufferLen)
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



CFileLogWorkThread::~CFileLogWorkThread(void)
{
	SAFE_RELEASE(m_pLogFile);
}


BOOL CFileLogWorkThread::OnStart()
{
	return TRUE;
}

BOOL CFileLogWorkThread::OnRun()
{
	int DataSize=m_LogDataBuffer.GetSmoothUsedSize();
	if(DataSize)
	{
		m_pLogFile->Write(m_LogDataBuffer.GetUsedBuffer(),DataSize);
		m_LogDataBuffer.PopFront((LPVOID)NULL,DataSize);
		//DataSize=m_LogDataBuffer.GetSmoothUsedSize();
		//if(DataSize>=m_LogDataBuffer.GetBufferSize()/2)
		//{
		//	PrintImportantLog(0,"%s怀疑有问题，Size=%d,Head=%d,Tail=%d",
		//		(LPCTSTR)m_LogFileName,
		//		m_LogDataBuffer.GetBufferSize(),
		//		m_LogDataBuffer.GetHeadPos(),
		//		m_LogDataBuffer.GetTailPos());
		//	//m_LogDataBuffer.Clear();
		//}
	}
	else
	{
		DoSleep(1);
	}

	return TRUE;
}

void CFileLogWorkThread::OnTerminate()
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

BOOL CFileLogWorkThread::PushLog(LPCTSTR LogData)
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

	int DataLen=(int)strlen(LogData);
	m_LogDataBuffer.PushBack((LPVOID)LogData,DataLen);

	return TRUE;
}

BOOL CFileLogWorkThread::WriteLogDirect(LPCTSTR LogData)
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

	m_pLogFile->Write((LPVOID)LogData,strlen(LogData));

	return TRUE;

}

void CFileLogWorkThread::CreateDir(LPCTSTR FilePath)
{
	CEasyString FileDir=GetPathDirectory(FilePath);
	CreateDirEx(FileDir);
}
