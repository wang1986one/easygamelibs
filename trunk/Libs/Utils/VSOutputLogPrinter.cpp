/****************************************************************************/
/*                                                                          */
/*      �ļ���:    VSOutputLogPrinter.cpp                                   */
/*      ��������:  2009��07��06��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#include "StdAfx.h"

IMPLEMENT_CLASS_INFO_STATIC(CVSOutputLogPrinter,CNameObject);

CVSOutputLogPrinter::CVSOutputLogPrinter(void):ILogPrinter()
{
}

CVSOutputLogPrinter::~CVSOutputLogPrinter(void)
{
}

void CVSOutputLogPrinter::PrintLogVL(int Level,DWORD Color,LPCTSTR Format,va_list vl)
{
#ifdef WIN32
	TCHAR MsgBuff[4096];
	_vstprintf_s( MsgBuff, 4096, Format, vl );
	OutputDebugString(MsgBuff);
	OutputDebugString(_T("\r\n"));
#else
	vprintf(Format,vl);
	printf(_T("\n"));
#endif
}
