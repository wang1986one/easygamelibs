/****************************************************************************/
/*                                                                          */
/*      文件名:    MPQFileSystem.cpp                                        */
/*      创建日期:  2011年10月07日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#include "StdAfx.h"

IMPLEMENT_CLASS_INFO_STATIC(CMPQFileSystem,IFileObjectCreator);

CMPQFileSystem::CMPQFileSystem(void)
{
}

CMPQFileSystem::~CMPQFileSystem(void)
{
	Destory();
}

void CMPQFileSystem::Destory()
{
	ClearAllMPQ();
	CNameObject::Destory();
}

IFileAccessor * CMPQFileSystem::CreateFileAccessor()
{
	return new CMPQFileAccessor(this);
}

bool CMPQFileSystem::AddMPQ(LPCTSTR szMPQFileName)
{
	HANDLE hMPQ=NULL;

	if(SFileOpenArchive(szMPQFileName,0,0,&hMPQ))
	{
		m_MPQList.Add(hMPQ);
	}
	return false;
}

void CMPQFileSystem::ClearAllMPQ()
{
	for(UINT i=0;i<m_MPQList.GetCount();i++)
	{
		SFileCloseArchive(m_MPQList[i]);
	}
	m_MPQList.Clear();
}

HANDLE CMPQFileSystem::FindFileMPQ(LPCTSTR szFileName)
{
	for(int i=m_MPQList.GetCount()-1;i>=0;i--)
	{
		CEasyStringA FName=szFileName;
		if(SFileHasFile(m_MPQList[i],FName))
			return m_MPQList[i];
	}
	return NULL;
}

void CMPQFileSystem::GetFileList(CEasyArray<CEasyString>& FileNameList)
{
	CEasyMap<CEasyString,CEasyString>	FileNameMap;
	CEasyBuffer ListFileBuffer;
	CStringFile StringFile;

	for(int j=m_MPQList.GetCount()-1;j>=0;j--)
	{
		HANDLE hFile;
		if( SFileOpenFileEx(m_MPQList[j],"(listfile)",0,&hFile))
		{
			DWORD FileSize=SFileGetFileSize(hFile);
			ListFileBuffer.Create(FileSize);
			SFileReadFile(hFile,ListFileBuffer.GetBuffer(),FileSize);
			SFileCloseFile(hFile);
			CEasyString FileList=(LPCSTR)ListFileBuffer.GetBuffer();
			StringFile.LoadFromString(FileList,FileSize);
			for(UINT i=0;i<StringFile.GetLineCount();i++)
			{
				CEasyString Key=StringFile[i];
				Key.MakeUpper();
				FileNameMap[Key]=StringFile[i];
			}
		}
	}
	FileNameList.Reserve(FileNameMap.GetObjectCount());
	void * Pos=FileNameMap.GetSortedFirstObjectPos();
	while(Pos)
	{
		CEasyString Key;
		FileNameList.Add(*FileNameMap.GetSortedNextObject(Pos,Key));
	}
}