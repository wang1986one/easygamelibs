/****************************************************************************/
/*                                                                          */
/*      �ļ���:    AsyncFileLogPrinter.h                                       */
/*      ��������:  2009��07��06��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once
#define DEFAULT_ASYNC_FILE_LOG_BUFFER_LEN	40960

class CAsyncFileLogPrinter :
	public ILogPrinter
{
protected:
	CAsyncFileLogWorkThread		m_FileLogWorkThread;
	CEasyCriticalSection		m_EasyCriticalSection;
	CEasyString					m_LogFileName;
	int							m_LogLevel;
public:
	
	CAsyncFileLogPrinter();
	CAsyncFileLogPrinter(int Level,LPCTSTR FileName,int FileLogBufferLen=DEFAULT_ASYNC_FILE_LOG_BUFFER_LEN);
	virtual ~CAsyncFileLogPrinter(void);

	
	int GetLogLevel()
	{
		return m_LogLevel;
	}
	LPCTSTR GetLogFileName()
	{
		return m_LogFileName;
	}

	void ResetLog(int Level,LPCTSTR FileName,int FileLogQueueLen=DEFAULT_ASYNC_FILE_LOG_BUFFER_LEN);
	void CloseLog();

	virtual void PrintLog(int Level,DWORD Color,LPCTSTR Format,va_list vl);

};
