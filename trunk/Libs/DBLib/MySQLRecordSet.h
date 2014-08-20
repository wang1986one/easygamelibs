/****************************************************************************/
/*                                                                          */
/*      �ļ���:    MySQLRecordSet.h                                         */
/*      ��������:  2009��07��06��                                           */
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

class CMySQLConnection;
typedef MYSQL_RES * MYSQL_RES_HANDLE;

class CMySQLRecordSet :
	public IDBRecordSet
{
protected:
	CMySQLConnection *				m_pDBConnection;
	MYSQL_RES_HANDLE				m_hResults;
	CEasyArray<DB_COLUMN_INFO>		m_pColumnInfos;
	CEasyArray<CDBValue>			m_RowBuffer;
	int								m_CurRow;
	CDBValue						m_EmptyValue;

	DECLARE_CLASS_INFO(CMySQLRecordSet)
public:
	CMySQLRecordSet(void);
	virtual ~CMySQLRecordSet(void);

	int Init(CMySQLConnection * pDBConnection,MYSQL_RES_HANDLE hResults);
	virtual void Destory();

	virtual int GetRecordCount();
	virtual int GetColumnCount();
	virtual LPCSTR GetColumnName(int Index);
	virtual int GetIndexByColumnName(LPCSTR Name);
	virtual DB_COLUMN_INFO * GetColumnInfo(int Index);


	virtual CDBValue& GetField(int Index);
	virtual CDBValue& GetField(LPCSTR Name);
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