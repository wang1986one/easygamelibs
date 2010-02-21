#pragma once
#include "ilogprinter.h"

class CVSOutputLogPrinter :
	public ILogPrinter
{
protected:
	

	DECLARE_CLASS_INFO_STATIC(CVSOutputLogPrinter);
public:
	CVSOutputLogPrinter(void);
	virtual ~CVSOutputLogPrinter(void);

	virtual void PrintLog(int Level,DWORD Color,LPCTSTR Format,va_list vl);
};
