/****************************************************************************/
/*                                                                          */
/*      �ļ���:    ConsoleLogPrinter.h                                      */
/*      ��������:  2009��02��09��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
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
