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

void CVSOutputLogPrinter::PrintLog(int Level,DWORD Color,LPCTSTR Format,va_list vl)
{
#ifdef WIN32
	char MsgBuff[2048];
	vsprintf_s( MsgBuff, 2048, Format, vl );
	OutputDebugString(MsgBuff);
	OutputDebugString("\r\n");
#else
	vprintf(Format,vl);
	printf("\n");
#endif
}
