#pragma once

typedef std::vector<CEasyString> CFilePathList;

#define DECLARE_FILE_PATH_MANAGER  \
	protected:\
	static CFilePathList m_FilePathList;\
	public:\
		static void AddPath(LPCTSTR Path);\
		static void ClearPath();\
		static CEasyString FindFileOne(LPCTSTR FileName);\
		static CFilePathList& GetFilePathList();\
		static void AppendPathList(CFilePathList& PathList);\
		static void AssignPathList(CFilePathList& PathList);

#define DECLARE_FILE_CHANNEL_MANAGER  \
	protected:\
			  static int			m_FileChannel;\
	public:\
		   static void SetFileChannel(int Channel);\
		   static IFileAccessor * CreateFileAccessor();



#define IMPLEMENT_FILE_PATH_MANAGER(ClassName) \
	CFilePathList ClassName::m_FilePathList;\
	void ClassName::AddPath(LPCTSTR Path)\
	{	\
		m_FilePathList.push_back(CEasyString(Path));\
	}\
	void ClassName::ClearPath()\
	{\
		m_FilePathList.clear();\
	}\
	CEasyString ClassName::FindFileOne(LPCTSTR FileName)\
	{\
		CFileSearcher FileFind;\
		if(FileFind.FindFirst(FileName))\
		{\
			FileFind.FindNext();\
			return FileFind.GetFilePath();\
		}\
		for(int i=0;i<(int)m_FilePathList.size();i++)\
		{\
			if(FileFind.FindFirst(m_FilePathList[i]+"/"+FileName))\
			{\
				FileFind.FindNext();	\
				return FileFind.GetFilePath();\
			}\
		}\
		return FileName;\
	}\
	CFilePathList& ClassName::GetFilePathList()\
	{\
		return m_FilePathList;\
	}\
	void ClassName::AppendPathList(CFilePathList& PathList)\
	{\
		for(int i=0;i<(int)PathList.size();i++)\
		{\
			m_FilePathList.push_back(PathList[i]);\
		}\
	}\
	void ClassName::AssignPathList(CFilePathList& PathList)\
	{\
		m_FilePathList.assign(PathList.begin(),PathList.end());\
	}

#define IMPLEMENT_FILE_CHANNEL_MANAGER(ClassName) \
	int	ClassName::m_FileChannel=0;\
	void ClassName::SetFileChannel(int Channel)\
	{\
		m_FileChannel=Channel;\
	}\
	IFileAccessor * ClassName::CreateFileAccessor()\
	{\
		return CFileSystemManager::GetInstance()->CreateFileAccessor(m_FileChannel);\
	}
