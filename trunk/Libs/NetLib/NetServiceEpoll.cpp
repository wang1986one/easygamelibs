/****************************************************************************/
/*                                                                          */
/*      �ļ���:    NetServiceEpoll.cpp                                      */
/*      ��������:  2010��02��09��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#include "StdAfx.h"

IMPLEMENT_CLASS_INFO_STATIC(CNetService,CBaseService);

CNetService::CNetService()
{
	m_WantClose=FALSE;
	m_pServer=NULL;
	m_CurProtocol=IPPROTO_TCP;
	m_AcceptQueueSize=0;
	m_RecvQueueSize=0;
	m_SendQueueSize=0;
	m_ParallelAcceptCount=0;
	m_ParallelRecvCount=0;
	m_pEpollEventRouter=NULL;	
	
}

CNetService::~CNetService(void)
{
	Destory();
}

BOOL CNetService::OnEpollEvent(UINT EventID)
{		
	if(IsWorking())
	{
		if(EventID&(EPOLLERR|EPOLLHUP))
		{
			PrintNetLog(0xffffffff,"CNetService::Epoll��������%d��",errno);
			QueryClose();
			return TRUE;
		}
		if(EventID&EPOLLIN)
		{
			if(m_CurProtocol==IPPROTO_UDP)
				DoUDPRecv();
			else
				DoAcceptSocket();
		}
		if(EventID&EPOLLOUT)
		{
			DoUDPSend();
		}			
	}
	else
		PrintNetLog(0xffffffff,"Serviceδ���ã��¼������ԣ�");

	return FALSE;
}

BOOL CNetService::Create(int Protocol,int AcceptQueueSize,int RecvQueueSize,int SendQueueSize,int ParallelAcceptCount,int ParallelRecvCount,bool IsUseListenThread)
{	
	if(GetServer()==NULL)
		return FALSE;

	Destory();

	if(m_pEpollEventRouter==NULL)
	{
		m_pEpollEventRouter=GetServer()->CreateEventRouter();
		m_pEpollEventRouter->Init(this);
	}
	m_AcceptQueueSize=AcceptQueueSize;
	m_RecvQueueSize=RecvQueueSize;
	m_SendQueueSize=SendQueueSize;
	m_ParallelAcceptCount=ParallelAcceptCount;
	m_ParallelRecvCount=ParallelRecvCount;
	if(Protocol==IPPROTO_UDP)
	{
		m_Socket.MakeSocket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);	
		m_RecvQueue.Create(m_RecvQueueSize);
		m_SendQueue.Create(m_SendQueueSize);
	}
	else
	{
		m_Socket.MakeSocket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
		m_RecvQueue.Create(m_AcceptQueueSize);
		
	}
	m_WantClose=FALSE;
	m_CurProtocol=Protocol;

	m_Socket.EnableBlocking(FALSE);
	return TRUE;
}

void CNetService::Destory()
{

	if(m_pEpollEventRouter)
	{
		m_pEpollEventRouter->SetEventHander(NULL);
		GetServer()->DeleteEventRouter(m_pEpollEventRouter);
		m_pEpollEventRouter=NULL;
	}


	m_Socket.Close();	

	m_WantClose=FALSE;


	CEpollEventObject * pEpollEventObject;
	while(m_RecvQueue.PopFront(pEpollEventObject))
	{	
		GetServer()->DeleteEventObject(pEpollEventObject);		
	}
	while(m_SendQueue.PopFront(pEpollEventObject))
	{	
		GetServer()->DeleteEventObject(pEpollEventObject);		
	}

	
	
	
}


BOOL CNetService::StartListen(const CIPAddress& Address)
{		

	if(m_Socket.GetState()==CNetSocket::SS_UNINITED)
	{
		if(!Create())
			return FALSE;
	}	

	m_pEpollEventRouter->Init(this);

	if(!m_Socket.EnableBlocking(FALSE))
	{		
		PrintNetLog(0xffffffff,"(%d)Service��ʼ������ģʽʧ�ܣ�",GetID());		
		return FALSE;
	}
	
	if(m_CurProtocol==IPPROTO_UDP)
	{		
		if(!m_Socket.Bind(Address))
		{
			PrintNetLog(0xffffffff,"(%d)Service��ʧ�ܣ�",GetID());
			return FALSE;
		}	
		m_Socket.SetState(CNetSocket::SS_CONNECTED);
	}
	else
	{
		if(!m_Socket.Listen(Address))
		{
			PrintNetLog(0xffffffff,"(%d)Service����ʧ�ܣ�",GetID());
			return FALSE;
		}
	}


	if(!GetServer()->BindSocket(m_Socket.GetSocket(),m_pEpollEventRouter))
	{
		PrintNetLog(0xffffffff,"(%d)Service��Socketʧ�ܣ�",GetID());	
		return FALSE;
	}	
	
	OnStartUp();
	return TRUE;
}
void CNetService::Close()
{
	m_Socket.Close();
	
	m_WantClose=FALSE;

	CEpollEventObject * pEpollEventObject;
	while(m_RecvQueue.PopFront(pEpollEventObject))
	{	
		GetServer()->DeleteEventObject(pEpollEventObject);		
	}
	while(m_SendQueue.PopFront(pEpollEventObject))
	{	
		GetServer()->DeleteEventObject(pEpollEventObject);		
	}

	OnClose();	
}
void CNetService::QueryClose()
{
	m_WantClose=TRUE;
}
void CNetService::OnStartUp()
{
}
void CNetService::OnClose()
{
}

int CNetService::Update(int ProcessPacketLimit)
{	
	

	//����Accept
	int PacketCount=0;
	CEpollEventObject * pEpollEventObject;
	while(m_RecvQueue.PopFront(pEpollEventObject))
	{
		if(pEpollEventObject->GetType()==IO_ACCEPT)
		{
			if(!AcceptSocket(pEpollEventObject->GetAcceptSocket()))
			{
				PrintNetLog(0xffffffff,"(%d)AcceptSocketʧ�ܣ�",GetID());
			}
		}
		else if(pEpollEventObject->GetType()==IO_RECV)
		{			
			OnRecvData(pEpollEventObject->GetAddress(),*pEpollEventObject->GetDataBuff());
		}
		else
		{
			PrintNetLog(0xffffffff,"(%d)Servicec�յ��������͵�OverLapped��",GetID());			
		}
		GetServer()->DeleteEventObject(pEpollEventObject);
		PacketCount++;
		if(PacketCount>=ProcessPacketLimit)
			break;
	}	

	//����ر�
	if(m_WantClose)
	{
		Close();		
	}		
	return PacketCount;
}

CBaseTCPConnection * CNetService::CreateConnection(CIPAddress& RemoteAddress)
{
	return NULL;
}

BOOL CNetService::DeleteConnection(CBaseTCPConnection * pConnection)
{
	return FALSE;
}

BOOL CNetService::QueryUDPSend(const CIPAddress& IPAddress,LPCVOID pData,int Size)
{	

	CEpollEventObject * pEpollEventObject=GetServer()->CreateEventObject();
	if(pEpollEventObject==NULL)
	{
		PrintNetLog(0xffffffff,"(%d)Service����UDPSend��OverLappedObjectʧ�ܣ�",GetID());
		return FALSE;
	}

	pEpollEventObject->SetAddress(IPAddress.GetSockAddr());

	pEpollEventObject->SetType(IO_SEND);
	pEpollEventObject->GetDataBuff()->SetUsedSize(0);
	pEpollEventObject->SetParentID(GetID());

	if(!pEpollEventObject->GetDataBuff()->PushBack(pData,Size))
	{
		GetServer()->DeleteEventObject(pEpollEventObject);
		PrintNetLog(0xffffffff,"(%d)ServiceҪ���͵����ݰ�����",GetID());
		return FALSE;
	}

	if(m_SendQueue.PushBack(pEpollEventObject))
	{		
		DoUDPSend();
		return TRUE;
	}
	PrintNetLog(0xffffffff,"(%d)Service�ķ��Ͷ���������",GetID());	
	GetServer()->DeleteEventObject(pEpollEventObject);
	return FALSE;
}

void CNetService::OnRecvData(const CIPAddress& IPAddress,const CEasyBuffer& DataBuffer)
{
}




BOOL CNetService::AcceptSocket(SOCKET Socket)
{

	CIPAddress LocalAddress;
	CIPAddress RemoteAddress;			

	socklen_t LocalAddressLen=sizeof(sockaddr_in);
	socklen_t RemoteAddressLen=sizeof(sockaddr_in);

	getsockname(Socket,
		(sockaddr *)&(LocalAddress.GetSockAddr()),
		&LocalAddressLen);

	getpeername(Socket,
		(sockaddr *)&(RemoteAddress.GetSockAddr()),
		&RemoteAddressLen);

	CBaseTCPConnection * pConnection=dynamic_cast<CBaseTCPConnection *>(CreateConnection(RemoteAddress));
	if(pConnection)
	{
		if(pConnection->Create(Socket,m_RecvQueueSize,m_SendQueueSize))	
		{					
			pConnection->SetRemoteAddress(RemoteAddress);				
			pConnection->SetLocalAddress(LocalAddress);					

			if(pConnection->StartWork())
			{				
				return TRUE;	
			}
			else
				PrintNetLog(0xffffffff,"(%d)Service����Connectionʧ�ܣ�",GetID());
		}
		else
		{
			PrintNetLog(0xffffffff,"(%d)Service��ʼ��Connectionʧ�ܣ�",GetID());
			closesocket(Socket);
		}
		DeleteConnection(pConnection);
	}
	else
	{
		PrintNetLog(0xffffffff,"(%d)Servicec����Connectionʧ�ܣ�",GetID());	
		closesocket(Socket);
	}
	return FALSE;
}

void CNetService::DoAcceptSocket()
{
	CIPAddress Address;

	while(true)
	{
		socklen_t AddressLen=sizeof(sockaddr_in);
		SOCKET AcceptSocket=accept(m_Socket.GetSocket(),(sockaddr*)&(Address.GetSockAddr()),&AddressLen);
		if(AcceptSocket==INVALID_SOCKET)
		{
			int ErrorCode=errno;
			if(ErrorCode==EAGAIN)
			{
				return;
			}
			else
			{
				PrintNetLog(0xffffffff,"Acceptʧ��(%u)",ErrorCode);
				QueryClose();
				return;
			}			
		}
		else
		{
			CEpollEventObject * pEpollEventObject=GetServer()->CreateEventObject();
			if(pEpollEventObject)
			{
				pEpollEventObject->SetType(IO_ACCEPT);
				pEpollEventObject->SetAcceptSocket(AcceptSocket);
				pEpollEventObject->SetParentID(GetID());

				if(m_RecvQueue.PushBack(pEpollEventObject))
				{
					continue;
				}
				else
				{
					GetServer()->DeleteEventObject(pEpollEventObject);
					PrintNetLog(0xffffffff,"CNetService��Accept����������");
				}
				
			}
			else
			{
				PrintNetLog(0xffffffff,"CNetService����Accept��EpollEventObjectʧ�ܣ�");
			}

			closesocket(AcceptSocket);
		}
	}
}

void CNetService::DoUDPRecv()
{
	while(true)
	{
		CEpollEventObject * pEpollEventObject=GetServer()->CreateEventObject();
		int RecvSize=0;
		if(pEpollEventObject)
		{
			pEpollEventObject->SetType(IO_RECV);
			pEpollEventObject->SetParentID(GetID());

			 RecvSize=recvfrom(
				m_Socket.GetSocket(),
				pEpollEventObject->GetDataBuff()->GetBuffer(),
				pEpollEventObject->GetDataBuff()->GetBufferSize(),
				0,
				(sockaddr*)&(pEpollEventObject->GetAddress().GetSockAddr()),
				&(pEpollEventObject->GetAddressLen()));
		}
		else
		{
			PrintNetLog(0xffffffff,"CNetService����Recv��EpollEventObjectʧ��,���ݽ���������");

			static char RecvBuffer[MAX_DATA_PACKET_SIZE];
			static sockaddr_in FromAddr;
			static socklen_t FromAddrLen;

			FromAddrLen=sizeof(FromAddr);
			RecvSize=recvfrom(
				m_Socket.GetSocket(),
				RecvBuffer,
				MAX_DATA_PACKET_SIZE,
				0,
				(sockaddr*)&FromAddr,
				&FromAddrLen);
		}

		if(RecvSize>=0)
		{
			GetServer()->AddUDPRecvBytes(RecvSize);
			if(pEpollEventObject)
			{
				pEpollEventObject->GetDataBuff()->SetUsedSize(RecvSize);
				if(m_RecvQueue.PushBack(pEpollEventObject))
				{
					continue;
				}
				else
				{
					GetServer()->DeleteEventObject(pEpollEventObject);
					PrintNetLog(0xffffffff,"CNetService��Recv����������");
				}
			}
			
		}
		else
		{
			if(pEpollEventObject)
				GetServer()->DeleteEventObject(pEpollEventObject);

			int ErrorCode=errno;
			switch(ErrorCode)
			{
			case EAGAIN:
				return;
			case EINTR:
				PrintNetLog(0xffffffff,"Recv��ϵͳ�ж�");
				break;			
			default:
				PrintNetLog(0xffffffff,"Recvʧ��(%u),Socket�ر�",ErrorCode);
				QueryClose();
				return;
			}			
		}
		
		
	}
}

void CNetService::DoUDPSend()
{
	CEpollEventObject * pEpollEventObject=NULL;
	while(m_SendQueue.PopFront(pEpollEventObject))
	{
		int SendSize=sendto(
			m_Socket.GetSocket(),
			pEpollEventObject->GetDataBuff()->GetBuffer(),
			pEpollEventObject->GetDataBuff()->GetUsedSize(),
			0,
			(sockaddr*)&(pEpollEventObject->GetAddress().GetSockAddr()),
			sizeof(sockaddr_in));

		
			
		if(SendSize>=0)
		{
			GetServer()->AddUDPSendBytes(SendSize);
			if(SendSize<(int)pEpollEventObject->GetDataBuff()->GetUsedSize())
			{
				PrintNetLog(0xffffffff,"Sendû����ɣ����ݱ����ֶ���");			
			}			
		}
		else
		{
			int ErrorCode=errno;
			if(ErrorCode==EAGAIN)
			{
				m_SendQueue.PushFront(pEpollEventObject);
				return;
			}
			else
			{
				PrintNetLog(0xffffffff,"Sendʧ��(%u)",ErrorCode);
			}	
			
		}
		GetServer()->DeleteEventObject(pEpollEventObject);
	}
}