/****************************************************************************/
/*                                                                          */
/*      �ļ���:    StringSplitter.h                                         */
/*      ��������:  2009��07��06��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      ��������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ������������κ���ҵ�ͷ���ҵ������������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once


template < typename T>
class CStringSplitterT
{
protected:
	T *					m_pBuffer;
	CEasyArray< T * >	m_StringList;
public:
	CStringSplitterT()
	{
		m_pBuffer=NULL;
	}
	CStringSplitterT(const T* pStr,T SplitterChar,size_t StrLen=0)
	{
		m_pBuffer=NULL;
		Splitter(pStr,SplitterChar,StrLen);
	}
	~CStringSplitterT()
	{
		for(int i=0;i<(int)m_StringList.GetCount();i++)
		{
			SAFE_DELETE_ARRAY(m_StringList[i]);
			m_StringList.Clear();
		}
	}
	size_t Splitter(const char * pStr,char SplitterChar,size_t StrLen=0)
	{		
		if(pStr==NULL)
			return 0;
		if(StrLen==0)
			StrLen=strlen(pStr);

		SAFE_DELETE_ARRAY(m_pBuffer);
		m_pBuffer=new char[StrLen+1];
		strncpy_s(m_pBuffer,StrLen+1,pStr,StrLen);
		m_pBuffer[StrLen]=0;

		char * pSrc=m_pBuffer;
		char * pFinded=NULL;
	
		do{
			pFinded=strchr(pSrc,SplitterChar);
			if(pFinded)
			{
				m_StringList.Add(pSrc);
				*pFinded=0;
				pFinded++;
				pSrc=pFinded;				
			}
			else
			{
				m_StringList.Add(pSrc);
			}
		}while(pFinded);

		return GetCount();
	}
	size_t Splitter(const wchar_t * pStr,wchar_t SplitterChar,size_t StrLen=0)
	{
		if(pStr==NULL)
			return 0;
		if(StrLen==0)
			StrLen=wcslen(pStr);

		SAFE_DELETE_ARRAY(m_pBuffer);
		m_pBuffer=new wchar_t[StrLen+1];
		strncpy(m_pBuffer,pStr,StrLen);
		m_pBuffer[StrLen]=0;

		wchar_t * pSrc=m_pBuffer;
		wchar_t * pFinded=NULL;

		do{
			pFinded=wcschr(pSrc,SplitterChar);
			if(pFinded)
			{
				m_StringList.Add(pSrc);
				*pFinded=0;
				pFinded++;
				pSrc=pFinded;				
			}
			else
			{
				m_StringList.Add(pSrc);
			}
		}while(pFinded);

		return GetCount();
	}
	size_t GetCount()
	{
		return m_StringList.GetCount();
	}
	const T* GetAt(size_t Index)
	{
		if(Index<m_StringList.GetCount())
			return m_StringList[Index];
		else
			return NULL;
	}
	const T * operator[](size_t Index)
	{
		return GetAt(Index);
	}
};

typedef CStringSplitterT<wchar_t> CStringSplitterW;

typedef CStringSplitterT<char> CStringSplitterA;

typedef CStringSplitterT<TCHAR> CStringSplitter;