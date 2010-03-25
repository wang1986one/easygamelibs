/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DBStaticRecordSet.h                                      */
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



class CDBStaticRecordSet :
	public IDBRecordSet
{
protected:
	typedef std::vector<CDBValue> CDBValueRow;	
	std::vector<DB_COLUMN_INFO>		m_pColumnInfos;
	std::vector<CDBValueRow>		m_Records;
	std::vector<CDBValue>			m_Params;
	int								m_CurRow;
	CDBValue						m_EmptyValue;

	DECLARE_CLASS_INFO(CDBStaticRecordSet)
public:
	CDBStaticRecordSet(void);
	virtual ~CDBStaticRecordSet(void);	

	virtual int Init(LPCVOID pData,int DataSize);	
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

};

}
