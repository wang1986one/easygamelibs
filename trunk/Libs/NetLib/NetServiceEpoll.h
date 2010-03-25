/****************************************************************************/
/*                                                                          */
/*      �ļ���:    NetServiceEpoll.h                                        */
/*      ��������:  2009��09��11��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once


class CNetServer;

class CNetService :	
	public CBaseService,public IEpollEventHandler
{
protected:
	volatile BOOL							m_WantClose;
	CNetServer *							m_pServer;	
	int										m_CurProtocol;	
	CThreadSafeList<CEpollEventObject *>	m_RecvQueue;
	CThreadSafeList<CEpollEventObject *>	m_SendQueue;
	int										m_AcceptQueueSize;
	int										m_RecvQueueSize;
	int										m_SendQueueSize;
	int										m_ParallelAcceptCount;
	int										m_ParallelRecvCount;
	CEpollEventRouter *						m_pEpollEventRouter;

	

	DECLARE_CLASS_INFO_STATIC(CNetService);
public:
	CNetService();
	virtual ~CNetService(void);

	

	void SetServer(CNetServer * pServer);

	CNetServer * GetServer();

	virtual BOOL OnEpollEvent(UINT EventID);
	

	virtual BOOL Create(int Protocol = IPPROTO_TCP,
		int AcceptQueueSize=DEFAULT_SERVER_ACCEPT_QUEUE,
		int RecvQueueSize=DEFAULT_SERVER_RECV_DATA_QUEUE,
		int SendQueueSize=DEFAULT_SERVER_SEND_DATA_QUEUE,
		int ParallelAcceptCount=DEFAULT_PARALLEL_ACCEPT,
		int ParallelRecvCount=DEFAULT_PARALLEL_RECV,
		bool IsUseListenThread=false);
	virtual void Destory();
	
	BOOL StartListen(const CIPAddress& Address);
	void Close();
	void QueryClose();

	virtual void OnStartUp();
	virtual void OnClose();

	virtual int Update(int ProcessPacketLimit=DEFAULT_SERVER_PROCESS_PACKET_LIMIT);

	virtual CBaseTCPConnection * CreateConnection(CIPAddress& RemoteAddress);
	virtual BOOL DeleteConnection(CBaseTCPConnection * pConnection);
	

	BOOL QueryUDPSend(const CIPAddress& IPAddress,LPCVOID pData,int Size);

	virtual void OnRecvData(const CIPAddress& IPAddress,const CEasyBuffer& DataBuffer);

	

	
	
protected:	
	
	BOOL AcceptSocket(SOCKET Socket);

	void DoAcceptSocket();
	void DoUDPRecv();
	void DoUDPSend();
};



inline void CNetService::SetServer(CNetServer * pServer)
{
	m_pServer=pServer;
}

inline CNetServer * CNetService::GetServer()
{
	return m_pServer;
}


