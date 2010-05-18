/****************************************************************************/
/*                                                                          */
/*      文件名:    DOSProxyConnection.h                                     */
/*      创建日期:  2009年09月11日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once

class CDOSObjectProxyService;

class CDOSProxyConnection :
	public CNetConnection
{
protected:
	OBJECT_ID									m_ObjectID;
	CDOSObjectProxyService						*m_pService;

	CEasyBuffer									m_AssembleBuffer;		
	CThreadSafeIDStorage<CDOSMessagePacket *>	m_MsgQueue;
	CStaticMap<WORD,OBJECT_ID>					m_MessageMap;

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