#pragma once
#include <vector>
#include <string>

inline void TrimStr(std::string& str,char TrimChar)
{
	int Start=0,End=-1;
	for(int i=0;i<(int)str.size();i++)
	{
		if(str[i]!=TrimChar)
		{
			Start=i;
			break;
		}
	}

	for(int i=(int)str.size()-1;i>=0;i--)
	{
		if(str[i]!=TrimChar)
		{
			End=i;
			break;
		}
	}

	str=str.substr(Start,End-Start+1);

}

class CSettingFile
{
protected:
	std::vector<std::string> m_SettingStrings;
	int	m_FileChannel;
public:
	CSettingFile(int FileChannel=0)
	{
		m_FileChannel=FileChannel;
	}
	bool LoadFromFile(LPCTSTR FileName)
	{
		IFileAccessor * pFile;

		pFile=CFileSystemManager::GetInstance()->CreateFileAccessor(m_FileChannel);
		if(pFile==NULL)
			return false;
		if(!pFile->Open(FileName,IFileAccessor::modeRead))
		{
			pFile->Release();
			return false;	
		}
		int FileSize=(int)pFile->GetSize();	
		char * Str=new char[FileSize+1];
		pFile->Read(Str,FileSize);
		pFile->Release();
		Str[FileSize]=0;
		bool ret=Load(Str);
		delete[] Str;
		return ret;		
	}

	bool Load(LPCTSTR SettingString,char LineDelimiter='\r',char CommentHead=';')
	{
		char * Source;
		char * Line;
		int Len=(int)strlen(SettingString)+1;
		char * pBuff=new char[Len];
		Source=pBuff;
		strcpy_s(Source,Len,SettingString);
		m_SettingStrings.clear();
		while(Source)
		{
			char * Find=(char *)FindNextLine(Source,LineDelimiter);
			if(Find)
			{
				Find[0]=0;
				Find++;
				Line=Source;
				Source=Find;
			}
			else
			{
				Line=Source;
				Source=NULL;
			}
			Find=(char *)FindNextLine(Line,CommentHead);
			if(Find)
				Find[0]=0;
			std::string temp=Line;
			TrimStr(temp,' ');
			TrimStr(temp,LineDelimiter);
			TrimStr(temp,'\r');
			TrimStr(temp,'\n');			
			if(!temp.empty())
				m_SettingStrings.push_back(temp);
		}
		delete[] pBuff;
		return true;
	}

	const char * GetString( const char * pszSection, const char * pszItemName, const char * pszDefValue = NULL )
	{
		LPCTSTR p = GetSettingString( pszSection, pszItemName );
		if( p == NULL )return pszDefValue;
		return p;
	}
	int	GetInteger( const char * pszSection, const char * pszItemName, int DefValue = 0 )
	{
		LPCTSTR p = GetSettingString( pszSection, pszItemName );
		if( p == NULL )return DefValue;
		return atoi(p);
	}
	__int64	GetInteger64( const char * pszSection, const char * pszItemName, int DefValue = 0 )
	{
		LPCTSTR p = GetSettingString( pszSection, pszItemName );
		if( p == NULL )return DefValue;
		return _atoi64(p);
	}
	double GetDouble(const char * pszSection, const char * pszItemName, double DefValue = 0 )
	{
		LPCTSTR p = GetSettingString( pszSection, pszItemName );
		if( p == NULL ) return DefValue;
		return atof(p);
	}
protected:
	int	FindSectionLine( const char * pszSection )
	{
		if( pszSection == NULL )
			return 0;
		int linecount = (int)m_SettingStrings.size();
		int sectionlength = (int)strlen( pszSection );
		for( int i = 0;i < linecount;i ++ )
		{
			LPCTSTR p = m_SettingStrings[i].c_str();

			if( *p == '[' && *(p+sectionlength+1)==']' )
			{
				if( _strnicmp( p+1, pszSection, sectionlength ) == 0 )
					return (i+1);
			}
		}
		return -1;
	}
	LPCTSTR GetSettingString(  const char * pszSection, const char * pszItemName )
	{
		int startindex = 0;
		if( pszSection != NULL )
		{
			startindex = FindSectionLine( pszSection );
			if( startindex == -1 )return NULL;
		}

		int itemnamelength = (int)strlen( pszItemName );
		if( itemnamelength == 0 )return NULL;

		int linecount = (int)m_SettingStrings.size();
		for( int i = startindex;i < linecount;i ++ )
		{
			LPCTSTR p = m_SettingStrings[i].c_str();
			//	���������һ��section�� ���ش���
			if( *p == '[' )return NULL;
			//	�����ItemName=�����ľ�ʽ�������һ������
			if( *(p+itemnamelength) == '=' )
			{
				if( _strnicmp( p, pszItemName, itemnamelength ) == 0 )
				{
					if( *(p+itemnamelength+1) == '\"' )
					{
						char * pret = (char *)(p+itemnamelength+2);
						int length = (int)strlen( pret );
						if( *( pret + length-1 ) == '\"' )
							*( pret + length-1 ) = 0;
						return pret;
					}
					return (p+itemnamelength+1);
				}
			}
		}
		return NULL;

	}		
protected:
	LPCTSTR FindNextLine(LPCTSTR pStr,char LineDelimiter)
	{
		bool InStr=false;
		while(*pStr)
		{
			if(*pStr=='"')
				InStr=!InStr;
			if(*pStr==LineDelimiter&&(!InStr))
				return pStr;
			pStr++;

		}
		return NULL;
	}
};
