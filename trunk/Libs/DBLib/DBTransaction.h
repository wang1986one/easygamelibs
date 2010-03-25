/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DBTransaction.h                                          */
/*      ��������:  2009��07��06��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once

namespace DBLib
{

class CDBTransaction :
	public CNameObject
{
protected:
	bool		m_IsSucceed;

	DECLARE_CLASS_INFO_STATIC(CDBTransaction)
public:
	CDBTransaction(void);
	~CDBTransaction(void);

	virtual bool OnExecute(IDBConnection * pConnection);
	virtual void OnFinish();

	void SetResult(bool IsSucceed)
	{
		m_IsSucceed=IsSucceed;
	}
};

}