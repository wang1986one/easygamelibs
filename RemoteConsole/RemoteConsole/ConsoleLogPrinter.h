#pragma once

class CConsoleLogPrinter :
	public ILogPrinter
{
public:
	CConsoleLogPrinter(void);
	virtual ~CConsoleLogPrinter(void);

	virtual void PrintLog(int Level,DWORD Color,LPCTSTR Format,va_list vl);
};
