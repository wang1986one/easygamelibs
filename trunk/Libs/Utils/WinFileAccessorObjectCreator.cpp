/****************************************************************************/
/*                                                                          */
/*      �ļ���:    WinFileAccessorObjectCreator.cpp                         */
/*      ��������:  2009��07��06��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#include "StdAfx.h"
#include ".\winfileaccessorobjectcreator.h"

IMPLEMENT_CLASS_INFO_STATIC(CWinFileAccessorObjectCreator,IFileObjectCreator);

CWinFileAccessorObjectCreator::CWinFileAccessorObjectCreator(void)
{
}

CWinFileAccessorObjectCreator::~CWinFileAccessorObjectCreator(void)
{
}

IFileAccessor * CWinFileAccessorObjectCreator::CreateFileAccessor()
{
	return new CWinFileAccessor();
}