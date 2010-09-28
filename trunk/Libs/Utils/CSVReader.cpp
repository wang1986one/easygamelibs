#include "StdAfx.h"

IMPLEMENT_FILE_CHANNEL_MANAGER(CCSVReader)

IMPLEMENT_CLASS_INFO(CCSVReader,CNameObject);
CCSVReader::CCSVReader(void)
{
	m_pData=NULL;
	m_DataSize=0;

}

CCSVReader::~CCSVReader(void)
{
	Destory();	
}

void CCSVReader::Destory()
{
	SAFE_DELETE(m_pData);
	m_DataSize=0;
	m_Records.Clear();
	m_ColumnNames.Clear();
}

bool CCSVReader::Open(LPCTSTR szFileName,bool HaveHeader)
{
	bool Ret=false;
	IFileAccessor * pFileAccessor=CreateFileAccessor();
	if(pFileAccessor)
	{
		
		if(pFileAccessor->Open(szFileName,IFileAccessor::modeOpen|IFileAccessor::modeRead|IFileAccessor::shareShareAll))
		{
			Ret=Open(pFileAccessor,HaveHeader);
		}
		SAFE_RELEASE(pFileAccessor);
	}
	return Ret;
}
bool CCSVReader::Open(IFileAccessor * pFileAccessor,bool HaveHeader)
{
	Destory();

	m_DataSize=(UINT)pFileAccessor->GetSize();
	m_pData=new char[m_DataSize+1];
	if(pFileAccessor->Read(m_pData,m_DataSize)<m_DataSize)
	{
		return false;
	}
	m_pData[m_DataSize]=0;

	UINT RowCount=GetLineCount(m_pData);
	LPTSTR szLine=m_pData;
	if(HaveHeader&&RowCount)
	{
		szLine=ParseLine(szLine,m_ColumnNames);
		RowCount--;
	}
	m_Records.Resize(RowCount);
	for(UINT i=0;i<RowCount;i++)
	{
		szLine=ParseLine(szLine,m_Records[i]);
	}
	return true;
}

UINT CCSVReader::GetRowCount()
{
	return m_Records.GetCount();
}
UINT CCSVReader::GetColCount()
{
	return m_ColumnNames.GetCount();
}

LPCTSTR CCSVReader::GetDataString(UINT Row,UINT Col,LPCTSTR Default)
{
	if(Row<m_Records.GetCount())
	{
		if(Col<m_Records[Row].GetCount())
		{
			return m_Records[Row][Col];
		}
	}
	return Default;
}
LPCTSTR CCSVReader::GetDataString(UINT Row,LPCTSTR ColName,LPCTSTR Default)
{
	for(UINT i=0;i<m_ColumnNames.GetCount();i++)
	{
		if(_stricmp(m_ColumnNames[i],ColName)==0)
		{
			return GetDataString(Row,i,Default);
		}
	}
	return Default;
}

int CCSVReader::GetDataInt(UINT Row,UINT Col,int Default)
{
	LPCTSTR szData=GetDataString(Row,Col,NULL);
	if(szData)
	{
		if(*szData)
		{
			return atoi(szData);
		}
	}
	return Default;
}
int CCSVReader::GetDataInt(UINT Row,LPCTSTR ColName,int Default)
{
	LPCTSTR szData=GetDataString(Row,ColName,NULL);
	if(szData)
	{
		if(*szData)
		{
			return atoi(szData);
		}
	}
	return Default;
}

INT64 CCSVReader::GetDataInt64(UINT Row,UINT Col,INT64 Default)
{
	LPCTSTR szData=GetDataString(Row,Col,NULL);
	if(szData)
	{
		if(*szData)
		{
			return _atoi64(szData);
		}
	}
	return Default;
}
INT64 CCSVReader::GetDataInt64(UINT Row,LPCTSTR ColName,INT64 Default)
{
	LPCTSTR szData=GetDataString(Row,ColName,NULL);
	if(szData)
	{
		if(*szData)
		{
			return _atoi64(szData);
		}
	}
	return Default;
}

double CCSVReader::GetDataDouble(UINT Row,UINT Col,double Default)
{
	LPCTSTR szData=GetDataString(Row,Col,NULL);
	if(szData)
	{
		if(*szData)
		{
			return atof(szData);
		}
	}
	return Default;
}
double CCSVReader::GetDataDouble(UINT Row,LPCTSTR ColName,double Default)
{
	LPCTSTR szData=GetDataString(Row,ColName,NULL);
	if(szData)
	{
		if(*szData)
		{
			return atof(szData);
		}
	}
	return Default;
}


bool CCSVReader::GetDataBool(UINT Row,UINT Col,bool Default)
{
	LPCTSTR szData=GetDataString(Row,Col,NULL);
	if(szData)
	{
		if(*szData)
		{
			return atoi(szData)!=0;
		}
	}
	return Default;
}
bool CCSVReader::GetDataBool(UINT Row,LPCTSTR ColName,bool Default)
{
	LPCTSTR szData=GetDataString(Row,ColName,NULL);
	if(szData)
	{
		if(*szData)
		{
			return atoi(szData)!=0;
		}
	}
	return Default;
}


UINT CCSVReader::GetLineCount(LPCTSTR szData)
{
	UINT LineCount=0;
	UINT LineLen=0;
	while(*szData)
	{
		LineLen++;
		if(*szData=='\r'||*szData=='\n')
		{
			LineCount++;
			LineLen=0;
			if(*szData=='\r'&&*(szData+1)=='\n')
			{
				szData++;
			}
		}
		szData++;
	}
	if(LineLen)
	{
		LineCount++;
	}
	return LineCount;
}

LPTSTR CCSVReader::ParseLine(LPTSTR szLine,CEasyArray<LPTSTR>& LineRecord)
{
	bool IsInStr=false;
	LPTSTR szLineHead=szLine;
	while((*szLine!='\r')&&(*szLine!='\n')&&(*szLine!=0))
	{
		switch(*szLine)
		{
		case ',':
			if(!IsInStr)
			{
				*szLine=0;
				LineRecord.Add(szLineHead);
				szLineHead=szLine+1;
			}
			break;
		case '"':
			IsInStr=!IsInStr;
			break;
				
		}
		szLine++;
	}
	if((*szLineHead!='\r')&&(*szLineHead!='\n')&&(*szLineHead!=0))
	{
		LineRecord.Add(szLineHead);
	}
	if(*szLine=='\r')
	{
		if((*szLine=='\r')&&(*(szLine+1)=='\n'))
		{
			*szLine=0;
			szLine++;
		}		
		*szLine=0;
		szLine++;
		
	}
	for(UINT i=0;i<LineRecord.GetCount();i++)
	{
		LPTSTR szField=LineRecord[i];
		UINT Len=strlen(szField);
		if(Len)
		{
			if(szField[Len-1]=='"')
			{
				szField[Len-1]=0;
			}
			if(szField[0]=='"')
			{
				szField[0]=0;
				szField++;
			}
			UINT QMCount=0;		
			while(*szField)
			{
				if(*szField=='"')
				{
					if(QMCount)
					{
						*szField=0;
						QMCount=0;
					}
					else
					{
						QMCount++;
					}					
				}
				else
				{
					QMCount=0;
				}
				szField++;
			}
			LPTSTR szDest=LineRecord[i];
			LPTSTR szSrc=szDest;
			for(UINT j=0;j<Len;j++)
			{
				*szDest=*szSrc;
				if(*szSrc)
				{
					szDest++;
				}
				szSrc++;
			}
		}
	}
	return szLine;
}