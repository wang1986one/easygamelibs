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



BOOL CDOSClient::SendMessage(WORD CmdID,LPVOID pData,UINT DataSize)
{
	static char g_StaticBuff[MAX_MESSAGE_PACKET_SIZE];

	CDOSSimpleMessage * pSimpleMessage=(CDOSSimpleMessage *)g_StaticBuff;

	pSimpleMessage->Init();
	pSimpleMessage->SetCmdID(CmdID);
	pSimpleMessage->SetDataLength(DataSize);
	if(pData)
		memcpy(pSimpleMessage->GetDataBuffer(),pData,DataSize);
	
	return QuerySend(pSimpleMessage,pSimpleMessage->GetMsgLength());
}

void CDOSClient::OnRecvData(const CEasyBuffer& DataBuffer)
{
	WORD PacketSize=0;
	UINT PeekPos=0;
	if(DataBuffer.GetUsedSize()>m_AssembleBuffer.GetFreeSize())
	{
		Close();
		PrintDOSLog(0xff0000,"(%d)ƴ��������������ӶϿ���",GetID());
		return;
	}
	m_AssembleBuffer.PushBack(DataBuffer.GetBuffer(),DataBuffer.GetUsedSize());
	m_AssembleBuffer.Peek(PeekPos,&PacketSize,sizeof(WORD));
	while(m_AssembleBuffer.GetUsedSize()>=PacketSize&&PacketSize)
	{
		if(PacketSize<sizeof(CDOSSimpleMessage::GetMsgHeaderLength()))
		{
			Close();
			PrintDOSLog(0xff0000,"(%d)�յ��Ƿ��������ӶϿ���",GetID());
		}
		OnMsg((CDOSSimpleMessage *)m_AssembleBuffer.GetBuffer());
		m_AssembleBuffer.PopFront(NULL,PacketSize);
		PeekPos=0;
		PacketSize=0;
		m_AssembleBuffer.Peek(PeekPos,&PacketSize,sizeof(WORD));
	}
}

void CDOSClient::OnMsg(CDOSSimpleMessage * pMessage)
{

}

