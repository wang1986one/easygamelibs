/****************************************************************************/
/*                                                                          */
/*      �ļ���:    FileLogPrinter.h                                         */
/*      ��������:  2009��09��11��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once


enum FILE_LOG_MODE
{
	FILE_LOG_APPEND=1,
	FILE_LOG_SAFE_WRITE=(1<<1),
	FILE_LOG_SPLIT_BY_DAY=(1<<2)
};

class IFileAccessor;

class CFileLogPrinter :
	public ILogPrinter
{
protected:
	TCHAR			m_MsgBuff[40960];
	DWORD			m_Flag;
	IFileAccessor *	m_pFileAccessor;
	CEasyTime		m_RecentLogTime;
	int				m_FileOpenMode;
	CEasyString		m_LogFileName;
	UINT			m_LogLevel;

	CEasyCriticalSection	m_EasyCriticalSection;
public:
	CFileLogPrinter();
	CFileLogPrinter(LPCTSTR FileName,DWORD Flag);
	~CFileLogPrinter(void);

	bool Create(LPCTSTR FileName,DWORD Flag);

	virtual void PrintLogVL(int Level,DWORD Color,LPCTSTR Format,va_list vl);
};
