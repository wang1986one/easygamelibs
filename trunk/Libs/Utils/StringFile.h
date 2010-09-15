/****************************************************************************/
/*                                                                          */
/*      文件名:    StringFile.h                                             */
/*      创建日期:  2009年07月06日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once


class CStringFile
{
protected:
	char *	m_pData;
	int		m_iDataSize;
	int		m_iLineCount;
	char**	m_pLines;
	int		m_FileChannel;
	char	m_LineDelimiter;
public:
	CStringFile( const char * pszTextFile ,int FileChannel=0);
	CStringFile(int FileChannel=0);
	~CStringFile();
	void Destroy();
	void MakeDeflate();
	BOOL LoadFile( const char * pszTextFile );
	BOOL LoadFile( IFileAccessor * pFile );
	BOOL LoadFromString(const char * pStr,int Len=-1);
	char * operator[]( int line );
	int	GetLineCount()
	{
		return m_iLineCount;
	}
	void SetLineDelimiter(char Delimiter)
	{
		m_LineDelimiter=Delimiter;
	}
protected:
	BOOL BuildLines( );
	int	ProcData();
	
};