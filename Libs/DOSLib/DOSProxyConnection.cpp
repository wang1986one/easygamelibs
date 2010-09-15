/****************************************************************************/
/*                                                                          */
/*      文件名:    DOSProxyConnection.cpp                                   */
/*      创建日期:  2009年10月23日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#include "StdAfx.h"

IMPLEMENT_CLASS_INFO(CDOSProxyConnection,CNetConnection);

CDOSProxyConnection::CDOSProxyConnection(void):CNetConnection()
{
	FUNCTION_BEGIN;
	m_pService=NULL;
	
	FUNCTION_END;
}

CDOSProxyConnection::~CDOSProxyConnection(void)
{
}

BOOL CDOSProxyConnection::Init(CDOSObjectProxyService * pService,UINT ID)
{
	FUNCTION_BEGIN;
	SetID(ID);	
	m_pService=pService;
	SetServer(pService->GetServer());

	m_ObjectID.ObjectIndex=GetID();
	m_ObjectID.GroupIndex=0;
	m_ObjectID.ObjectTypeID=DOT_PROXY_OBJECT;
	m_ObjectID.RouterID=GetServer()->GetConfig().RouterID;

	if(m_AssembleBuffer.GetBufferSize()<((CDOSServer *)GetServer())->GetConfig().MaxProxyMsgSize*2)
	{
		if(!m_AssembleBuffer.Create(((CDOSServer *)GetServer())->GetConfig().MaxProxyMsgSize*2))
		{
			PrintDOSLog(0xff0000,"创建%u大小的拼包缓冲失败！",
				((CDOSServer *)GetServer())->GetConfig().MaxProxyMsgSize*2);
			return FALSE;
		}
	}

	if(m_MsgQueue.GetBufferSize()<((CDOSServer *)GetServer())->GetConfig().MaxProxyConnectionMsgQueue)
	{
		if(!m_MsgQueue.Create(((CDOSServer *)GetServer())->GetConfig().MaxProxyConnectionMsgQueue))
		{
			PrintDOSLog(0xff0000,"创建%u大小的消息队列失败！",
				((CDOSServer *)GetServer())->GetConfig().MaxProxyConnectionMsgQueue);
			return FALSE;
		}
	}

	if(m_MessageMap.GetBufferSize()<((CDOSServer *)GetServer())->GetConfig().MaxProxyMsgMap)
	{
		if(!m_MessageMap.Create(((CDOSServer *)GetServer())->GetConfig().MaxProxyMsgMap))
		{
			PrintDOSLog(0xff0000,"创建%u大小的消息映射表失败！",
				((CDOSServer *)GetServer())->GetConfig().MaxProxyMsgMap);
			return FALSE;
		}
	}
	return TRUE;
	FUNCTION_END;
	return FALSE;
}

void CDOSProxyConnection::Destory()
{
	FUNCTION_BEGIN;
	m_MsgQueue.Clear();
	m_MessageMap.Clear();
	FUNCTION_END;
}

void CDOSProxyConnection::OnConnection(BOOL IsSucceed)
{	
	FUNCTION_BEGIN;
	PrintDOSDebugLog(0xff0000,"收到代理对象的连接！");
	FUNCTION_END;
}
void CDOSProxyConnection::OnDisconnection()
{	
	FUNCTION_BEGIN;
	PrintDOSDebugLog(0xff0000,"对象代理(%d)的连接断开！",GetID());
	SendDisconnectNotify();	
	FUNCTION_END;
}

void CDOSProxyConnection::OnRecvData(const CEasyBuffer& DataBuffer)
{
	FUNCTION_BEGIN;
	WORD PacketSize=0;
	UINT PeekPos=0;
	m_AssembleBuffer.PushBack(DataBuffer.GetBuffer(),DataBuffer.GetUsedSize());

	m_AssembleBuffer.Peek(PeekPos,&PacketSize,sizeof(WORD));
	while(m_AssembleBuffer.GetUsedSize()>=PacketSize&&PacketSize)
	{
		if(PacketSize<CDOSSimpleMessage::GetMsgHeaderLength())
		{
			Disconnect();
			PrintDOSLog(0xff0000,"对象代理(%d)收到非法包，连接断开！",GetID());
		}
		OnMsg((CDOSSimpleMessage *)m_AssembleBuffer.GetBuffer());
		m_AssembleBuffer.PopFront(NULL,PacketSize);
		PeekPos=0;
		PacketSize=0;
		m_AssembleBuffer.Peek(PeekPos,&PacketSize,sizeof(WORD));
	}
	FUNCTION_END;
}

void CDOSProxyConnection::OnMsg(CDOSSimpleMessage * pMessage)
{
	FUNCTION_BEGIN;
	SendInSideMsg(pMessage->GetCmdID(),pMessage->GetDataBuffer(),pMessage->GetDataLength());
	FUNCTION_END;
}

BOOL CDOSProxyConnection::PushMessage(CDOSMessagePacket * pPacket)
{
	FUNCTION_BEGIN;
	((CDOSServer *)GetServer())->AddRefMessagePacket(pPacket);
	if(m_MsgQueue.PushBack(pPacket))
	{			
		return TRUE;
	}
	else
	{
		((CDOSServer *)GetServer())->ReleaseMessagePacket(pPacket);
	}
	FUNCTION_END;
	return FALSE;
}

int CDOSProxyConnection::Update(int ProcessPacketLimit)
{
	FUNCTION_BEGIN;
	int PacketLimit=ProcessPacketLimit/2;
	int PacketCount=0;

	CDOSMessagePacket * pPacket;
	while(m_MsgQueue.PopFront(pPacket))
	{
		PrintDOSDebugLog(0,"发送了消息[%u]",pPacket->GetMessage().GetCmdID());

		SendOutSideMsg(pPacket);
		if(!GetServer()->ReleaseMessagePacket(pPacket))
		{
			PrintDOSLog(0xff0000,"释放消息内存块失败！");
		}

		PacketCount++;
		if(PacketCount>=PacketLimit&&(!m_WantClose))
			break;
	}

	PacketCount+=CNetConnection::Update(PacketLimit);

	return PacketCount;
	FUNCTION_END;
	return 0;
}

inline CDOSServer * CDOSProxyConnection::GetServer()
{
	FUNCTION_BEGIN;
	return (CDOSServer *)CNetConnection::GetServer();
	FUNCTION_END;
	return NULL;
}


BOOL CDOSProxyConnection::SendInSideMsg(WORD CmdID,LPVOID pData,UINT DataSize)
{
	FUNCTION_BEGIN;
	int PacketSize=CDOSMessagePacket::CaculatePacketLength(DataSize,1);

	
	OBJECT_ID TargetObjectID=GetMsgMapObjectID(CmdID);
	if(TargetObjectID.ID)
	{
		return GetServer()->GetRouter()->RouterMessage(m_ObjectID,TargetObjectID,CmdID,pData,DataSize);
	}
	else
	{
		PrintDOSLog(0xff0000,"无法找到消息%u的接收者！",CmdID);
		return FALSE;
	}
	FUNCTION_END;
	return FALSE;
}



inline BOOL CDOSProxyConnection::SendOutSideMsg(CDOSMessagePacket * pPacket)
{
	FUNCTION_BEGIN;
	switch(pPacket->GetMessage().GetCmdID())
	{
	case DSM_PROXY_REGISTER_MSG_MAP:
		if(pPacket->GetMessage().GetDataLength()>=sizeof(WORD))
		{
			int Count=(pPacket->GetMessage().GetDataLength())/sizeof(WORD);
			WORD * pCmdIDs=(WORD *)(pPacket->GetMessage().GetDataBuffer());
			for(int i=0;i<Count;i++)
			{
				RegisterMsgMap(pCmdIDs[i],pPacket->GetMessage().GetSenderID());
			}
		}
		return TRUE;
	case DSM_PROXY_UNREGISTER_MSG_MAP:
		if(pPacket->GetMessage().GetDataLength()>=sizeof(WORD))
		{
			int Count=(pPacket->GetMessage().GetDataLength())/sizeof(WORD);
			WORD * pCmdIDs=(WORD *)(pPacket->GetMessage().GetDataBuffer());
			for(int i=0;i<Count;i++)
			{
				UnregisterMsgMap(pCmdIDs[i],pPacket->GetMessage().GetSenderID());
			}
		}
		return TRUE;
	case DSM_PROXY_DISCONNECT:
		Disconnect();
		PrintDOSDebugLog(0xff0000,"连接被0x%llX断开了！",pPacket->GetMessage().GetSenderID().ID);
		return TRUE;
	default:
		{
			CDOSSimpleMessage * pSimpleMessage=pPacket->GetMessage().MakeSimpleMessage();
			return Send(pSimpleMessage,pSimpleMessage->GetMsgLength());
		}
	}	
	FUNCTION_END;
	return FALSE;
}

BOOL CDOSProxyConnection::SendDisconnectNotify()
{
	FUNCTION_BEGIN;
	int MsgMapCount=m_MessageMap.GetObjectCount();
	int PacketSize=CDOSMessagePacket::CaculatePacketLength(0,MsgMapCount);

	CDOSMessagePacket * pNewPacket=GetServer()->NewMessagePacket(PacketSize);
	if(pNewPacket==NULL)
	{
		PrintDOSLog(0xff0000,"分配消息内存块失败！");
		return FALSE;
	}
	
	pNewPacket->SetTargetIDs(0,NULL);
	pNewPacket->GetMessage().SetCmdID(DSM_PROXY_DISCONNECT);
	pNewPacket->GetMessage().SetSenderID(m_ObjectID);
	OBJECT_ID * pTargetObjectIDs=pNewPacket->GetTargetIDs();

	void * Pos=m_MessageMap.GetFirstObjectPos();
	while(Pos)
	{
		WORD CmdID;
		OBJECT_ID * pTargetObjectID=m_MessageMap.GetNextObject(Pos,CmdID);
		pNewPacket->AddTargetID(*pTargetObjectID);
	}
	
	UINT RealTargetIDCount=SortTargetObjectID(pTargetObjectIDs,MsgMapCount);
	pNewPacket->SetTargetIDs(RealTargetIDCount,NULL);
	pNewPacket->MakePacketLength();

	BOOL Ret=GetServer()->GetRouter()->RouterMessage(pNewPacket);
	GetServer()->ReleaseMessagePacket(pNewPacket);
	return Ret;
	FUNCTION_END;
	return FALSE;
}


OBJECT_ID CDOSProxyConnection::GetMsgMapObjectID(WORD CmdID)
{	
	FUNCTION_BEGIN;
	OBJECT_ID * pObjectID=m_MessageMap.Find(CmdID);
	if(pObjectID)
	{
		return *pObjectID;
	}
	else
	{
		return m_pService->GetGlobalMsgMapObjectID(CmdID);
	}
	FUNCTION_END;
	return 0;
}


BOOL CDOSProxyConnection::RegisterMsgMap(WORD CmdID,OBJECT_ID ObjectID)
{
	FUNCTION_BEGIN;
	PrintDOSDebugLog(0xff0000,"0x%llX注册了代理[0x%X]消息映射[%u]！",ObjectID.ID,GetID(),CmdID);
	return m_MessageMap.Insert(CmdID,ObjectID);
	FUNCTION_END;
	return FALSE;
}
BOOL CDOSProxyConnection::UnregisterMsgMap(WORD CmdID,OBJECT_ID ObjectID)
{
	FUNCTION_BEGIN;
	PrintDOSDebugLog(0xff0000,"0x%llX注销了代理[0x%X]消息映射[%u]！",ObjectID.ID,GetID(),CmdID);
	return m_MessageMap.Delete(CmdID);
	FUNCTION_END;
	return FALSE;
}

UINT CDOSProxyConnection::SortTargetObjectID(OBJECT_ID * pObjectIDs,UINT Count)
{
	FUNCTION_BEGIN;
	UINT FinishCount=1;
	for(UINT i=1;i<Count;i++)
	{
		int MinPtr=FindMinObjectID(pObjectIDs+i,Count-i);
		if(MinPtr>0)
		{
			if(pObjectIDs[i-1].ID>pObjectIDs[MinPtr].ID)
			{
				UINT64 Temp;
				Temp=pObjectIDs[i-1].ID;
				pObjectIDs[i-1].ID=pObjectIDs[MinPtr].ID;
				pObjectIDs[MinPtr].ID=Temp;
			}
			else if(pObjectIDs[i-1].ID==pObjectIDs[MinPtr].ID)
			{
				pObjectIDs[MinPtr].ID=0;
			}

		}
		else
		{
			break;
		}
	}
	return FinishCount;
	FUNCTION_END;
	return 0;
}

int CDOSProxyConnection::FindMinObjectID(OBJECT_ID * pObjectIDs,UINT Count)
{	
	FUNCTION_BEGIN;
	int MinPtr=-1;
	UINT64 MinID=0;
	for(UINT i=0;i<Count;i++)
	{
		if(pObjectIDs[i].ID)
		{
			if(MinPtr<0)
			{
				MinPtr=i;
				MinID=pObjectIDs[i].ID;
			}
			else
			{
				if(pObjectIDs[i].ID<MinID)
				{
					MinPtr=i;
					MinID=pObjectIDs[i].ID;
				}
			}
		}
	}	
	return MinPtr;
	FUNCTION_END;
	return 0;
}