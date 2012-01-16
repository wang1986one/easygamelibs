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


CEasyBuffer	CDOSProxyConnection::m_CompressBuffer;

char g_LZOCompressWorkMemory[LZO1X_1_MEM_COMPRESS];


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

	if(((CDOSServer *)GetServer())->GetConfig().ProxyMsgMinCompressSize)
	{
		if(m_CompressBuffer.GetBufferSize()<((CDOSServer *)GetServer())->GetConfig().MaxProxyMsgSize)
		{
			if(!m_CompressBuffer.Create(((CDOSServer *)GetServer())->GetConfig().MaxProxyMsgSize))
			{
				PrintDOSLog(0xff0000,"创建%u大小的压缩缓冲失败！",
					((CDOSServer *)GetServer())->GetConfig().MaxProxyMsgSize);
				return FALSE;
			}
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
	SetSendDelay(((CDOSServer *)GetServer())->GetConfig().ProxySendDelay);
	SetSendQueryLimit(((CDOSServer *)GetServer())->GetConfig().ProxySendQueryLimit);
	PrintDOSDebugLog(0xff0000,"发送延时设置为%u,并发发送限制为%u",
		((CDOSServer *)GetServer())->GetConfig().ProxySendDelay,
		((CDOSServer *)GetServer())->GetConfig().ProxySendQueryLimit);
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
	MSG_LEN_TYPE PacketSize=0;
	UINT PeekPos=0;
	m_AssembleBuffer.PushBack(DataBuffer.GetBuffer(),DataBuffer.GetUsedSize());

	m_AssembleBuffer.Peek(PeekPos,&PacketSize,sizeof(MSG_LEN_TYPE));
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
		m_AssembleBuffer.Peek(PeekPos,&PacketSize,sizeof(MSG_LEN_TYPE));
	}
	FUNCTION_END;
}

void CDOSProxyConnection::OnMsg(CDOSSimpleMessage * pMessage)
{
	FUNCTION_BEGIN;
	SendInSideMsg(pMessage->GetMsgID(),pMessage->GetDataBuffer(),pMessage->GetDataLength());
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
		//PrintDOSDebugLog(0,"发送了消息[%u]",pPacket->GetMessage().GetMsgID());

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


BOOL CDOSProxyConnection::SendInSideMsg(MSG_ID_TYPE MsgID,LPVOID pData,UINT DataSize)
{
	FUNCTION_BEGIN;
	int PacketSize=CDOSMessagePacket::CaculatePacketLength(DataSize,1);

	
	OBJECT_ID TargetObjectID=GetMsgMapObjectID(MsgID);
	if(TargetObjectID.ID)
	{
		return GetServer()->GetRouter()->RouterMessage(m_ObjectID,TargetObjectID,MsgID,0,pData,DataSize);
	}
	else
	{
		PrintDOSLog(0xff0000,"无法找到消息%u的接收者！",MsgID);
		return FALSE;
	}
	FUNCTION_END;
	return FALSE;
}



inline BOOL CDOSProxyConnection::SendOutSideMsg(CDOSMessagePacket * pPacket)
{
	FUNCTION_BEGIN;
	switch(pPacket->GetMessage().GetMsgID())
	{
	case DSM_PROXY_REGISTER_MSG_MAP:
		if(pPacket->GetMessage().GetDataLength()>=sizeof(MSG_ID_TYPE))
		{
			int Count=(pPacket->GetMessage().GetDataLength())/sizeof(MSG_ID_TYPE);
			MSG_ID_TYPE * pMsgIDs=(MSG_ID_TYPE *)(pPacket->GetMessage().GetDataBuffer());
			for(int i=0;i<Count;i++)
			{
				RegisterMsgMap(pMsgIDs[i],pPacket->GetMessage().GetSenderID());
			}
		}
		return TRUE;
	case DSM_PROXY_UNREGISTER_MSG_MAP:
		if(pPacket->GetMessage().GetDataLength()>=sizeof(MSG_ID_TYPE))
		{
			int Count=(pPacket->GetMessage().GetDataLength())/sizeof(MSG_ID_TYPE);
			MSG_ID_TYPE * pMsgIDs=(MSG_ID_TYPE *)(pPacket->GetMessage().GetDataBuffer());
			for(int i=0;i<Count;i++)
			{
				UnregisterMsgMap(pMsgIDs[i],pPacket->GetMessage().GetSenderID());
			}
		}
		return TRUE;
	case DSM_PROXY_DISCONNECT:
		Disconnect();
		PrintDOSDebugLog(0xff0000,"连接被0x%llX断开了！",pPacket->GetMessage().GetSenderID().ID);
		return TRUE;
	case DSM_ROUTE_LINK_LOST:
		ClearMsgMapByRouterID(pPacket->GetMessage().GetSenderID().RouterID);
		return TRUE;
	case DSM_OBJECT_ALIVE_TEST:
		{
			BYTE IsEcho=1;
			GetServer()->GetRouter()->RouterMessage(m_ObjectID,pPacket->GetMessage().GetSenderID(),
				DSM_OBJECT_ALIVE_TEST,DOS_MESSAGE_FLAG_SYSTEM_MESSAGE,&IsEcho,sizeof(IsEcho));
		}
		return TRUE;
	default:
		{
			CDOSSimpleMessage * pSimpleMessage=pPacket->GetMessage().MakeSimpleMessage();

			if(((CDOSServer *)GetServer())->GetConfig().ProxyMsgMinCompressSize&&
				pSimpleMessage->GetDataLength()>=((CDOSServer *)GetServer())->GetConfig().ProxyMsgMinCompressSize)
			{
				if(m_CompressBuffer.GetBufferSize()<pSimpleMessage->GetMsgLength())
				{
					if(!m_CompressBuffer.Create(pSimpleMessage->GetMsgLength()))
					{
						PrintDOSLog(0xff0000,"创建%u大小的压缩缓冲失败，关闭连接！",
							pSimpleMessage->GetMsgLength());
						Disconnect();
						return FALSE;
					}
				}
				CDOSSimpleMessage * pNewMsg=(CDOSSimpleMessage *)m_CompressBuffer.GetBuffer();
				pNewMsg->GetMsgHeader()=pSimpleMessage->GetMsgHeader();
				pNewMsg->GetMsgHeader().MsgFlag|=DOS_MESSAGE_FLAG_COMPRESSED;

				lzo_uint OutLen=m_CompressBuffer.GetBufferSize()-sizeof(CDOSSimpleMessage::DOS_SIMPLE_MESSAGE_HEAD);
				int Result=lzo1x_1_compress((BYTE *)pSimpleMessage->GetDataBuffer(),pSimpleMessage->GetDataLength(),
					(BYTE *)pNewMsg->GetDataBuffer(),&OutLen,
					g_LZOCompressWorkMemory);

				if(Result==LZO_E_OK)
				{
					pNewMsg->SetDataLength(OutLen);
					pSimpleMessage=pNewMsg;
				}
				else
				{
					PrintDOSLog(0xff0000,"压缩消息失败(%d)，将直接发送",
						Result);
				}
			}			

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
	pNewPacket->GetMessage().SetMsgID(DSM_PROXY_DISCONNECT);
	pNewPacket->GetMessage().SetMsgFlag(DOS_MESSAGE_FLAG_SYSTEM_MESSAGE);
	pNewPacket->GetMessage().SetSenderID(m_ObjectID);
	OBJECT_ID * pTargetObjectIDs=pNewPacket->GetTargetIDs();

	void * Pos=m_MessageMap.GetFirstObjectPos();
	while(Pos)
	{
		MSG_ID_TYPE MsgID;
		OBJECT_ID * pTargetObjectID=m_MessageMap.GetNextObject(Pos,MsgID);
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


OBJECT_ID CDOSProxyConnection::GetMsgMapObjectID(MSG_ID_TYPE CmdID)
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


BOOL CDOSProxyConnection::RegisterMsgMap(MSG_ID_TYPE MsgID,OBJECT_ID ObjectID)
{
	FUNCTION_BEGIN;
	PrintDOSDebugLog(0xff0000,"0x%llX注册了代理[0x%X]消息映射[%u]！",ObjectID.ID,GetID(),MsgID);
	return m_MessageMap.Insert(MsgID,ObjectID);
	FUNCTION_END;
	return FALSE;
}
BOOL CDOSProxyConnection::UnregisterMsgMap(MSG_ID_TYPE MsgID,OBJECT_ID ObjectID)
{
	FUNCTION_BEGIN;
	PrintDOSDebugLog(0xff0000,"0x%llX注销了代理[0x%X]消息映射[%u]！",ObjectID.ID,GetID(),MsgID);
	return m_MessageMap.Delete(MsgID);
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

void CDOSProxyConnection::ClearMsgMapByRouterID(UINT RouterID)
{
	FUNCTION_BEGIN;
	void * Pos=m_MessageMap.GetFirstObjectPos();
	while(Pos)
	{
		MSG_ID_TYPE MsgID;
		OBJECT_ID * pObjectID=m_MessageMap.GetNextObject(Pos,MsgID);
		if(pObjectID->RouterID==RouterID)
		{
			m_MessageMap.Delete(MsgID);
		}
	}
	if(m_MessageMap.GetObjectCount()<=0)
	{
		PrintDOSLog(0xff0000,"代理[0x%X]已经没有任何消息映射，连接断开！",GetID());
		Disconnect();
	}
	FUNCTION_END;
}