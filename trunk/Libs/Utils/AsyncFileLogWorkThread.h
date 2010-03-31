/****************************************************************************/
/*                                                                          */
/*      �ļ���:    AsyncFileLogWorkThread.h                                      */
/*      ��������:  2009��07��06��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once

#define ASYNC_FILE_LOG_DATA_BLOCK_SIZE	128


struct FILE_LOG_DATA_BLOCK
{
	char Data[ASYNC_FILE_LOG_DATA_BLOCK_SIZE];
	int DataSize;
	CEasyCriticalSection Lock;
};


class CAsyncFileLogWorkThread :
	public CEasyThread
{
protected:
	CEasyString								m_LogFileName;
	IFileAccessor							*m_pLogFile;
	CThreadSafeCycleBuffer					m_LogDataBuffer;	
	CEasyTime								m_RecentLogTime;	

	DECLARE_CLASS_INFO_STATIC(CAsyncFileLogWorkThread);
public:
	CAsyncFileLogWorkThread();
	BOOL Init(LPCTSTR FileName,int LogBufferLen);
	virtual ~CAsyncFileLogWorkThread(void);

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
