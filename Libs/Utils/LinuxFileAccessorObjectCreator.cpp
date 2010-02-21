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