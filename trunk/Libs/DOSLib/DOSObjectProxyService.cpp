/****************************************************************************/
/*                                                                          */
/*      文件名:    DOSObjectProxyService.cpp                                */
/*      创建日期:  2009年10月23日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
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

	m_ThreadPerformanceCounter.Init(GetThreadHandle(),THREAD_CPU_COUNT_TIME);

	if(!m_ConnectionPool.Create(((CDOSServer *)GetServer())->GetConfig().MaxProxyConnection))
	{
		PrintDOSLog(0xff0000,_T("创建%u大小的连接池失败！"),
			((CDOSServer *)GetServer())->GetConfig().MaxProxyConnection);
		return FALSE;
	}
	if(!m_MsgQueue.Create(((CDOSServer *)GetServer())->GetConfig().MaxProxyMsgQueue))
	{
		PrintDOSLog(0xff0000,_T("创建%u大小的消息队列失败！"),
			((CDOSServer *)GetServer())->GetConfig().MaxProxyMsgQueue);
		return FALSE;
	}
	
	if(!m_MessageMap.Create(((CDOSServer *)GetServer())->GetConfig().MaxProxyGlobalMsgMap))
	{
		PrintDOSLog(0xff0000,_T("创建%u大小的消息映射表失败！"),
			((CDOSServer *)GetServer())->GetConfig().MaxProxyGlobalMsgMap);
		return FALSE;
	}

	if(((CDOSServer *)GetServer())->GetConfig().ProxyMsgMinCompressSize)
	{
		if (lzo_init() != LZO_E_OK)
		{
			PrintDOSLog(0xff0000,_T("代理服务开启消息压缩失败"));
			return FALSE;
		}
		PrintDOSDebugLog(0xff0000,_T("代理服务开启消息压缩"));
	}

	if(!Create(IPPROTO_TCP,
		DEFAULT_SERVER_ACCEPT_QUEUE,
		DEFAULT_SERVER_RECV_DATA_QUEUE,
		((CDOSServer *)GetServer())->GetConfig().ProxySendBufferSize,
		DEFAULT_PARALLEL_ACCEPT,
		DEFAULT_PARALLEL_RECV,
		false))
	{
		PrintDOSLog(0xffff,_T("代理服务创建失败！"));
		return FALSE;
	}

	if(!StartListen(((CDOSServer *)GetServer())->GetConfig().ObjectProxyServiceListenAddress))
	{
		PrintDOSLog(0xffff,_T("代理服务侦听于(%s:%u)失败！"),
			((CDOSServer *)GetServer())->GetConfig().ObjectProxyServiceListenAddress.GetIPString(),
			((CDOSServer *)GetServer())->GetConfig().ObjectProxyServiceListenAddress.GetPort());
		return FALSE;
	}
	PrintDOSLog(0xffff,_T("代理服务侦听于(%s:%u)！"),
		((CDOSServer *)GetServer())->GetConfig().ObjectProxyServiceListenAddress.GetIPString(),
		((CDOSServer *)GetServer())->GetConfig().ObjectProxyServiceListenAddress.GetPort());

	PrintDOSLog(0xff0000,_T("对象代理线程[%u]已启动"),GetThreadID());
	return TRUE;
	FUNCTION_END;
	return FALSE;
}

void CDOSObjectProxyService::OnClose()
{
	FUNCTION_BEGIN;
	CAutoLock Lock(m_EasyCriticalSection);

	CDOSMessagePacket * pPacket;
	while(m_MsgQueue.PopFront(pPacket))
	{		
		if(!((CDOSServer *)GetServer())->ReleaseMessagePacket(pPacket))
		{
			PrintDOSLog(0xff0000,_T("CDOSObjectProxyService::OnClose:释放消息内存块失败！"));
		}
	}

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

	m_ThreadPerformanceCounter.DoPerformanceCount();

	int ProcessCount=CNetService::Update();
	LPVOID Pos=m_ConnectionPool.GetFirstObjectPos();
	while(Pos)
	{
		CDOSProxyConnection * pConnection=m_ConnectionPool.GetNext(Pos);
		if(pConnection->IsConnected())
			ProcessCount+=pConnection->Update();
		else
			DeleteConnection(pConnection);
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
	CAutoLock Lock(m_EasyCriticalSection);

	((CDOSServer *)GetServer())->AddRefMessagePacket(pPacket);
	pPacket->SetAllocTime(4);
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

BOOL CDOSObjectProxyService::PushBroadcastMessage(CDOSMessagePacket * pPacket)
{
	FUNCTION_BEGIN;
	LPVOID Pos=m_ConnectionPool.GetFirstObjectPos();
	while(Pos)
	{
		CDOSProxyConnection * pConnection=m_ConnectionPool.GetNext(Pos);
		if(pConnection)
		{
			pConnection->PushMessage(pPacket);
		}
	}
	return TRUE;
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
		//PrintDOSDebugLog(0,_T("发送了消息[%u]"),pPacket->GetMessage().GetMsgID());

		OnMsg(&(pPacket->GetMessage()));
		if(!((CDOSServer *)GetServer())->ReleaseMessagePacket(pPacket))
		{
			PrintDOSLog(0xff0000,_T("释放消息内存块失败！"));
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
	PrintDOSDebugLog(0xff0000,_T("0x%llX注册了全局代理消息映射[0x%X]！"),ObjectID.ID,MsgID);
	return m_MessageMap.Insert(MsgID,ObjectID);
	FUNCTION_END;
	return FALSE;
}
BOOL CDOSObjectProxyService::UnregisterGlobalMsgMap(MSG_ID_TYPE MsgID,OBJECT_ID ObjectID)
{
	FUNCTION_BEGIN;
	PrintDOSDebugLog(0xff0000,_T("0x%llX注销了全局代理消息映射[0x%X]！"),ObjectID.ID,MsgID);
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