/****************************************************************************/
/*                                                                          */
/*      文件名:    NetConnectionEpoll.h                                     */
/*      创建日期:  2009年07月06日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once
#define NO_CONNECTION_TIME_OUT		0


class CNetService;

class CNetConnection :	
	public CBaseTCPConnection,public IEpollEventHandler
{
protected:	
	CNetServer*									m_pServer;	
	volatile BOOL								m_WantClose;
	bool										m_UseSafeDisconnect;
	CThreadSafeIDStorage<CEpollEventObject *>	m_RecvQueue;
	CThreadSafeIDStorage<CEpollEventObject *>	m_SendQueue;
	CEpollEventRouter *							m_pEpollEventRouter;

	CEasyCriticalSection						m_RecvLock;
	CEasyCriticalSection						m_SendLock;

	DECLARE_CLASS_INFO_STATIC(CNetConnection);
public:
	CNetConnection(void);
	virtual ~CNetConnection(void);

	virtual BOOL OnEpollEvent(UINT EventID);

	virtual BOOL Create(UINT RecvQueueSize=DEFAULT_SERVER_RECV_DATA_QUEUE,
		UINT SendQueueSize=DEFAULT_SERVER_SEND_DATA_QUEUE);
	virtual BOOL Create(SOCKET Socket,UINT RecvQueueSize,UINT SendQueueSize);
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

	void EnableSafeDisconnect(bool Enable);

	virtual bool StealFrom(CNameObject * pObject,UINT Param=0);

	
protected:
	void DoRecv();
	void DoSend();
};



inline void CNetConnection::SetServer(CNetServer* pServer)
{
	m_pServer=pServer;
}

inline CNetServer* CNetConnection::GetServer()
{
	return m_pServer;
}


inline void CNetConnection::EnableSafeDisconnect(bool Enable)
{
	m_UseSafeDisconnect=Enable;
}


