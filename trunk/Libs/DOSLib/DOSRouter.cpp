/****************************************************************************/
/*                                                                          */
/*      文件名:    DOSRouter.cpp                                            */
/*      创建日期:  2009年10月23日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#include "StdAfx.h"

IMPLEMENT_CLASS_INFO(CDOSRouter,CEasyNetLinkManager);

CDOSRouter::CDOSRouter(void)
{
	FUNCTION_BEGIN;
	m_pServer=NULL;
	ResetStatData();
	FUNCTION_END;
}

CDOSRouter::~CDOSRouter(void)
{
}

BOOL CDOSRouter::Init(CDOSServer * pServer)
{
	FUNCTION_BEGIN;
	m_pServer=pServer;
	return Start();
	FUNCTION_END;
	return FALSE;
}

BOOL CDOSRouter::OnStart()
{
	FUNCTION_BEGIN;
	DOS_CONFIG& Config=m_pServer->GetConfig();

	m_MsgProcessLimit=Config.RouterMsgProcessLimit;

	if(m_MsgProcessLimit==0)
	{
		PrintDOSLog(0xff0000,"路由消息处理限制不能为0！");
		return FALSE;
	}

	if(!m_MsgQueue.Create(Config.MaxRouterSendMsgQueue))
	{
		PrintDOSLog(0xff0000,"创建%d大小的路由消息队列失败！",Config.MaxRouterSendMsgQueue);
		return FALSE;
	}

	xml_parser Parser;

	if(!Parser.parse_file(Config.RouterLinkConfigFileName,pug::parse_trim_attribute))
	{
		PrintDOSLog(0xff0000,"打开路由连接配置文件[%s]失败！",(LPCTSTR)Config.RouterLinkConfigFileName);
		return FALSE;
	}
	xml_node LinkConfig=Parser.document();
	if(!LinkConfig.moveto_child("RouterLink"))
	{
		PrintDOSLog(0xff0000,"路由连接配置文件[%s]格式错误！",(LPCTSTR)Config.RouterLinkConfigFileName);
		return FALSE;
	}

	ResetStatData();

	m_ThreadPerformanceCounter.Init(GetThreadHandle(),THREAD_CPU_COUNT_TIME);

	return CEasyNetLinkManager::Init(m_pServer,LinkConfig);
	FUNCTION_END;
	return FALSE;
}

void CDOSRouter::OnClose()
{	
	FUNCTION_BEGIN;
	CEasyNetLinkManager::Destory();
	m_MsgQueue.Destory();
	FUNCTION_END;
}

BOOL CDOSRouter::OnRun()
{
	FUNCTION_BEGIN;
	EXCEPTION_CATCH_START

	int ProcessCount=CEasyNetLinkManager::Update();	
	//处理消息发送
	ProcessCount+=DoMessageRoute(m_MsgProcessLimit);
	if(ProcessCount==0)
	{
		DoSleep(1);
	}

	m_ThreadPerformanceCounter.DoPerformanceCount();

	EXCEPTION_CATCH_END
	return TRUE;
	FUNCTION_END;
	return FALSE;
}

CEasyNetLinkConnection * CDOSRouter::OnCreateConnection(UINT ID)
{
	FUNCTION_BEGIN;
	return new CDOSRouterLink();
	FUNCTION_END;
	return NULL;
}

void CDOSRouter::OnLinkStart(CEasyNetLinkConnection * pConnection)
{

}

void CDOSRouter::OnLinkEnd(CEasyNetLinkConnection * pConnection)
{

}


BOOL CDOSRouter::RouterMessage(OBJECT_ID SenderID,OBJECT_ID ReceiverID,MSG_ID_TYPE MsgID,WORD MsgFlag,LPCVOID pData,UINT DataSize)
{
	FUNCTION_BEGIN;
	UINT PacketSize=CDOSMessagePacket::CaculatePacketLength(DataSize,1);
	CDOSMessagePacket * pPacket=((CDOSServer *)GetServer())->NewMessagePacket(PacketSize);
	if(pPacket==NULL)
		return FALSE;	
	pPacket->GetMessage().SetMsgID(MsgID);
	pPacket->GetMessage().SetSenderID(SenderID);
	pPacket->GetMessage().SetDataLength(DataSize);
	pPacket->GetMessage().SetMsgFlag(MsgFlag);
	if(pData)
		memcpy(pPacket->GetMessage().GetDataBuffer(),pData,DataSize);
	pPacket->SetTargetIDs(1,&ReceiverID);	
	pPacket->MakePacketLength();
	
	BOOL Ret=RouterMessage(pPacket);	
	((CDOSServer *)GetServer())->ReleaseMessagePacket(pPacket);
	return Ret;
	FUNCTION_END;
	return FALSE;
}

BOOL CDOSRouter::RouterMessage(CDOSMessagePacket * pPacket)
{
	FUNCTION_BEGIN;
	pPacket->MakePacketLength();
	((CDOSServer *)GetServer())->AddRefMessagePacket(pPacket);
	if(!m_MsgQueue.PushBack(pPacket))
	{
		((CDOSServer *)GetServer())->ReleaseMessagePacket(pPacket);
		PrintDOSLog(0xff0000,"将消息压入路由发送队列失败！");
		return FALSE;
	}
	
	return TRUE;
	FUNCTION_END;
	return FALSE;
}

UINT CDOSRouter::GetRouterID()
{
	FUNCTION_BEGIN;
	return ((CDOSServer *)GetServer())->GetConfig().RouterID;
	FUNCTION_END;
	return 0;
}

int CDOSRouter::DoMessageRoute(int ProcessPacketLimit)
{
	FUNCTION_BEGIN;
	CDOSMessagePacket * pPacket;

	int ProcessCount=0;
	while(m_MsgQueue.PopFront(pPacket))
	{
		//PrintDOSDebugLog(0,"路由了消息[%u]",pPacket->GetMessage().GetCmdID());

		AtomicInc(&m_RouteInMsgCount);
		AtomicAdd(&m_RouteInMsgFlow,pPacket->GetPacketLength());

		ID_LIST_COUNT_TYPE ReceiverIDCount=pPacket->GetTargetIDCount();
		OBJECT_ID * pReceiverIDs=pPacket->GetTargetIDs();
		if(ReceiverIDCount==0)
		{
			PrintDOSLog(0xff0000,"错误的消息格式！");
			continue;
		}
		if((ReceiverIDCount==1||IsSameRouter(pReceiverIDs,ReceiverIDCount))
			&&pReceiverIDs[0].RouterID!=BROAD_CAST_ROUTER_ID)
		{			
			//只有一个接收对象，或者接受对象在同一个路由,路由广播除外
			if(pReceiverIDs->RouterID==0||pReceiverIDs->RouterID==GetRouterID())
			{				
				//发给自己路由的消息，路由ID=0等同自己
				DispatchMessage(pPacket,pReceiverIDs,ReceiverIDCount);
			}
			else
			{
				CDOSRouterLink * pRouterLink=dynamic_cast<CDOSRouterLink *>(GetConnection(pReceiverIDs->RouterID));
				if(pRouterLink)
				{
					AtomicInc(&m_RouteOutMsgCount);
					AtomicAdd(&m_RouteOutMsgFlow,pPacket->GetPacketLength());
					pRouterLink->SendData(pPacket->GetPacketBuffer(),pPacket->GetPacketLength());
				}
				else
				{
					PrintDOSLog(0xff0000,"无法找到路由%u！",pReceiverIDs->RouterID);
				}
			}
		}
		else
		{
			//接收对象分布于多个路由


			ID_LIST_COUNT_TYPE GroupCount=0;
			OBJECT_ID * pReceiverIDGroup=pReceiverIDs;

			while(ReceiverIDCount)
			{
				GroupCount=GetGroupCount(pReceiverIDGroup,ReceiverIDCount);
				if(pReceiverIDGroup->RouterID==0||pReceiverIDGroup->RouterID==GetRouterID())
				{					
					//发给自己路由的消息，路由ID=0等同自己
					DispatchMessage(pPacket,pReceiverIDGroup,GroupCount);
				}
				else if(pReceiverIDGroup->RouterID==BROAD_CAST_ROUTER_ID)
				{
					//处理广播
					pPacket->SetTargetIDs(GroupCount,NULL);
					if(pReceiverIDs!=pReceiverIDGroup)
						memmove(pReceiverIDs,pReceiverIDGroup,GroupCount);
					for(UINT i=0;i<GroupCount;i++)
					{
						//路由ID设置为0避免被再次广播
						pReceiverIDs[i].RouterID=0;
					}
					pPacket->MakePacketLength();
					for(UINT i=0;i<GetConnectionCount();i++)
					{
						CDOSRouterLink * pRouterLink=dynamic_cast<CDOSRouterLink *>(GetConnectionByIndex(i));
						if(pRouterLink)
						{
							AtomicInc(&m_RouteOutMsgCount);
							AtomicAdd(&m_RouteOutMsgFlow,pPacket->GetPacketLength());
							pRouterLink->SendData(pPacket->GetPacketBuffer(),pPacket->GetPacketLength());
						}
					}					
					//在本路由广播
					DispatchMessage(pPacket,pReceiverIDs,GroupCount);
				}
				else
				{
					CDOSRouterLink * pRouterLink=dynamic_cast<CDOSRouterLink *>(GetConnection(pReceiverIDGroup->RouterID));
					if(pRouterLink)
					{
						

						pPacket->SetTargetIDs(GroupCount,NULL);
						if(pReceiverIDs!=pReceiverIDGroup)
							memmove(pReceiverIDs,pReceiverIDGroup,GroupCount);
						pPacket->MakePacketLength();

						AtomicInc(&m_RouteOutMsgCount);
						AtomicAdd(&m_RouteOutMsgFlow,pPacket->GetPacketLength());

						pRouterLink->SendData(pPacket->GetPacketBuffer(),pPacket->GetPacketLength());
					}
					else
					{
						PrintDOSLog(0xff0000,"无法找到路由%u！",pReceiverIDGroup->RouterID);
					}
				}
				pReceiverIDGroup+=GroupCount;
				ReceiverIDCount-=GroupCount;
			}

		}

		if(!((CDOSServer *)GetServer())->ReleaseMessagePacket(pPacket))
		{
			PrintDOSLog(0xff0000,"释放消息内存块失败！");
		}
		ProcessPacketLimit--;
		ProcessCount++;
		if(ProcessPacketLimit<=0)
			break;

	}
	return ProcessCount;
	FUNCTION_END;
	return 0;
}


BOOL CDOSRouter::IsSameRouter(OBJECT_ID * pReceiverIDs,int Count)
{
	FUNCTION_BEGIN;
	ROUTE_ID_TYPE RouterID=pReceiverIDs->RouterID;
	for(int i=1;i<Count;i++)
	{
		if(pReceiverIDs[i].RouterID!=RouterID)
			return FALSE;
	}
	return TRUE;
	FUNCTION_END;
	return FALSE;
}

int CDOSRouter::GetGroupCount(OBJECT_ID * pReceiverIDs,int Count)
{
	FUNCTION_BEGIN;
	ROUTE_ID_TYPE RouterID=pReceiverIDs->RouterID;
	for(int i=1;i<Count;i++)
	{
		if(pReceiverIDs[i].RouterID!=RouterID)
			return i-1;
	}
	return Count;
	FUNCTION_END;
	return 0;
}


BOOL CDOSRouter::DispatchMessage(CDOSMessagePacket * pPacket,OBJECT_ID * pReceiverIDs,int Count)
{
	FUNCTION_BEGIN;
	UINT RouterID=GetRouterID();
	for(int i=0;i<Count;i++)
	{
		if(pReceiverIDs[i].ObjectTypeID==DOT_PROXY_OBJECT)
		{
			if(pReceiverIDs[i].ObjectIndex==0||
				(pReceiverIDs[i].ObjectIndex==BROAD_CAST_OBJECT_INDEX&&
				(pPacket->GetMessage().GetMsgFlag()&DOS_MESSAGE_FLAG_SYSTEM_MESSAGE)))
			{
				AtomicInc(&m_RouteOutMsgCount);
				AtomicAdd(&m_RouteOutMsgFlow,pPacket->GetMessage().GetMsgLength());
				((CDOSServer *)GetServer())->GetObjectProxy()->PushMessage(pPacket);
			}
			if((pReceiverIDs[i].ObjectIndex!=0&&pReceiverIDs[i].ObjectIndex!=BROAD_CAST_OBJECT_INDEX)||
				(pReceiverIDs[i].ObjectIndex==BROAD_CAST_OBJECT_INDEX&&
				(pPacket->GetMessage().GetMsgFlag()&DOS_MESSAGE_FLAG_SYSTEM_MESSAGE)))
			{
				CDOSProxyConnection * pProxy=((CDOSServer *)GetServer())->GetObjectProxy()->GetConnection(pReceiverIDs[i].ObjectIndex);
				if(pProxy)
				{
					AtomicInc(&m_RouteOutMsgCount);
					AtomicAdd(&m_RouteOutMsgFlow,pPacket->GetPacketLength());
					pProxy->PushMessage(pPacket);
				}
			}			
		}
		else
		{
			pReceiverIDs[i].RouterID=RouterID;
			if(((CDOSServer *)GetServer())->GetObjectManager()->PushMessage(pReceiverIDs[i],pPacket))
			{
				AtomicInc(&m_RouteOutMsgCount);
				AtomicAdd(&m_RouteOutMsgFlow,pPacket->GetMessage().GetMsgLength());
			}
			else
			{
				PrintDOSLog(0,"CDOSRouter::DispatchMessage:将消息递送到对象[%llX]失败",
					pReceiverIDs[i]);
			}
		}
	}
	return TRUE;
	FUNCTION_END;
	return FALSE;
}

