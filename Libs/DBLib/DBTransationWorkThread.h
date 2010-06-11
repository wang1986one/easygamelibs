/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DBTransationWorkThread.h                                 */
/*      ��������:  2009��07��06��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      ��������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ������������κ���ҵ�ͷ���ҵ������������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once

namespace DBLib
{

#define DEFAULT_TRANS_QUEUE		128
#define CONNECTION_TEST_TIME	60000

class CDBTransationManager;

class CDBTransationWorkThread :
	public CEasyThread
{
protected:
	CDBTransationManager *					m_pManager;
	IDBConnection *							m_pConnection;
	CEasyString								m_ConnectString;
	CThreadSafeIDStorage<CDBTransaction *>	m_TransQueue;
	CThreadSafeIDStorage<CDBTransaction *>	m_FinishTransQueue;
	CEasyTimer								m_ConnectionTestTimer;

	DECLARE_CLASS_INFO_STATIC(CDBTransationWorkThread)
public:
	CDBTransationWorkThread(CDBTransationManager * pManager);
	~CDBTransationWorkThread(void);

	void Destory();

	bool Init(IDBConnection * pConnection,LPCTSTR ConnectStr,int QueueSize=DEFAULT_TRANS_QUEUE);
	bool AddTransaction(CDBTransaction * pDBTansaction);
	CDBTransaction *PopFinishTransaction();

	UINT GetQueueLen()
	{
		return m_TransQueue.GetObjectCount();
	}
	UINT GetFinishQueueLen()
	{
		return m_FinishTransQueue.GetObjectCount();
	}

protected:
	virtual BOOL OnStart();	
	virtual BOOL OnRun();
	virtual void OnTerminate();
};

}