/****************************************************************************/
/*                                                                          */
/*      �ļ���:    MySQLConnection.cpp                                      */
/*      ��������:  2009��09��11��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#include "StdAfx.h"


namespace DBLib
{

IMPLEMENT_CLASS_INFO(CMySQLConnection,IDBConnection);

CMySQLConnection::CMySQLConnection(void)
{
	m_MySQLHandle=NULL;
	m_pDatabase=NULL;
	m_MySQLStmt=NULL;
	m_UseSTMTMode=false;
}

CMySQLConnection::~CMySQLConnection(void)
{
	Destory();
}

int CMySQLConnection::Init(CMySQLDatabase * pDatabase)
{
	m_pDatabase=pDatabase;
	return DBERR_SUCCEED;
}

void CMySQLConnection::Destory()
{
	Disconnect();
	m_pDatabase=NULL;
}

IDBRecordSet * CMySQLConnection::CreateRecordSet(int RecordSetType)
{
	switch(RecordSetType)
	{
	case DB_RS_TYPE_STATIC:
		return new CMySQLRecordSet();
	case DB_RS_TYPE_GENERAL_STATIC:
		return new CDBStaticRecordSet();
	case DB_RS_TYPE_DYNAMIC:
		return new CMySQLDynamicRecordSet();
	}
	return NULL;
}

IDBParameterSet * CMySQLConnection::CreateParameterSet(int RecordSetType)
{
	return NULL;
}

int CMySQLConnection::Connect(LPCTSTR ConnectStr)
{
	Disconnect();
	m_MySQLHandle=mysql_init(NULL);
	if(m_MySQLHandle==NULL)
		return DBERR_MYSQL_MYSQLINITFAIL;
	//mysql_options(m_MySQLHandle,MYSQL_SET_CHARSET_NAME,"gb2312");
	CSettingFile StrAnalyzer;
	StrAnalyzer.Load(ConnectStr,';',0);
	const char * Host=StrAnalyzer.GetString(NULL,"Server","");
	const char * User=StrAnalyzer.GetString(NULL,"UID","");
	const char * Password=StrAnalyzer.GetString(NULL,"PWD","");
	const char * FlagsStr=StrAnalyzer.GetString(NULL,"Flag","");
	const char * CharSetStr=StrAnalyzer.GetString(NULL,"CharSet","");
	if(Password[0]==0)
		Password=NULL;
	const char * DB=StrAnalyzer.GetString(NULL,"DB","");
	if(DB[0]==0)
	{
		DB=StrAnalyzer.GetString(NULL,"DATABASE","");
	}
	if(DB[0]==0)
		DB=NULL;
	const char * UnixSocket=StrAnalyzer.GetString(NULL,"UnixSocket","");
	if(UnixSocket[0]==0)
		UnixSocket=NULL;
	int Port=StrAnalyzer.GetInteger(NULL,"Port",3306);
	DWORD Flags=FetchConnectFlags(FlagsStr);

	if(mysql_real_connect(m_MySQLHandle,Host,User,Password,DB,Port,UnixSocket,
		Flags)==NULL)
	{
		ProcessErrorMsg(NULL,"����ʧ��\r\n");
		return DBERR_MYSQL_CONNECTFAIL;
	}

	if(CharSetStr[0])
	{
		if(mysql_set_character_set(m_MySQLHandle,CharSetStr))
			ProcessErrorMsg(NULL,"�����ַ���ʧ��\r\n");
	}

	return DBERR_SUCCEED;
}

int CMySQLConnection::Disconnect()
{
	if(m_MySQLStmt)
	{
		mysql_stmt_close(m_MySQLStmt);
		m_MySQLStmt=NULL;
	}
	if(m_MySQLHandle)
	{
		mysql_close(m_MySQLHandle);
		m_MySQLHandle=NULL;
	}
	return DBERR_SUCCEED;
}
BOOL CMySQLConnection::IsConnected()
{
	if(m_MySQLHandle==NULL)
		return FALSE;
	return mysql_ping(m_MySQLHandle)==0;
}

int CMySQLConnection::ExecuteSQL(LPCSTR SQLStr,int StrLen,IDBParameterSet * pParamSet)
{
	if(m_UseSTMTMode)
	{
		return ExecuteSQLWithParam(SQLStr,StrLen,(CDBParameterSet *)pParamSet);
	}
	else
	{
		return ExecuteSQLDirect(SQLStr,StrLen);
	}
}

int CMySQLConnection::GetResults(IDBRecordSet * pDBRecordset)
{
	if(!pDBRecordset)
	{
		return DBERR_INVALID_PARAM;
	}
	if(m_UseSTMTMode)
	{
		if(pDBRecordset->IsKindOf(GET_CLASS_INFO(CMySQLDynamicRecordSet)))
		{
			return FetchDynamicResult((CMySQLDynamicRecordSet *)pDBRecordset);
		}
	}
	else
	{
		if(pDBRecordset->IsKindOf(GET_CLASS_INFO(CDBStaticRecordSet)))
		{
			return FetchStaticResult((CDBStaticRecordSet *)pDBRecordset);
		}
		else if(pDBRecordset->IsKindOf(GET_CLASS_INFO(CMySQLRecordSet)))
		{
			return FetchResult((CMySQLRecordSet *)pDBRecordset);
		}
	}
	

	return DBERR_INVALID_PARAM;

}

int CMySQLConnection::NextResults(IDBRecordSet * pDBRecordset)
{
	if(!mysql_more_results(m_MySQLHandle))
	{
		return DBERR_NO_MORE_RESULTS;
	}
	int RetCode;
	if(pDBRecordset->IsKindOf(GET_CLASS_INFO(CMySQLDynamicRecordSet)))
	{
		//RetCode=mysql_stmt_next_result(m_MySQLHandle);
	}
	else
	{
		RetCode=mysql_next_result(m_MySQLHandle);
	}
	if(RetCode>0)
	{
		ProcessErrorMsg(NULL,"ִ��SQLʧ��\r\n");
		return DBERR_EXE_SQL_FAIL;
	}
	return DBERR_SUCCEED;
}

int CMySQLConnection::GetAffectedRowCount()
{
	int RowCount=(int)mysql_affected_rows(m_MySQLHandle);
	if(RowCount==-1)
	{
		ProcessErrorMsg(NULL,"��ȡӰ������ʧ��\r\n");
	}
	return RowCount;
}

int CMySQLConnection::EnableTransaction(BOOL IsEnable)
{
	if(m_MySQLHandle==NULL)
		return DBERR_MYSQL_WANTCONNECT;
	if(mysql_autocommit(m_MySQLHandle,!IsEnable))
	{
		ProcessErrorMsg(NULL,"��������ʧ��\r\n");
		return DBERR_MYSQL_ENABLETRANSACTIONFAIL;
	}
	return DBERR_SUCCEED;
}
int CMySQLConnection::Commit()
{
	if(m_MySQLHandle==NULL)
		return DBERR_MYSQL_WANTCONNECT;
	if(mysql_commit(m_MySQLHandle))
	{
		ProcessErrorMsg(NULL,"�ύ����ʧ��\r\n");
		return DBERR_MYSQL_COMMITFAIL;
	}
	return DBERR_SUCCEED;
}
int CMySQLConnection::RollBack()
{
	if(m_MySQLHandle==NULL)
		return DBERR_MYSQL_WANTCONNECT;
	if(mysql_rollback(m_MySQLHandle))
	{
		ProcessErrorMsg(NULL,"�ع�����ʧ��\r\n");
		return DBERR_MYSQL_ROLLBACKFAIL;
	}
	return DBERR_SUCCEED;
}

UINT CMySQLConnection::GetLastDatabaseErrorCode()
{
	return mysql_errno(m_MySQLHandle);
}

LPCTSTR CMySQLConnection::GetLastDatabaseErrorString()
{
	return mysql_error(m_MySQLHandle);
}

int CMySQLConnection::TranslateString(LPCTSTR szSource,int SrcLen,LPTSTR szTarget,int MaxLen)
{
	return (int)mysql_real_escape_string(m_MySQLHandle,szTarget,szSource,SrcLen);
}

void CMySQLConnection::ProcessErrorMsg(MYSQL_STMT_HANDLE hStmt,LPCTSTR Msg)
{
	if(hStmt)
	{
		PrintDBLog(0xff,"%s %s\r\n",Msg,mysql_stmt_error(hStmt));
	}
	else
	{
		PrintDBLog(0xff,"%s %s\r\n",Msg,mysql_error(m_MySQLHandle));
	}
	
}

int CMySQLConnection::FetchStaticResult(CDBStaticRecordSet * pDBRecordset)
{
	CEasyBuffer ResultBuff;


	if(m_MySQLHandle==NULL)
		return DBERR_MYSQL_WANTCONNECT;

	MYSQL_RES_HANDLE hResults=NULL;
	if(m_MySQLStmt)
	{

	}
	else
	{
		hResults=mysql_store_result(m_MySQLHandle);
		if(hResults==NULL)
		{
			if(mysql_errno(m_MySQLHandle)==0)
				return DBERR_NO_RECORDS;
			else
			{
				ProcessErrorMsg(NULL,"��ȡ�����ʧ��\r\n");
				return DBERR_FETCH_RESULT_FAIL;
			}
		}
	}	

	

	//�������ݼ���С
	int ColNum=mysql_num_fields(hResults);

	if(ColNum<=0)
		return DBERR_NO_RECORDS;

	int RowNum=(int)mysql_num_rows(hResults);
	if(RowNum<=0)
		return DBERR_NO_RECORDS;

	int DataSize=sizeof(int)*2+sizeof(DB_COLUMN_INFO)*ColNum;


	for(int i=0;i<RowNum;i++)
	{
		mysql_data_seek(hResults,i);
		MYSQL_ROW RowData=mysql_fetch_row(hResults);
		ULONG * ValueLen=mysql_fetch_lengths(hResults);
		for(int j=0;j<ColNum;j++)
		{
			DataSize+=sizeof(int)+ValueLen[j]+1;
		}
	}

	DataSize+=1024;
	ResultBuff.Create(DataSize);

	//��ȡ���������Ϣ
	ResultBuff.PushBack(&ColNum,sizeof(int));
	int RecordLineLen=0;
	if(ResultBuff.GetFreeSize()<sizeof(DB_COLUMN_INFO)*ColNum)
		return DBERR_BUFFER_OVERFLOW;

	DB_COLUMN_INFO * pColInfos=(DB_COLUMN_INFO *)ResultBuff.GetFreeBuffer();
	ZeroMemory(pColInfos,sizeof(DB_COLUMN_INFO)*ColNum);
	ResultBuff.SetUsedSize(ResultBuff.GetUsedSize()+sizeof(DB_COLUMN_INFO)*ColNum);
	MYSQL_FIELD * pFields=mysql_fetch_fields(hResults);
	for(int i=0;i<ColNum;i++)
	{
		strncpy_s(pColInfos[i].Name,MAX_COLUMN_NAME,pFields[i].name,MAX_COLUMN_NAME);
		pColInfos[i].Name[MAX_COLUMN_NAME-1]=0;
		pColInfos[i].Type=pFields[i].type;
		pColInfos[i].Size=pFields[i].length;
		pColInfos[i].DigitSize=pFields[i].decimals;

	}


	ResultBuff.PushBack(&RowNum,sizeof(int));
	for(int i=0;i<RowNum;i++)
	{
		mysql_data_seek(hResults,i);
		MYSQL_ROW RowData=mysql_fetch_row(hResults);
		ULONG * ValueLen=mysql_fetch_lengths(hResults);
		for(int j=0;j<ColNum;j++)
		{
			if(RowData[j])
			{
				ResultBuff.PushConstBack(int(ValueLen[j]),sizeof(int));
				ResultBuff.PushBack(RowData[j],ValueLen[j]);
			}
			else
			{
				ResultBuff.PushConstBack(-1,sizeof(int));
			}
		}
	}
	mysql_free_result(hResults);
	return pDBRecordset->Init(ResultBuff.GetBuffer(),ResultBuff.GetUsedSize());

}

int CMySQLConnection::FetchResult(CMySQLRecordSet * pDBRecordset)
{
	if(m_MySQLHandle==NULL)
		return DBERR_MYSQL_WANTCONNECT;
	MYSQL_RES_HANDLE hResults=NULL;

	hResults=mysql_store_result(m_MySQLHandle);
	if(hResults==NULL)
	{
		if(mysql_errno(m_MySQLHandle)==0)
			return DBERR_NO_RECORDS;
		else
		{
			ProcessErrorMsg(NULL,"��ȡ�����ʧ��\r\n");
			return DBERR_FETCH_RESULT_FAIL;
		}
	}
	return pDBRecordset->Init(this,hResults);	
}

int CMySQLConnection::FetchDynamicResult(CMySQLDynamicRecordSet * pDBRecordset)
{
	int Ret=pDBRecordset->Init(this,m_MySQLStmt);
	m_MySQLStmt=NULL;
	return Ret;
}

DWORD CMySQLConnection::FetchConnectFlags(LPCTSTR FlagsStr)
{
	DWORD Flags=0;
	CStringSplitter Splitter(FlagsStr,'|');
	for(int i=0;i<(int)Splitter.GetCount();i++)
	{
		CEasyString Flag=Splitter[i];
		Flag.Trim(' ');
		if(Flag.CompareNoCase("CLIENT_COMPRESS")==0)
		{
			Flags|=CLIENT_COMPRESS;
		}
		else if(Flag.CompareNoCase("CLIENT_FOUND_ROWS")==0)
		{
			Flags|=CLIENT_FOUND_ROWS;
		}
		else if(Flag.CompareNoCase("CLIENT_IGNORE_SPACE")==0)
		{
			Flags|=CLIENT_IGNORE_SPACE;
		}
		else if(Flag.CompareNoCase("CLIENT_INTERACTIVE")==0)
		{
			Flags|=CLIENT_INTERACTIVE;
		}
		else if(Flag.CompareNoCase("CLIENT_LOCAL_FILES")==0)
		{
			Flags|=CLIENT_LOCAL_FILES;
		}
		else if(Flag.CompareNoCase("CLIENT_MULTI_STATEMENTS")==0)
		{
			Flags|=CLIENT_MULTI_STATEMENTS;
		}
		else if(Flag.CompareNoCase("CLIENT_MULTI_RESULTS")==0)
		{
			Flags|=CLIENT_MULTI_RESULTS;
		}
		else if(Flag.CompareNoCase("CLIENT_NO_SCHEMA")==0)
		{
			Flags|=CLIENT_NO_SCHEMA;
		}
		else if(Flag.CompareNoCase("CLIENT_ODBC")==0)
		{
			Flags|=CLIENT_ODBC;
		}
		else if(Flag.CompareNoCase("CLIENT_SSL")==0)
		{
			Flags|=CLIENT_SSL;
		}
		else if(Flag.CompareNoCase("CLIENT_STMT_MODE")==0)
		{
			m_UseSTMTMode=true;
		}

	}
	return Flags;
}

int CMySQLConnection::DBLibTypeToMySQLType(int Type,UINT& Size,UINT& DitigalSize)
{	
	switch(Type)
	{	
	case DB_TYPE_STRING:
	case DB_TYPE_WSTRING:
		return MYSQL_TYPE_VAR_STRING;
	case DB_TYPE_TINY:
		Size=sizeof(BYTE);
		return MYSQL_TYPE_TINY;
	case DB_TYPE_SMALLINT:
		Size=sizeof(short);
		return MYSQL_TYPE_SHORT;
	case DB_TYPE_INTEGER:
		Size=sizeof(int);
		return MYSQL_TYPE_LONG;
	case DB_TYPE_BIGINT:
		Size=sizeof(INT64);
		return MYSQL_TYPE_LONGLONG;
	case DB_TYPE_FLOAT:
		Size=sizeof(float);
		return MYSQL_TYPE_FLOAT;
	case DB_TYPE_DOUBLE:
		Size=sizeof(double);
		return MYSQL_TYPE_DOUBLE;
	case DB_TYPE_DATE:
		Size=sizeof(MYSQL_TIME);
		return MYSQL_TYPE_DATE;
	case DB_TYPE_TIME:
		Size=sizeof(MYSQL_TIME);
		return MYSQL_TYPE_DATE;
	case DB_TYPE_TIMESTAMP:
		Size=sizeof(MYSQL_TIME);
		return MYSQL_TYPE_TIMESTAMP;	
	case DB_TYPE_BINARY:
		return MYSQL_TYPE_BIT;		
	default:
		return -1;
	}
}

int CMySQLConnection::MySQLTypeToDBLibType(int Type,UINT& Size,UINT& DitigalSize)
{
	switch(Type)
	{	
	case MYSQL_TYPE_DECIMAL:
		Size=32+DitigalSize+1;
		return DB_TYPE_STRING;
	case MYSQL_TYPE_TINY:
		Size=sizeof(BYTE);
		return DB_TYPE_TINY;
	case MYSQL_TYPE_SHORT:
		Size=sizeof(short);
		return DB_TYPE_SMALLINT;
	case MYSQL_TYPE_LONG:
		Size=sizeof(long);
		return DB_TYPE_INTEGER;
	case MYSQL_TYPE_FLOAT:
		Size=sizeof(float);
		return DB_TYPE_FLOAT;
	case MYSQL_TYPE_DOUBLE:
		Size=sizeof(double);
		return DB_TYPE_DOUBLE;
	case MYSQL_TYPE_NULL:
		Size=0;
		return DB_TYPE_NULL;
	case MYSQL_TYPE_TIMESTAMP:
		Size=sizeof(DB_TIMESTAMP);
		return DB_TYPE_TIMESTAMP;
	case MYSQL_TYPE_LONGLONG:
		Size=sizeof(INT64);
		return DB_TYPE_BIGINT;
	case MYSQL_TYPE_INT24:
		Size=sizeof(long);
		return DB_TYPE_INTEGER;
	case MYSQL_TYPE_DATE:
		Size=sizeof(DB_DATE);
		return DB_TYPE_DATE;
	case MYSQL_TYPE_TIME:
		Size=sizeof(DB_TIME);
		return DB_TYPE_TIME;
	case MYSQL_TYPE_DATETIME:
		Size=sizeof(DB_TIMESTAMP);
		return DB_TYPE_TIMESTAMP;
	case MYSQL_TYPE_YEAR:
		Size=sizeof(long);
		return DB_TYPE_INTEGER;
	case MYSQL_TYPE_NEWDATE:
		Size=sizeof(DB_DATE);
		return DB_TYPE_DATE;
	case MYSQL_TYPE_VARCHAR:
		return DB_TYPE_STRING;
	case MYSQL_TYPE_BIT:
		return DB_TYPE_BINARY;
	case MYSQL_TYPE_NEWDECIMAL:
		Size=32+DitigalSize+1;
		return DB_TYPE_STRING;
	case MYSQL_TYPE_ENUM:
		return DB_TYPE_STRING;
	case MYSQL_TYPE_SET:
		return DB_TYPE_STRING;
	case MYSQL_TYPE_TINY_BLOB:		
	case MYSQL_TYPE_MEDIUM_BLOB:
	case MYSQL_TYPE_LONG_BLOB:
	case MYSQL_TYPE_BLOB:
		return DB_TYPE_BINARY;
	case MYSQL_TYPE_VAR_STRING:
	case MYSQL_TYPE_STRING:
		return DB_TYPE_STRING;
	case MYSQL_TYPE_GEOMETRY:
		return DB_TYPE_BINARY;
	default:
		return DB_TYPE_UNKNOW;
	}
}

UINT CMySQLConnection::GetMySQLTypeBinLength(int Type,UINT Size,UINT DitigalSize)
{
	switch(Type)
	{	
	case MYSQL_TYPE_DECIMAL:		
		break;
	case MYSQL_TYPE_TINY:
		Size=sizeof(BYTE);
		break;
	case MYSQL_TYPE_SHORT:
		Size=sizeof(short);
		break;
	case MYSQL_TYPE_LONG:
		Size=sizeof(long);
		break;
	case MYSQL_TYPE_FLOAT:
		Size=sizeof(float);
		break;
	case MYSQL_TYPE_DOUBLE:
		Size=sizeof(double);
		break;
	case MYSQL_TYPE_NULL:
		Size=0;
		break;
	case MYSQL_TYPE_TIMESTAMP:
		Size=sizeof(MYSQL_TIME);
		break;
	case MYSQL_TYPE_LONGLONG:
		Size=sizeof(INT64);
		break;
	case MYSQL_TYPE_INT24:
		Size=sizeof(long);
		break;
	case MYSQL_TYPE_DATE:		
	case MYSQL_TYPE_TIME:		
	case MYSQL_TYPE_DATETIME:
		Size=sizeof(MYSQL_TIME);
		break;
	case MYSQL_TYPE_YEAR:
		Size=sizeof(long);
		break;
	case MYSQL_TYPE_NEWDATE:
		Size=sizeof(MYSQL_TIME);
		break;
	case MYSQL_TYPE_VARCHAR:		
	case MYSQL_TYPE_BIT:		
	case MYSQL_TYPE_NEWDECIMAL:		
	case MYSQL_TYPE_ENUM:		
	case MYSQL_TYPE_SET:		
	case MYSQL_TYPE_TINY_BLOB:		
	case MYSQL_TYPE_MEDIUM_BLOB:
	case MYSQL_TYPE_LONG_BLOB:
	case MYSQL_TYPE_BLOB:		
	case MYSQL_TYPE_VAR_STRING:
	case MYSQL_TYPE_STRING:		
	case MYSQL_TYPE_GEOMETRY:
		break;
	}
	return Size;
}


BOOL CMySQLConnection::MySQLStrValueToDBValue(int ValueType,LPCVOID pData,int DataSize,int DitigalSize,LPVOID pDBValue,int DBValueSize)
{
	switch(ValueType)
	{
	case MYSQL_TYPE_TINY:
		if(DBValueSize>=sizeof(char))
		{			
			*((char *)pDBValue)=(char)atol((LPCTSTR)pData);			
			return TRUE;
		}
	case MYSQL_TYPE_SHORT:
		if(DBValueSize>=sizeof(short))
		{
			*((short *)pDBValue)=(short)atol((LPCTSTR)pData);				
			return TRUE;
		}
	case MYSQL_TYPE_LONG:
		if(DBValueSize>=sizeof(long))
		{
			*((long *)pDBValue)=(long)atol((LPCTSTR)pData);
			return TRUE;
		}
	case MYSQL_TYPE_INT24:
		if(DBValueSize>=sizeof(long))
		{
			*((long *)pDBValue)=(long)atol((LPCTSTR)pData);
			return TRUE;
		}
	case MYSQL_TYPE_LONGLONG:
		if(DBValueSize>=sizeof(INT64))
		{
			*((INT64 *)pDBValue)=(INT64)_atoi64((LPCTSTR)pData);
			return TRUE;
		}
	case MYSQL_TYPE_DECIMAL:
		if(DBValueSize>=DataSize)
		{
			memcpy(pDBValue,pData,DataSize);
			return TRUE;
		}
	case MYSQL_TYPE_FLOAT:
		if(DBValueSize>=sizeof(float))
		{
			*((float *)pDBValue)=(float)atof((LPCTSTR)pData);
			return TRUE;
		}
	case MYSQL_TYPE_DOUBLE:
		if(DBValueSize>=sizeof(double))
		{
			*((double *)pDBValue)=(double)atof((LPCTSTR)pData);
			return TRUE;
		}
	case MYSQL_TYPE_NEWDECIMAL:
		if(DBValueSize>=DataSize)
		{
			memcpy(pDBValue,pData,DataSize);
			return TRUE;
		}
	case MYSQL_TYPE_BIT:
		if(DBValueSize>=DataSize)
		{
			memcpy(pDBValue,pData,DataSize);
			return TRUE;
		}
	case MYSQL_TYPE_TIMESTAMP:
		if(DBValueSize>=sizeof(DB_TIMESTAMP))
		{
			DB_TIMESTAMP * pValue=(DB_TIMESTAMP *)pDBValue;
			sscanf((LPCTSTR)pData,"%hd-%hd-%hd %hd:%hd:%hd.&d",
				&pValue->year,&pValue->month,&pValue->day,
				&pValue->hour,&pValue->minute,&pValue->second,
				&pValue->fraction);
			return TRUE;
		}
	case MYSQL_TYPE_DATE:
		if(DBValueSize>=sizeof(DB_DATE))
		{
			DB_DATE * pValue=(DB_DATE *)pDBValue;
			sscanf((LPCTSTR)pData,"%hd-%hd-%hd",
				&pValue->year,&pValue->month,&pValue->day);
			return TRUE;
		}
	case MYSQL_TYPE_TIME:
		if(DBValueSize>=sizeof(DB_TIME))
		{
			DB_TIME * pValue=(DB_TIME *)pDBValue;
			sscanf((LPCTSTR)pData,"%hd:%hd:%hd",
				&pValue->hour,&pValue->minute,&pValue->second);
			return TRUE;
		}
	case MYSQL_TYPE_DATETIME:
		if(DBValueSize>=sizeof(DB_TIMESTAMP))
		{
			DB_TIMESTAMP * pValue=(DB_TIMESTAMP *)pDBValue;
			sscanf((LPCTSTR)pData,"%hd-%hd-%hd %hd:%hd:%hd",
				&pValue->year,&pValue->month,&pValue->day,
				&pValue->hour,&pValue->minute,&pValue->second);
			return TRUE;
		}
	case MYSQL_TYPE_YEAR:
		if(DBValueSize>=sizeof(long))
		{
			*((double *)pDBValue)=(long)atol((LPCTSTR)pData);
			return TRUE;
		}
	case MYSQL_TYPE_SET:
	case MYSQL_TYPE_ENUM:
	case MYSQL_TYPE_STRING:		
	case MYSQL_TYPE_VAR_STRING:		
	case MYSQL_TYPE_GEOMETRY:
	case MYSQL_TYPE_BLOB:
		if(DBValueSize>=sizeof(DataSize))
		{
			memcpy(pDBValue,pData,DataSize);
			return TRUE;
		}		
	}
	return FALSE;
}

BOOL CMySQLConnection::DBValueToMySQLBinValue(int Type,LPCVOID pDBValue,int DBValueSize,LPVOID pData,int DataSize)
{
	switch(Type)
	{	
	case DB_TYPE_STRING:
	case DB_TYPE_WSTRING:
		if(DataSize>=DBValueSize)
		{
			memcpy(pData,pDBValue,DBValueSize);
			return TRUE;
		}
		break;
	case DB_TYPE_TINY:
		if(DataSize>=sizeof(BYTE))
		{
			memcpy(pData,pDBValue,sizeof(BYTE));
			return TRUE;
		}
		break;
	case DB_TYPE_SMALLINT:
		if(DataSize>=sizeof(short))
		{
			memcpy(pData,pDBValue,sizeof(short));
			return TRUE;
		}
		break;
	case DB_TYPE_INTEGER:
		if(DataSize>=sizeof(int))
		{
			memcpy(pData,pDBValue,sizeof(int));
			return TRUE;
		}
		break;
	case DB_TYPE_BIGINT:
		if(DataSize>=sizeof(INT64))
		{
			memcpy(pData,pDBValue,sizeof(INT64));
			return TRUE;
		}
		break;
	case DB_TYPE_FLOAT:
		if(DataSize>=sizeof(float))
		{
			memcpy(pData,pDBValue,sizeof(float));
			return TRUE;
		}
		break;
	case DB_TYPE_DOUBLE:
		if(DataSize>=sizeof(double))
		{
			memcpy(pData,pDBValue,sizeof(double));
			return TRUE;
		}
		break;
	case DB_TYPE_DATE:
		if(DataSize>=sizeof(MYSQL_TIME))
		{
			MYSQL_TIME * pMySQLTime=(MYSQL_TIME *)pData;
			DB_DATE * pDBTime=(DB_DATE *)(pDBValue);
			pMySQLTime->year=pDBTime->year;
			pMySQLTime->month=pDBTime->month;
			pMySQLTime->day=pDBTime->day;
			pMySQLTime->hour=0;
			pMySQLTime->minute=0;
			pMySQLTime->second=0;
			pMySQLTime->second_part=0;
			pMySQLTime->neg=0;
			return TRUE;
		}
		break;
	case DB_TYPE_TIME:
		if(DataSize>=sizeof(MYSQL_TIME))
		{
			MYSQL_TIME * pMySQLTime=(MYSQL_TIME *)pData;
			DB_TIME * pDBTime=(DB_TIME *)(pDBValue);
			pMySQLTime->year=0;
			pMySQLTime->month=0;
			pMySQLTime->day=0;
			pMySQLTime->hour=pDBTime->hour;
			pMySQLTime->minute=pDBTime->minute;
			pMySQLTime->second=pDBTime->minute;
			pMySQLTime->second_part=0;
			pMySQLTime->neg=0;
			return TRUE;
		}
		break;
	case DB_TYPE_TIMESTAMP:
		if(DataSize>=sizeof(MYSQL_TIME))
		{
			MYSQL_TIME * pMySQLTime=(MYSQL_TIME *)pData;
			DB_TIMESTAMP * pDBTime=(DB_TIMESTAMP *)(pDBValue);
			pMySQLTime->year=pDBTime->year;
			pMySQLTime->month=pDBTime->month;
			pMySQLTime->day=pDBTime->day;
			pMySQLTime->hour=pDBTime->hour;
			pMySQLTime->minute=pDBTime->minute;
			pMySQLTime->second=pDBTime->second;
			pMySQLTime->second_part=pDBTime->fraction;
			pMySQLTime->neg=0;
			return TRUE;
		}
		break;
	case DB_TYPE_BINARY:
		if(DataSize>=DBValueSize)
		{
			memcpy(pData,pDBValue,DBValueSize);
			return TRUE;
		}
		break;	
	}
	return FALSE;
}

BOOL CMySQLConnection::MySQLBinValueToDBValue(int Type,LPCVOID pData,int DataSize,LPVOID pDBValue,int DBValueSize)
{
	switch(Type)
	{	
	case MYSQL_TYPE_DECIMAL:
		if(DBValueSize>=DataSize)
		{
			memcpy(pDBValue,pData,DataSize);			
			return TRUE;
		}
		break;
	case MYSQL_TYPE_TINY:
		if(DBValueSize>=sizeof(BYTE))
		{
			memcpy(pDBValue,pData,sizeof(BYTE));
			return TRUE;
		}
		break;
	case MYSQL_TYPE_SHORT:
		if(DBValueSize>=sizeof(short))
		{
			memcpy(pDBValue,pData,sizeof(short));
			return TRUE;
		}
		break;
	case MYSQL_TYPE_LONG:
		if(DBValueSize>=sizeof(long))
		{
			memcpy(pDBValue,pData,sizeof(long));
			return TRUE;
		}
		break;
	case MYSQL_TYPE_FLOAT:
		if(DBValueSize>=sizeof(float))
		{
			memcpy(pDBValue,pData,sizeof(float));
			return TRUE;
		}
		break;
	case MYSQL_TYPE_DOUBLE:
		if(DBValueSize>=sizeof(double))
		{
			memcpy(pDBValue,pData,sizeof(double));
			return TRUE;
		}
		break;
	case MYSQL_TYPE_NULL:
		return TRUE;
		break;
	case MYSQL_TYPE_TIMESTAMP:
		if(DBValueSize>=sizeof(DB_TIMESTAMP))
		{
			MYSQL_TIME * pMySQLTime=(MYSQL_TIME *)pData;
			DB_TIMESTAMP * pDBTime=(DB_TIMESTAMP *)pDBValue;
			pDBTime->year=pMySQLTime->year;
			pDBTime->month=pMySQLTime->month;
			pDBTime->day=pMySQLTime->day;
			pDBTime->hour=pMySQLTime->hour;
			pDBTime->minute=pMySQLTime->minute;
			pDBTime->second=pMySQLTime->second;
			pDBTime->fraction=pMySQLTime->second_part;			
			return TRUE;
		}
		break;
	case MYSQL_TYPE_LONGLONG:
		if(DBValueSize>=sizeof(INT64))
		{
			memcpy(pDBValue,pData,sizeof(INT64));
			return TRUE;
		}
		break;
	case MYSQL_TYPE_INT24:
		if(DBValueSize>=sizeof(long))
		{
			memcpy(pDBValue,pData,sizeof(long));
			return TRUE;
		}
		break;
	case MYSQL_TYPE_DATE:
		if(DBValueSize>=sizeof(DB_DATE))
		{
			MYSQL_TIME * pMySQLTime=(MYSQL_TIME *)pData;
			DB_DATE * pDBTime=(DB_DATE *)pDBValue;
			pDBTime->year=pMySQLTime->year;
			pDBTime->month=pMySQLTime->month;
			pDBTime->day=pMySQLTime->day;			
			return TRUE;
		}
		break;
	case MYSQL_TYPE_TIME:
		if(DBValueSize>=sizeof(DB_TIME))
		{
			MYSQL_TIME * pMySQLTime=(MYSQL_TIME *)pData;
			DB_TIME * pDBTime=(DB_TIME *)pDBValue;			
			pDBTime->hour=pMySQLTime->hour;
			pDBTime->minute=pMySQLTime->minute;
			pDBTime->minute=pMySQLTime->second;			
			return TRUE;
		}
		break;
	case MYSQL_TYPE_DATETIME:
		if(DBValueSize>=sizeof(DB_TIMESTAMP))
		{
			MYSQL_TIME * pMySQLTime=(MYSQL_TIME *)pData;
			DB_TIMESTAMP * pDBTime=(DB_TIMESTAMP *)pDBValue;
			pDBTime->year=pMySQLTime->year;
			pDBTime->month=pMySQLTime->month;
			pDBTime->day=pMySQLTime->day;
			pDBTime->hour=pMySQLTime->hour;
			pDBTime->minute=pMySQLTime->minute;
			pDBTime->second=pMySQLTime->second;
			pDBTime->fraction=pMySQLTime->second_part;			
			return TRUE;
		}
		break;
	case MYSQL_TYPE_YEAR:
		if(DBValueSize>=sizeof(long))
		{
			memcpy(pDBValue,pData,sizeof(long));
			return TRUE;
		}
		break;
	case MYSQL_TYPE_NEWDATE:
		if(DBValueSize>=sizeof(DB_DATE))
		{
			MYSQL_TIME * pMySQLTime=(MYSQL_TIME *)pData;
			DB_DATE * pDBTime=(DB_DATE *)pDBValue;
			pDBTime->year=pMySQLTime->year;
			pDBTime->month=pMySQLTime->month;
			pDBTime->day=pMySQLTime->day;			
			return TRUE;
		}
		break;
	case MYSQL_TYPE_VARCHAR:	
	case MYSQL_TYPE_BIT:
	case MYSQL_TYPE_NEWDECIMAL:
	case MYSQL_TYPE_ENUM:
	case MYSQL_TYPE_SET:
	case MYSQL_TYPE_TINY_BLOB:
	case MYSQL_TYPE_MEDIUM_BLOB:
	case MYSQL_TYPE_LONG_BLOB:
	case MYSQL_TYPE_BLOB:
	case MYSQL_TYPE_VAR_STRING:
	case MYSQL_TYPE_STRING:
	case MYSQL_TYPE_GEOMETRY:
		if(DBValueSize>=DataSize)
		{
			memcpy(pDBValue,pData,DataSize);			
			return TRUE;
		}
		break;
	}	
	return FALSE;
}

int CMySQLConnection::ExecuteSQLDirect(LPCSTR SQLStr,int StrLen)
{
	if(m_MySQLHandle==NULL)
		return DBERR_MYSQL_WANTCONNECT;
	if(!SQLStr)
	{
		return DBERR_INVALID_PARAM;
	}


	if(StrLen==0)
		StrLen=(int)strlen(SQLStr);
	if(mysql_real_query(m_MySQLHandle,SQLStr,StrLen))
	{
		ProcessErrorMsg(NULL,"ִ��SQLʧ��\r\n");
		return DBERR_EXE_SQL_FAIL;
	}

	return DBERR_SUCCEED;
}

int CMySQLConnection::ExecuteSQLWithParam(LPCSTR SQLStr,int StrLen,CDBParameterSet * pParamSet)
{
	if(m_MySQLHandle==NULL)
		return DBERR_MYSQL_WANTCONNECT;
	if(!SQLStr)
	{
		return DBERR_INVALID_PARAM;
	}

	if(m_MySQLStmt)
	{
		mysql_stmt_close(m_MySQLStmt);
		m_MySQLStmt=NULL;
	}

	m_MySQLStmt=mysql_stmt_init(m_MySQLHandle);
	if(m_MySQLStmt==NULL)
	{
		ProcessErrorMsg(NULL,"���价�����ʧ��\r\n");
		return DBERR_SQLALLOCHANDLEFAIL;
	}


	if(StrLen==0)
		StrLen=(int)strlen(SQLStr);
	if(mysql_stmt_prepare(m_MySQLStmt,SQLStr,StrLen))
	{
		ProcessErrorMsg(m_MySQLStmt,"׼��SQLʧ��\r\n");
		return DBERR_EXE_SQL_FAIL;
	}
	vector<MYSQL_BIND> ParamList;

	UINT ParamNum=mysql_stmt_param_count(m_MySQLStmt);
	CEasyBuffer ParamBuffer;

	if(ParamNum>0)
	{

		if(ParamNum>(UINT)pParamSet->GetCount())
		{
			return DBERR_NOTENOUGHPARAM;
		}	

		

		ParamList.resize(ParamNum);
		ZeroMemory(&(ParamList[0]),sizeof(MYSQL_BIND)*ParamNum);

		//���������Ҫ�Ļ����С
		UINT ParamBufferSize=0;
		for(UINT i=0;i<ParamNum;i++)
		{
			UINT Size=pParamSet->GetParam(i).GetLength();
			UINT DigitalSize=pParamSet->GetParam(i).GetDigitalLength();
			ParamList[i].buffer_type=(enum_field_types)DBLibTypeToMySQLType(pParamSet->GetParamInfo(i)->Type,Size,DigitalSize);
			ParamBufferSize+=Size+sizeof(UINT)+sizeof(my_bool);
		}
		//ת�����ݵ�����
		ParamBuffer.Create(ParamBufferSize);
		for(UINT i=0;i<ParamNum;i++)
		{
			
			UINT Size=pParamSet->GetParam(i).GetLength();
			UINT DigitalSize=pParamSet->GetParam(i).GetDigitalLength();
			ParamList[i].buffer_type=(enum_field_types)DBLibTypeToMySQLType(pParamSet->GetParamInfo(i)->Type,Size,DigitalSize);
			
			ParamList[i].buffer=(char *)ParamBuffer.GetFreeBuffer();
			ParamList[i].buffer_length=Size;
			ParamBuffer.PushBack(NULL,Size);
			ParamList[i].length=(ULONG *)ParamBuffer.GetFreeBuffer();
			ParamBuffer.PushBack(&Size,sizeof(UINT));
			ParamList[i].is_null=(my_bool *)ParamBuffer.GetFreeBuffer();
			ParamBuffer.PushConstBack(0,sizeof(my_bool));

			if(pParamSet->GetParam(i).IsNull())
			{
				*(ParamList[i].is_null)=false;
				*(ParamList[i].length)=Size;
				ParamList[i].length_value=Size;
			
			}
			else
			{	
				*(ParamList[i].is_null)=false;
				*(ParamList[i].length)=Size;
				ParamList[i].length_value=Size;
				if(!DBValueToMySQLBinValue(pParamSet->GetParam(i).GetType(),
					pParamSet->GetParam(i).GetBuffer(),pParamSet->GetParam(i).GetLength(),
					ParamList[i].buffer,ParamList[i].buffer_length))
				{
					return DBERR_BINDPARAMFAIL;
				}
				
			}
		}

		//�󶨲���
		if(mysql_stmt_bind_param(m_MySQLStmt,&(ParamList[0])))
		{
			ProcessErrorMsg(m_MySQLStmt,"�󶨲���ʧ��\r\n");
			return DBERR_BINDPARAMFAIL;
		}
	}
	if(mysql_stmt_execute(m_MySQLStmt))
	{
		ProcessErrorMsg(m_MySQLStmt,"ִ��SQLʧ��\r\n");
		return DBERR_EXE_SQL_FAIL;
	}
	if(ParamNum)
	{
		//�ӻ��彫����ת������
		for(UINT i=0;i<ParamNum;i++)
		{
			if(pParamSet->GetParamInfo(i)->ParamType==DB_PARAM_TYPE_OUTPUT||
				pParamSet->GetParamInfo(i)->ParamType==DB_PARAM_TYPE_INPUT_OUTPUT)
			{
				if(*(ParamList[i].is_null))
				{
					pParamSet->GetParam(i).SetValue(pParamSet->GetParam(i).GetType(),NULL,0,0);
				}
				else
				{
					UINT Size=*(ParamList[i].length);
					UINT DigitalSize=0;
					int DBType=MySQLTypeToDBLibType(ParamList[i].buffer_type,Size,DigitalSize);
					pParamSet->GetParam(i).SetEmptyValue(DBType,Size,DigitalSize);
					if(!MySQLBinValueToDBValue(ParamList[i].buffer_type,
						ParamList[i].buffer,*(ParamList[i].length),
						pParamSet->GetParam(i).GetBuffer(),pParamSet->GetParam(i).GetLength()))
					{
						return DBERR_BINDPARAMFAIL;
					}
				}
			}
		}
	}

	return DBERR_SUCCEED;
}

}
