/****************************************************************************/
/*                                                                          */
/*      �ļ���:    USOObjectCreateFilter.cpp                                */
/*      ��������:  2009��07��06��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#include "StdAfx.h"

IMPLEMENT_CLASS_INFO(CUSOObjectCreateFilter,CNameObject);

CUSOObjectCreateFilter::CUSOObjectCreateFilter(void)
{
}

CUSOObjectCreateFilter::~CUSOObjectCreateFilter(void)
{
}

bool CUSOObjectCreateFilter::OnObjectCreate(CNameObject * pObject)
{
	return true;
}

CNameObject * CUSOObjectCreateFilter::FindObject(CLASS_INFO * pClassInfo,LPCTSTR ObjectName)
{
	return NULL;
}
