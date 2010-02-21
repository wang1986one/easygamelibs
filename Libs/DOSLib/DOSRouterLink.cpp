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
	PrintDOSLog(0xff0000,"收到路由(%d)的连接",GetID());	
	FUNCTION_END;
}
void CDOSRouterLink::OnLinkEnd()
{
	FUNCTION_BEGIN;
	PrintDOSLog(0xff0000,"路由(%d)的连接断开！",GetID());
	FUNCTION_END;
}

void CDOSRouterLink::OnData(const CEasyBuffer& DataBuffer)
{	
	FUNCTION_BEGIN;
	WORD PacketLen=*((WORD *)DataBuffer.GetBuffer());
	CDOSMessagePacket * pNewPacket=GetRouter()->GetServer()->NewMessagePacket(PacketLen);
	if(pNewPacket)
	{
		memcpy(pNewPacket->GetPacketBuffer(),DataBuffer.GetBuffer(),PacketLen);
		GetRouter()->RouterMessage(pNewPacket);
		GetRouter()->GetServer()->ReleaseMessagePacket(pNewPacket);
	}
	else
		PrintDOSLog(0xff0000,"分配消息内存块失败！");
	FUNCTION_END;
}
