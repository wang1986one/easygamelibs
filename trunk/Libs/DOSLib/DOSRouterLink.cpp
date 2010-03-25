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
	PrintDOSLog(0xff0000,"�յ�·��(%d)������",GetID());	
	FUNCTION_END;
}
void CDOSRouterLink::OnLinkEnd()
{
	FUNCTION_BEGIN;
	PrintDOSLog(0xff0000,"·��(%d)�����ӶϿ���",GetID());
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
		PrintDOSLog(0xff0000,"������Ϣ�ڴ��ʧ�ܣ�");
	FUNCTION_END;
}
