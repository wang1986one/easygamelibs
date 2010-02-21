#pragma once

class ILogPrinter:public CNameObject
{
public:
	enum LOG_LEVEL
	{
		LOG_LEVEL_NORMAL=(1),
		LOG_LEVEL_DEBUG=(1<<1),
	};
	ILogPrinter(void):CNameObject(){};
	virtual ~ILogPrinter(void){};
	virtual void PrintLog(int Level,DWORD Color,LPCTSTR Format,va_list vl)=0;

};