/****************************************************************************/
/*                                                                          */
/*      文件名:    MPQFileAccessor.cpp                                      */
/*      创建日期:  2011年10月07日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#include "StdAfx.h"

IMPLEMENT_CLASS_INFO_STATIC(CMPQFileAccessor,IFileAccessor);

CMPQFileAccessor::CMPQFileAccessor(CMPQFileSystem * pManager)
{
	m_pManager=pManager;
	m_pRealFSFile=NULL;
	m_hMPQ=NULL;
	m_hFile=NULL;
}

CMPQFileAccessor::~CMPQFileAccessor(void)
{
	Close();
}

BOOL CMPQFileAccessor::Open(LPCTSTR FileName,int OpenMode)
{
	Close();
	m_hMPQ=m_pManager->FindFileMPQ(FileName);
	if(m_hMPQ)
	{		
		CEasyStringA FName=FileName;
		if(SFileOpenFileEx(m_hMPQ,FName,0,&m_hFile))
		{
			return TRUE;
		}
	}
	else
	{
		m_pRealFSFile=CFileSystemManager::GetInstance()->CreateFileAccessor(FILE_CHANNEL_NORMAL1);
		if(m_pRealFSFile)
		{
			return m_pRealFSFile->Open(FileName,OpenMode);
		}
	}
	return FALSE;
}
void CMPQFileAccessor::Close()
{
	if(m_hFile)
	{
		SFileCloseFile(m_hFile);
	}
	SAFE_RELEASE(m_pRealFSFile);
	m_hMPQ=NULL;
	m_hFile=NULL;
}

ULONG64 CMPQFileAccessor::GetSize()
{
	if(m_hFile)
	{
		ULONG64_CONVERTER FileSize;
		FileSize.LowPart=SFileGetFileSize(m_hFile,&FileSize.HighPart);
		if(FileSize.LowPart==0xFFFFFFFF)
		{
			if(GetLastError()==NO_ERROR)
				return FileSize.QuadPart;
			else
				return 0;
		}
		else
		{
			return FileSize.QuadPart;
		}				
	}
	else if(m_pRealFSFile)
	{
		return m_pRealFSFile->GetSize();
	}
	return 0;	
}

ULONG64 CMPQFileAccessor::Read(LPVOID pBuff,ULONG64 Size)
{
	if(m_hFile)
	{
		DWORD ReadSize;
		if(SFileReadFile(m_hFile,pBuff,Size,&ReadSize))
			return ReadSize;
	}
	else if(m_pRealFSFile)
	{
		return m_pRealFSFile->Read(pBuff,Size);
	}
	return 0;
}
ULONG64 CMPQFileAccessor::Write(LPCVOID pBuff,ULONG64 Size)
{
	return 0;
}


BOOL CMPQFileAccessor::IsEOF()
{
	if(m_hFile)
	{
		return GetCurPos()==GetSize();
	}
	else if(m_pRealFSFile)
	{
		return m_pRealFSFile->IsEOF();
	}
	return TRUE;
}

BOOL CMPQFileAccessor::Seek(LONG64 Offset,int SeekMode)
{
	if(m_hFile)
	{
		ULONG64_CONVERTER FilePos;
		FilePos.QuadPart=Offset;
		FilePos.LowPart=SFileSetFilePointer(m_hFile,FilePos.LowPart,(LONG *)&FilePos.HighPart,SeekMode);
		if(FilePos.LowPart==0xFFFFFFFF&&GetLastError()!=NO_ERROR)
			return false;
		else
			return true;
	}
	else if(m_pRealFSFile)
	{
		return m_pRealFSFile->Seek(Offset,SeekMode);
	}
	return false;
}
ULONG64 CMPQFileAccessor::GetCurPos()
{
	if(m_hFile)
	{
		ULONG64_CONVERTER FilePos;
		FilePos.QuadPart=0;
		FilePos.LowPart=SFileSetFilePointer(m_hFile,FilePos.LowPart,(LONG *)&FilePos.HighPart,FILE_CURRENT);
		if(FilePos.LowPart==0xFFFFFFFF)
		{
			if(GetLastError()==NO_ERROR)
				return FilePos.QuadPart;
			else
				return 0;
		}
		else
		{
			return FilePos.QuadPart;
		}		
	}
	else if(m_pRealFSFile)
	{
		return m_pRealFSFile->GetCurPos();
	}
	return 0;
}

BOOL CMPQFileAccessor::SetCreateTime(const CEasyTime& Time)
{
	return FALSE;
}
BOOL CMPQFileAccessor::GetCreateTime(CEasyTime& Time)
{
	if(m_hFile)
	{
		FILETIME MPQFileTime;
		if(SFileGetFileInfo(m_hFile,SFILE_INFO_FILETIME,&MPQFileTime,sizeof(FILETIME)))
		{
			Time=MPQFileTime;
			return TRUE;
		}
	}
	else if(m_pRealFSFile)
	{
		return m_pRealFSFile->GetCreateTime(Time);
	}
	return FALSE;
}

BOOL CMPQFileAccessor::SetLastAccessTime(const CEasyTime& Time)
{
	return FALSE;
}
BOOL CMPQFileAccessor::GetLastAccessTime(CEasyTime& Time)
{
	if(m_hFile)
	{
		FILETIME MPQFileTime;
		if(SFileGetFileInfo(m_hFile,SFILE_INFO_FILETIME,&MPQFileTime,sizeof(FILETIME)))
		{
			Time=MPQFileTime;
			return TRUE;
		}
	}
	else if(m_pRealFSFile)
	{
		return m_pRealFSFile->GetLastAccessTime(Time);
	}
	return FALSE;
}

BOOL CMPQFileAccessor::SetLastWriteTime(const CEasyTime& Time)
{
	return FALSE;
}
BOOL CMPQFileAccessor::GetLastWriteTime(CEasyTime& Time)
{
	if(m_hFile)
	{
		FILETIME MPQFileTime;
		if(SFileGetFileInfo(m_hFile,SFILE_INFO_FILETIME,&MPQFileTime,sizeof(FILETIME)))
		{
			Time=MPQFileTime;
			return TRUE;
		}
	}
	else if(m_pRealFSFile)
	{
		return m_pRealFSFile->GetLastWriteTime(Time);
	}
	return FALSE;
}
