/****************************************************************************/
/*                                                                          */
/*      �ļ���:    NetConnectionIOCP.h                                      */
/*      ��������:  2010��02��09��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      ��������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ������������κ���ҵ�ͷ���ҵ������������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once
#define NO_CONNECTION_TIME_OUT		0


class CNetService;

class CNetConnection :	
	public CBaseTCPConnection,public IIOCPEventHandler
{
protected:	
	CNetServer*									m_pServer;	
	volatile BOOL								m_WantClose;
	int											m_CurProtocol;		
	
	CThreadSafeIDStorage<COverLappedObject *>	m_DataQueue;
	CIOCPEventRouter *							m_pIOCPEventRouter;

	volatile UINT								m_SendQueryCount;	
	

	DECLARE_CLASS_INFO_STATIC(CNetConnection);
public:
	CNetConnection(void);
	virtual ~CNetConnection(void);

	virtual BOOL OnIOCPEvent(int EventID,COverLappedObject * pOverLappedObject);

	virtual BOOL Create(UINT RecvQueueSize=DEFAULT_SERVER_RECV_DATA_QUEUE,
		UINT SendQueueSize=DEFAULT_SERVER_SEND_DATA_QUEUE);
	virtual BOOL Create(SOCKET Socket,
		UINT RecvQueueSize=DEFAULT_SERVER_RECV_DATA_QUEUE,
		UINT SendQueueSize=DEFAULT_SERVER_SEND_DATA_QUEUE);
	virtual void Destory();

	

	BOOL Connect(const CIPAddress& Address,DWORD TimeOut=NO_CONNECTION_TIME_OUT);
	void Disconnect();
	void QueryDisconnect();
	

	BOOL StartWork();

	virtual void OnConnection(BOOL IsSucceed);
	virtual void OnDisconnection();

	BOOL QuerySend(LPCVOID pData,int Size);
	

	virtual void OnRecvData(const CEasyBuffer& DataBuffer);

	virtual int Update(int ProcessPacketLimit=DEFAULT_SERVER_PROCESS_PACKET_LIMIT);
	

	void SetServer(CNetServer* pServer);

	CNetServer* GetServer();		

		

	virtual bool StealFrom(CNameObject * pObject,UINT Param=0);

	void SetDataQueueSize(UINT Size);
	UINT GetDataQueueSize();

	virtual UINT GetCurSendQueryCount();
protected:
	BOOL QueryRecv();

};



inline void CNetConnection::SetServer(CNetServer* pServer)
{
	m_pServer=pServer;
}

inline CNetServer* CNetConnection::GetServer()
{
	return m_pServer;
}
