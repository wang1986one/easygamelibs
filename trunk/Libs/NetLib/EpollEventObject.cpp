#include "StdAfx.h"


CEpollEventObject::CEpollEventObject(void)
{
	m_ID=0;
	m_DataBuff.Create(MAX_DATA_PACKET_SIZE);
	Create(NULL);
}

CEpollEventObject::~CEpollEventObject(void)
{
	Destory();
}

BOOL CEpollEventObject::Create(CNetServer * pCreator)
{
	m_pCreator=pCreator;
	m_DataBuff.SetUsedSize(0);
	m_TypeID=IO_NOTSET;
	m_AcceptSocket=INVALID_SOCKET;
	m_Address.SetIP((DWORD)0);
	m_Address.SetPort(0);
	m_AddressLen=sizeof(sockaddr_in);
	m_ParentID=0;
	m_ErrorCode=0;
	m_EventID=0;
	return TRUE;
}
void CEpollEventObject::Destory()
{
	m_DataBuff.SetUsedSize(0);
	m_TypeID=IO_NOTSET;
	m_AcceptSocket=INVALID_SOCKET;
	m_Address.SetIP((DWORD)0);
	m_Address.SetPort(0);
	m_AddressLen=sizeof(sockaddr_in);
	m_ParentID=0;
	m_ErrorCode=0;
	m_EventID=0;
}

void CEpollEventObject::Release()
{
	m_pCreator->DeleteEventObject(this);
}
