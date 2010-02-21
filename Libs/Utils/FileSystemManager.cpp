#include "StdAfx.h"


CFileSystemManager::CFileSystemManager(void)
{
#ifdef WIN32
	m_FileObjectCreators[FILE_CHANNEL_NORMAL1]=new CWinFileAccessorObjectCreator();
#else 
	m_FileObjectCreators[FILE_CHANNEL_NORMAL1]=new CLinuxFileAccessorObjectCreator();
#endif
	m_FileObjectCreators[FILE_CHANNEL_NORMAL2]=new CStandardFileObjectCreator();
	m_FileObjectCreators[FILE_CHANNEL_NORMAL3]=new CStandardFileObjectCreator();
}

CFileSystemManager::~CFileSystemManager(void)
{
	for(int i=0;i<FILE_CHANNEL_MAX;i++)
		SAFE_RELEASE(m_FileObjectCreators[i]);
}

void CFileSystemManager::SetFileObjectCreator(int Channel,IFileObjectCreator * pFileObjectCreator)
{
	if(Channel<0||Channel>=FILE_CHANNEL_MAX)
		return;
	SAFE_RELEASE(m_FileObjectCreators[Channel]);
	m_FileObjectCreators[Channel]=pFileObjectCreator;
}

IFileAccessor * CFileSystemManager::CreateFileAccessor(int Channel)
{
	if(Channel<0||Channel>=FILE_CHANNEL_MAX)
		return NULL;
	if(m_FileObjectCreators[Channel]==NULL)
		return NULL;
	return m_FileObjectCreators[Channel]->CreateFileAccessor();
}

