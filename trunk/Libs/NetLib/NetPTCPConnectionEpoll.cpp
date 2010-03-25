/****************************************************************************/
/*                                                                          */
/*      �ļ���:    NetPTCPConnectionEpoll.cpp                               */
/*      ��������:  2009��09��11��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#include "StdAfx.h"

IMPLEMENT_CLASS_INFO_STATIC(CNetPTCPConnection,CBaseTCPConnection);


CNetPTCPConnection::CNetPTCPConnection(void)
{
	m_pServer=NULL;
	m_pEpollEventRouter=NULL;
}

CNetPTCPConnection::~CNetPTCPConnection(void)
{
	Destory();
}

BOOL CNetPTCPConnection::OnEpollEvent(UINT EventID)
{
	if(m_Socket.IsConnected())
	{
		if(EventID&(EPOLLERR|EPOLLHUP))
		{
			PrintNetLog(0xffffffff,"CNetPTCPConnection::Epoll��������%d��",errno);
			Disconnect();
			return TRUE;
		}
		if(EventID&EPOLLIN)
		{
			DoRecv();
		}
		if(EventID&EPOLLOUT)
		{
			DoSend();
		}
	}
	else
	{
		PrintNetLog(0xffffffff,"(%d)Connectionδ���ӣ�Epoll�¼������ԣ�",GetID());
	}


	return FALSE;
}

BOOL CNetPTCPConnection::Create(UINT RecvQueueSize,UINT SendQueueSize)
{


	if(GetServer()==NULL)
		return FALSE;

	Destory();


	if(m_pEpollEventRouter==NULL)
	{
		m_pEpollEventRouter=GetServer()->CreateEventRouter();
		m_pEpollEventRouter->Init(this);
	}
	m_Socket.MakeSocket(AF_INET,SOCK_STREAM,IPPROTO_TCP);


	if(m_SendQueue.GetBufferSize()<SendQueueSize)
	{
		m_SendQueue.Create(SendQueueSize);
	}
	else
	{
		m_SendQueue.Clear();
	}

	return TRUE;
}

BOOL CNetPTCPConnection::Create(SOCKET Socket,UINT RecvQueueSize,UINT SendQueueSize)
{


	if(GetServer()==NULL)
		return FALSE;

	Destory();


	if(m_pEpollEventRouter==NULL)
	{
		m_pEpollEventRouter=GetServer()->CreateEventRouter();
		m_pEpollEventRouter->Init(this);
	}

	m_Socket.SetSocket(Socket);


	if(m_SendQueue.GetBufferSize()<SendQueueSize)
	{
		m_SendQueue.Create(SendQueueSize);
	}
	else
	{
		m_SendQueue.Clear();
	}
	return TRUE;
}

void CNetPTCPConnection::Destory()
{
	if(IsConnected())
	{
		m_Socket.Close();
		OnDisconnection();
	}
	else
	{
		m_Socket.Close();
	}



	if(m_pEpollEventRouter)
	{
		m_pEpollEventRouter->SetEventHander(NULL);
		GetServer()->DeleteEventRouter(m_pEpollEventRouter);
		m_pEpollEventRouter=NULL;
	}

	CEpollEventObject * pEpollEventObject;
	while(m_SendQueue.PopFront(pEpollEventObject))
	{
		GetServer()->DeleteEventObject(pEpollEventObject);
	}

}

BOOL CNetPTCPConnection::Connect(const CIPAddress& Address ,DWORD TimeOut)
{
	if(GetServer()==NULL)
		return FALSE;

	if(m_Socket.GetState()==CNetSocket::SS_UNINITED)
	{
		if(!Create())
			return FALSE;
	}

	m_pEpollEventRouter->Init(this);

	if(TimeOut==NO_CONNECTION_TIME_OUT)
	{
		if(m_Socket.Connect(Address))
		{
			StartWork();
			return TRUE;
		}
	}
	else
	{
		if(m_Socket.ConnectNoBlocking(Address,TimeOut))
		{
			return TRUE;
		}
	}
	return FALSE;
}
void CNetPTCPConnection::Disconnect()
{
	//PrintNetLog(0xffffffff,"(%d)Connection�ر�",GetID());

	//PrintNetLog(0xffffffff,"%s���ӹر�",GetName());


	m_Socket.Close();


	CEpollEventObject * pEpollEventObject;
	while(m_SendQueue.PopFront(pEpollEventObject))
	{
		GetServer()->DeleteEventObject(pEpollEventObject);
	}

	OnDisconnection();
}

BOOL CNetPTCPConnection::StartWork()
{
	//PrintNetLog(0xffffffff,"(%d)Connection��ʼ����",GetID());

	m_pEpollEventRouter->Init(this);

	m_Socket.SetState(CNetSocket::SS_CONNECTED);

	if(!m_Socket.EnableBlocking(FALSE))
	{
		OnConnection(FALSE);
		PrintNetLog(0xffffffff,"(%d)Connection��ʼ������ģʽʧ�ܣ�",GetID());
		m_Socket.Close();
		return FALSE;
	}

	if(!GetServer()->BindSocket(m_Socket.GetSocket(),m_pEpollEventRouter))
	{
		OnConnection(FALSE);
		PrintNetLog(0xffffffff,"(%d)Connection��Epollʧ�ܣ�",GetID());
		m_Socket.Close();
		return FALSE;
	}

	//CEasyString ConnectionName;
	//ConnectionName.Format("[%s:%d]",GetRemoteAddress().GetIPString(),GetRemoteAddress().GetPort());
	//SetName(ConnectionName);;


	//PrintNetLog(0xffffffff,"%s���ӽ���[%u]",GetName(),(UINT)m_Socket.GetSocket());

	OnConnection(TRUE);
	return TRUE;
}

void CNetPTCPConnection::OnConnection(BOOL IsSucceed)
{

}

void CNetPTCPConnection::OnDisconnection()
{
}

BOOL CNetPTCPConnection::QuerySend(LPCVOID pData,int Size)
{
	//CAutoLock Lock(m_EasyCriticalSection);


	if(m_Socket.IsConnected())
	{
		while(Size)
		{
			int PacketSize=Size;
			if(PacketSize>MAX_DATA_PACKET_SIZE)
				PacketSize=MAX_DATA_PACKET_SIZE;
			Size-=PacketSize;

			CEpollEventObject * pEpollEventObject=GetServer()->CreateEventObject();
			if(pEpollEventObject==NULL)
			{
				PrintNetLog(0xffffffff,"Connection����Send��OverLappedObjectʧ�ܣ�");
				return FALSE;
			}

			pEpollEventObject->SetType(IO_SEND);
			pEpollEventObject->GetDataBuff()->SetUsedSize(0);
			pEpollEventObject->SetParentID(GetID());

			if(!pEpollEventObject->GetDataBuff()->PushBack(pData,PacketSize))
			{
				GetServer()->DeleteEventObject(pEpollEventObject);
				PrintNetLog(0xffffffff,"ConnectionҪ���͵����ݰ�����");
				return FALSE;
			}
			pData=(char *)pData+PacketSize;

			if(!m_SendQueue.PushBack(pEpollEventObject))
			{
				PrintNetLog(0xffffffff,"CNetPTCPConnection::���Ͷ���������");
				GetServer()->DeleteEventObject(pEpollEventObject);
				Disconnect();
				return FALSE;
			}
		}
		DoSend();
		return TRUE;
	}
	return FALSE;
}



void CNetPTCPConnection::OnRecvData(const CEasyBuffer& DataBuffer)
{
}

int CNetPTCPConnection::Update(int ProcessPacketLimit)
{

	int PacketCount=0;
	//����Connect
	if(m_Socket.GetState()==CNetSocket::SS_CONNECTING)
	{
		m_Socket.Connected();
		if(m_Socket.GetState()==CNetSocket::SS_CONNECTED)
		{
			StartWork();
		}
		if(m_Socket.GetState()==CNetSocket::SS_UNUSED)
		{
			OnConnection(FALSE);
		}
	}

	return PacketCount;
}

void CNetPTCPConnection::DoRecv()
{
	CAutoLock Lock(m_RecvLock);

	while(true)
	{
		CEpollEventObject * pEpollEventObject=GetServer()->CreateEventObject();
		if(pEpollEventObject)
		{
			pEpollEventObject->SetType(IO_RECV);
			pEpollEventObject->SetParentID(GetID());

			int RecvSize=recv(
				m_Socket.GetSocket(),
				pEpollEventObject->GetDataBuff()->GetBuffer(),
				pEpollEventObject->GetDataBuff()->GetBufferSize(),
				0);

			if(RecvSize>0)
			{
				GetServer()->AddTCPRecvBytes(RecvSize);
				pEpollEventObject->GetDataBuff()->SetUsedSize(RecvSize);
				OnRecvData(*(pEpollEventObject->GetDataBuff()));
				GetServer()->DeleteEventObject(pEpollEventObject);
				continue;

			}
			else if(RecvSize==0)
			{
				GetServer()->DeleteEventObject(pEpollEventObject);
				PrintNetLog(0xffffffff,"CNetPTCPConnection�յ����ӹر��źţ�");
				Disconnect();
				return;
			}
			else
			{
				GetServer()->DeleteEventObject(pEpollEventObject);

				int ErrorCode=errno;
				switch(ErrorCode)
				{
				case EAGAIN:
					return;
				default:
					PrintNetLog(0xffffffff,"CNetPTCPConnection::Recvʧ��(%u),Socket�ر�",ErrorCode);
					Disconnect();
					return;
				}
			}
		}
		else
		{
			PrintNetLog(0xffffffff,"CNetPTCPConnection����Recv��EpollEventObjectʧ�ܣ�");
			Disconnect();
			return;
		}
	}
}
void CNetPTCPConnection::DoSend()
{
	CAutoLock Lock(m_SendLock);

	CEpollEventObject * pEpollEventObject=NULL;
	while(m_SendQueue.PopFront(pEpollEventObject))
	{
		int SendSize=send(
			m_Socket.GetSocket(),
			pEpollEventObject->GetDataBuff()->GetBuffer(),
			pEpollEventObject->GetDataBuff()->GetUsedSize(),
			0);
		if(SendSize>=0)
		{
			GetServer()->AddTCPSendBytes(SendSize);
			if(SendSize<(int)pEpollEventObject->GetDataBuff()->GetUsedSize())
			{
				pEpollEventObject->GetDataBuff()->PopFront(NULL,SendSize);
				m_SendQueue.PushFront(pEpollEventObject);
				return;
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
				Disconnect();
				return;
			}

		}
		GetServer()->DeleteEventObject(pEpollEventObject);
	}
}


