/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DOSObjectProxyService.h                                  */
/*      ��������:  2009��07��06��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once


class CDOSObjectProxyService :
	public CNetService,public CEasyThread
{
protected:
	CIDStorage<CDOSProxyConnection>				m_ConnectionPool;
	CThreadSafeIDStorage<CDOSMessagePacket *>	m_MsgQueue;
	CStaticMap<MSG_ID_TYPE,OBJECT_ID>			m_MessageMap;
	

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

	OBJECT_ID GetGlobalMsgMapObjectID(MSG_ID_TYPE MsgID);
protected:
	int DoMessageProcess(int ProcessPacketLimit=DEFAULT_SERVER_PROCESS_PACKET_LIMIT);

	void OnMsg(CDOSMessage * pMessage);

	BOOL RegisterGlobalMsgMap(MSG_ID_TYPE MsgID,OBJECT_ID ObjectID);
	BOOL UnregisterGlobalMsgMap(MSG_ID_TYPE MsgID,OBJECT_ID ObjectID);

	void ClearMsgMapByRouterID(UINT RouterID);
};

inline CDOSProxyConnection * CDOSObjectProxyService::GetConnection(UINT ID)
{
	return m_ConnectionPool.GetObject(ID);
}
