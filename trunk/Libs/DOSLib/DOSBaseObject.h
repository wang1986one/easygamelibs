/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DOSBaseObject.h                                          */
/*      ��������:  2009��10��23��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once

class CDOSRouter;
class CDOSObjectManager;
class CDOSObjectGroup;



class CDOSBaseObject :
	public CNameObject
{
protected:
	struct CONCERNED_OBJECT_INFO
	{
		OBJECT_ID		ObjectID;
		UINT			AliveFailedCount;
		bool			NeedTest;
	};

	OBJECT_ID									m_ObjectID;

	CDOSRouter *								m_pRouter;
	CDOSObjectManager *							m_pManager;
	CDOSObjectGroup *							m_pGroup;
	UINT										m_MsgProcessLimit;
	CEasyArray<CONCERNED_OBJECT_INFO>			m_ConcernedObject;
	CEasyTimer									m_ConcernedObjectTestTimer;
	UINT										m_ConcernedObjectTestTime;
	UINT										m_ConcernedObjectKeepAliveCount;

	CThreadSafeIDStorage<CDOSMessagePacket *>	m_MsgQueue;

	DECLARE_CLASS_INFO(CDOSBaseObject);
public:
	CDOSBaseObject(void);
	virtual ~CDOSBaseObject(void);

	bool Init(DOS_OBJECT_REGISTER_INFO& ObjectRegisterInfo);

	virtual bool Initialize();
	virtual void Destory();

	void SetRouter(CDOSRouter * pRouter);
	void SetManager(CDOSObjectManager * pManager);
	void SetGroup(CDOSObjectGroup * pGroup);

	CDOSRouter * GetRouter();
	CDOSObjectManager * GetManager();
	CDOSObjectGroup * GetGroup();
	UINT GetRouterID();

	void SetObjectID(OBJECT_ID ID);
	OBJECT_ID GetObjectID();

	int DoCycle(int ProcessPacketLimit=DEFAULT_SERVER_PROCESS_PACKET_LIMIT);
	BOOL PushMessage(CDOSMessagePacket * pPacket);

	BOOL SendMessage(OBJECT_ID ReceiverID,MSG_ID_TYPE MsgID,WORD MsgFlag=0,LPCVOID pData=0,UINT DataSize=0);
	BOOL SendMessageMulti(OBJECT_ID * pReceiverIDList,UINT ReceiverCount,bool IsSorted,MSG_ID_TYPE MsgID,WORD MsgFlag=0,LPCVOID pData=0,UINT DataSize=0);

	CDOSMessagePacket * NewMessagePacket(UINT DataSize,UINT ReceiverCount);
	BOOL ReleaseMessagePacket(CDOSMessagePacket * pPacket);
	BOOL SendMessagePacket(CDOSMessagePacket * pPacket);

	BOOL RegisterMsgMap(OBJECT_ID ProxyObjectID,MSG_ID_TYPE * pMsgIDList,int CmdCount);
	BOOL UnregisterMsgMap(OBJECT_ID ProxyObjectID,MSG_ID_TYPE * pMsgIDList,int CmdCount);
	BOOL RegisterGlobalMsgMap(ROUTE_ID_TYPE ProxyRouterID,MSG_ID_TYPE * pMsgIDList,int CmdCount);
	BOOL UnregisterGlobalMsgMap(ROUTE_ID_TYPE ProxyRouterID,MSG_ID_TYPE * pMsgIDList,int CmdCount);

	BOOL AddConcernedObject(OBJECT_ID ObjectID,bool NeedTest);
	BOOL DeleteConcernedObject(OBJECT_ID ObjectID);

	BOOL FindObject(UINT ObjectType);
	BOOL ReportObject(OBJECT_ID TargetID,const CSmartStruct& ObjectInfo);

protected:
	virtual BOOL OnMessage(CDOSMessage * pMessage);
	virtual BOOL OnSystemMessage(CDOSMessage * pMessage);
	virtual void OnConcernedObjectLost(OBJECT_ID ObjectID);
	virtual void OnFindObject(OBJECT_ID CallerID);
	virtual void OnObjectReport(OBJECT_ID ObjectID,const CSmartStruct& ObjectInfo);
	virtual int Update(int ProcessPacketLimit=DEFAULT_SERVER_PROCESS_PACKET_LIMIT);

	virtual void OnProxyObjectDisconnect(OBJECT_ID ProxyObjectID);
	virtual void OnAliveTest(OBJECT_ID SenderID,BYTE IsEcho);
	virtual void OnRouteLinkLost(UINT RouteID);
	

	int DoConcernedObjectTest();
	
};



inline void CDOSBaseObject::SetRouter(CDOSRouter * pRouter)
{
	m_pRouter=pRouter;
}
inline void CDOSBaseObject::SetManager(CDOSObjectManager * pManager)
{
	m_pManager=pManager;
}
inline void CDOSBaseObject::SetGroup(CDOSObjectGroup * pGroup)
{
	m_pGroup=pGroup;
}

inline CDOSRouter * CDOSBaseObject::GetRouter()
{
	return m_pRouter;
}
inline CDOSObjectManager * CDOSBaseObject::GetManager()
{
	return m_pManager;
}
inline CDOSObjectGroup * CDOSBaseObject::GetGroup()
{
	return m_pGroup;
}


inline void CDOSBaseObject::SetObjectID(OBJECT_ID ID)
{
	m_ObjectID=ID;
}
inline OBJECT_ID CDOSBaseObject::GetObjectID()
{
	return m_ObjectID;
}

