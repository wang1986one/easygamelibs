/****************************************************************************/
/*                                                                          */
/*      �ļ���:    MySQLConnection.h                                        */
/*      ��������:  2009��09��11��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once
#include "dbinterfaces.h"


namespace DBLib
{

typedef MYSQL * MYSQL_HANDLE;
typedef MYSQL_RES * MYSQL_RES_HANDLE;
typedef MYSQL_STMT * MYSQL_STMT_HANDLE;

class CMySQLDatabase;

class CMySQLConnection :
	public IDBConnection
{
protected:
	MYSQL_HANDLE		m_MySQLHandle;
	MYSQL_STMT_HANDLE	m_MySQLStmt;
	CMySQLDatabase *	m_pDatabase;
	bool				m_UseSTMTMode;

	DECLARE_CLASS_INFO(CMySQLConnection)
public:
	CMySQLConnection(void);
	virtual ~CMySQLConnection(void);

	int Init(CMySQLDatabase * pDatabase);
	virtual void Destory();

	virtual IDBRecordSet * CreateRecordSet(int RecordSetType);
	virtual IDBParameterSet * CreateParameterSet(int RecordSetType);

	virtual int Connect(LPCTSTR ConnectStr);
	virtual int Disconnect();
	virtual BOOL IsConnected();
	virtual int ExecuteSQL(LPCSTR SQLStr,int StrLen=0,IDBParameterSet * pParamSet=NULL);		
	virtual int GetResults(IDBRecordSet * pDBRecordset);
	virtual int NextResults(IDBRecordSet * pDBRecordset);	
	virtual int GetAffectedRowCount();
	virtual int EnableTransaction(BOOL IsEnable);
	virtual int Commit();
	virtual int RollBack();

	virtual UINT GetLastDatabaseErrorCode();
	virtual LPCTSTR GetLastDatabaseErrorString();

	virtual int TranslateString(LPCTSTR szSource,int SrcLen,LPTSTR szTarget,int MaxLen);

	static int DBLibTypeToMySQLType(int Type,UINT& Size,UINT& DitigalSize);
	static int MySQLTypeToDBLibType(int Type,UINT& Size,UINT& DitigalSize);
	static UINT GetMySQLTypeBinLength(int Type,UINT Size,UINT DitigalSize);

	static BOOL MySQLStrValueToDBValue(int ValueType,LPCVOID pData,int DataSize,int DitigalSize,LPVOID pDBValue,int DBValueSize);

	static BOOL DBValueToMySQLBinValue(int Type,LPCVOID pDBValue,int DBValueSize,LPVOID pData,int DataSize);
	static BOOL MySQLBinValueToDBValue(int Type,LPCVOID pData,int DataSize,LPVOID pDBValue,int DBValueSize);

	void ProcessErrorMsg(MYSQL_STMT_HANDLE hStmt,LPCTSTR Msg);
protected:
	int FetchStaticResult(CDBStaticRecordSet * pDBRecordset);
	int FetchResult(CMySQLRecordSet * pDBRecordset);
	int FetchDynamicResult(CMySQLDynamicRecordSet * pDBRecordset);
	DWORD FetchConnectFlags(LPCTSTR FlagsStr);

	int ExecuteSQLDirect(LPCSTR SQLStr,int StrLen);
	int ExecuteSQLWithParam(LPCSTR SQLStr,int StrLen,CDBParameterSet * pParamSet);
};

}