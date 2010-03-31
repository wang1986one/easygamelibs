/****************************************************************************/
/*                                                                          */
/*      �ļ���:    ServerLogPrinter.h                                       */
/*      ��������:  2009��07��06��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once


class CServerThread;

class CServerLogPrinter :
	public CAsyncFileLogPrinter
{
protected:
	IBaseServer *			m_pServer;
	UINT					m_LogOutputMode;
	
public:
	enum LOG_OUTPUT_MODE
	{
		LOM_VS=1,
		LOM_CONSOLE=(1<<1),
		LOM_FILE=(1<<2),
	};

	CServerLogPrinter(IBaseServer * pServer,UINT Mode,int Level,LPCTSTR FileName,int FileLogBufferLen=DEFAULT_ASYNC_FILE_LOG_BUFFER_LEN);
	virtual ~CServerLogPrinter(void);

	UINT GetLogMode()
	{
		return m_LogOutputMode;
	}
	

	void SetLogMode(UINT Mode,int Level,LPCTSTR FileName,int FileLogQueueLen=DEFAULT_ASYNC_FILE_LOG_BUFFER_LEN);


	virtual void PrintLog(int Level,DWORD Color,LPCTSTR Format,va_list vl);

};
