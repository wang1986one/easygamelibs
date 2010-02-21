#include "StdAfx.h"

IMPLEMENT_CLASS_INFO_STATIC(CResourceFileObjectCreator,IFileObjectCreator);

CResourceFileObjectCreator::CResourceFileObjectCreator(void)
{
}

CResourceFileObjectCreator::~CResourceFileObjectCreator(void)
{
}

IFileAccessor * CResourceFileObjectCreator::CreateFileAccessor()
{
	return new CResourceFileAccessor();
}