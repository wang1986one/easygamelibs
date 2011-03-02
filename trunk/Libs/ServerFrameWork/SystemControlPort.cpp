/****************************************************************************/
/*                                                                          */
/*      �ļ���:    SystemControlPort.cpp                                    */
/*      ��������:  2009��07��06��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#include "StdAfx.h"
#include "SystemControlPort.h"

CSystemControlPort::CSystemControlPort(void)
{
	m_pServer=NULL;
}

CSystemControlPort::~CSystemControlPort(void)
{
}

bool CSystemControlPort::Init(IBaseServer * pServer)
{
	if(pServer)
	{
		m_pServer=pServer;
		SetServer(m_pServer->GetServer());
	}	

	if(CSystemConfig::GetInstance()->GetUDPControlAddress().GetPort())
	{
		Create(IPPROTO_UDP);	
		if(!StartListen(CSystemConfig::GetInstance()->GetUDPControlAddress()))
		{
			Log("SystemControlPort����ʧ��%s:%u",
				CSystemConfig::GetInstance()->GetUDPControlAddress().GetIPString(),
				CSystemConfig::GetInstance()->GetUDPControlAddress().GetPort());
			return false;
		}
		Log("SystemControlPort�ɹ�������%s:%u",
			CSystemConfig::GetInstance()->GetUDPControlAddress().GetIPString(),
			CSystemConfig::GetInstance()->GetUDPControlAddress().GetPort());
	}
	else
	{
		Log("UDP���ƶ˿�δ����");
	}
	return true;
}

void CSystemControlPort::OnStartUp()
{

}

void CSystemControlPort::OnClose()
{

}

void CSystemControlPort::OnRecvData(const CIPAddress& IPAddress,const CEasyBuffer& DataBuffer)
{		

	if(m_pServer==NULL)
		return;

	if(DataBuffer.GetUsedSize()<sizeof(SMSG_HEADER))
		return;

	SMSG * pMsg=(SMSG *)DataBuffer.GetBuffer();
	UINT DataSize=DataBuffer.GetUsedSize()-sizeof(SMSG_HEADER);

	switch(pMsg->Header.MsgID)
	{
	case SC_MSG_QUERY_SERVER_STATUS:
		{
			CSmartStruct QueryInfo(pMsg->Data,DataSize,false);
			CSmartStruct ServerStatus(SERVER_STATUS_BLOCK_SIZE);
			for(int i=0;i<QueryInfo.GetMemberCount();i++)
			{
				if(QueryInfo.IndexToID(i)==SC_SST_QSS_STATUS_ID)
				{
					WORD StatusID=QueryInfo.GetMemberByIndex(i);
					ServerStatus.AddMember(StatusID,
						m_pServer->GetServerStatus(StatusID));
				}
			}
			SendMsg(IPAddress,SC_MSG_QUERY_SERVER_STATUS_RESULT,
				ServerStatus.GetData(),ServerStatus.GetDataLen());
		}
		break;	
	};
}

void CSystemControlPort::SendMsg(const CIPAddress& Address,WORD Msg,LPCVOID pData,int DataLen)
{
	
	static char s_SendBuffer[65536];



	CEasyBuffer	SendBuffer(s_SendBuffer,65536);
	SMSG_HEADER MsgHeader;

	MsgHeader.MsgID=Msg;
	MsgHeader.Size=(WORD)(sizeof(SMSG_HEADER)+DataLen);	
	SendBuffer.PushBack(&MsgHeader,sizeof(SMSG_HEADER));

	if(DataLen)
		SendBuffer.PushBack(pData,DataLen);
	QueryUDPSend(Address,SendBuffer.GetBuffer(),SendBuffer.GetUsedSize());

	
}