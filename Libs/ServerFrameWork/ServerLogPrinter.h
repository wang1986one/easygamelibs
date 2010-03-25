/****************************************************************************/
/*                                                                          */
/*      文件名:    ServerLogPrinter.h                                       */
/*      创建日期:  2009年07月06日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once
#define DEFAULT_FILE_LOG_BUFFER_LEN	40960

class CServerThread;

class CServerLogPrinter :
	public ILogPrinter
{
protected:
	IBaseServer *			m_pServer;
	UINT					m_LogOutputMode;
	CFileLogWorkThread		m_FileLogWorkThread;
	CEasyCriticalSection	m_EasyCriticalSection;
	CEasyString				m_LogFileName;
	int						m_LogLevel;
public:
	enum LOG_OUTPUT_MODE
	{
		LOM_VS=1,
		LOM_CONSOLE=(1<<1),
		LOM_FILE=(1<<2),
	};

	CServerLogPrinter(IBaseServer * pServer,UINT Mode,int Level,LPCTSTR FileName,int FileLogBufferLen=DEFAULT_FILE_LOG_BUFFER_LEN);
	virtual ~CServerLogPrinter(void);

	UINT GetLogMode()
	{
		return m_LogOutputMode;
	}
	int GetLogLevel()
	{
		return m_LogLevel;
	}
	LPCTSTR GetLogFileName()
	{
		return m_LogFileName;
	}

	void SetLogMode(UINT Mode,int Level,LPCTSTR FileName,int FileLogQueueLen=DEFAULT_FILE_LOG_BUFFER_LEN);

	virtual void PrintLog(int Level,DWORD Color,LPCTSTR Format,va_list vl);

	void OutputLogString(int Level,DWORD Color,LPCTSTR szLog);


};
