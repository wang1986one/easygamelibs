/****************************************************************************/
/*                                                                          */
/*      �ļ���:    LinuxFileAccessorObjectCreator.cpp                       */
/*      ��������:  2009��07��06��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#include "StdAfx.h"

IMPLEMENT_CLASS_INFO_STATIC(CLinuxFileAccessorObjectCreator,IFileObjectCreator);
CLinuxFileAccessorObjectCreator::CLinuxFileAccessorObjectCreator(void)
{
}

CLinuxFileAccessorObjectCreator::~CLinuxFileAccessorObjectCreator(void)
{
}

IFileAccessor * CLinuxFileAccessorObjectCreator::CreateFileAccessor()
{
	return new CLinuxFileAccessor();
}