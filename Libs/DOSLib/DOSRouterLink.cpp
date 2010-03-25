/****************************************************************************/
/*                                                                          */
/*      文件名:    DOSRouterLink.cpp                                        */
/*      创建日期:  2009年07月06日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
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
