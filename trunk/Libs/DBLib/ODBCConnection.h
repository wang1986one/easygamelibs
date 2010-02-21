#pragma once
#include "dbinterfaces.h"


namespace DBLib
{

class CODBCDatabase;
class CDBRecordSet;

class CODBCConnection :
	public IDBConnection
{
protected:
	SQLHDBC					m_hDBConn;	
	CODBCDatabase *			m_pDatabase;
	SQLHSTMT				m_hStmt;
	CEasyString				m_LastErrorString;
	int						m_LastErrorCode;
	
	DECLARE_CLASS_INFO(CODBCConnection)
public:
	CODBCConnection(void);
	virtual ~CODBCConnection(void);

	int Init(CODBCDatabase * pDatabase);
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
	
	void ProcessMessagesODBC(SQLSMALLINT plm_handle_type,SQLHANDLE plm_handle,char *logstring,int ConnInd);
	static int ODBCCTypeTODBLibType(int Type,UINT& Size);
	static int DBLibTypeToODBCCType(int Type,UINT& Size);
	static int DBLibTypeToODBCSQLType(int Type,UINT& Size);
	static int ODBCSQLTypeTOODBCCType(int Type,UINT& Size);
	
	static int DBParamTypeToODBCParamType(int Type);

protected:
	virtual int FetchStaticResult(SQLHSTMT hStmt,CDBStaticRecordSet * pDBRecordset);
	int ExecuteSQLDirect(LPCSTR SQLStr,int StrLen);
	int ExecuteSQLWithParam(LPCSTR SQLStr,int StrLen,CDBParameterSet * pParamSet);
	
};



}