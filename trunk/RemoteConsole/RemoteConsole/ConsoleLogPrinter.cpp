#include "StdAfx.h"
#include ".\consolelogprinter.h"

CConsoleLogPrinter::CConsoleLogPrinter(void)
{
}

CConsoleLogPrinter::~CConsoleLogPrinter(void)
{
}


void CConsoleLogPrinter::PrintLog(int Level,DWORD Color,LPCTSTR Format,va_list vl)
{
	char MsgBuff[4096];	

	_vsnprintf(MsgBuff,sizeof(MsgBuff), Format, vl );
	MsgBuff[4095]=0;	

	CControlPanel::GetInstance()->PushMsg(MsgBuff);
	
}