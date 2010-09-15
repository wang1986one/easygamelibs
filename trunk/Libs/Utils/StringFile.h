/****************************************************************************/
/*                                                                          */
/*      �ļ���:    StringFile.h                                             */
/*      ��������:  2009��07��06��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
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