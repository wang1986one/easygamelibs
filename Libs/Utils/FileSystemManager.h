#pragma once


enum FILE_CHANNELS
{
	FILE_CHANNEL_NORMAL1,
	FILE_CHANNEL_NORMAL2,	
	FILE_CHANNEL_NORMAL3,
	FILE_CHANNEL_MAX,
};


class CFileSystemManager:public CStaticObject3<CFileSystemManager>
{
protected:
	IFileObjectCreator *		m_FileObjectCreators[FILE_CHANNEL_MAX];
	
public:
	CFileSystemManager(void);
	~CFileSystemManager(void);

	void SetFileObjectCreator(int Channel,IFileObjectCreator * pFileObjectCreator);
	IFileAccessor * CreateFileAccessor(int Channel);

	
};
