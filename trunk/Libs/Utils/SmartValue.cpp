/****************************************************************************/
/*                                                                          */
/*      �ļ���:    SmartValue.cpp                                           */
/*      ��������:  2009��09��11��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#include "StdAfx.h"

bool CSmartValue::m_ConvertWideCharToUTF8=false;

int CSmartValue::GetTypeFromData(LPCVOID pData,UINT DataSize)
{
	if(pData==NULL||DataSize<sizeof(BYTE))
		return VT_UNKNOWN;

	return *((BYTE *)pData);
}
void CSmartValue::EnableConvertWideCharToUTF8(bool Enable)
{
	m_ConvertWideCharToUTF8=Enable;
}
bool CSmartValue::IsConvertWideCharToUTF8()
{
	return m_ConvertWideCharToUTF8;
}