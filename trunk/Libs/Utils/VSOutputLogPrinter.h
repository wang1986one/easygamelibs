/****************************************************************************/
/*                                                                          */
/*      �ļ���:    VSOutputLogPrinter.h                                     */
/*      ��������:  2009��07��06��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once


class CVSOutputLogPrinter :
	public ILogPrinter
{
protected:
	

	DECLARE_CLASS_INFO_STATIC(CVSOutputLogPrinter);
public:
	CVSOutputLogPrinter(void);
	virtual ~CVSOutputLogPrinter(void);

	virtual void PrintLogVL(int Level,DWORD Color,LPCTSTR Format,va_list vl);
};
