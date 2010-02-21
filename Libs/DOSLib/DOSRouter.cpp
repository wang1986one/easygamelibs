#include "StdAfx.h"

IMPLEMENT_CLASS_INFO(CDOSRouter,CEasyNetLinkManager);

CDOSRouter::CDOSRouter(void)
{
	FUNCTION_BEGIN;
	m_pServer=NULL;
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
	ProcessCount+=DoMessageRoute();
	if(ProcessCount==0)
	{
		DoSleep(1);
	}

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


BOOL CDOSRouter::RouterMessage(OBJECT_ID SenderID,OBJECT_ID ReceiverID,WORD MsgID,LPCVOID pData,UINT DataSize)
{
	FUNCTION_BEGIN;
	UINT PacketSize=CDOSMessagePacket::CaculatePacketLength(DataSize,1);
	CDOSMessagePacket * pPacket=((CDOSServer *)GetServer())->NewMessagePacket(PacketSize);
	if(pPacket==NULL)
		return FALSE;	
	pPacket->GetMessage().SetCmdID(MsgID);
	pPacket->GetMessage().SetSenderID(SenderID);
	pPacket->GetMessage().SetDataLength(DataSize);
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
		PrintDOSDebugLog(0,"路由了消息[%u]",pPacket->GetMessage().GetCmdID());
		

		WORD ReceiverIDCount=pPacket->GetTargetIDCount();
		OBJECT_ID * pReceiverIDs=pPacket->GetTargetIDs();
		if(ReceiverIDCount==0)
		{
			PrintDOSLog(0xff0000,"错误的消息格式！");
			continue;
		}
		if((ReceiverIDCount==1||IsSameRouter(pReceiverIDs,ReceiverIDCount))
			&&pReceiverIDs[0].RouterID!=0xFFFF)
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


			WORD GroupCount=0;
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
	WORD RouterID=pReceiverIDs->RouterID;
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
	WORD RouterID=pReceiverIDs->RouterID;
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
			if(pReceiverIDs[i].ObjectIndex==0)
			{
				((CDOSServer *)GetServer())->GetObjectProxy()->PushMessage(pPacket);
			}
			else
			{
				CDOSProxyConnection * pProxy=((CDOSServer *)GetServer())->GetObjectProxy()->GetConnection(pReceiverIDs[i].ObjectIndex);
				if(pProxy)
					pProxy->PushMessage(pPacket);
			}			
		}
		else
		{
			pReceiverIDs[i].RouterID=RouterID;
			if(!((CDOSServer *)GetServer())->GetObjectManager()->PushMessage(pReceiverIDs[i],pPacket))
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

