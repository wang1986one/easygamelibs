/****************************************************************************/
/*                                                                          */
/*      �ļ���:    FilePathManager.h                                        */
/*      ��������:  2009��07��14��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once

typedef CEasyMap<CEasyString,CEasyString> CFilePathList;

#define DECLARE_FILE_PATH_MANAGER  \
	protected:\
	static CFilePathList m_FilePathList;\
	public:\
		static void AddPath(LPCTSTR Path);\
		static void ClearPath();\
		static CEasyString FindFileOne(LPCTSTR FileName);\
		static CFilePathList& GetFilePathList();\
		static void AddPathList(CFilePathList& PathList);\
		static void AddPathList(CEasyArray<CEasyString>& PathList);

#define DECLARE_FILE_CHANNEL_MANAGER  \
	protected:\
			  static int			m_FileChannel;\
	public:\
		   static void SetFileChannel(int Channel);\
		   static int GetFileChannel();\
		   static IFileAccessor * CreateFileAccessor();



#define IMPLEMENT_FILE_PATH_MANAGER(ClassName) \
	CFilePathList ClassName::m_FilePathList;\
	void ClassName::AddPath(LPCTSTR Path)\
	{	\
		CEasyString ThePath(Path);\
		ThePath.MakeUpper();\
		m_FilePathList.Insert(ThePath,ThePath);\
	}\
	void ClassName::ClearPath()\
	{\
		m_FilePathList.Clear();\
	}\
	CEasyString ClassName::FindFileOne(LPCTSTR FileName)\
	{\
		CFileSearcher FileFind;\
		if(FileFind.FindFirst(FileName))\
		{\
			FileFind.FindNext();\
			return FileFind.GetFilePath();\
		}\
		void * Pos=m_FilePathList.GetFirstObjectPos();\
		while(Pos)\
		{\
			CEasyString Key;\
			m_FilePathList.GetNextObject(Pos,Key);\
			if(FileFind.FindFirst(Key+_T("/")+FileName))\
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
	void ClassName::AddPathList(CFilePathList& PathList)\
	{\
		void * Pos=PathList.GetFirstObjectPos();\
		while(Pos)\
		{\
			CEasyString Key;\
			PathList.GetNextObject(Pos,Key);\
			m_FilePathList.Insert(Key,Key);\
		}\
	}\
	void ClassName::AddPathList(CEasyArray<CEasyString>& PathList)\
	{\
		for(UINT i=0;i<PathList.GetCount();i++)\
		{\
			CEasyString ThePath(PathList[i]);\
			ThePath.MakeUpper();\
			m_FilePathList.Insert(ThePath,ThePath);\
		}\
	}

#define IMPLEMENT_FILE_CHANNEL_MANAGER(ClassName) \
	int	ClassName::m_FileChannel=0;\
	void ClassName::SetFileChannel(int Channel)\
	{\
		m_FileChannel=Channel;\
	}\
	int ClassName::GetFileChannel()\
	{\
		return m_FileChannel;\
	}\
	IFileAccessor * ClassName::CreateFileAccessor()\
	{\
		return CFileSystemManager::GetInstance()->CreateFileAccessor(m_FileChannel);\
	}
