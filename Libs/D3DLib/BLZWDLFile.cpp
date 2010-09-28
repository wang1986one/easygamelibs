#include "StdAfx.h"


namespace D3DLib{

IMPLEMENT_FILE_CHANNEL_MANAGER(CBLZWDLFile)

CBLZWDLFile::CBLZWDLFile(void)
{
}

CBLZWDLFile::~CBLZWDLFile(void)
{
}

bool CBLZWDLFile::Load(LPCTSTR FileName)
{
	IFileAccessor * pFile;

	pFile=CreateFileAccessor();
	if(pFile==NULL)
		return false;
	if(!pFile->Open(FileName,IFileAccessor::modeRead))
	{
		pFile->Release();
		return false;	
	}

	bool Ret=Load(pFile);
	pFile->Release();
	return Ret;
}

bool CBLZWDLFile::Load(IFileAccessor * pFile)
{
	CBLZChunkFile WDLFile;

	if(!WDLFile.Load(pFile))
	{
		return false;
	}


	return true;
}

}