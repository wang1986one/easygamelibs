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
	FUNCTION_END;
}

CDOSBaseObject::~CDOSBaseObject(void)
{
	FUNCTION_BEGIN;
	Destory();
	FUNCTION_END;
}

bool CDOSBaseObject::Initialize()
{
	FUNCTION_BEGIN;
	return m_MsgQueue.Create(GetManager()->GetServer()->GetConfig().MaxObjectMsgQueue);	
	FUNCTION_END;
	return FALSE;
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

int CDOSBaseObject::DoCycle(int ProcessPacketLimit)
{
	FUNCTION_BEGIN;
	CDOSMessagePacket * pPacket;
	int ProcessCount=0;
	int Limit=ProcessPacketLimit;
	while(m_MsgQueue.PopFront(pPacket))
	{		
		if(!OnPreTranslateMessage(&(pPacket->GetMessage())))
			m_MsgManager.DealMsg(&(pPacket->GetMessage()));

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

BOOL CDOSBaseObject::SendMessage(OBJECT_ID ReceiverID,WORD MsgID,LPCVOID pData,UINT DataSize)
{
	FUNCTION_BEGIN;
	if(m_pRouter)
		return m_pRouter->RouterMessage(GetObjectID(),ReceiverID,MsgID,pData,DataSize);
	FUNCTION_END;
	return FALSE;
}

BOOL CDOSBaseObject::SendMessageMulti(OBJECT_ID * pReceiverIDList,UINT ReceiverCount,bool IsSorted,WORD MsgID,LPCVOID pData,UINT DataSize)
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
	pNewPacket->GetMessage().SetCmdID(MsgID);
	pNewPacket->GetMessage().SetSenderID(GetObjectID());
	pNewPacket->GetMessage().SetDataLength(DataSize);
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
	WORD PacketSize=CDOSMessagePacket::CaculatePacketLength(DataSize,ReceiverCount);
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

BOOL CDOSBaseObject::OnPreTranslateMessage(CDOSMessage * pMessage)
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

UINT CDOSBaseObject::GetRouterID()
{
	FUNCTION_BEGIN;
	return m_pRouter->GetRouterID();
	FUNCTION_END;
	return 0;
}

BOOL CDOSBaseObject::InitMsgProc(int MaxMsgCount)
{
	FUNCTION_BEGIN;
	return m_MsgManager.Init(MaxMsgCount);
	FUNCTION_END;
	return FALSE;
}
void CDOSBaseObject::RegisterMsgProc(WORD CmdID,DOS_MSG_PROC MsgProc)
{
	FUNCTION_BEGIN;
	m_MsgManager.RegisterMsgProc(true,CmdID,this,MsgProc);
	FUNCTION_END;
}

void CDOSBaseObject::UnregisterMsgProc(WORD CmdID,DOS_MSG_PROC MsgProc)
{
	FUNCTION_BEGIN;
	m_MsgManager.RegisterMsgProc(false,CmdID,this,MsgProc);
	FUNCTION_END;
}

BOOL CDOSBaseObject::RegisterMsgMap(OBJECT_ID ProxyObjectID,WORD * pCmdIDList,int CmdCount)
{
	FUNCTION_BEGIN;
	return SendMessage(ProxyObjectID,DSM_PROXY_REGISTER_MSG_MAP,pCmdIDList,sizeof(WORD)*CmdCount);
	FUNCTION_END;
	return FALSE;
}

BOOL CDOSBaseObject::UnregisterMsgMap(OBJECT_ID ProxyObjectID,WORD * pCmdIDList,int CmdCount)
{
	FUNCTION_BEGIN;
	return SendMessage(ProxyObjectID,DSM_PROXY_UNREGISTER_MSG_MAP,pCmdIDList,sizeof(WORD)*CmdCount);
	FUNCTION_END;
	return FALSE;
}


BOOL CDOSBaseObject::RegisterGlobalMsgMap(WORD ProxyRouterID,WORD * pCmdIDList,int CmdCount)
{
	FUNCTION_BEGIN;
	OBJECT_ID ProxyID;
	ProxyID.RouterID=ProxyRouterID;
	ProxyID.ObjectTypeID=DOT_PROXY_OBJECT;
	ProxyID.GroupIndex=0;
	ProxyID.ObjectIndex=0;
	return SendMessage(ProxyID,DSM_PROXY_REGISTER_GLOBAL_MSG_MAP,pCmdIDList,sizeof(WORD)*CmdCount);
	FUNCTION_END;
	return FALSE;
}


BOOL CDOSBaseObject::UnregisterGlobalMsgMap(WORD ProxyRouterID,WORD * pCmdIDList,int CmdCount)
{
	FUNCTION_BEGIN;
	OBJECT_ID ProxyID;
	ProxyID.RouterID=ProxyRouterID;
	ProxyID.ObjectTypeID=DOT_PROXY_OBJECT;
	ProxyID.GroupIndex=0;
	ProxyID.ObjectIndex=0;
	return SendMessage(ProxyID,DSM_PROXY_UNREGISTER_GLOBAL_MSG_MAP,pCmdIDList,sizeof(WORD)*CmdCount);
	FUNCTION_END;
	return FALSE;
}
