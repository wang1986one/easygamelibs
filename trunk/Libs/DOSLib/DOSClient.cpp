/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DOSClient.cpp                                            */
/*      ��������:  2009��10��15��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#include "StdAfx.h"

IMPLEMENT_CLASS_INFO(CDOSClient,CNameObject);

CDOSClient::CDOSClient(void)
{
	m_KeepAliveCount=0;
	m_MaxKeepAliveCount=10;
	m_KeepAliveTime=30000;

	m_MsgCompressType=MSG_COMPRESS_LZO;
}

CDOSClient::~CDOSClient(void)
{
	Destory();
}


void CDOSClient::Destory()
{
	CNetConnection::Destory();
}

BOOL CDOSClient::Start(UINT SendBufferSize,UINT AssembleBufferSize,const CIPAddress& Address,DWORD TimeOut)
{
	if(!m_SendBuffer.Create(SendBufferSize))
		return FALSE;
	if(!m_AssembleBuffer.Create(AssembleBufferSize))
		return FALSE;
	if(!CNetConnection::Create())
		return FALSE;

	m_KeepAliveTimer.SaveTime();
	m_KeepAliveCount=0;
	return CNetConnection::Connect(Address,TimeOut);
}

void CDOSClient::Close()
{
	CNetConnection::Disconnect();
}



UINT CDOSClient::GetRouterID()
{
	return 0;
}
OBJECT_ID CDOSClient::GetObjectID()
{
	return 0;
}
int CDOSClient::GetGroupIndex()
{
	return -1;
}
BOOL CDOSClient::SendMessage(OBJECT_ID ReceiverID,MSG_ID_TYPE MsgID,WORD MsgFlag,LPCVOID pData,UINT DataSize)
{
	m_SendBuffer.SetUsedSize(0);
	CDOSSimpleMessage * pSimpleMessage=(CDOSSimpleMessage *)m_SendBuffer.GetBuffer();
	m_SendBuffer.PushBack(NULL,CDOSSimpleMessage::GetMsgHeaderLength());

	pSimpleMessage->Init();
	pSimpleMessage->SetMsgID(MsgID);
	pSimpleMessage->SetDataLength(DataSize);
	pSimpleMessage->SetMsgFlag(MsgFlag);
	
	if(!m_SendBuffer.PushBack(pData,DataSize))
		return false;

	return Send(pSimpleMessage,pSimpleMessage->GetMsgLength());
}

BOOL CDOSClient::SendMessageMulti(OBJECT_ID * pReceiverIDList,UINT ReceiverCount,bool IsSorted,MSG_ID_TYPE MsgID,WORD MsgFlag,LPCVOID pData,UINT DataSize)
{
	return FALSE;
}

CDOSMessagePacket * CDOSClient::NewMessagePacket(UINT DataSize,UINT ReceiverCount)
{
	MSG_LEN_TYPE PacketLen=CDOSMessagePacket::CaculatePacketLength(DataSize,ReceiverCount);
	MSG_LEN_TYPE PacketRealLen=CDOSMessagePacket::CaculateRealPacketLength(PacketLen);
	if(PacketRealLen>m_SendBuffer.GetBufferSize())
		return NULL;
	CDOSMessagePacket * pMsgPacket=(CDOSMessagePacket *)m_SendBuffer.GetBuffer();
	pMsgPacket->Init();
	pMsgPacket->SetPacketLength(PacketLen);
	pMsgPacket->IncRefCount();
	return pMsgPacket;
}
BOOL CDOSClient::ReleaseMessagePacket(CDOSMessagePacket * pPacket)
{
	pPacket->DecRefCount();
	return TRUE;
}
BOOL CDOSClient::SendMessagePacket(CDOSMessagePacket * pPacket)
{
	CDOSSimpleMessage * pSimpleMessage=pPacket->GetMessage().MakeSimpleMessage();
	return Send(pSimpleMessage,pSimpleMessage->GetMsgLength());
}

BOOL CDOSClient::RegisterMsgMap(OBJECT_ID ProxyObjectID,MSG_ID_TYPE * pMsgIDList,int CmdCount)
{
	return FALSE;
}
BOOL CDOSClient::UnregisterMsgMap(OBJECT_ID ProxyObjectID,MSG_ID_TYPE * pMsgIDList,int CmdCount)
{
	return FALSE;
}
BOOL CDOSClient::RegisterGlobalMsgMap(ROUTE_ID_TYPE ProxyRouterID,MSG_ID_TYPE * pMsgIDList,int CmdCount)
{
	return FALSE;
}
BOOL CDOSClient::UnregisterGlobalMsgMap(ROUTE_ID_TYPE ProxyRouterID,MSG_ID_TYPE * pMsgIDList,int CmdCount)
{
	return FALSE;
}

BOOL CDOSClient::AddConcernedObject(OBJECT_ID ObjectID,bool NeedTest)
{
	return FALSE;
}

BOOL CDOSClient::DeleteConcernedObject(OBJECT_ID ObjectID)
{
	return FALSE;
}

BOOL CDOSClient::FindObject(UINT ObjectType)
{
	return FALSE;
}
BOOL CDOSClient::ReportObject(OBJECT_ID TargetID,const CSmartStruct& ObjectInfo)
{
	return FALSE;
}
BOOL CDOSClient::CloseProxyObject(OBJECT_ID ProxyObjectID,UINT Delay)
{
	return FALSE;
}
BOOL CDOSClient::RequestProxyObjectIP(OBJECT_ID ProxyObjectID)
{
	return FALSE;
}

BOOL CDOSClient::RegisterObject(DOS_OBJECT_REGISTER_INFO_EX& ObjectRegisterInfo)
{
	return false;
}
void CDOSClient::Release()
{
	CNetConnection::Release();
}

BOOL CDOSClient::QueryShutDown(OBJECT_ID TargetID,int Level)
{
	return FALSE;
}

void CDOSClient::ShutDown(UINT PluginID)
{

}

void CDOSClient::OnRecvData(const CEasyBuffer& DataBuffer)
{
	MSG_LEN_TYPE PacketSize=0;
	UINT PeekPos=0;
	if(DataBuffer.GetUsedSize()>m_AssembleBuffer.GetFreeSize())
	{
		Close();
		PrintDOSLog(0xff0000,_T("(%d)ƴ��������������ӶϿ���"),GetID());
		return;
	}
	m_AssembleBuffer.PushBack(DataBuffer.GetBuffer(),DataBuffer.GetUsedSize());
	m_AssembleBuffer.Peek(PeekPos,&PacketSize,sizeof(MSG_LEN_TYPE));
	while(m_AssembleBuffer.GetUsedSize()>=PacketSize&&PacketSize)
	{
		if(PacketSize<sizeof(CDOSSimpleMessage::GetMsgHeaderLength()))
		{
			Close();
			PrintDOSLog(0xff0000,_T("(%d)�յ��Ƿ��������ӶϿ���"),GetID());
		}
		CDOSSimpleMessage * pMsg=(CDOSSimpleMessage *)m_AssembleBuffer.GetBuffer();		
		if(pMsg->GetMsgFlag()&DOS_MESSAGE_FLAG_COMPRESSED)
		{
			switch(m_MsgCompressType)
			{
			case MSG_COMPRESS_ZIP_FAST:
			case MSG_COMPRESS_ZIP_NORMAL:
			case MSG_COMPRESS_ZIP_SLOW:
				//{
				//	CDOSSimpleMessage * pNewMsg=(CDOSSimpleMessage *)m_SendBuffer.GetBuffer();
				//	pNewMsg->GetMsgHeader()=pMsg->GetMsgHeader();
				//	uLongf OutLen=m_SendBuffer.GetBufferSize()-sizeof(CDOSSimpleMessage::DOS_SIMPLE_MESSAGE_HEAD);
				//	int Result=uncompress((Bytef *)pNewMsg->GetDataBuffer(),&OutLen,
				//		(Bytef *)pMsg->GetDataBuffer(),pMsg->GetDataLength());
				//	if(Result==Z_OK)
				//	{
				//		pNewMsg->SetDataLength(OutLen);
				//		OnDOSMessage(pNewMsg);
				//	}
				//	else
				//	{
				//		Close();
				//		PrintDOSLog(0xff0000,_T("(%d��Ϣzip��ѹ��ʧ�ܣ����ӶϿ���"),GetID());
				//	}
				//}
				break;
			default:
				{
					CDOSSimpleMessage * pNewMsg=(CDOSSimpleMessage *)m_SendBuffer.GetBuffer();
					pNewMsg->GetMsgHeader()=pMsg->GetMsgHeader();
					lzo_uint OutLen=m_SendBuffer.GetBufferSize()-sizeof(CDOSSimpleMessage::DOS_SIMPLE_MESSAGE_HEAD);
					int Result=lzo1x_decompress_safe((BYTE *)pMsg->GetDataBuffer(),pMsg->GetDataLength(),
						(BYTE *)pNewMsg->GetDataBuffer(),&OutLen,
						NULL);
					if(Result==LZO_E_OK)
					{
						pNewMsg->SetDataLength(OutLen);
						OnDOSMessage(pNewMsg);
					}
					else
					{
						Close();
						PrintDOSLog(0xff0000,_T("(%d��Ϣlzo��ѹ��ʧ�ܣ����ӶϿ���"),GetID());
					}
				}
				break;
			}
			
		}
		else
		{
			switch(pMsg->GetMsgID())
			{
			case DSM_PROXY_KEEP_ALIVE:
				m_KeepAliveCount=0;
				break;
			default:
				OnDOSMessage(pMsg);
			}
		}
		
		m_AssembleBuffer.PopFront(NULL,PacketSize);
		PeekPos=0;
		PacketSize=0;
		m_AssembleBuffer.Peek(PeekPos,&PacketSize,sizeof(MSG_LEN_TYPE));
	}
}

BOOL CDOSClient::OnDOSMessage(CDOSSimpleMessage * pMessage)
{
	return TRUE;
}

int CDOSClient::Update(int ProcessPacketLimit)
{
	if(IsConnected()&&m_KeepAliveTimer.IsTimeOut(m_KeepAliveTime))
	{
		m_KeepAliveTimer.SaveTime();
		m_KeepAliveCount++;
		if(m_KeepAliveCount>=m_MaxKeepAliveCount)
		{
			PrintDOSLog(0xff0000,_T("CDOSClient::Update:KeepAlive��ʱ��"));
			m_KeepAliveCount=0;
			Disconnect();
		}
		SendMessage(0,DSM_PROXY_KEEP_ALIVE);
	}
	return CNetConnection::Update(ProcessPacketLimit);
}