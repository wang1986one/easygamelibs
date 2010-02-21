#include "StdAfx.h"

namespace DBLib
{

IMPLEMENT_CLASS_INFO(CMySQLDynamicRecordSet,IDBRecordSet);

CMySQLDynamicRecordSet::CMySQLDynamicRecordSet(bool CacheAllData)
{
	m_pDBConnection=NULL;
	m_hStmt=NULL;
	m_IsBOF=true;
	m_IsEOF=true;
	m_CacheAllData=CacheAllData;
}

CMySQLDynamicRecordSet::~CMySQLDynamicRecordSet(void)
{
	Destory();
}

int CMySQLDynamicRecordSet::Init(CMySQLConnection * pDBConnection,MYSQL_STMT_HANDLE hStmt)
{
	if(pDBConnection==NULL||hStmt==NULL)
		return DBERR_INVALID_PARAM;
	Destory();
	m_pDBConnection=pDBConnection;
	m_hStmt=hStmt;
	MYSQL_RES_HANDLE hResult=mysql_stmt_result_metadata(m_hStmt);
	if(hResult==NULL)
	{
		return DBERR_NO_RECORDS;
	}
	int ColNum=mysql_num_fields(hResult);
	m_pColumnInfos.resize(ColNum);
	m_RowBuffer.resize(ColNum);
	m_FetchBuffer.resize(ColNum);	
	ZeroMemory(&(m_FetchBuffer[0]),sizeof(MYSQL_BIND)*ColNum);
	MYSQL_FIELD * pFields=mysql_fetch_fields(hResult);
	UINT FetchBufferLen=0;
	for(int i=0;i<ColNum;i++)
	{
		UINT Size=pFields[i].length;
		UINT DigitalSize=pFields[i].decimals;
		int DBType=CMySQLConnection::MySQLTypeToDBLibType(pFields[i].type,Size,DigitalSize);
		strncpy_0(m_pColumnInfos[i].Name,MAX_COLUMN_NAME,pFields[i].name,MAX_COLUMN_NAME);		
		m_pColumnInfos[i].Type=DBType;
		m_pColumnInfos[i].Size=Size;
		m_pColumnInfos[i].DigitSize=DigitalSize;		
		FetchBufferLen+=CMySQLConnection::GetMySQLTypeBinLength(pFields[i].type,pFields[i].length,pFields[i].decimals)+sizeof(UINT)+sizeof(my_bool);
	}
	m_FetchDataBuffer.Create(FetchBufferLen);
	for(int i=0;i<ColNum;i++)
	{	
		m_FetchBuffer[i].buffer_type=pFields[i].type;
		m_FetchBuffer[i].buffer=(char *)m_FetchDataBuffer.GetFreeBuffer();
		m_FetchBuffer[i].buffer_length=CMySQLConnection::GetMySQLTypeBinLength(pFields[i].type,pFields[i].length,pFields[i].decimals);
		m_FetchDataBuffer.PushBack(NULL,m_FetchBuffer[i].buffer_length);		

		m_FetchBuffer[i].length=(ULONG *)m_FetchDataBuffer.GetFreeBuffer();
		m_FetchDataBuffer.PushConstBack(m_pColumnInfos[i].Size,sizeof(ULONG));

		m_FetchBuffer[i].is_null=(my_bool *)m_FetchDataBuffer.GetFreeBuffer();
		m_FetchDataBuffer.PushConstBack(0,sizeof(my_bool));

		
	}
	mysql_free_result(hResult);

	if(mysql_stmt_bind_result(m_hStmt,&(m_FetchBuffer[0])))
	{
		m_pDBConnection->ProcessErrorMsg(m_hStmt,"�󶨽����ʧ��");
		return DBERR_BINDCOLFAIL;
	}
	if(m_CacheAllData)
	{
		if(mysql_stmt_store_result(m_hStmt))
		{
			m_pDBConnection->ProcessErrorMsg(m_hStmt,"��������ʧ��");
			return DBERR_BUFFER_OVERFLOW;
		}
	}
	int Ret=FetchRow();	
	return Ret;
}

void CMySQLDynamicRecordSet::Destory()
{
	if(m_hStmt)
	{
		mysql_stmt_close(m_hStmt);
		m_hStmt=NULL;
	}
	m_pDBConnection=NULL;
	m_pColumnInfos.clear();
	m_RowBuffer.clear();
	m_IsBOF=true;
	m_IsEOF=true;	
}

int CMySQLDynamicRecordSet::GetRecordCount()
{
	if(m_hStmt)
		return (int)mysql_stmt_num_rows(m_hStmt);
	else
		return 0;
}

int CMySQLDynamicRecordSet::GetColumnCount()
{
	return (int)m_pColumnInfos.size();
}

LPCTSTR CMySQLDynamicRecordSet::GetColumnName(int Index)
{
	if(Index>=0&&Index<(int)m_pColumnInfos.size())
	{
		return m_pColumnInfos[Index].Name;
	}
	return NULL;
}

int CMySQLDynamicRecordSet::GetIndexByColumnName(LPCTSTR Name)
{
	for(int i=0;i<(int)m_pColumnInfos.size();i++)
	{
		if(_strnicmp(m_pColumnInfos[i].Name,Name,MAX_COLUMN_NAME)==0)
			return i;
	}
	return -1;
}

DB_COLUMN_INFO * CMySQLDynamicRecordSet::GetColumnInfo(int Index)
{
	if(Index>=0&&Index<(int)m_pColumnInfos.size())
	{
		return &(m_pColumnInfos[Index]);
	}
	return NULL;
}


CDBValue& CMySQLDynamicRecordSet::GetField(int Index)
{
	if(Index>=0&&Index<(int)m_RowBuffer.size())
	{
		return m_RowBuffer[Index];
	}
	return m_EmptyValue;
}

CDBValue& CMySQLDynamicRecordSet::GetField(LPCTSTR Name)
{
	int Index=GetIndexByColumnName(Name);
	if(Index>=0)
	{
		return m_RowBuffer[Index];
	}
	return m_EmptyValue;
}

int CMySQLDynamicRecordSet::MoveFirst()
{
	if(m_hStmt)
	{		
		if(m_CacheAllData)
		{
			mysql_stmt_data_seek(m_hStmt,0);
			return FetchRow();			
		}
		else
		{
			return DBERR_NOT_IMPLEMENT;
		}
	}
	else
	{
		return DBERR_NO_RECORDS;
	}

}

int CMySQLDynamicRecordSet::MoveLast()
{
	if(m_hStmt)
	{		
		if(m_CacheAllData)
		{
			my_ulonglong RowCount=mysql_stmt_num_rows(m_hStmt);			
			mysql_stmt_data_seek(m_hStmt,RowCount-1);
			return FetchRow();			
		}
		else
		{
			return DBERR_NOT_IMPLEMENT;
		}
	}
	else
	{
		return DBERR_NO_RECORDS;
	}
}

int CMySQLDynamicRecordSet::MoveNext()
{
	if(m_hStmt)
	{	
		return FetchRow();			
	}
	else
	{
		return DBERR_NO_RECORDS;
	}
}

int CMySQLDynamicRecordSet::MovePrevious()
{
	if(m_hStmt)
	{		
		if(m_CacheAllData)
		{
			MYSQL_ROW_OFFSET Offset=mysql_stmt_row_tell(m_hStmt);
			Offset--;
			mysql_stmt_row_seek(m_hStmt,Offset);
			int Ret=FetchRow();
			if(Ret=DBERR_IS_RECORDSET_TAIL)
			{
				m_IsBOF=true;
				return DBERR_IS_RECORDSET_HEAD;
			}
			else
			{
				return Ret;
			}
		}
		else
		{
			return DBERR_NOT_IMPLEMENT;
		}
	}
	else
	{
		return DBERR_NO_RECORDS;
	}
}

int CMySQLDynamicRecordSet::MoveTo(int Index)
{
	if(m_hStmt)
	{		
		if(m_CacheAllData)
		{
			mysql_stmt_data_seek(m_hStmt,Index);
			return FetchRow();
		}
		else
		{
			return DBERR_NOT_IMPLEMENT;
		}
	}
	else
	{
		return DBERR_NO_RECORDS;
	}
}

bool CMySQLDynamicRecordSet::IsEOF()
{
	return m_IsEOF;
}

bool CMySQLDynamicRecordSet::IsBOF()
{
	return m_IsBOF;
}

bool CMySQLDynamicRecordSet::Close()
{
	return true;
}

int CMySQLDynamicRecordSet::FetchRow()
{
	if(m_hStmt)
	{
		int Ret=mysql_stmt_fetch(m_hStmt);
		if(Ret)
		{
			if(Ret==MYSQL_DATA_TRUNCATED)
			{

			}
			else if(Ret==MYSQL_NO_DATA)
			{
				m_IsEOF=true;
				return DBERR_IS_RECORDSET_TAIL;					
			}
			else
			{
				m_pDBConnection->ProcessErrorMsg(m_hStmt,"��ȡ�����ʧ��");
				return DBERR_FETCH_RESULT_FAIL;
			}
		}
		
		int ColNum=GetColumnCount();
		for(int i=0;i<ColNum;i++)
		{			

			if(*(m_FetchBuffer[i].is_null))
			{
				m_RowBuffer[i].SetNULLValue(m_pColumnInfos[i].Type);
			}
			else
			{
				m_RowBuffer[i].SetEmptyValue(m_pColumnInfos[i].Type,m_pColumnInfos[i].Size,m_pColumnInfos[i].DigitSize);

				if(!CMySQLConnection::MySQLBinValueToDBValue(m_FetchBuffer[i].buffer_type,
					m_FetchBuffer[i].buffer,*(m_FetchBuffer[i].length),
					m_RowBuffer[i].GetBuffer(),m_RowBuffer[i].GetLength()))
				{
					return DBERR_BINDPARAMFAIL;
				}
			}
		}
		m_IsBOF=false;
		m_IsEOF=false;
		return DBERR_SUCCEED;
	}	
	else
	{
		return DBERR_NO_RECORDS;
	}
}

}