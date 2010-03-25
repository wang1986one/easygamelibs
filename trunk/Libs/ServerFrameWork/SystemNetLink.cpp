/****************************************************************************/
/*                                                                          */
/*      �ļ���:    SystemNetLink.cpp                                        */
/*      ��������:  2009��07��06��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#include "StdAfx.h"

CSystemNetLink::CSystemNetLink(IBaseServer * pServer)
{
	FUNCTION_BEGIN;
	m_pServer=pServer;
	m_IsLinkLog=FALSE;
	FUNCTION_END;
}

CSystemNetLink::~CSystemNetLink(void)
{
}


void CSystemNetLink::SendMsg(WORD Msg,LPCVOID pData,int DataLen)
{
	FUNCTION_BEGIN;
	static char s_SendBuffer[65536];



	CEasyBuffer	SendBuffer(s_SendBuffer,65536);
	SMSG_HEADER MsgHeader;

	MsgHeader.MsgID=Msg;
	MsgHeader.Size=(WORD)(sizeof(SMSG_HEADER)+DataLen);
	SendBuffer.PushBack(&MsgHeader,sizeof(SMSG_HEADER));

	if(DataLen)
		SendBuffer.PushBack(pData,DataLen);
	SendData(SendBuffer.GetBuffer(),SendBuffer.GetUsedSize());


	FUNCTION_END;
}


void CSystemNetLink::OnLinkStart()
{
	FUNCTION_BEGIN;
	CClassifiedID ID=GetID();
	LogDebug("������[%s][%s:%u]������",
		ID.ToStr(),
		GetRemoteAddress().GetIPString(),
		GetRemoteAddress().GetPort());
	FUNCTION_END;
}

void CSystemNetLink::OnLinkEnd()
{
	FUNCTION_BEGIN;
	CClassifiedID ID=GetID();
	LogDebug("�Ͽ���[%s]������",ID.ToStr());
	FUNCTION_END;
}

//��������
void CSystemNetLink::OnData(const CEasyBuffer& DataBuffer)
{
	FUNCTION_BEGIN;


	SMSG * pMsg=(SMSG *)DataBuffer.GetBuffer();
	UINT DataSize=pMsg->Header.Size-sizeof(SMSG_HEADER);

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
			SendMsg(SC_MSG_QUERY_SERVER_STATUS_RESULT,
				ServerStatus.GetData(),ServerStatus.GetDataLen());
		}
		break;
	case SC_MSG_QUERY_ALL_SERVER_STATUS:
		{
			SendMsg(SC_MSG_QUERY_SERVER_STATUS_RESULT,
				m_pServer->GetAllServerStatus().GetData(),
				m_pServer->GetAllServerStatus().GetDataLen());
		}
		break;
	case SC_MSG_SET_SERVER_STATUS:
		{
		}
		break;
	case SC_MSG_LINK_LOG:
		m_IsLinkLog=TRUE;
		Log("�ѿ�������Log");
		break;
	case SC_MSG_UNLINK_LOG:
		Log("�ѹرս���Log");
		m_IsLinkLog=FALSE;
		break;
	case SC_MSG_EXEC_COMMAND:
		{
			CSmartStruct CommandInfo(pMsg->Data,DataSize,false);
			LPCTSTR szCommand=CommandInfo.GetMember(SC_SST_EC_COMMAND_STR);
			m_pServer->ExecCommand(szCommand);
		}
		break;
	}

	FUNCTION_END;
}
