/****************************************************************************/
/*                                                                          */
/*      �ļ���:    NetConnectionEpoll.cpp                                   */
/*      ��������:  2009��09��11��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#include "StdAfx.h"

IMPLEMENT_CLASS_INFO_STATIC(CNetConnection,CBaseTCPConnection);


CNetConnection::CNetConnection(void)
{		
	m_pServer=NULL;
	m_WantClose=FALSE;
	m_UseSafeDisconnect=false;
	m_pEpollEventRouter=NULL;
}

CNetConnection::~CNetConnection(void)
{
	Destory();
}

BOOL CNetConnection::OnEpollEvent(UINT EventID)
{	
	if(m_Socket.IsConnected())
	{
		if(EventID&(EPOLLERR|EPOLLHUP))
		{
			PrintNetLog(0xffffffff,"CNetConnection::Epoll��������%d��",errno);
			QueryDisconnect();
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

BOOL CNetConnection::Create(UINT RecvQueueSize,UINT SendQueueSize)
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

	if(m_RecvQueue.GetBufferSize()<RecvQueueSize)
	{
		m_RecvQueue.Create(RecvQueueSize);
	}
	else
	{
		m_RecvQueue.Clear();
	}

	if(m_SendQueue.GetBufferSize()<SendQueueSize)
	{
		m_SendQueue.Create(SendQueueSize);
	}
	else
	{
		m_SendQueue.Clear();
	}
	m_WantClose=FALSE;
	
	return TRUE;
}

BOOL CNetConnection::Create(SOCKET Socket,UINT RecvQueueSize,UINT SendQueueSize)
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

	if(m_RecvQueue.GetBufferSize()<RecvQueueSize)
	{
		m_RecvQueue.Create(RecvQueueSize);
	}
	else
	{
		m_RecvQueue.Clear();
	}

	if(m_SendQueue.GetBufferSize()<SendQueueSize)
	{
		m_SendQueue.Create(SendQueueSize);
	}
	else
	{
		m_SendQueue.Clear();
	}
	m_WantClose=FALSE;	
	return TRUE;
}

void CNetConnection::Destory()
{	
	if(IsConnected())
	{		
		OnDisconnection();	
	}

	m_Socket.Close();

	if(m_pEpollEventRouter)
	{
		m_pEpollEventRouter->SetEventHander(NULL);
		GetServer()->DeleteEventRouter(m_pEpollEventRouter);
		m_pEpollEventRouter=NULL;
	}
	
	CEpollEventObject * pEpollEventObject;
	while(m_RecvQueue.PopFront(pEpollEventObject))
	{			
		GetServer()->DeleteEventObject(pEpollEventObject);		
	}	
	while(m_SendQueue.PopFront(pEpollEventObject))
	{			
		GetServer()->DeleteEventObject(pEpollEventObject);		
	}	

	m_WantClose=FALSE;
}

BOOL CNetConnection::Connect(const CIPAddress& Address ,DWORD TimeOut)
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
void CNetConnection::Disconnect()
{
	//PrintNetLog(0xffffffff,"(%d)Connection�ر�",GetID());

	//PrintNetLog(0xffffffff,"%s���ӹر�",GetName());

	
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

	OnDisconnection();
}
void CNetConnection::QueryDisconnect()
{
	m_WantClose=TRUE;
}



BOOL CNetConnection::StartWork()
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

void CNetConnection::OnConnection(BOOL IsSucceed)
{
	
}

void CNetConnection::OnDisconnection()
{
}

BOOL CNetConnection::Send(LPCVOID pData,int Size)
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
				PrintNetLog(0xffffffff,"CNetConnection::���Ͷ���������");	
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



void CNetConnection::OnRecvData(const CEasyBuffer& DataBuffer)
{
}

int CNetConnection::Update(int ProcessPacketLimit)
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
	else
	{
		CEpollEventObject * pEpollEventObject;
		while(m_RecvQueue.PopFront(pEpollEventObject))
		{				
			OnRecvData(*(pEpollEventObject->GetDataBuff()));
			GetServer()->DeleteEventObject(pEpollEventObject);
			PacketCount++;
			if(PacketCount>=ProcessPacketLimit)
				break;
		}			
	}

	//����ر�
	if(m_WantClose)
	{
		if(m_UseSafeDisconnect)
		{
			if(m_SendQueue.GetObjectCount()<=0)
			{
				Disconnect();		
			}			
		}
		else
		{
			Disconnect();
		}
	}
	return PacketCount;
}


bool CNetConnection::StealFrom(CNameObject * pObject,UINT Param)
{
	CAutoLock Lock1(m_RecvLock);
	CAutoLock Lock2(m_SendLock);

	PrintNetLog(0xffffffff,"(%d)ִ�������滻(%d)��",GetID(),pObject->GetID());
	if(pObject->IsKindOf(GET_CLASS_INFO(CNetConnection)))
	{
		Destory();

		if(!CNameObject::StealFrom(pObject,Param))
			return false;
		CNetConnection * pConnection=(CNetConnection *)pObject;
		if(!m_Socket.StealFrom(&(pConnection->m_Socket),Param))
			return false;
		
		m_pServer=pConnection->m_pServer;
		m_WantClose=pConnection->m_WantClose;
		m_pEpollEventRouter=pConnection->m_pEpollEventRouter;
		pConnection->m_pEpollEventRouter=NULL;
		if(m_pEpollEventRouter)
			m_pEpollEventRouter->SetEventHander(this);

		
		
		CEpollEventObject * pEpollEventObject;

		m_RecvQueue.Create(pConnection->m_RecvQueue.GetBufferSize());		
		while(pConnection->m_RecvQueue.PopFront(pEpollEventObject))
		{
			m_RecvQueue.PushBack(pEpollEventObject);
		}		

		m_SendQueue.Create(pConnection->m_SendQueue.GetBufferSize());		
		while(pConnection->m_SendQueue.PopFront(pEpollEventObject))
		{
			m_SendQueue.PushBack(pEpollEventObject);
		}	

		return true;
		
	}
	return false;
}

void CNetConnection::DoRecv()
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
				if(m_RecvQueue.PushBack(pEpollEventObject))
				{
					continue;
				}
				else
				{
					GetServer()->DeleteEventObject(pEpollEventObject);
					PrintNetLog(0xffffffff,"CNetConnection��Recv����������");
					QueryDisconnect();
					return;
				}
			}
			else if(RecvSize==0)
			{
				GetServer()->DeleteEventObject(pEpollEventObject);
				PrintNetLog(0xffffffff,"CNetConnection�յ����ӹر��źţ�");
				QueryDisconnect();
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
					PrintNetLog(0xffffffff,"CNetConnection::Recvʧ��(%u),Socket�ر�",ErrorCode);
					QueryDisconnect();
					return;
				}			
			}
		}
		else
		{
			PrintNetLog(0xffffffff,"CNetConnection����Recv��EpollEventObjectʧ�ܣ�");
			QueryDisconnect();
			return;			
		}
	}
}
void CNetConnection::DoSend()
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
				QueryDisconnect();
				return;			
			}	

		}
		GetServer()->DeleteEventObject(pEpollEventObject);
	}
}


