/****************************************************************************/
/*                                                                          */
/*      文件名:    DOSBaseObject.cpp                                        */
/*      创建日期:  2009年10月23日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#include "StdAfx.h"

IMPLEMENT_CLASS_INFO(CDOSBaseObject,CNameObject);



CDOSBaseObject::CDOSBaseObject(void)
{
	FUNCTION_BEGIN;
	m_pRouter=NULL;
	m_pManager=NULL;
	m_pGroup=NULL;
	m_MsgProcessLimit=DEFAULT_SERVER_PROCESS_PACKET_LIMIT;
	m_ConcernedObjectTestTime=20*1000;
	m_ConcernedObjectKeepAliveCount=5;
	FUNCTION_END;
}

CDOSBaseObject::~CDOSBaseObject(void)
{
	FUNCTION_BEGIN;
	Destory();
	FUNCTION_END;
}

bool CDOSBaseObject::Init(DOS_OBJECT_REGISTER_INFO& ObjectRegisterInfo)
{
	FUNCTION_BEGIN;
	UINT MsgQueueSize=ObjectRegisterInfo.MsgQueueSize;
	if(MsgQueueSize==0)
		MsgQueueSize=GetManager()->GetServer()->GetConfig().MaxObjectMsgQueue;
	if(ObjectRegisterInfo.MsgProcessLimit)
		m_MsgProcessLimit=ObjectRegisterInfo.MsgProcessLimit;
	
	if(!m_MsgQueue.Create(MsgQueueSize))
	{
		PrintDOSLog(0,"对象[%llX]创建%u大小的消息队列失败",
			GetObjectID().ID,MsgQueueSize);
		return false;
	}
	m_ConcernedObjectTestTime=GetManager()->GetServer()->GetConfig().ObjectAliveTestTime;
	m_ConcernedObjectKeepAliveCount=GetManager()->GetServer()->GetConfig().ObjectKeepAliveCount;
	m_ConcernedObjectTestTimer.SaveTime();
	return Initialize();
	FUNCTION_END;
	return false;
}

bool CDOSBaseObject::Initialize()
{
	FUNCTION_BEGIN;
	return TRUE;	
	FUNCTION_END;
	return false;
}

void CDOSBaseObject::Destory()
{
	FUNCTION_BEGIN;
	m_MsgQueue.Destory();
	m_pRouter=NULL;
	m_pManager=NULL;
	m_pGroup=NULL;
	FUNCTION_END;
}


UINT CDOSBaseObject::GetRouterID()
{
	FUNCTION_BEGIN;
	return m_pRouter->GetRouterID();
	FUNCTION_END;
	return 0;
}


int CDOSBaseObject::DoCycle(int ProcessPacketLimit)
{
	FUNCTION_BEGIN;
	CDOSMessagePacket * pPacket;
	int ProcessCount=0;
	int Limit=m_MsgProcessLimit;
	while(m_MsgQueue.PopFront(pPacket))
	{		
		if(pPacket->GetMessage().GetMsgFlag()&DOS_MESSAGE_FLAG_SYSTEM_MESSAGE)
		{
			OnSystemMessage(&(pPacket->GetMessage()));
		}
		else
		{
			OnMessage(&(pPacket->GetMessage()));
		}
		if(!ReleaseMessagePacket(pPacket))
		{
			PrintDOSLog(0xff0000,"释放消息内存块失败！");
		}		
		Limit--;
		ProcessCount++;
		if(Limit<=0)
			break;
	}
	ProcessCount+=DoConcernedObjectTest();
	ProcessCount+=Update(ProcessPacketLimit);
	return ProcessCount;
	FUNCTION_END;
	return 0;
}

BOOL CDOSBaseObject::PushMessage(CDOSMessagePacket * pPacket)
{
	FUNCTION_BEGIN;
	((CDOSServer *)m_pRouter->GetServer())->AddRefMessagePacket(pPacket);
	if(m_MsgQueue.PushBack(pPacket))
	{			
		return TRUE;
	}
	else
	{	
		ReleaseMessagePacket(pPacket);		
		PrintDOSLog(0xff0000,"CDOSBaseObject::PushMessage:消息压栈失败！");
	}
	FUNCTION_END;
	return FALSE;
}

BOOL CDOSBaseObject::SendMessage(OBJECT_ID ReceiverID,MSG_ID_TYPE MsgID,WORD MsgFlag,LPCVOID pData,UINT DataSize)
{
	FUNCTION_BEGIN;
	if(m_pRouter)
		return m_pRouter->RouterMessage(GetObjectID(),ReceiverID,MsgID,MsgFlag,pData,DataSize);
	FUNCTION_END;
	return FALSE;
}

BOOL CDOSBaseObject::SendMessageMulti(OBJECT_ID * pReceiverIDList,UINT ReceiverCount,bool IsSorted,MSG_ID_TYPE MsgID,WORD MsgFlag,LPCVOID pData,UINT DataSize)
{	
	FUNCTION_BEGIN;
	if(pReceiverIDList==NULL||ReceiverCount==0)
	{
		return FALSE;
	}
	int PacketSize=CDOSMessagePacket::CaculatePacketLength(DataSize,ReceiverCount);

	CDOSMessagePacket * pNewPacket=m_pRouter->GetServer()->NewMessagePacket(PacketSize);
	if(pNewPacket==NULL)
	{
		PrintDOSLog(0xff0000,"分配消息内存块失败！");
		return FALSE;
	}	
	pNewPacket->GetMessage().SetMsgID(MsgID);
	pNewPacket->GetMessage().SetSenderID(GetObjectID());
	pNewPacket->GetMessage().SetDataLength(DataSize);
	pNewPacket->GetMessage().SetMsgFlag(MsgFlag);
	if(pData)
		memcpy(pNewPacket->GetMessage().GetDataBuffer(),pData,DataSize);
	pNewPacket->SetTargetIDs(ReceiverCount,pReceiverIDList);
	if(!IsSorted)
	{
		qsort(pNewPacket->GetTargetIDs(),ReceiverCount,sizeof(OBJECT_ID),OBJECT_ID::Compare);
	}
	
	pNewPacket->MakePacketLength();

	BOOL Ret=m_pRouter->RouterMessage(pNewPacket);
	ReleaseMessagePacket(pNewPacket);
	return Ret;
	FUNCTION_END;
	return FALSE;
}

CDOSMessagePacket * CDOSBaseObject::NewMessagePacket(UINT DataSize,UINT ReceiverCount)
{
	FUNCTION_BEGIN;
	MSG_LEN_TYPE PacketSize=CDOSMessagePacket::CaculatePacketLength(DataSize,ReceiverCount);
	return m_pRouter->GetServer()->NewMessagePacket(PacketSize);		
	FUNCTION_END;
	return NULL;
}

BOOL CDOSBaseObject::ReleaseMessagePacket(CDOSMessagePacket * pPacket)
{
	FUNCTION_BEGIN;
	return m_pRouter->GetServer()->ReleaseMessagePacket(pPacket);
	FUNCTION_END;
	return FALSE;
}

BOOL CDOSBaseObject::SendMessagePacket(CDOSMessagePacket * pPacket)
{
	FUNCTION_BEGIN;
	return m_pRouter->RouterMessage(pPacket);
	FUNCTION_END;
	return FALSE;
}

BOOL CDOSBaseObject::RegisterMsgMap(OBJECT_ID ProxyObjectID,MSG_ID_TYPE * pMsgIDList,int CmdCount)
{
	FUNCTION_BEGIN;
	return SendMessage(ProxyObjectID,DSM_PROXY_REGISTER_MSG_MAP,DOS_MESSAGE_FLAG_SYSTEM_MESSAGE,pMsgIDList,sizeof(MSG_ID_TYPE)*CmdCount);
	FUNCTION_END;
	return FALSE;
}

BOOL CDOSBaseObject::UnregisterMsgMap(OBJECT_ID ProxyObjectID,MSG_ID_TYPE * pMsgIDList,int CmdCount)
{
	FUNCTION_BEGIN;
	return SendMessage(ProxyObjectID,DSM_PROXY_UNREGISTER_MSG_MAP,DOS_MESSAGE_FLAG_SYSTEM_MESSAGE,pMsgIDList,sizeof(MSG_ID_TYPE)*CmdCount);
	FUNCTION_END;
	return FALSE;
}


BOOL CDOSBaseObject::RegisterGlobalMsgMap(ROUTE_ID_TYPE ProxyRouterID,MSG_ID_TYPE * pMsgIDList,int CmdCount)
{
	FUNCTION_BEGIN;
	OBJECT_ID ProxyID;
	ProxyID.RouterID=ProxyRouterID;
	ProxyID.ObjectTypeID=DOT_PROXY_OBJECT;
	ProxyID.GroupIndex=0;
	ProxyID.ObjectIndex=0;
	return SendMessage(ProxyID,DSM_PROXY_REGISTER_GLOBAL_MSG_MAP,DOS_MESSAGE_FLAG_SYSTEM_MESSAGE,pMsgIDList,sizeof(MSG_ID_TYPE)*CmdCount);
	FUNCTION_END;
	return FALSE;
}


BOOL CDOSBaseObject::UnregisterGlobalMsgMap(ROUTE_ID_TYPE ProxyRouterID,MSG_ID_TYPE * pMsgIDList,int CmdCount)
{
	FUNCTION_BEGIN;
	OBJECT_ID ProxyID;
	ProxyID.RouterID=ProxyRouterID;
	ProxyID.ObjectTypeID=DOT_PROXY_OBJECT;
	ProxyID.GroupIndex=0;
	ProxyID.ObjectIndex=0;
	return SendMessage(ProxyID,DSM_PROXY_UNREGISTER_GLOBAL_MSG_MAP,DOS_MESSAGE_FLAG_SYSTEM_MESSAGE,pMsgIDList,sizeof(MSG_ID_TYPE)*CmdCount);
	FUNCTION_END;
	return FALSE;
}

BOOL CDOSBaseObject::AddConcernedObject(OBJECT_ID ObjectID,bool NeedTest)
{
	FUNCTION_BEGIN;
	for(UINT i=0;i<m_ConcernedObject.GetCount();i++)
	{
		if(m_ConcernedObject[i].ObjectID==ObjectID)
		{
			return FALSE;
		}
	}
	CONCERNED_OBJECT_INFO Info;
	Info.ObjectID=ObjectID;
	Info.AliveFailedCount=0;
	Info.NeedTest=NeedTest;
	m_ConcernedObject.Add(Info);
	FUNCTION_END;
	return FALSE;
}

BOOL CDOSBaseObject::DeleteConcernedObject(OBJECT_ID ObjectID)
{
	FUNCTION_BEGIN;
	for(UINT i=0;i<m_ConcernedObject.GetCount();i++)
	{
		if(m_ConcernedObject[i].ObjectID==ObjectID)
		{
			m_ConcernedObject.Delete(i);
			return TRUE;
		}
	}
	FUNCTION_END;
	return FALSE;
}

BOOL CDOSBaseObject::FindObject(UINT ObjectType)
{
	FUNCTION_BEGIN;
	OBJECT_ID ObjectID;
	ObjectID.RouterID=BROAD_CAST_ROUTER_ID;
	ObjectID.ObjectTypeID=ObjectType;
	ObjectID.GroupIndex=BROAD_CAST_GROUP_INDEX;
	ObjectID.ObjectIndex=BROAD_CAST_OBJECT_INDEX;
	return SendMessage(ObjectID,DSM_OBJECT_FIND,DOS_MESSAGE_FLAG_SYSTEM_MESSAGE);
	FUNCTION_END;
	return FALSE;
}
BOOL CDOSBaseObject::ReportObject(OBJECT_ID TargetID,const CSmartStruct& ObjectInfo)
{
	FUNCTION_BEGIN;
	return SendMessage(TargetID,DSM_OBJECT_REPORT,DOS_MESSAGE_FLAG_SYSTEM_MESSAGE,ObjectInfo.GetData(),ObjectInfo.GetDataLen());
	FUNCTION_END;
	return FALSE;
}

BOOL CDOSBaseObject::OnMessage(CDOSMessage * pMessage)
{
	FUNCTION_BEGIN;
	FUNCTION_END;
	return FALSE;
}

BOOL CDOSBaseObject::OnSystemMessage(CDOSMessage * pMessage)
{
	FUNCTION_BEGIN;
	switch(pMessage->GetMsgID())
	{
	case DSM_PROXY_REGISTER_MSG_MAP_RESULT:
	case DSM_PROXY_UNREGISTER_MSG_MAP_RESULT:
	case DSM_PROXY_REGISTER_GLOBAL_MSG_MAP_RESULT:
	case DSM_PROXY_UNREGISTER_GLOBAL_MSG_MAP_RESULT:
		return FALSE;
	case DSM_PROXY_DISCONNECT:
		OnProxyObjectDisconnect(pMessage->GetSenderID());
		return TRUE;	
	case DSM_ROUTE_LINK_LOST:
		OnRouteLinkLost(pMessage->GetSenderID().RouterID);
		return TRUE;
	case DSM_OBJECT_ALIVE_TEST:
		if(pMessage->GetDataLength()>=sizeof(BYTE))
		{
			OnAliveTest(pMessage->GetSenderID(),*((BYTE *)pMessage->GetDataBuffer()));
		}
		return TRUE;
	case DSM_OBJECT_FIND:
		OnFindObject(pMessage->GetSenderID());
		return TRUE;
	case DSM_OBJECT_REPORT:
		OnObjectReport(pMessage->GetSenderID(),pMessage->GetDataPacket());
		return TRUE;
	}
	FUNCTION_END;
	return FALSE;
}

void CDOSBaseObject::OnConcernedObjectLost(OBJECT_ID ObjectID)
{
	FUNCTION_BEGIN;
	FUNCTION_END;
}

void CDOSBaseObject::OnFindObject(OBJECT_ID CallerID)
{
	FUNCTION_BEGIN;
	CSmartStruct ObjectInfo;
	ReportObject(CallerID,ObjectInfo);
	FUNCTION_END;
}

void CDOSBaseObject::OnObjectReport(OBJECT_ID ObjectID,const CSmartStruct& ObjectInfo)
{
	FUNCTION_BEGIN;
	FUNCTION_END;
}

int CDOSBaseObject::Update(int ProcessPacketLimit)
{
	FUNCTION_BEGIN;
	FUNCTION_END;
	return 0;
}

void CDOSBaseObject::OnProxyObjectDisconnect(OBJECT_ID ProxyObjectID)
{
	FUNCTION_BEGIN;
	for(UINT i=0;i<m_ConcernedObject.GetCount();i++)
	{
		if(m_ConcernedObject[i].ObjectID==ProxyObjectID)
		{
			PrintDOSDebugLog(0,"被关注的代理对象[0x%llX]已断线",
				ProxyObjectID.ID);
			OnConcernedObjectLost(m_ConcernedObject[i].ObjectID);
		}
	}
	FUNCTION_END;
}
void CDOSBaseObject::OnAliveTest(OBJECT_ID SenderID,BYTE IsEcho)
{
	FUNCTION_BEGIN;
	if(IsEcho)
	{
		for(UINT i=0;i<m_ConcernedObject.GetCount();i++)
		{
			if(m_ConcernedObject[i].ObjectID==SenderID)
			{
				m_ConcernedObject[i].AliveFailedCount=0;
			}
		}
	}
	else
	{
		IsEcho=1;
		SendMessage(SenderID,DSM_OBJECT_ALIVE_TEST,DOS_MESSAGE_FLAG_SYSTEM_MESSAGE,&IsEcho,sizeof(IsEcho));
	}
	FUNCTION_END;
}
void CDOSBaseObject::OnRouteLinkLost(UINT RouteID)
{
	FUNCTION_BEGIN;
	for(UINT i=0;i<m_ConcernedObject.GetCount();i++)
	{
		if(m_ConcernedObject[i].ObjectID.RouterID==RouteID)
		{
			PrintDOSLog(0,"被关注的对象[0x%llX]对应的路由连接已断开",
				m_ConcernedObject[i].ObjectID.ID);
			OnConcernedObjectLost(m_ConcernedObject[i].ObjectID);
		}
	}
	FUNCTION_END;
}



int CDOSBaseObject::DoConcernedObjectTest()
{
	FUNCTION_BEGIN;
	int ProcessCount=0;
	if(m_ConcernedObjectTestTimer.IsTimeOut(m_ConcernedObjectTestTime))
	{
		m_ConcernedObjectTestTimer.SaveTime();
		for(int i=m_ConcernedObject.GetCount()-1;i>=0;i--)
		{
			if(m_ConcernedObject[i].NeedTest)
			{
				if(m_ConcernedObject[i].AliveFailedCount>m_ConcernedObjectKeepAliveCount)
				{
					PrintDOSLog(0,"被关注的对象[0x%llX]活动测试失败",
						m_ConcernedObject[i].ObjectID.ID);
					OnConcernedObjectLost(m_ConcernedObject[i].ObjectID);
					m_ConcernedObject.Delete(i);
				}
				else
				{
					m_ConcernedObject[i].AliveFailedCount++;
					BYTE IsEcho=0;
					SendMessage(m_ConcernedObject[i].ObjectID,DSM_OBJECT_ALIVE_TEST,DOS_MESSAGE_FLAG_SYSTEM_MESSAGE,&IsEcho,sizeof(IsEcho));
				}
				ProcessCount++;
			}
		}
	}
	return ProcessCount;
	FUNCTION_END;
	return 0;
}

