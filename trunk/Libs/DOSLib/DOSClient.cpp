/****************************************************************************/
/*                                                                          */
/*      文件名:    DOSClient.cpp                                            */
/*      创建日期:  2009年10月15日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#include "StdAfx.h"

IMPLEMENT_CLASS_INFO(CDOSClient,CNameObject);

CDOSClient::CDOSClient(void)
{
	
}

CDOSClient::~CDOSClient(void)
{
	Destory();
}


void CDOSClient::Destory()
{
	CNetConnection::Destory();
}

BOOL CDOSClient::Start(UINT MaxMsgSize,const CIPAddress& Address,DWORD TimeOut)
{
	if(!m_SendBuffer.Create(MaxMsgSize))
		return FALSE;
	if(!m_AssembleBuffer.Create(MaxMsgSize*2))
		return FALSE;
	if(!CNetConnection::Create())
		return FALSE;
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

void CDOSClient::PrintLog(int Level,DWORD Color,LPCTSTR Format,va_list vl)
{

}

BOOL CDOSClient::RegisterObject(DOS_OBJECT_REGISTER_INFO_EX& ObjectRegisterInfo)
{
	return false;
}
void CDOSClient::Release()
{
	CNetConnection::Release();
}

void CDOSClient::OnRecvData(const CEasyBuffer& DataBuffer)
{
	MSG_LEN_TYPE PacketSize=0;
	UINT PeekPos=0;
	if(DataBuffer.GetUsedSize()>m_AssembleBuffer.GetFreeSize())
	{
		Close();
		PrintDOSLog(0xff0000,"(%d)拼包缓冲溢出，连接断开！",GetID());
		return;
	}
	m_AssembleBuffer.PushBack(DataBuffer.GetBuffer(),DataBuffer.GetUsedSize());
	m_AssembleBuffer.Peek(PeekPos,&PacketSize,sizeof(MSG_LEN_TYPE));
	while(m_AssembleBuffer.GetUsedSize()>=PacketSize&&PacketSize)
	{
		if(PacketSize<sizeof(CDOSSimpleMessage::GetMsgHeaderLength()))
		{
			Close();
			PrintDOSLog(0xff0000,"(%d)收到非法包，连接断开！",GetID());
		}
		OnDOSMessage((CDOSSimpleMessage *)m_AssembleBuffer.GetBuffer());
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

