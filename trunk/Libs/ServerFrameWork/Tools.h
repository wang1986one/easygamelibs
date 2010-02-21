#pragma once

extern LPCTSTR g_ProgramName;
extern LPCTSTR g_ServiceName;
extern LPCTSTR g_ServiceDesc;
extern WORD g_ProgramVersion[4];
extern int g_ServiceWorkMode;

enum SERVICE_WORK_MODE
{
	SWM_SERVICE,
	SWM_APPLICATION,
};



inline void Log(const char* Format, ...)
{
	
	
	va_list vl;

	va_start(vl, Format);
	CLogManager::GetInstance()->PrintLogVL(SERVER_LOG_CHANNEL,ILogPrinter::LOG_LEVEL_NORMAL,0,Format,vl);
	va_end(vl);
	
	
	
}

inline void LogDebug(const char* Format, ...)
{
	va_list vl;

	va_start(vl, Format);
	CLogManager::GetInstance()->PrintLogVL(SERVER_LOG_CHANNEL,ILogPrinter::LOG_LEVEL_DEBUG,0,Format,vl);
	va_end(vl);
}


inline void LogServerInfo(const char* Format, ...)
{	
	va_list vl;

	va_start(vl, Format);
	CLogManager::GetInstance()->PrintLogVL(SERVER_STATUS_LOG_CHANNEL,ILogPrinter::LOG_LEVEL_NORMAL,0,Format,vl);
	va_end(vl);

	

}

