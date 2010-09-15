/****************************************************************************/
/*                                                                          */
/*      文件名:    ConsoleLogPrinter.cpp                                    */
/*      创建日期:  2009年02月09日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#include "StdAfx.h"
#include ".\consolelogprinter.h"

CConsoleLogPrinter::CConsoleLogPrinter(void)
{
}

CConsoleLogPrinter::~CConsoleLogPrinter(void)
{
}


void CConsoleLogPrinter::PrintLogVL(int Level,DWORD Color,LPCTSTR Format,va_list vl)
{
	char MsgBuff[4096];	

	_vsnprintf_s(MsgBuff,sizeof(MsgBuff),sizeof(MsgBuff), Format, vl );
	MsgBuff[4095]=0;	

	CControlPanel::GetInstance()->PushMsg(MsgBuff);
	
}