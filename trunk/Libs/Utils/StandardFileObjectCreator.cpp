/****************************************************************************/
/*                                                                          */
/*      �ļ���:    StandardFileObjectCreator.cpp                            */
/*      ��������:  2009��07��06��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#include "StdAfx.h"


IMPLEMENT_CLASS_INFO_STATIC(CStandardFileObjectCreator,IFileObjectCreator);

CStandardFileObjectCreator::CStandardFileObjectCreator(void):IFileObjectCreator()
{
}

CStandardFileObjectCreator::~CStandardFileObjectCreator(void)
{
}

IFileAccessor * CStandardFileObjectCreator::CreateFileAccessor()
{
	return new CStandardFileAccessor();
}

