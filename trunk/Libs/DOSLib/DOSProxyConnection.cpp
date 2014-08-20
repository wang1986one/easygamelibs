/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DOSProxyConnection.cpp                                   */
/*      ��������:  2009��10��23��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
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
	m_KeepAliveCount=0;
	m_MaxKeepAliveCount=10;
	m_KeepAliveTime=30000;
	m_NeedDelayClose=false;
	m_UseServerInitiativeKeepAlive=false;
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

	m_MaxKeepAliveCount=GetServer()->GetConfig().ProxyKeepAliveCount;
	m_KeepAliveTime=GetServer()->GetConfig().ProxyKeepAliveTime;
	m_UseServerInitiativeKeepAlive=GetServer()->GetConfig().ProxyUseServerInitiativeKeepAlive;

	if(m_AssembleBuffer.GetBufferSize()<((CDOSServer *)GetServer())->GetConfig().MaxProxyMsgSize*2)
	{
		if(!m_AssembleBuffer.Create(((CDOSServer *)GetServer())->GetConfig().MaxProxyMsgSize*2))
		{
			PrintDOSLog(0xff0000,_T("����%u��С��ƴ������ʧ�ܣ�"),
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
				PrintDOSLog(0xff0000,_T("����%u��С��ѹ������ʧ�ܣ�"),
					((CDOSServer *)GetServer())->GetConfig().MaxProxyMsgSize);
				return FALSE;
			}
		}
	}

	CDOSMessagePacket * pPacket;
	while(m_MsgQueue.PopFront(pPacket))
	{
		if(!GetServer()->ReleaseMessagePacket(pPacket))
		{
			PrintDOSLog(0xff0000,_T("CDOSProxyConnection::OnDisconnection:�ͷ���Ϣ�ڴ��ʧ�ܣ�"));
		}		
	}
	
	if(m_MsgQueue.GetBufferSize()<((CDOSServer *)GetServer())->GetConfig().MaxProxyConnectionMsgQueue)
	{
		if(!m_MsgQueue.Create(((CDOSServer *)GetServer())->GetConfig().MaxProxyConnectionMsgQueue))
		{
			PrintDOSLog(0xff0000,_T("����%u��С����Ϣ����ʧ�ܣ�"),
				((CDOSServer *)GetServer())->GetConfig().MaxProxyConnectionMsgQueue);
			return FALSE;
		}
	}

	if(m_MessageMap.GetBufferSize()<((CDOSServer *)GetServer())->GetConfig().MaxProxyMsgMap)
	{
		if(!m_MessageMap.Create(((CDOSServer *)GetServer())->GetConfig().MaxProxyMsgMap))
		{
			PrintDOSLog(0xff0000,_T("����%u��С����Ϣӳ���ʧ�ܣ�"),
				((CDOSServer *)GetServer())->GetConfig().MaxProxyMsgMap);
			return FALSE;
		}
	}
	else
	{
		m_MessageMap.Clear();
	}

	m_KeepAliveTimer.SaveTime();
	m_KeepAliveCount=0;
	m_MaxKeepAliveCount=((CDOSServer *)GetServer())->GetConfig().ProxyKeepAliveCount;
	m_UnacceptConnectionKeepTimer.SaveTime();
	m_NeedDelayClose=false;
	return TRUE;
	FUNCTION_END;
	return FALSE;
}

void CDOSProxyConnection::Destory()
{
	FUNCTION_BEGIN;
	CNetConnection::Destory();
	
	//CDOSMessagePacket * pPacket;
	//while(m_MsgQueue.PopFront(pPacket))
	//{
	//	if(!GetServer()->ReleaseMessagePacket(pPacket))
	//	{
	//		PrintDOSLog(0xff0000,_T("CDOSProxyConnection::Destory:�ͷ���Ϣ�ڴ��ʧ�ܣ�"));
	//	}		
	//}
	//m_MessageMap.Clear();
	FUNCTION_END;
}

void CDOSProxyConnection::OnConnection(BOOL IsSucceed)
{	
	FUNCTION_BEGIN;
	PrintDOSDebugLog(0xff0000,_T("�յ������������ӣ�"));
	SetSendDelay(((CDOSServer *)GetServer())->GetConfig().ProxySendDelay);
	SetSendQueryLimit(((CDOSServer *)GetServer())->GetConfig().ProxySendQueryLimit);
	PrintDOSDebugLog(0xff0000,_T("������ʱ����Ϊ%u,������������Ϊ%u"),
		((CDOSServer *)GetServer())->GetConfig().ProxySendDelay,
		((CDOSServer *)GetServer())->GetConfig().ProxySendQueryLimit);
	m_NeedDelayClose=false;
	m_KeepAliveTimer.SaveTime();
	m_KeepAliveCount=0;
	FUNCTION_END;
}
void CDOSProxyConnection::OnDisconnection()
{	
	FUNCTION_BEGIN;
	PrintDOSDebugLog(0xff0000,_T("�������(%d)�����ӶϿ���"),GetID());
	SendDisconnectNotify();

	CDOSMessagePacket * pPacket;
	while(m_MsgQueue.PopFront(pPacket))
	{
		if(!GetServer()->ReleaseMessagePacket(pPacket))
		{
			PrintDOSLog(0xff0000,_T("CDOSProxyConnection::OnDisconnection:�ͷ���Ϣ�ڴ��ʧ�ܣ�"));
		}		
	}
	m_MessageMap.Clear();
	m_UnacceptConnectionKeepTimer.SaveTime();
	FUNCTION_END;
}

void CDOSProxyConnection::OnRecvData(const CEasyBuffer& DataBuffer)
{
	FUNCTION_BEGIN;
	MSG_LEN_TYPE PacketSize=0;
	UINT PeekPos=0;
	if(!m_AssembleBuffer.PushBack(DataBuffer.GetBuffer(),DataBuffer.GetUsedSize()))
	{
		Disconnect();
		PrintDOSLog(0xff0000,_T("�������(%d)ƴ��������������ӶϿ���"),GetID());
	}
	else
	{
		if(m_AssembleBuffer.Peek(PeekPos,&PacketSize,sizeof(MSG_LEN_TYPE)))
		{
			while(m_AssembleBuffer.GetUsedSize()>=PacketSize&&PacketSize)
			{
				if(PacketSize<CDOSSimpleMessage::GetMsgHeaderLength())
				{
					Disconnect();
					PrintDOSLog(0xff0000,_T("�������(%d)�յ��Ƿ��������ӶϿ���"),GetID());
				}
				OnMsg((CDOSSimpleMessage *)m_AssembleBuffer.GetBuffer());
				m_AssembleBuffer.PopFront(NULL,PacketSize);
				PeekPos=0;
				PacketSize=0;
				if(!m_AssembleBuffer.Peek(PeekPos,&PacketSize,sizeof(MSG_LEN_TYPE)))
					break;
			}
		}
	}
	FUNCTION_END;
}

void CDOSProxyConnection::OnMsg(CDOSSimpleMessage * pMessage)
{
	FUNCTION_BEGIN;
	switch(pMessage->GetMsgID())
	{
	case DSM_PROXY_KEEP_ALIVE:
		{
			if(!m_UseServerInitiativeKeepAlive)
			{
				Send(pMessage,pMessage->GetMsgLength());
			}			
			m_KeepAliveCount=0;
		}
		
		break;
	default:
		SendInSideMsg(pMessage->GetMsgID(),pMessage->GetDataBuffer(),pMessage->GetDataLength());
	}
	
	FUNCTION_END;
}

BOOL CDOSProxyConnection::PushMessage(CDOSMessagePacket * pPacket)
{
	FUNCTION_BEGIN;


	if(!IsConnected())
		return FALSE;

	((CDOSServer *)GetServer())->AddRefMessagePacket(pPacket);
	pPacket->SetAllocTime(3);
	if(m_MsgQueue.PushBack(pPacket))
	{		
		pPacket->SetAllocTime(31);
		return TRUE;
	}
	else
	{
		if(!GetServer()->ReleaseMessagePacket(pPacket))
		{
			PrintDOSLog(0xff0000,_T("CDOSProxyConnection::PushMessage:�ͷ���Ϣ�ڴ��ʧ�ܣ�"));
		}
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
		pPacket->SetAllocTime(32);
		SendOutSideMsg(pPacket);
		if(!GetServer()->ReleaseMessagePacket(pPacket))
		{
			PrintDOSLog(0xff0000,_T("CDOSProxyConnection::Update:�ͷ���Ϣ�ڴ��ʧ�ܣ�"));
		}
		PacketCount++;
		if(PacketCount>=PacketLimit&&(!m_WantClose))
			break;
	}

	PacketCount+=CNetConnection::Update(PacketLimit);

	if(m_KeepAliveTimer.IsTimeOut(m_KeepAliveTime))
	{
		m_KeepAliveTimer.SaveTime();
		m_KeepAliveCount++;
		if(m_UseServerInitiativeKeepAlive)
		{
			CDOSSimpleMessage KeepAliveMsg;
			KeepAliveMsg.Init();
			KeepAliveMsg.SetMsgID(DSM_PROXY_KEEP_ALIVE);
			KeepAliveMsg.SetDataLength(0);
			KeepAliveMsg.SetMsgFlag(DOS_MESSAGE_FLAG_SYSTEM_MESSAGE);
			Send(&KeepAliveMsg,KeepAliveMsg.GetMsgLength());
		}
		if(m_KeepAliveCount>=m_MaxKeepAliveCount)
		{
			PrintDOSLog(0xff0000,_T("CDOSProxyConnection::Update:KeepAlive��ʱ��"));
			m_KeepAliveCount=0;
			Disconnect();
		}
	}

	if(m_MessageMap.GetObjectCount()==0)
	{
		if(m_UnacceptConnectionKeepTimer.IsTimeOut(((CDOSServer *)GetServer())->GetConfig().ProxyUnacceptConnectionKeepTime))
		{
			PrintDOSLog(0xff0000,_T("CDOSProxyConnection::Update:δȷ�����Ӵ���ʱ�������"));
			Disconnect();
		}
	}

	if(m_NeedDelayClose)
	{
		if(m_DelayCloseTimer.IsTimeOut())
		{
			PrintDOSLog(0xff0000,_T("CDOSProxyConnection::Update:������ʱ�رգ�"));
			Disconnect();
		}
	}

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
		PrintDOSLog(0xff0000,_T("�޷��ҵ���Ϣ0X%x�Ľ����ߣ�"),MsgID);
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
		if(pPacket->GetMessage().GetDataLength()>=sizeof(UINT))
		{
			UINT Delay=*((UINT *)(pPacket->GetMessage().GetDataBuffer()));
			m_NeedDelayClose=true;
			m_DelayCloseTimer.SetTimeOut(Delay);
			PrintDOSDebugLog(0xff0000,_T("0x%llX������%uMS��Ͽ����ӣ�"),pPacket->GetMessage().GetSenderID().ID,Delay);
		}
		return TRUE;
	case DSM_PROXY_GET_IP:
		{
			PROXY_CLIENT_IP_INFO IPInfo;
			IPInfo.IP=GetRemoteAddress().GetIP();
			IPInfo.Port=GetRemoteAddress().GetPort();

			GetServer()->GetRouter()->RouterMessage(m_ObjectID,pPacket->GetMessage().GetSenderID(),
				DSM_PROXY_IP_REPORT,DOS_MESSAGE_FLAG_SYSTEM_MESSAGE,&IPInfo,sizeof(IPInfo));
		}
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
			bool NeedSaveHead=pPacket->GetRefCount()>1;

			CDOSMessage::DOS_MESSAGE_HEAD SaveHeader;

			if(NeedSaveHead)
			{
				SaveHeader=pPacket->GetMessage().GetMsgHeader();
			}
			CDOSSimpleMessage * pSimpleMessage=pPacket->GetMessage().MakeSimpleMessage();			

			pSimpleMessage=CompressMsg(pSimpleMessage);
			if(pSimpleMessage==NULL)
			{
				if(NeedSaveHead)
				{
					pPacket->GetMessage().GetMsgHeader()=SaveHeader;
				}
				Disconnect();
				return FALSE;
			}

			BOOL Ret=Send(pSimpleMessage,pSimpleMessage->GetMsgLength());

			if(NeedSaveHead)
			{
				pPacket->GetMessage().GetMsgHeader()=SaveHeader;
			}

			return Ret;
		}
	}	
	FUNCTION_END;
	return FALSE;
}

BOOL CDOSProxyConnection::SendDisconnectNotify()
{
	FUNCTION_BEGIN;
	int MsgMapCount=m_MessageMap.GetObjectCount();
	if(MsgMapCount)
	{
		int PacketSize=CDOSMessagePacket::CaculatePacketLength(0,MsgMapCount);

		CDOSMessagePacket * pNewPacket=GetServer()->NewMessagePacket(PacketSize);
		if(pNewPacket==NULL)
		{
			PrintDOSLog(0xff0000,_T("������Ϣ�ڴ��ʧ�ܣ�"));
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

		UINT RealTargetIDCount=DistinctObjectID(pTargetObjectIDs,MsgMapCount);
		for(UINT i=0;i<RealTargetIDCount;i++)
		{
			PrintDOSDebugLog(0xff0000,_T("��[0x%llX]���ʹ���������֪ͨ"),pTargetObjectIDs[i]);
		}
		pNewPacket->SetTargetIDs(RealTargetIDCount,NULL);
		pNewPacket->MakePacketLength();

		BOOL Ret=GetServer()->GetRouter()->RouterMessage(pNewPacket);
		GetServer()->ReleaseMessagePacket(pNewPacket);
		return Ret;
	}
	else
	{
		return TRUE;
	}
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
	OBJECT_ID * pObjectID=m_MessageMap.Find(MsgID);
	//if(pObjectID)
	//{
	//	PrintDOSDebugLog(0xff0000,_T("0x%llXע��Ĵ���[0x%X]��Ϣӳ��[0x%X]��[0x%llX]ȡ����"),*pObjectID,GetID(),MsgID,ObjectID);
	//}
	//else
	//{
	//	PrintDOSDebugLog(0xff0000,_T("0x%llXע���˴���[0x%X]��Ϣӳ��[0x%X]��"),ObjectID.ID,GetID(),MsgID);
	//}
	
	m_UnacceptConnectionKeepTimer.SaveTime();
	return m_MessageMap.Insert(MsgID,ObjectID);
	FUNCTION_END;
	return FALSE;
}
BOOL CDOSProxyConnection::UnregisterMsgMap(MSG_ID_TYPE MsgID,OBJECT_ID ObjectID)
{
	FUNCTION_BEGIN;
	OBJECT_ID * pObjectID=m_MessageMap.Find(MsgID);
	if(pObjectID)
	{
		if(*pObjectID==ObjectID)
		{
			//PrintDOSDebugLog(0xff0000,_T("0x%llXע���˴���[0x%X]��Ϣӳ��[0x%X]��"),ObjectID.ID,GetID(),MsgID);
			m_UnacceptConnectionKeepTimer.SaveTime();
			return m_MessageMap.Delete(MsgID);
		}
		else
		{
			//PrintDOSDebugLog(0xff0000,_T("0x%llXע������[0x%X]��Ϣӳ��[0x%X],��ע�������[0x%llX],�޷�ע����"),ObjectID.ID,GetID(),MsgID,*pObjectID);
			return FALSE;
		}
	}
	else
	{
		PrintDOSDebugLog(0xff0000,_T("0x%llXע������[0x%X]��Ϣӳ��[0x%X],δ�ҵ�ӳ���¼��"),ObjectID.ID,GetID(),MsgID);
		return FALSE;
	}
	FUNCTION_END;
	return FALSE;
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
		PrintDOSLog(0xff0000,_T("����[0x%X]�Ѿ�û���κ���Ϣӳ�䣬���ӶϿ���"),GetID());
		Disconnect();
	}
	FUNCTION_END;
}

CDOSSimpleMessage * CDOSProxyConnection::CompressMsg(CDOSSimpleMessage * pMsg)
{
	FUNCTION_BEGIN;
	if(((CDOSServer *)GetServer())->GetConfig().ProxyMsgCompressType!=MSG_COMPRESS_NONE&&
		((CDOSServer *)GetServer())->GetConfig().ProxyMsgMinCompressSize&&
		pMsg->GetDataLength()>=((CDOSServer *)GetServer())->GetConfig().ProxyMsgMinCompressSize)
	{
		
		switch(((CDOSServer *)GetServer())->GetConfig().ProxyMsgCompressType)
		{
		case MSG_COMPRESS_LZO:
			{
				if(m_CompressBuffer.GetBufferSize()<pMsg->GetMsgLength())
				{
					if(!m_CompressBuffer.Create(pMsg->GetMsgLength()))
					{
						PrintDOSLog(0xff0000,_T("����%u��С��ѹ������ʧ�ܣ��ر����ӣ�"),
							pMsg->GetMsgLength());

						return NULL;
					}
				}
				CDOSSimpleMessage * pNewMsg=(CDOSSimpleMessage *)m_CompressBuffer.GetBuffer();
				pNewMsg->GetMsgHeader()=pMsg->GetMsgHeader();
				pNewMsg->GetMsgHeader().MsgFlag|=DOS_MESSAGE_FLAG_COMPRESSED;

				lzo_uint OutLen=m_CompressBuffer.GetBufferSize()-sizeof(CDOSSimpleMessage::DOS_SIMPLE_MESSAGE_HEAD);
				int Result=lzo1x_1_compress((BYTE *)pMsg->GetDataBuffer(),pMsg->GetDataLength(),
					(BYTE *)pNewMsg->GetDataBuffer(),&OutLen,
					g_LZOCompressWorkMemory);

				if(Result==LZO_E_OK)
				{
					pNewMsg->SetDataLength((MSG_LEN_TYPE)OutLen);
					pMsg=pNewMsg;
				}
				else
				{
					PrintDOSLog(0xff0000,_T("lzoѹ����Ϣʧ��(%d)����ֱ�ӷ���"),
						Result);
				}
			}
			break;
		case MSG_COMPRESS_ZIP_FAST:
		case MSG_COMPRESS_ZIP_NORMAL:			
		case MSG_COMPRESS_ZIP_SLOW:
			//{
			//	UINT CompressBuffSize=compressBound(pMsg->GetMsgLength());
			//	if(m_CompressBuffer.GetBufferSize()<CompressBuffSize)
			//	{
			//		if(!m_CompressBuffer.Create(CompressBuffSize))
			//		{
			//			PrintDOSLog(0xff0000,_T("����%u��С��ѹ������ʧ�ܣ��ر����ӣ�"),
			//				pMsg->GetMsgLength());

			//			return NULL;
			//		}
			//	}
			//	CDOSSimpleMessage * pNewMsg=(CDOSSimpleMessage *)m_CompressBuffer.GetBuffer();
			//	pNewMsg->GetMsgHeader()=pMsg->GetMsgHeader();
			//	pNewMsg->GetMsgHeader().MsgFlag|=DOS_MESSAGE_FLAG_COMPRESSED;

			//	int ZipLevel=Z_DEFAULT_COMPRESSION;
			//	switch(((CDOSServer *)GetServer())->GetConfig().ProxyMsgCompressType)
			//	{
			//	case MSG_COMPRESS_ZIP_FAST:
			//		ZipLevel=Z_BEST_SPEED;
			//		break;
			//	case MSG_COMPRESS_ZIP_NORMAL:
			//		ZipLevel=Z_DEFAULT_COMPRESSION;
			//		break;
			//	case MSG_COMPRESS_ZIP_SLOW:
			//		ZipLevel=Z_BEST_COMPRESSION;
			//		break;
			//	}
			//	uLongf OutLen=m_CompressBuffer.GetBufferSize()-sizeof(CDOSSimpleMessage::DOS_SIMPLE_MESSAGE_HEAD);
			//	int Result=compress2((Bytef *)pNewMsg->GetDataBuffer(),&OutLen,
			//		(Bytef *)pMsg->GetDataBuffer(),pMsg->GetDataLength(),
			//		ZipLevel);

			//	if(Result==Z_OK)
			//	{
			//		pNewMsg->SetDataLength(OutLen);
			//		pMsg=pNewMsg;
			//	}
			//	else
			//	{
			//		PrintDOSLog(0xff0000,_T("zipѹ����Ϣʧ��(%d)����ֱ�ӷ���"),
			//			Result);
			//	}
			//}
			break;
		}

		
	}

	return pMsg;

	FUNCTION_END;
	return NULL;
}