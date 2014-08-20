/****************************************************************************/
/*                                                                          */
/*      �ļ���:    ODBCRecordSet.h                                          */
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
class CODBCConnection;

class CODBCRecordSet :
	public IDBRecordSet
{
protected:
	CODBCConnection *			m_pDBConnection;
	SQLHSTMT					m_hStmt;
	CEasyArray<DB_COLUMN_INFO>	m_ColInfos;	
	CEasyArray<int>				m_BindTypes;
	CEasyArray<SQLLEN>			m_FieldSize;
	CEasyBuffer					m_RecordLineBuffer;
	CEasyArray<CDBValue>		m_RowBuffer;
	int							m_CurRow;
	int							m_RecordCount;
	CDBValue					m_EmptyValue;
	bool						m_IsBOF;
	bool						m_IsEOF;
	bool						m_IsForwardOnly;
	CEasyArray<DB_COLUMN_INFO>	m_ParamInfos;
	CEasyArray<CDBValue>		m_ParamBuffer;

	DECLARE_CLASS_INFO(CODBCRecordSet)
public:
	CODBCRecordSet(void);
	virtual ~CODBCRecordSet(void);

	int Init(CODBCConnection * pDBConnection,SQLHSTMT hStmt);
	virtual void Destory();

	SQLHSTMT GetHSTMT();
	SQLHSTMT StealHSTMT();

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
	int FetchRow(int Orientation,int Offset);
	void UpdateStatus(bool IsValid,int Orientation,int Offset);
};

}