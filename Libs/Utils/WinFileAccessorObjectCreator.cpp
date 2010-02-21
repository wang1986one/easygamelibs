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