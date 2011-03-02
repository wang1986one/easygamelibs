/****************************************************************************/
/*                                                                          */
/*      �ļ���:    NetPTCPConnectionIOCP.cpp                                */
/*      ��������:  2010��02��09��                                           */
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
	m_pIOCPEventRouter=NULL;
	m_SendQueryCount=0;
}

CNetPTCPConnection::~CNetPTCPConnection(void)
{
	Destory();
}

BOOL CNetPTCPConnection::OnIOCPEvent(int EventID,COverLappedObject * pOverLappedObject)
{	
	if(m_Socket.IsConnected())
	{
		
		if(EventID==IOE_PACKET)
		{
			if(pOverLappedObject->GetType()==IO_SEND)
			{
				AtomicDec(&m_SendQueryCount);
				GetServer()->AddTCPSendBytes(pOverLappedObject->GetDataBuff()->GetUsedSize());
				GetServer()->DeleteOverLappedObject(pOverLappedObject);	
				return TRUE;
			}
			else if(pOverLappedObject->GetType()==IO_RECV)
			{
				if(pOverLappedObject->GetDataBuff()->GetUsedSize()<=0)
				{						
					GetServer()->DeleteOverLappedObject(pOverLappedObject);
					//PrintNetLog(0xffffffff,"%s(%d)Connection�նԷ����ӹر��ź�(%d)��",
					//	GetName(),m_Session,
					//	pOverLappedObject->GetDataBuff()->GetUsedSize());
					Disconnect();
					return TRUE;
				}
				else
				{			
					GetServer()->AddTCPRecvBytes(pOverLappedObject->GetDataBuff()->GetUsedSize());
					OnRecvData(*(pOverLappedObject->GetDataBuff()));
					GetServer()->DeleteOverLappedObject(pOverLappedObject);
					if(!QueryRecv())
					{
						PrintNetLog(0xffffffff,"�޷����������Recv����,���ӹرգ�");
						Disconnect();	
						return FALSE;
					}
					else
					{								
						return TRUE;							
					}
				}
			}
			else
				PrintNetLog(0xffffffff,"Connection�յ��Ƿ�IOCP����");
			
		}

		//else
		//	PrintNetLog(0xffffffff,"(%d)(%d)Connection�յ�IOCP����",GetID(),m_Session);
		if(IsConnected())
		{
			//PrintNetLog(0xffffffff,"(%d)(%d)Connection IOCP����ʱ���ӻ�δ�Ͽ�,���ӹرգ�",GetID(),m_Session);
			Disconnect();
		}		
	}
	else
	{
		//PrintNetLog(0xffffffff,"(%d)(%d)Connectionδ���ӣ�IOCP��(%u)�����ԣ�",GetID(),m_Session,pOverLappedObject->GetParentID());
	}
	GetServer()->DeleteOverLappedObject(pOverLappedObject);


	return FALSE;
}

BOOL CNetPTCPConnection::Create(UINT RecvQueueSize,UINT SendQueueSize)
{

	if(GetServer()==NULL)
		return FALSE;

	Destory();

	if(m_pIOCPEventRouter==NULL)
	{
		m_pIOCPEventRouter=GetServer()->CreateEventRouter();	
		m_pIOCPEventRouter->Init(this);
	}
	m_Socket.MakeSocket(AF_INET,SOCK_STREAM,IPPROTO_TCP);		
	
	return TRUE;
}

BOOL CNetPTCPConnection::Create(SOCKET Socket,UINT RecvQueueSize,UINT SendQueueSize)
{

	if(GetServer()==NULL)
		return FALSE;

	Destory();

	if(m_pIOCPEventRouter==NULL)
	{
		m_pIOCPEventRouter=GetServer()->CreateEventRouter();
		m_pIOCPEventRouter->Init(this);
	}
	
	m_Socket.SetSocket(Socket);
	
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

	if(m_pIOCPEventRouter)
	{
		m_pIOCPEventRouter->SetEventHander(NULL);
		GetServer()->DeleteEventRouter(m_pIOCPEventRouter);
		m_pIOCPEventRouter=NULL;
	}	

	m_SendQueryCount=0;
	
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

	m_pIOCPEventRouter->Init(this);


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

	OnDisconnection();

	m_Socket.Close();
	
	
}


BOOL CNetPTCPConnection::IsConnected()
{
	return m_Socket.IsConnected();
}

BOOL CNetPTCPConnection::IsDisconnected()
{
	return m_Socket.IsDisconnected();
}

BOOL CNetPTCPConnection::StartWork()
{	
	//PrintNetLog(0xffffffff,"(%d)Connection��ʼ����",GetID());	

	m_Socket.SetState(CNetSocket::SS_CONNECTED);	
	m_pIOCPEventRouter->Init(this);

	m_SendQueryCount=0;

	if(!m_Socket.EnableBlocking(FALSE))
	{
		OnConnection(FALSE);
		PrintNetLog(0xffffffff,"(%d)Connection��ʼ������ģʽʧ�ܣ�",GetID());
		m_Socket.Close();		
		return FALSE;
	}

	if(!GetServer()->BindSocket(m_Socket.GetSocket()))
	{
		OnConnection(FALSE);
		PrintNetLog(0xffffffff,"(%d)Connection��IOCPʧ�ܣ�",GetID());
		m_Socket.Close();		
		return FALSE;
	}
	
	if(!QueryRecv())
	{
		OnConnection(FALSE);
		PrintNetLog(0xffffffff,"(%d)Connection��ʼ��Recvʧ�ܣ�",GetID());
		m_Socket.Close();
		return FALSE;
	}
		
	
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

	if(m_Socket.IsConnected())
	{
		while(Size)
		{
			int PacketSize=Size;
			if(PacketSize>MAX_DATA_PACKET_SIZE)
				PacketSize=MAX_DATA_PACKET_SIZE;
			Size-=PacketSize;
		

			COverLappedObject * pOverLappedObject=GetServer()->CreateOverLappedObject();
			if(pOverLappedObject==NULL)
			{
				PrintNetLog(0xffffffff,"Connection����Send��OverLappedObjectʧ�ܣ�");
				return FALSE;
			}

			pOverLappedObject->SetType(IO_SEND);	
			pOverLappedObject->SetIOCPEventRouter(m_pIOCPEventRouter);
			pOverLappedObject->GetDataBuff()->SetUsedSize(0);
			pOverLappedObject->SetParentID(GetID());

			if(!pOverLappedObject->GetDataBuff()->PushBack(pData,PacketSize))
			{
				GetServer()->DeleteOverLappedObject(pOverLappedObject);
				PrintNetLog(0xffffffff,"ConnectionҪ���͵����ݰ�����");
				return FALSE;
			}
			pData=(char *)pData+PacketSize;

			static DWORD NumberOfBytes;
			static DWORD Flag;

			NumberOfBytes=0;
			Flag=0;

			if(m_Socket.SendOverlapped(
				pOverLappedObject->GetDataBuff()->GetBuffer(),
				pOverLappedObject->GetDataBuff()->GetUsedSize(),
				NumberOfBytes,Flag,
				pOverLappedObject->GetOverlapped()))
			{			
				AtomicInc(&m_SendQueryCount);
				continue;
			}
			PrintNetLog(0xffffffff,"����Send����ʧ�ܣ�");	
			GetServer()->DeleteOverLappedObject(pOverLappedObject);
			return FALSE;
		}
		return TRUE;
	}	
	return FALSE;	
}

BOOL CNetPTCPConnection::QueryRecv()
{
	if(m_Socket.IsConnected())
	{
		COverLappedObject * pOverLappedObject=GetServer()->CreateOverLappedObject();
		if(pOverLappedObject==NULL)
		{
			PrintNetLog(0xffffffff,"(%d)Connection����Recv��OverLappedObjectʧ�ܣ�",GetID());
			return FALSE;
		}

		pOverLappedObject->SetType(IO_RECV);	
		pOverLappedObject->GetDataBuff()->SetUsedSize(0);
		pOverLappedObject->SetIOCPEventRouter(m_pIOCPEventRouter);
		pOverLappedObject->SetParentID(GetID());

		static DWORD NumberOfBytes;
		static DWORD Flag;

		NumberOfBytes=0;
		Flag=0;

		if(m_Socket.RecvOverlapped(
			pOverLappedObject->GetDataBuff()->GetBuffer(),
			pOverLappedObject->GetDataBuff()->GetBufferSize(),
			NumberOfBytes,Flag,
			pOverLappedObject->GetOverlapped()))
		{
			return TRUE;
		}
		PrintNetLog(0xffffffff,"(%d)����Recv����ʧ�ܣ�",GetID());	
		GetServer()->DeleteOverLappedObject(pOverLappedObject);
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
		PacketCount++;
	}
		
	return PacketCount;
}


UINT CNetPTCPConnection::GetCurSendQueryCount()
{
	return m_SendQueryCount;
}
