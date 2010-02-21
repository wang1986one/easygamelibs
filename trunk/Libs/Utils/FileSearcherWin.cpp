#include "StdAfx.h"


CFileSearcher::CFileSearcher(void)
{
	m_hFile=NULL;
	ZeroMemory(&m_FindedFileInfo,sizeof(m_FindedFileInfo));
	ZeroMemory(&m_FindWorkData,sizeof(m_FindWorkData));
	m_IsFinded=false;
	m_LogicDriveCount=0;
}

CFileSearcher::~CFileSearcher(void)
{
	Close();
}

BOOL CFileSearcher::FindFirst(LPCTSTR FindPattern)
{
	Close();
	m_hFile=FindFirstFile(FindPattern,&m_FindWorkData);
	if(m_hFile==INVALID_HANDLE_VALUE)
	{
		m_hFile=NULL;
		return FALSE;
	}
	m_IsFinded=true;
	char FullPath[MAX_PATH];
	if(_tfullpath(FullPath,FindPattern,MAX_PATH)==NULL)
	{
		Close();
		return FALSE;
	}
	char Dir[MAX_PATH];
	_splitpath_s(FullPath,m_SearchDir,MAX_PATH,Dir,MAX_PATH,NULL,0,NULL,0);
	strcat_s(m_SearchDir,MAX_PATH,Dir);
	int len=(int)strlen(m_SearchDir);
	if(m_SearchDir[len-1]!='\\'&&m_SearchDir[len-1]!='/')
		strcat_s(m_SearchDir,MAX_PATH,"\\");
	return TRUE;
}

BOOL CFileSearcher::FindNext()
{	
	if(m_IsFinded)
	{
		m_FindedFileInfo=m_FindWorkData;
		if(!FindNextFile(m_hFile,&m_FindWorkData))
		{
			m_IsFinded=false;
		}
		return TRUE;
	}
	return FALSE;
}

void CFileSearcher::Close()
{
	if(m_hFile)
	{
		FindClose(m_hFile);
		m_hFile=NULL;
		m_IsFinded=false;
		ZeroMemory(&m_FindedFileInfo,sizeof(m_FindedFileInfo));
		ZeroMemory(&m_FindWorkData,sizeof(m_FindWorkData));
	}
}

CEasyString CFileSearcher::GetFileName()
{
	return m_FindedFileInfo.cFileName;
}

CEasyString CFileSearcher::GetFilePath()
{
	return m_SearchDir+GetFileName();
}

CEasyString CFileSearcher::GetFileDirect()
{
	return m_SearchDir;
}

CEasyString CFileSearcher::GetFileTitle()
{
	char Title[MAX_PATH];

	_tsplitpath_s(m_FindedFileInfo.cFileName,NULL,0,NULL,0,Title,MAX_PATH,NULL,0);
	return Title;
}

CEasyString CFileSearcher::GetFileExt()
{
	char Ext[MAX_PATH];

	_tsplitpath_s(m_FindedFileInfo.cFileName,NULL,0,NULL,0,NULL,0,Ext,MAX_PATH);
	return Ext;
}

CEasyString CFileSearcher::GetFileURL()
{
	return "file://"+GetFilePath();
}

ULONG64 CFileSearcher::GetFileSize()
{
	ULONG64_CONVERTER Size;

	Size.LowPart = m_FindedFileInfo.nFileSizeLow;
	Size.HighPart = m_FindedFileInfo.nFileSizeHigh;

	return Size.QuadPart;
}

CEasyTime CFileSearcher::GetLastWriteTime()
{
	FILETIME LocalTime;

	FileTimeToLocalFileTime(&m_FindedFileInfo.ftLastWriteTime,&LocalTime);
	return LocalTime;
}

CEasyTime CFileSearcher::GetLastAccessTime()
{
	FILETIME LocalTime;

	FileTimeToLocalFileTime(&m_FindedFileInfo.ftLastAccessTime,&LocalTime);
	return LocalTime;
}

CEasyTime CFileSearcher::GetCreationTime()
{
	FILETIME LocalTime;

	FileTimeToLocalFileTime(&m_FindedFileInfo.ftCreationTime,&LocalTime);
	return LocalTime;
}

BOOL CFileSearcher::MatchesMask(DWORD dwMask)
{
	return (m_FindedFileInfo.dwFileAttributes & dwMask);
}

BOOL CFileSearcher::IsDots()
{	
	if (IsDirectory())
	{		
		if (m_FindedFileInfo.cFileName[0] == '.')
		{
			if (m_FindedFileInfo.cFileName[1] == '\0' ||
				(m_FindedFileInfo.cFileName[1] == '.' &&
				m_FindedFileInfo.cFileName[2] == '\0'))
			{
				return TRUE;
			}
		}
	}

	return FALSE;
}

int CFileSearcher::FetchLogicalDrive()
{	
	m_LogicDriveCount=GetLogicalDriveStrings(MAX_PATH,m_LogicalDriveNames)/4;
	return m_LogicDriveCount;
}

int CFileSearcher::GetLogicalDriveCount()
{
	return m_LogicDriveCount;
}

CEasyString CFileSearcher::GetLogicDriveName(UINT Index)
{
	if(Index<m_LogicDriveCount)
		return m_LogicalDriveNames+Index*4;
	else
		return "";
}