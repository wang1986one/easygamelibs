/****************************************************************************/
/*                                                                          */
/*      文件名:    ConsoleLogPrinter.h                                      */
/*      创建日期:  2009年02月09日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once

class CConsoleLogPrinter :
	public ILogPrinter
{
public:
	CConsoleLogPrinter(void);
	virtual ~CConsoleLogPrinter(void);

	virtual void PrintLog(int Level,DWORD Color,LPCTSTR Format,va_list vl);
};
