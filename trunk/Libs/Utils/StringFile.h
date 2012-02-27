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
	TCHAR *	m_pData;
	UINT	m_iDataSize;
	UINT	m_iLineCount;
	TCHAR**	m_pLines;
	int		m_FileChannel;
	TCHAR	m_LineDelimiter;
public:
	CStringFile( LPCTSTR pszTextFile ,int FileChannel=0);
	CStringFile(int FileChannel=0);
	~CStringFile();
	void Destroy();
	void MakeDeflate();
	BOOL LoadFile( LPCTSTR pszTextFile );
	BOOL LoadFile( IFileAccessor * pFile );
	BOOL LoadFromString(LPCTSTR pStr,int Len=-1);
	TCHAR * operator[]( UINT line );
	UINT	GetLineCount()
	{
		return m_iLineCount;
	}
	void SetLineDelimiter(TCHAR Delimiter)
	{
		m_LineDelimiter=Delimiter;
	}
protected:
	BOOL BuildLines( );
	UINT	ProcData();
	
};