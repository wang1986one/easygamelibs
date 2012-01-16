/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DOSObjectProxyService.cpp                                */
/*      ��������:  2009��10��23��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#include "StdAfx.h"

IMPLEMENT_CLASS_INFO_STATIC(CDOSObjectProxyService,CNetService);

CDOSObjectProxyService::CDOSObjectProxyService(void):CNetService(),CEasyThread()
{
	
}

CDOSObjectProxyService::~CDOSObjectProxyService(void)
{
}

BOOL CDOSObjectProxyService::Init(CDOSServer * pServer)
{
	FUNCTION_BEGIN;
	SetServer(pServer);	
	return Start();
	FUNCTION_END;
	return FALSE;
}

BOOL CDOSObjectProxyService::OnStart()
{
	FUNCTION_BEGIN;
	if(!m_ConnectionPool.Create(((CDOSServer *)GetServer())->GetConfig().MaxProxyConnection))
	{
		PrintDOSLog(0xff0000,"����%u��С�����ӳ�ʧ�ܣ�",
			((CDOSServer *)GetServer())->GetConfig().MaxProxyConnection);
		return FALSE;
	}
	if(!m_MsgQueue.Create(((CDOSServer *)GetServer())->GetConfig().MaxProxyMsgQueue))
	{
		PrintDOSLog(0xff0000,"����%u��С����Ϣ����ʧ�ܣ�",
			((CDOSServer *)GetServer())->GetConfig().MaxProxyMsgQueue);
		return FALSE;
	}
	
	if(!m_MessageMap.Create(((CDOSServer *)GetServer())->GetConfig().MaxProxyGlobalMsgMap))
	{
		PrintDOSLog(0xff0000,"����%u��С����Ϣӳ���ʧ�ܣ�",
			((CDOSServer *)GetServer())->GetConfig().MaxProxyGlobalMsgMap);
		return FALSE;
	}

	if(((CDOSServer *)GetServer())->GetConfig().ProxyMsgMinCompressSize)
	{
		if (lzo_init() != LZO_E_OK)
		{
			PrintDOSLog(0xff0000,"�����������Ϣѹ��ʧ��");
			return FALSE;
		}
		PrintDOSDebugLog(0xff0000,"�����������Ϣѹ��");
	}

	if(!Create(IPPROTO_TCP,
		DEFAULT_SERVER_ACCEPT_QUEUE,
		DEFAULT_SERVER_RECV_DATA_QUEUE,
		((CDOSServer *)GetServer())->GetConfig().ProxySendBufferSize,
		DEFAULT_PARALLEL_ACCEPT,
		DEFAULT_PARALLEL_RECV,
		false))
	{
		PrintDOSLog(0xffff,"������񴴽�ʧ�ܣ�");
		return FALSE;
	}

	if(!StartListen(((CDOSServer *)GetServer())->GetConfig().ObjectProxyServiceListenAddress))
	{
		PrintDOSLog(0xffff,"�������������(%s:%u)ʧ�ܣ�",
			((CDOSServer *)GetServer())->GetConfig().ObjectProxyServiceListenAddress.GetIPString(),
			((CDOSServer *)GetServer())->GetConfig().ObjectProxyServiceListenAddress.GetPort());
		return FALSE;
	}
	PrintDOSLog(0xffff,"�������������(%s:%u)��",
		((CDOSServer *)GetServer())->GetConfig().ObjectProxyServiceListenAddress.GetIPString(),
		((CDOSServer *)GetServer())->GetConfig().ObjectProxyServiceListenAddress.GetPort());

	return TRUE;
	FUNCTION_END;
	return FALSE;
}

void CDOSObjectProxyService::OnClose()
{
	FUNCTION_BEGIN;
	LPVOID Pos=m_ConnectionPool.GetFirstObjectPos();
	while(Pos)
	{
		CDOSProxyConnection * pConnection=m_ConnectionPool.GetNext(Pos);

		pConnection->Destory();

	}
	m_ConnectionPool.Clear();
	FUNCTION_END;

}

BOOL CDOSObjectProxyService::OnRun()
{
	FUNCTION_BEGIN;
	EXCEPTION_CATCH_START

	int ProcessCount=CNetService::Update();
	LPVOID Pos=m_ConnectionPool.GetFirstObjectPos();
	while(Pos)
	{
		CDOSProxyConnection * pConnection=m_ConnectionPool.GetNext(Pos);
		if(pConnection->IsConnected())
			ProcessCount+=pConnection->Update();
		else
			m_ConnectionPool.DeleteObject(pConnection->GetID());
	}

	ProcessCount+=DoMessageProcess();

	if(ProcessCount==0)
	{
		DoSleep(1);
	}

	EXCEPTION_CATCH_END

	return TRUE;
	FUNCTION_END;
	return FALSE;
}

CBaseTCPConnection * CDOSObjectProxyService::CreateConnection(CIPAddress& RemoteAddress)
{
	FUNCTION_BEGIN;
	CDOSProxyConnection * pConnection=NULL;
	UINT ID=m_ConnectionPool.NewObject(&pConnection);
	if(pConnection)
	{
		if(!pConnection->Init(this,ID))
		{
			m_ConnectionPool.DeleteObject(ID);
		}		
		return pConnection;
	}
	FUNCTION_END;
	return NULL;
}

BOOL CDOSObjectProxyService::DeleteConnection(CBaseTCPConnection * pConnection)
{
	FUNCTION_BEGIN;
	pConnection->Destory();
	if(m_ConnectionPool.DeleteObject(pConnection->GetID()))
	{
		return TRUE;
	}
	FUNCTION_END;
	return FALSE;
}

BOOL CDOSObjectProxyService::PushMessage(CDOSMessagePacket * pPacket)
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

OBJECT_ID CDOSObjectProxyService::GetGlobalMsgMapObjectID(MSG_ID_TYPE MsgID)
{
	FUNCTION_BEGIN;
	OBJECT_ID * pObjectID=m_MessageMap.Find(MsgID);
	if(pObjectID)
	{
		return *pObjectID;
	}
	FUNCTION_END;
	return 0;
}

int CDOSObjectProxyService::DoMessageProcess(int ProcessPacketLimit)
{
	FUNCTION_BEGIN;
	int ProcessCount=0;
	CDOSMessagePacket * pPacket;
	while(m_MsgQueue.PopFront(pPacket))
	{
		//PrintDOSDebugLog(0,"��������Ϣ[%u]",pPacket->GetMessage().GetMsgID());

		OnMsg(&(pPacket->GetMessage()));
		if(!((CDOSServer *)GetServer())->ReleaseMessagePacket(pPacket))
		{
			PrintDOSLog(0xff0000,"�ͷ���Ϣ�ڴ��ʧ�ܣ�");
		}

		ProcessCount++;
		if(ProcessCount>=ProcessPacketLimit)
			break;
	}
	return ProcessCount;
	FUNCTION_END;
	return 0;
}



void CDOSObjectProxyService::OnMsg(CDOSMessage * pMessage)
{
	FUNCTION_BEGIN;
	switch(pMessage->GetMsgID())
	{		
	case DSM_PROXY_REGISTER_GLOBAL_MSG_MAP:
		if(pMessage->GetDataLength()>=sizeof(MSG_ID_TYPE))
		{
			int Count=(pMessage->GetDataLength())/sizeof(MSG_ID_TYPE);
			MSG_ID_TYPE * pMsgIDs=(MSG_ID_TYPE *)(pMessage->GetDataBuffer());
			for(int i=0;i<Count;i++)
			{
				RegisterGlobalMsgMap(pMsgIDs[i],pMessage->GetSenderID());
			}
		}
		break;
	case DSM_PROXY_UNREGISTER_GLOBAL_MSG_MAP:
		if(pMessage->GetDataLength()>=sizeof(MSG_ID_TYPE))
		{
			int Count=(pMessage->GetDataLength())/sizeof(MSG_ID_TYPE);
			MSG_ID_TYPE * pMsgIDs=(MSG_ID_TYPE *)(pMessage->GetDataBuffer());
			for(int i=0;i<Count;i++)
			{
				UnregisterGlobalMsgMap(pMsgIDs[i],pMessage->GetSenderID());
			}		
		}
		break;
	case DSM_ROUTE_LINK_LOST:
		ClearMsgMapByRouterID(pMessage->GetSenderID().RouterID);
		break;
	}
	FUNCTION_END;
}

BOOL CDOSObjectProxyService::RegisterGlobalMsgMap(MSG_ID_TYPE MsgID,OBJECT_ID ObjectID)
{
	FUNCTION_BEGIN;
	PrintDOSDebugLog(0xff0000,"0x%llXע����ȫ�ִ�����Ϣӳ��[%u]��",ObjectID.ID,MsgID);
	return m_MessageMap.Insert(MsgID,ObjectID);
	FUNCTION_END;
	return FALSE;
}
BOOL CDOSObjectProxyService::UnregisterGlobalMsgMap(MSG_ID_TYPE MsgID,OBJECT_ID ObjectID)
{
	FUNCTION_BEGIN;
	PrintDOSDebugLog(0xff0000,"0x%llXע����ȫ�ִ�����Ϣӳ��[%u]��",ObjectID.ID,MsgID);
	return m_MessageMap.Delete(MsgID);
	FUNCTION_END;
	return FALSE;
}

void CDOSObjectProxyService::ClearMsgMapByRouterID(UINT RouterID)
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
	FUNCTION_END;
}