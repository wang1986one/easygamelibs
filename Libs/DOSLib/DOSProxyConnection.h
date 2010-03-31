/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DOSProxyConnection.h                                     */
/*      ��������:  2009��09��11��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      ��������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ������������κ���ҵ�ͷ���ҵ������������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once

class CDOSObjectProxyService;

class CDOSProxyConnection :
	public CNetConnection
{
protected:
	OBJECT_ID								m_ObjectID;
	CDOSObjectProxyService					*m_pService;

	CEasyBuffer								m_AssembleBuffer;		
	CThreadSafeList<CDOSMessagePacket *>	m_MsgQueue;
	CStaticMap<WORD,OBJECT_ID>				m_MessageMap;

	DECLARE_CLASS_INFO(CDOSProxyConnection);
public:
	CDOSProxyConnection(void);
	virtual ~CDOSProxyConnection(void);

	BOOL Init(CDOSObjectProxyService * pService,UINT ID);

	OBJECT_ID GetObjectID();

	virtual void Destory();

	virtual void OnConnection(BOOL IsSucceed);
	virtual void OnDisconnection();

	virtual void OnRecvData(const CEasyBuffer& DataBuffer);
	void OnMsg(CDOSSimpleMessage * pMessage);

	BOOL PushMessage(CDOSMessagePacket * pPacket);

	virtual int Update(int ProcessPacketLimit=DEFAULT_SERVER_PROCESS_PACKET_LIMIT);
protected:
	CDOSServer * GetServer();	
	BOOL SendInSideMsg(WORD CmdID,LPVOID pData=NULL,UINT DataSize=0);	
	BOOL SendOutSideMsg(CDOSMessagePacket * pPacket);
	BOOL SendDisconnectNotify();

	OBJECT_ID GetMsgMapObjectID(WORD CmdID);

	BOOL RegisterMsgMap(WORD CmdID,OBJECT_ID ObjectID);
	BOOL UnregisterMsgMap(WORD CmdID,OBJECT_ID ObjectID);

	UINT SortTargetObjectID(OBJECT_ID * pObjectIDs,UINT Count);
	int FindMinObjectID(OBJECT_ID * pObjectIDs,UINT Count);
};

inline OBJECT_ID CDOSProxyConnection::GetObjectID()
{
	return m_ObjectID;
}