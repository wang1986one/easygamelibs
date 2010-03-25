/****************************************************************************/
/*                                                                          */
/*      �ļ���:    FileSearcherWin.h                                        */
/*      ��������:  2009��07��06��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once

class CFileSearcher 
{
protected:
	HANDLE				m_hFile;
	WIN32_FIND_DATA		m_FindedFileInfo;
	WIN32_FIND_DATA		m_FindWorkData;
	bool				m_IsFinded;
	char				m_SearchDir[MAX_PATH];
	char				m_LogicalDriveNames[MAX_PATH];
	UINT				m_LogicDriveCount;
public:
	CFileSearcher(void);
	~CFileSearcher(void);

	BOOL FindFirst(LPCTSTR FindPattern);
	BOOL FindNext();
	void Close();

	CEasyString GetFileName();
	CEasyString GetFilePath();

	CEasyString GetFileDirect();
	CEasyString GetFileTitle();
	CEasyString GetFileExt();
	CEasyString GetFileURL();
	DWORD GetFileAttributes();

	ULONG64 GetFileSize();

	CEasyTime GetLastWriteTime();
	CEasyTime GetLastAccessTime();
	CEasyTime GetCreationTime();

	BOOL IsDots();

	BOOL MatchesMask(DWORD dwMask);

	BOOL IsReadOnly();
	BOOL IsDirectory();
	BOOL IsCompressed();
	BOOL IsSystem();
	BOOL IsHidden();
	BOOL IsTemporary();
	BOOL IsNormal();
	BOOL IsArchived();

	int FetchLogicalDrive();
	int GetLogicalDriveCount();
	CEasyString GetLogicDriveName(UINT Index);
	
};

inline DWORD CFileSearcher::GetFileAttributes()
{
	return m_FindedFileInfo.dwFileAttributes;
}

inline BOOL CFileSearcher::IsReadOnly()
{ return MatchesMask(FILE_ATTRIBUTE_READONLY); }
inline BOOL CFileSearcher::IsDirectory()
{ return MatchesMask(FILE_ATTRIBUTE_DIRECTORY); }
inline BOOL CFileSearcher::IsCompressed()
{ return MatchesMask(FILE_ATTRIBUTE_COMPRESSED); }
inline BOOL CFileSearcher::IsSystem()
{ return MatchesMask(FILE_ATTRIBUTE_SYSTEM); }
inline BOOL CFileSearcher::IsHidden()
{ return MatchesMask(FILE_ATTRIBUTE_HIDDEN); }
inline BOOL CFileSearcher::IsTemporary()
{ return MatchesMask(FILE_ATTRIBUTE_TEMPORARY); }
inline BOOL CFileSearcher::IsNormal()
{ return MatchesMask(FILE_ATTRIBUTE_NORMAL); }
inline BOOL CFileSearcher::IsArchived()
{ return MatchesMask(FILE_ATTRIBUTE_ARCHIVE); }
