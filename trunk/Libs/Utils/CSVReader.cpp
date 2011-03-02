#include "StdAfx.h"

IMPLEMENT_FILE_CHANNEL_MANAGER(CCSVReader)

IMPLEMENT_CLASS_INFO(CCSVReader,CNameObject);
CCSVReader::CCSVReader(void)
{
	m_pData=NULL;
	m_DataSize=0;
	m_BufferSize=0;
	m_GrowSize=0;
}

CCSVReader::~CCSVReader(void)
{
	Destory();	
}

void CCSVReader::Destory()
{
	SAFE_DELETE_ARRAY(m_pData);
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
	m_pData=new TCHAR[m_DataSize+1];
	if(pFileAccessor->Read(m_pData,m_DataSize*sizeof(TCHAR))<m_DataSize*sizeof(TCHAR))
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
		if(_tcsicmp(m_ColumnNames[i],ColName)==0)
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
			return _tstoi(szData);
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
			return _tstoi(szData);
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
			return _tstoi64(szData);
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
			return _tstoi64(szData);
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
			return _tstof(szData);
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
			return _tstof(szData);
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
			return _tstoi(szData)!=0;
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
			return _tstoi(szData)!=0;
		}
	}
	return Default;
}


bool CCSVReader::Create(UINT BufferSize,UINT GrowSize)
{
	Destory();

	m_BufferSize=BufferSize;
	m_GrowSize=GrowSize;
	m_pData=new TCHAR[m_BufferSize+1];
	m_DataSize=0;
	m_pData[0]=0;

	return true;
}

bool CCSVReader::Save(LPCTSTR szFileName,bool WriteHeader)
{
	bool Ret=false;
	IFileAccessor * pFileAccessor=CreateFileAccessor();
	if(pFileAccessor)
	{

		if(pFileAccessor->Open(szFileName,IFileAccessor::modeCreate|IFileAccessor::modeCreateAlways|IFileAccessor::modeWrite|IFileAccessor::shareShareAll))
		{
			Ret=Save(pFileAccessor,WriteHeader);
		}
		SAFE_RELEASE(pFileAccessor);
	}
	return Ret;
}

bool CCSVReader::Save(IFileAccessor * pFileAccessor,bool WriteHeader)
{
	UINT BufferLen=GetSavedDataLen();
	TCHAR * pSaveBuffer=new TCHAR[BufferLen];
	UINT DataLen=SaveLine(pSaveBuffer,BufferLen,m_ColumnNames);
	for(UINT i=0;i<m_Records.GetCount();i++)
	{
		DataLen+=SaveLine(pSaveBuffer+DataLen,BufferLen-DataLen,m_Records[i]);
	}

	assert(BufferLen==DataLen);

	bool Ret=pFileAccessor->Write(pSaveBuffer,DataLen);

	SAFE_DELETE_ARRAY(pSaveBuffer);
	return Ret;
}

bool CCSVReader::AddColumn(LPCTSTR ColName)
{
	int NeedSize=strlen(ColName)+1;
	ConfirmBufferFreeSize(NeedSize);
	LPTSTR pData=m_pData+m_DataSize;
	_tcscpy_s(pData,NeedSize,ColName);
	m_ColumnNames.Add(pData);
	return true;
}

bool CCSVReader::AddRow()
{
	m_Records.Resize(m_Records.GetCount()+1);
	return true;
}

bool CCSVReader::AddDataString(LPCTSTR Data)
{
	int NeedSize=strlen(Data)+1;
	ConfirmBufferFreeSize(NeedSize);
	LPTSTR pData=m_pData+m_DataSize;
	_tcscpy_s(pData,NeedSize,Data);
	m_Records[m_Records.GetCount()-1].Add(pData);
	return true;
}

bool CCSVReader::AddDataInt(int Data)
{
	int NeedSize=_tcprintf("%d",Data);
	if(NeedSize<0)
		return false;
	NeedSize++;
	ConfirmBufferFreeSize(NeedSize);
	LPTSTR pData=m_pData+m_DataSize;
	_stprintf_s(pData,NeedSize,"%d",Data);
	m_Records[m_Records.GetCount()-1].Add(pData);
	return true;
}

bool CCSVReader::AddDataInt64(INT64 Data)
{
	int NeedSize=_tcprintf("%lld",Data);
	if(NeedSize<0)
		return false;
	NeedSize++;
	ConfirmBufferFreeSize(NeedSize);
	LPTSTR pData=m_pData+m_DataSize;
	_stprintf_s(pData,NeedSize,"%lld",Data);
	m_Records[m_Records.GetCount()-1].Add(pData);
	return true;
}

bool CCSVReader::AddDataDouble(double Data)
{
	int NeedSize=_tcprintf("%g",Data);
	if(NeedSize<0)
		return false;
	NeedSize++;
	ConfirmBufferFreeSize(NeedSize);
	LPTSTR pData=m_pData+m_DataSize;
	_stprintf_s(pData,NeedSize,"%g",Data);
	m_Records[m_Records.GetCount()-1].Add(pData);
	return true;
}

bool CCSVReader::AddDataBool(bool Data)
{
	int NeedSize=_tcprintf("%d",Data?1:0);
	if(NeedSize<0)
		return false;
	NeedSize++;
	ConfirmBufferFreeSize(NeedSize);
	LPTSTR pData=m_pData+m_DataSize;
	_stprintf_s(pData,NeedSize,"%d",Data?1:0);
	m_Records[m_Records.GetCount()-1].Add(pData);
	return true;
}


UINT CCSVReader::GetLineCount(LPCTSTR szData)
{
	UINT LineCount=0;
	UINT LineLen=0;
	bool IsInString=false;
	while(*szData)
	{
		LineLen++;
		if(*szData==_T('"'))
		{
			IsInString=!IsInString;
		}
		else if((!IsInString)&&(*szData==_T('\r')||*szData==_T('\n')))
		{
			LineCount++;
			LineLen=0;
			if(*szData==_T('\r')&&*(szData+1)==_T('\n'))
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
	while(*szLine)
	{
		if(*szLine==_T('"'))
		{
			IsInStr=!IsInStr;
		}
		else if(!IsInStr)
		{
			if(*szLine==_T('\r')||*szLine==_T('\n'))
			{				
				if(*szLine==_T('\r')&&*(szLine+1)==_T('\n'))
				{
					*szLine=0;
					LineRecord.Add(szLineHead);
					szLine+=2;
				}
				else
				{
					szLine++;
				}				
				break;
			}
			else if(*szLine==_T(','))
			{
				*szLine=0;
				LineRecord.Add(szLineHead);
				szLineHead=szLine+1;
			}
		}		
		szLine++;
	}	
	for(UINT i=0;i<LineRecord.GetCount();i++)
	{
		LPTSTR szField=LineRecord[i];
		UINT Len=strlen(szField);
		if(Len)
		{
			if(szField[Len-1]==_T('"'))
			{
				szField[Len-1]=0;
			}
			if(szField[0]==_T('"'))
			{
				szField[0]=0;
				szField++;
			}
			UINT QMCount=0;		
			while(*szField)
			{
				if(*szField==_T('"'))
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

void CCSVReader::ConfirmBufferFreeSize(UINT NeedSize)
{
	if(m_DataSize+NeedSize>m_BufferSize)
	{
		UINT NewBufferSize=m_DataSize+NeedSize;
		NewBufferSize=((NewBufferSize/m_GrowSize)+1)*m_GrowSize;
		TCHAR * pNewBuffer=new TCHAR[NewBufferSize+1];
		memcpy(pNewBuffer,m_pData,m_DataSize*sizeof(TCHAR));
		SAFE_DELETE_ARRAY(m_pData);
		m_pData=pNewBuffer;
		m_BufferSize=NewBufferSize;		
	}
}

UINT CCSVReader::GetSavedDataLen()
{
	UINT DataLen=GetSavedLineLen(m_ColumnNames);
	for(UINT i=0;i<m_Records.GetCount();i++)
	{
		DataLen+=GetSavedLineLen(m_Records[i]);
	}
	return DataLen;
}

UINT CCSVReader::GetSavedLineLen(CEasyArray<LPTSTR>& LineRecord)
{
	UINT DataLen=0;
	for(UINT i=0;i<LineRecord.GetCount();i++)
	{
		bool AddQuotationMark=false;
		LPCTSTR szField=LineRecord[i];
		while(*szField)
		{
			if(*szField==_T('"')||*szField==_T(',')||*szField==_T('\r')||*szField==_T('\n'))
			{
				AddQuotationMark=true;
			}
			if(*szField==_T('"'))
			{
				DataLen++;
			}
			szField++;
			DataLen++;
		}
		if(AddQuotationMark)
		{
			DataLen+=2;
		}
		if(i<LineRecord.GetCount()-1)
		{
			DataLen++;
		}
	}
	if(DataLen)
	{
		DataLen+=2;
	}
	return DataLen;
}

UINT CCSVReader::SaveLine(LPTSTR pSaveBuffer,UINT BufferSize,CEasyArray<LPTSTR>& LineRecord)
{
	UINT DataLen=0;
	for(UINT i=0;i<LineRecord.GetCount();i++)
	{
		bool AddQuotationMark=false;
		LPCTSTR szField=LineRecord[i];
		UINT FieldPtr=0;
		while(*szField&&BufferSize)
		{
			if(*szField==_T('"')||*szField==_T(',')||*szField==_T('\r')||*szField==_T('\n'))
			{
				if(!AddQuotationMark)
				{
					AddQuotationMark=true;
					memmove(pSaveBuffer+DataLen-FieldPtr+1,pSaveBuffer+DataLen-FieldPtr,FieldPtr*sizeof(TCHAR));
					pSaveBuffer[DataLen-FieldPtr]=_T('"');
					DataLen++;
					BufferSize--;					
					if(BufferSize==0)
						break;
				}				
			}
			if(*szField==_T('"'))
			{
				pSaveBuffer[DataLen]=_T('"');
				DataLen++;
				BufferSize--;
				if(BufferSize==0)
					break;
			}
			pSaveBuffer[DataLen]=*szField;
			szField++;
			FieldPtr++;
			DataLen++;
			BufferSize--;	
		}
		if(AddQuotationMark&&BufferSize)
		{
			pSaveBuffer[DataLen]=_T('"');
			DataLen++;
			BufferSize--;
		}
		if(i<LineRecord.GetCount()-1&&BufferSize)
		{
			pSaveBuffer[DataLen]=_T(',');
			DataLen++;
			BufferSize--;
		}
	}
	if(DataLen&&BufferSize>=2)
	{
		pSaveBuffer[DataLen]=_T('\r');
		DataLen++;
		pSaveBuffer[DataLen]=_T('\n');
		DataLen++;
	}
	return DataLen;
}