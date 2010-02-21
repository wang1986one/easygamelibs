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

