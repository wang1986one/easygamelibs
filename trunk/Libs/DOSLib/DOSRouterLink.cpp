/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DOSRouterLink.cpp                                        */
/*      ��������:  2009��07��06��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#include "StdAfx.h"

IMPLEMENT_CLASS_INFO(CDOSRouterLink,CEasyNetLinkConnection);

CDOSRouterLink::CDOSRouterLink()
{	
	
}

CDOSRouterLink::~CDOSRouterLink(void)
{
}

void CDOSRouterLink::OnLinkStart()
{
	FUNCTION_BEGIN;
	PrintDOSLog(0xff0000,_T("�յ�·��(%d)������"),GetID());	
	FUNCTION_END;
}
void CDOSRouterLink::OnLinkEnd()
{
	FUNCTION_BEGIN;
	PrintDOSLog(0xff0000,_T("·��(%d)�����ӶϿ���"),GetID());
	OBJECT_ID SenderID;
	SenderID.RouterID=GetID();
	OBJECT_ID TargetID;
	TargetID.RouterID=0;
	TargetID.ObjectTypeID=BROAD_CAST_OBJECT_TYPE_ID;
	TargetID.GroupIndex=BROAD_CAST_GROUP_INDEX;
	TargetID.ObjectIndex=BROAD_CAST_OBJECT_INDEX;
	GetRouter()->RouterMessage(SenderID,TargetID,DSM_ROUTE_LINK_LOST,DOS_MESSAGE_FLAG_SYSTEM_MESSAGE,NULL,0);
	FUNCTION_END;
}

void CDOSRouterLink::OnData(const CEasyBuffer& DataBuffer)
{	
	FUNCTION_BEGIN;
	MSG_LEN_TYPE PacketLen=*((MSG_LEN_TYPE *)DataBuffer.GetBuffer());
	CDOSMessagePacket * pNewPacket=GetRouter()->GetServer()->NewMessagePacket(PacketLen);
	if(pNewPacket)
	{
		memcpy(pNewPacket->GetPacketBuffer(),DataBuffer.GetBuffer(),PacketLen);
		GetRouter()->RouterMessage(pNewPacket);
		GetRouter()->GetServer()->ReleaseMessagePacket(pNewPacket);
	}
	else
		PrintDOSLog(0xff0000,_T("������Ϣ�ڴ��ʧ�ܣ�"));
	FUNCTION_END;
}
