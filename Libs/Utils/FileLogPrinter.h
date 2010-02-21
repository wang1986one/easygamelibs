#pragma once
#include "ilogprinter.h"

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
	char			m_MsgBuff[40960];
	DWORD			m_Flag;
	IFileAccessor *	m_pFileAccessor;
	CEasyTime		m_RecentLogTime;
	int				m_FileOpenMode;
	CEasyString		m_LogFileName;
	UINT			m_LogLevel;

	CEasyCriticalSection	m_EasyCriticalSection;
public:
	CFileLogPrinter(LPCTSTR FileName,DWORD Flag);
	~CFileLogPrinter(void);

	virtual void PrintLog(int Level,DWORD Color,LPCTSTR Format,va_list vl);
};
