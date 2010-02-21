#pragma once
#include <vector>

class CFileSearcher 
{
protected:
	struct FILE_INFO
	{
		struct dirent		FileInfo;
		struct stat			FileStat;	
	};	
	std::vector<FILE_INFO>	m_FileInfoList;	
	CEasyString				m_SearchDir;
	int						m_FetchIndex;
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

	BOOL CanRead();
	BOOL CanWrite();
	BOOL CanExec();

	int FetchLogicalDrive();
	int GetLogicalDriveCount();
	CEasyString GetLogicDriveName(UINT Index);
	
};




