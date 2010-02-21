#pragma once

namespace DBLib
{
class CMySQLConnection;
typedef MYSQL_STMT * MYSQL_STMT_HANDLE;

class CMySQLDynamicRecordSet :
	public IDBRecordSet
{
protected:
	CMySQLConnection *				m_pDBConnection;
	MYSQL_STMT_HANDLE				m_hStmt;
	std::vector<DB_COLUMN_INFO>		m_pColumnInfos;
	std::vector<CDBValue>			m_RowBuffer;
	std::vector<MYSQL_BIND>			m_FetchBuffer;
	CEasyBuffer						m_FetchDataBuffer;
	CDBValue						m_EmptyValue;
	bool							m_IsBOF;
	bool							m_IsEOF;
	bool							m_CacheAllData;

	DECLARE_CLASS_INFO(CMySQLDynamicRecordSet)
public:
	CMySQLDynamicRecordSet(bool CacheAllData=true);
	~CMySQLDynamicRecordSet(void);

	int Init(CMySQLConnection * pDBConnection,MYSQL_STMT_HANDLE hStmt);
	virtual void Destory();

	virtual int GetRecordCount();
	virtual int GetColumnCount();
	virtual LPCTSTR GetColumnName(int Index);
	virtual int GetIndexByColumnName(LPCTSTR Name);
	virtual DB_COLUMN_INFO * GetColumnInfo(int Index);


	virtual CDBValue& GetField(int Index);
	virtual CDBValue& GetField(LPCTSTR Name);
	virtual int MoveFirst();
	virtual int MoveLast();
	virtual int MoveNext();
	virtual int MovePrevious();
	virtual int MoveTo(int Index);	
	virtual bool IsEOF();
	virtual bool IsBOF();

	virtual bool Close();
protected:
	int FetchRow();
};

}