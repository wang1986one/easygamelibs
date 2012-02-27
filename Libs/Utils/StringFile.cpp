/****************************************************************************/
/*                                                                          */
/*      文件名:    StringFile.cpp                                           */
/*      创建日期:  2009年07月06日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#include "StdAfx.h"

CStringFile::CStringFile( LPCTSTR pszTextFile ,int FileChannel)
{
	m_FileChannel=FileChannel;
	m_pData = NULL;
	m_iDataSize = 0;
	m_pLines = NULL;
	m_iLineCount = 0;
	m_LineDelimiter=0;
	LoadFile( pszTextFile );
}
CStringFile::CStringFile(int FileChannel)
{
	m_FileChannel=FileChannel;
	m_pData = NULL;
	m_iDataSize = 0;
	m_pLines = NULL;
	m_iLineCount = 0;
	m_LineDelimiter=0;
}
CStringFile::~CStringFile()
{
	Destroy();
}
void CStringFile::Destroy()
{
	if( m_pLines != NULL )
	{
		delete []m_pLines;
		m_pLines = NULL;
	}
	m_iDataSize = 0;
	if( m_pData != NULL )
	{
		delete []m_pData;
		m_pData = NULL;
	}
	m_iLineCount = 0;
}
void CStringFile::MakeDeflate()
{
	UINT i = 0;
	TCHAR * p;
	TCHAR * p1;
	BOOL	bInString = FALSE;
	for( i = 0;i < GetLineCount();i ++ )
	{
		p = (*this)[i];
		p1 = p;
		while( *p != '\0' )
		{
			if( *p == '\"' )bInString = !bInString;
			if( (!bInString) && ( *p == ' ' || *p == '	' ) )
			{
				p++;
				continue;
			}
			*p1++=*p++;
		}
		*p1 = 0;
	}
}
BOOL CStringFile::LoadFile( LPCTSTR pszTextFile )
{
	IFileAccessor * pFile;

	pFile=CFileSystemManager::GetInstance()->CreateFileAccessor(m_FileChannel);
	if(pFile==NULL)
		return false;
	if(!pFile->Open(pszTextFile,IFileAccessor::modeRead))
	{
		pFile->Release();
		return false;
	}
	BOOL Ret=LoadFile(pFile);	
	pFile->Release();	
	return Ret;
}

BOOL CStringFile::LoadFile( IFileAccessor * pFile )
{
	UINT FileSize=(UINT)pFile->GetSize();
	BYTE * pBuffer=new BYTE[FileSize];
	bool IsUnicode=false;
	pFile->Read(pBuffer,FileSize);
	if(FileSize>=2)
	{
		if(pBuffer[0]==0xFF&&pBuffer[1]==0xFE)
			IsUnicode=true;
	}
#ifdef UNICODE
	if(IsUnicode)
	{
		m_iDataSize=FileSize/sizeof(WCHAR);
		m_pData=new WCHAR[m_iDataSize+2];
		memcpy(m_pData,pBuffer,FileSize);
	}
	else
	{
		m_iDataSize=AnsiToUnicode((char *)pBuffer,FileSize,NULL,0);
		m_pData=new WCHAR[m_iDataSize+2];
		AnsiToUnicode((char *)pBuffer,FileSize,m_pData,m_iDataSize);
	}
#else
	if(IsUnicode)
	{
		m_iDataSize=UnicodeToAnsi((WCHAR *)pBuffer,FileSize/sizeof(WCHAR),NULL,0);
		m_pData=new char[m_iDataSize+2];
		UnicodeToAnsi((WCHAR *)pBuffer,FileSize/sizeof(WCHAR),m_pData,m_iDataSize);
	}
	else
	{
		m_iDataSize=FileSize/sizeof(char);
		m_pData=new char[m_iDataSize+2];
		memcpy(m_pData,pBuffer,FileSize);
	}
#endif
	SAFE_DELETE_ARRAY(pBuffer);
	m_pData[m_iDataSize] = 0;
	m_pData[m_iDataSize+1] = 0;
	m_iLineCount = ProcData();
	return BuildLines();
}

BOOL CStringFile::LoadFromString(LPCTSTR pStr,int Len)
{
	if(Len<0)
		Len=(UINT)_tcslen(pStr);
	m_iDataSize=Len;
	m_pData=new TCHAR[m_iDataSize+2];
	_tcsncpy_s(m_pData,m_iDataSize+2,pStr,Len);
	m_pData[m_iDataSize] = 0;
	m_pData[m_iDataSize+1] = 0;
	m_iLineCount = ProcData();
	return BuildLines();
}

TCHAR * CStringFile::operator[]( UINT line )
{
	if( line >= m_iLineCount )return NULL;
	return m_pLines[line];
}

BOOL CStringFile::BuildLines( )
{
	if( m_iLineCount == 0 )return FALSE;
	TCHAR * p = m_pData;
	m_pLines = new TCHAR*[m_iLineCount];
	UINT len = 0;
	UINT ptr = 0;
	for( UINT i = 0;i < m_iLineCount;i ++ )
	{
		len = (UINT)_tcslen( p );
		if( len > 0 )
			m_pLines[ptr++] = p;
		else
			break;
		p = p+len+1;
	}
	return TRUE;
}
UINT	CStringFile::ProcData()
{
	UINT i = 0;
	TCHAR * p = NULL;
	UINT linecount = 0;

	UINT rptr = 0;
	bool	binstring = false;
	bool	newlinestart = false;

	for( i = 0;i < m_iDataSize;i ++ )
	{
		p = m_pData + i;
		if(m_LineDelimiter&&m_LineDelimiter==(*p)&&newlinestart)
		{
			*(m_pData+rptr++) = 0;
			newlinestart = false;
			linecount++;
		}
		switch( *p )
		{
			//case	' ':
			//case	'	':
			//	if( binstring )
			//	{
			//		*(m_pData+rptr++) = *p;
			//		if( !newlinestart )newlinestart = true;
			//	}
			//	break;
		case	'\n':
		case	'\r':
			{
				if( newlinestart )
				{
					*(m_pData+rptr++) = 0;
					newlinestart = false;
					linecount++;
				}
			}
			break;
			//case	'\"':
			//	binstring = !binstring;
		default:
			{
				*(m_pData+rptr++) = *p;
				if( !newlinestart )newlinestart = true;
			}
			break;
		}
	}
	if( newlinestart )
		linecount++;

	m_pData[rptr++] = 0;
	m_pData[rptr++] = 0;
	m_iDataSize = rptr;
	return linecount;
}
