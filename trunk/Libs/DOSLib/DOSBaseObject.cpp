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
		OnMessage(&(pPacket->GetMessage()));
		if(!ReleaseMessagePacket(pPacket))
		{
			PrintDOSLog(0xff0000,"释放消息内存块失败！");
		}
		Limit--;
		ProcessCount++;
		if(Limit<=0)
			break;
	}
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

BOOL CDOSBaseObject::OnMessage(CDOSMessage * pMessage)
{
	FUNCTION_BEGIN;
	FUNCTION_END;
	return FALSE;
}

int CDOSBaseObject::Update(int ProcessPacketLimit)
{
	FUNCTION_BEGIN;
	FUNCTION_END;
	return 0;
}


