/****************************************************************************/
/*                                                                          */
/*      文件名:    DOSBaseObject.h                                          */
/*      创建日期:  2009年10月23日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
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
	OBJECT_ID									m_ObjectID;

	CDOSRouter *								m_pRouter;
	CDOSObjectManager *							m_pManager;
	CDOSObjectGroup *							m_pGroup;

	CDOSMsgManager								m_MsgManager;
	CThreadSafeIDStorage<CDOSMessagePacket *>	m_MsgQueue;

	DECLARE_CLASS_INFO(CDOSBaseObject);
public:
	CDOSBaseObject(void);
	virtual ~CDOSBaseObject(void);

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

	BOOL SendMessage(OBJECT_ID ReceiverID,WORD MsgID,LPCVOID pData=0,UINT DataSize=0);
	BOOL SendMessageMulti(OBJECT_ID * pReceiverIDList,UINT ReceiverCount,bool IsSorted,WORD MsgID,LPCVOID pData=0,UINT DataSize=0);

	CDOSMessagePacket * NewMessagePacket(UINT DataSize,UINT ReceiverCount);
	BOOL ReleaseMessagePacket(CDOSMessagePacket * pPacket);
	BOOL SendMessagePacket(CDOSMessagePacket * pPacket);

	BOOL InitMsgProc(int MaxMsgCount);
	void RegisterMsgProc(WORD CmdID,DOS_MSG_PROC MsgProc);
	void UnregisterMsgProc(WORD CmdID,DOS_MSG_PROC MsgProc);

	BOOL RegisterMsgMap(OBJECT_ID ProxyObjectID,WORD * pCmdIDList,int CmdCount);
	BOOL UnregisterMsgMap(OBJECT_ID ProxyObjectID,WORD * pCmdIDList,int CmdCount);
	BOOL RegisterGlobalMsgMap(WORD ProxyRouterID,WORD * pCmdIDList,int CmdCount);
	BOOL UnregisterGlobalMsgMap(WORD ProxyRouterID,WORD * pCmdIDList,int CmdCount);
	
protected:
	virtual BOOL OnPreTranslateMessage(CDOSMessage * pMessage);
	virtual int Update(int ProcessPacketLimit=DEFAULT_SERVER_PROCESS_PACKET_LIMIT);
};

struct DOS_OBJECT_INFO
{
	OBJECT_ID			ObjectID;
	int					Weight;
	CDOSBaseObject *	pObject;
	UINT				Param;
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

