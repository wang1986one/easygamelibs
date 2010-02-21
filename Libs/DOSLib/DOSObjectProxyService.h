#pragma once


class CDOSObjectProxyService :
	public CNetService,public CEasyThread
{
protected:
	CIDStorage<CDOSProxyConnection>			m_ConnectionPool;
	CThreadSafeList<CDOSMessagePacket *>	m_MsgQueue;
	CStaticMap<WORD,OBJECT_ID>				m_MessageMap;
	

	DECLARE_CLASS_INFO(CDOSObjectProxyService);
public:
	CDOSObjectProxyService(void);
	virtual ~CDOSObjectProxyService(void);

	BOOL Init(CDOSServer * pServer);

	virtual BOOL OnStart();
	virtual void OnClose();	
	virtual BOOL OnRun();

	virtual CBaseTCPConnection * CreateConnection(CIPAddress& RemoteAddress);
	virtual BOOL DeleteConnection(CBaseTCPConnection * pConnection);
	

	CDOSProxyConnection * GetConnection(UINT ID);

	BOOL PushMessage(CDOSMessagePacket * pPacket);

	OBJECT_ID GetGlobalMsgMapObjectID(WORD CmdID);
protected:
	int DoMessageProcess(int ProcessPacketLimit=DEFAULT_SERVER_PROCESS_PACKET_LIMIT);

	void OnMsg(CDOSMessage * pMessage);

	BOOL RegisterGlobalMsgMap(WORD CmdID,OBJECT_ID ObjectID);
	BOOL UnregisterGlobalMsgMap(WORD CmdID,OBJECT_ID ObjectID);
};

inline CDOSProxyConnection * CDOSObjectProxyService::GetConnection(UINT ID)
{
	return m_ConnectionPool.GetObject(ID);
}
