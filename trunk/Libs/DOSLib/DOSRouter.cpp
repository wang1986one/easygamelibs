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
		PrintDOSLog(0xff0000,"����%d��С��·����Ϣ����ʧ�ܣ�",Config.MaxRouterSendMsgQueue);
		return FALSE;
	}

	xml_parser Parser;

	if(!Parser.parse_file(Config.RouterLinkConfigFileName,pug::parse_trim_attribute))
	{
		PrintDOSLog(0xff0000,"��·�����������ļ�[%s]ʧ�ܣ�",(LPCTSTR)Config.RouterLinkConfigFileName);
		return FALSE;
	}
	xml_node LinkConfig=Parser.document();
	if(!LinkConfig.moveto_child("RouterLink"))
	{
		PrintDOSLog(0xff0000,"·�����������ļ�[%s]��ʽ����",(LPCTSTR)Config.RouterLinkConfigFileName);
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
	//������Ϣ����
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
		PrintDOSLog(0xff0000,"����Ϣѹ��·�ɷ��Ͷ���ʧ�ܣ�");
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
		PrintDOSDebugLog(0,"·������Ϣ[%u]",pPacket->GetMessage().GetCmdID());
		

		WORD ReceiverIDCount=pPacket->GetTargetIDCount();
		OBJECT_ID * pReceiverIDs=pPacket->GetTargetIDs();
		if(ReceiverIDCount==0)
		{
			PrintDOSLog(0xff0000,"�������Ϣ��ʽ��");
			continue;
		}
		if((ReceiverIDCount==1||IsSameRouter(pReceiverIDs,ReceiverIDCount))
			&&pReceiverIDs[0].RouterID!=0xFFFF)
		{
			//ֻ��һ�����ն��󣬻��߽��ܶ�����ͬһ��·��,·�ɹ㲥����
			if(pReceiverIDs->RouterID==0||pReceiverIDs->RouterID==GetRouterID())
			{
				//�����Լ�·�ɵ���Ϣ��·��ID=0��ͬ�Լ�
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
					PrintDOSLog(0xff0000,"�޷��ҵ�·��%u��",pReceiverIDs->RouterID);
				}
			}
		}
		else
		{
			//���ն���ֲ��ڶ��·��


			WORD GroupCount=0;
			OBJECT_ID * pReceiverIDGroup=pReceiverIDs;

			while(ReceiverIDCount)
			{
				GroupCount=GetGroupCount(pReceiverIDGroup,ReceiverIDCount);
				if(pReceiverIDGroup->RouterID==0||pReceiverIDGroup->RouterID==GetRouterID())
				{
					//�����Լ�·�ɵ���Ϣ��·��ID=0��ͬ�Լ�
					DispatchMessage(pPacket,pReceiverIDGroup,GroupCount);
				}
				else if(pReceiverIDGroup->RouterID==BROAD_CAST_ROUTER_ID)
				{
					//����㲥
					pPacket->SetTargetIDs(GroupCount,NULL);
					if(pReceiverIDs!=pReceiverIDGroup)
						memmove(pReceiverIDs,pReceiverIDGroup,GroupCount);
					for(UINT i=0;i<GroupCount;i++)
					{
						//·��ID����Ϊ0���ⱻ�ٴι㲥
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
					//�ڱ�·�ɹ㲥
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
						PrintDOSLog(0xff0000,"�޷��ҵ�·��%u��",pReceiverIDGroup->RouterID);
					}
				}
				pReceiverIDGroup+=GroupCount;
				ReceiverIDCount-=GroupCount;
			}

		}

		if(!((CDOSServer *)GetServer())->ReleaseMessagePacket(pPacket))
		{
			PrintDOSLog(0xff0000,"�ͷ���Ϣ�ڴ��ʧ�ܣ�");
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
				PrintDOSLog(0,"CDOSRouter::DispatchMessage:����Ϣ���͵�����[%llX]ʧ��",
					pReceiverIDs[i]);
			}
		}
	}
	return TRUE;
	FUNCTION_END;
	return FALSE;
}

