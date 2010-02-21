#pragma once

#define FILE_LOG_DATA_BLOCK_SIZE	128


struct FILE_LOG_DATA_BLOCK
{
	char Data[FILE_LOG_DATA_BLOCK_SIZE];
	int DataSize;
	CEasyCriticalSection Lock;
};


class CFileLogWorkThread :
	public CEasyThread
{
protected:
	CEasyString								m_LogFileName;
	IFileAccessor							*m_pLogFile;
	CThreadSafeCycleBuffer					m_LogDataBuffer;	
	CEasyTime								m_RecentLogTime;	

	DECLARE_CLASS_INFO_STATIC(CFileLogWorkThread);
public:
	CFileLogWorkThread();
	BOOL Init(LPCTSTR FileName,int LogBufferLen);
	virtual ~CFileLogWorkThread(void);

	virtual BOOL OnStart();
	virtual BOOL OnRun();
	virtual void OnTerminate();

	BOOL PushLog(LPCTSTR LogData);
	BOOL WriteLogDirect(LPCTSTR LogData);

	LPCTSTR GetLogFileName()
	{
		return m_LogFileName;
	}
	
protected:
	void CreateDir(LPCTSTR FilePath);
};
