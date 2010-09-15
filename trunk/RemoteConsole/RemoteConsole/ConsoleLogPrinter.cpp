/****************************************************************************/
/*                                                                          */
/*      �ļ���:    ConsoleLogPrinter.cpp                                    */
/*      ��������:  2009��02��09��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
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