#include "StdAfx.h"

CIOCPListenThread::CIOCPListenThread(void)
{
	m_pServer=NULL;
	m_ListenSocket=INVALID_SOCKET;
}

CIOCPListenThread::~CIOCPListenThread(void)
{
}

bool CIOCPListenThread::Init(CNetService * pService,SOCKET ListenSocket)
{
	m_pServer=pService;
	m_ListenSocket=ListenSocket;
	return true;
}

BOOL CIOCPListenThread::OnStart()
{
	PrintNetLog(0xffffffff,"ListenThread����");
	return TRUE;
}

BOOL CIOCPListenThread::OnRun()
{
	CIPAddress Address;
	int AddressLen=sizeof(sockaddr_in);	
	SOCKET AcceptSocket=WSAAccept(m_ListenSocket,(sockaddr*)&(Address.GetSockAddr()),&AddressLen,NULL,0);
	if(AcceptSocket==INVALID_SOCKET)
	{
		int ErrorCode=GetLastError();
		PrintNetLog(0xffffffff,"Acceptʧ��(%u)",ErrorCode);
		if(ErrorCode!=WSAECONNRESET&&ErrorCode!=WSAEINTR&&
			ErrorCode!=WSAEINPROGRESS&&ErrorCode!=WSAEMFILE&&
			ErrorCode!=WSAENOBUFS&&ErrorCode!=WSAEWOULDBLOCK&&
			ErrorCode!=WSAECONNREFUSED&&ErrorCode!=WSAEACCES&&
			ErrorCode!=WSATRY_AGAIN)
		{
			COverLappedObject * pOverLappedObject=m_pServer->GetServer()->CreateOverLappedObject();
			if(pOverLappedObject)
			{
				pOverLappedObject->SetType(IO_ACCEPT2);				
				m_pServer->OnIOCPEvent(IOE_ERROR,pOverLappedObject);
				return FALSE;
			}
			else
			{
				PrintNetLog(0xffffffff,"CIOCPListenThread����Accept��OverLappedObjectʧ�ܣ�");			
			}
		}
	}
	else
	{
		//if(setsockopt(AcceptSocket,
		//	SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, 
		//	(char *)&m_ListenSocket, sizeof(m_ListenSocket))!=SOCKET_ERROR)
		//{
			COverLappedObject * pOverLappedObject=m_pServer->GetServer()->CreateOverLappedObject();
			if(pOverLappedObject)
			{
				pOverLappedObject->SetType(IO_ACCEPT2);
				pOverLappedObject->SetAcceptSocket(AcceptSocket);
				m_pServer->OnIOCPEvent(IOE_PACKET,pOverLappedObject);
				return TRUE;
			}
			else
			{
				PrintNetLog(0xffffffff,"CIOCPListenThread����Accept��OverLappedObjectʧ�ܣ�");			
			}
		//}				
		//else
		//	PrintNetLog(0xffffffff,"CIOCPListenThread����AcceptScoket״̬ʧ�ܣ�",GetID());

		closesocket(AcceptSocket);
	}
	return TRUE;
}

void CIOCPListenThread::OnTerminate()
{
	PrintNetLog(0xffffffff,"ListenThread�ر�");
}