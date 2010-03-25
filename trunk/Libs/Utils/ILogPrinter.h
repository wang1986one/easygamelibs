/****************************************************************************/
/*                                                                          */
/*      �ļ���:    ILogPrinter.h                                            */
/*      ��������:  2009��07��06��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
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