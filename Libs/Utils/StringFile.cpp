#include "StdAfx.h"

CStringFile::CStringFile( const char * pszTextFile ,int FileChannel)
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
	int i = 0;
	char * p;
	char * p1;
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
BOOL CStringFile::LoadFile( const char * pszTextFile )
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
	m_iDataSize=(int)pFile->GetSize();
	m_pData=new char[m_iDataSize+2];
	pFile->Read(m_pData,m_iDataSize);
	pFile->Release();
	m_pData[m_iDataSize] = 0;
	m_pData[m_iDataSize+1] = 0;
	m_iLineCount = ProcData();
	return BuildLines();
}

BOOL CStringFile::LoadFromString(const char * pStr,int Len)
{
	if(Len<0)
		Len=(int)strlen(pStr);
	m_iDataSize=Len;
	m_pData=new char[m_iDataSize+2];
	strncpy_s(m_pData,m_iDataSize+2,pStr,Len);
	m_pData[m_iDataSize] = 0;
	m_pData[m_iDataSize+1] = 0;
	m_iLineCount = ProcData();
	return BuildLines();
}

char * CStringFile::operator[]( int line )
{
	if( line < 0 || line >= m_iLineCount )return NULL;
	return m_pLines[line];
}

BOOL CStringFile::BuildLines( )
{
	if( m_iLineCount == 0 )return FALSE;
	char * p = m_pData;
	m_pLines = new char*[m_iLineCount];
	int len = 0;
	int ptr = 0;
	for( int i = 0;i < m_iLineCount;i ++ )
	{
		len = (int)strlen( p );
		if( len > 0 )
			m_pLines[ptr++] = p;
		else
			break;
		p = p+len+1;
	}
	return TRUE;
}
int	CStringFile::ProcData()
{
	int i = 0;
	char * p = NULL;
	int linecount = 0;
	//int charscount = 0;
	int rptr = 0;
	bool	binstring = false;
	bool	newlinestart = false;
	//char * pstart = NULL;
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
