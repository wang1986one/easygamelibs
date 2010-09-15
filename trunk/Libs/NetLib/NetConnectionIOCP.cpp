/****************************************************************************/
/*                                                                          */
/*      �ļ���:    NetConnectionIOCP.cpp                                    */
/*      ��������:  2010��02��09��                                           */
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
	m_CurProtocol=IPPROTO_TCP;	
	m_pIOCPEventRouter=NULL;
	m_SendQueryCount=0;
	m_UseSendBuffer=0;
	m_IsRecvPaused=false;
}

CNetConnection::~CNetConnection(void)
{
	Destory();
}

BOOL CNetConnection::OnIOCPEvent(int EventID,COverLappedObject * pOverLappedObject)
{	
	
	if(m_Socket.IsConnected())
	{
		
		
		if(EventID==IOE_PACKET)
		{
			if(pOverLappedObject->GetType()==IO_SEND)
			{
				GetServer()->AddTCPSendBytes(pOverLappedObject->GetDataBuff()->GetUsedSize());
				GetServer()->DeleteOverLappedObject(pOverLappedObject);	
				
				AtomicDec(&m_SendQueryCount);
				
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
					QueryDisconnect();
					return TRUE;
				}
				else
				{			
					GetServer()->AddTCPRecvBytes(pOverLappedObject->GetDataBuff()->GetUsedSize());
					
					if(m_DataQueue.PushBack(pOverLappedObject))
					{
						if(!QueryRecv())
						{
							PrintNetLog(0xffffffff,"�޷����������Recv����,���ӹرգ�");
							QueryDisconnect();	
							return FALSE;
						}
						else
						{
							return TRUE;
						}
						
					}
					else
						PrintNetLog(0xffffffff,"Recv���ݶ���������");
			}
			}
			else
				PrintNetLog(0xffffffff,"Connection�յ��Ƿ�IOCP����");
			
		}
		else
			PrintNetLog(0xffffffff,"(%d)Connection�յ�IOCP����",GetID());
		if(IsConnected())
		{
			//PrintNetLog(0xffffffff,"(%d)(%d)Connection IOCP����ʱ���ӻ�δ�Ͽ�,���ӹرգ�",GetID(),m_Session);
			QueryDisconnect();
		}
		
	}
	else
	{
		PrintNetLog(0xffffffff,"(%d)Connectionδ���ӣ�IOCP��(%u)�����ԣ�",GetID(),pOverLappedObject->GetParentID());
	}
	GetServer()->DeleteOverLappedObject(pOverLappedObject);
	

	return FALSE;
}

BOOL CNetConnection::Create(UINT RecvQueueSize,UINT SendQueueSize)
{
	

	if(GetServer()==NULL)
		return FALSE;
	

	if(m_pIOCPEventRouter==NULL)
	{
		m_pIOCPEventRouter=GetServer()->CreateEventRouter();	
		m_pIOCPEventRouter->Init(this);
	}
	m_Socket.MakeSocket(AF_INET,SOCK_STREAM,IPPROTO_TCP);	

	if(m_DataQueue.GetObjectCount())
	{
		PrintNetLog(0xffffffff,"(%d)CNetConnection::Create:���ݶ��в��ǿյģ�",GetID());
	}

	
	if(m_DataQueue.GetBufferSize()<RecvQueueSize)
	{
		m_DataQueue.Create(RecvQueueSize);
	}
	
		
	m_CurProtocol=m_Socket.GetProtocol();	
	InterlockedExchange((LPLONG)&(m_WantClose),FALSE);
	
	return TRUE;
}

BOOL CNetConnection::Create(SOCKET Socket,UINT RecvQueueSize,UINT SendQueueSize)
{
	

	if(GetServer()==NULL)
		return FALSE;
		

	if(m_pIOCPEventRouter==NULL)
	{
		m_pIOCPEventRouter=GetServer()->CreateEventRouter();
		m_pIOCPEventRouter->Init(this);
	}
	
	m_Socket.SetSocket(Socket);

	if(m_DataQueue.GetObjectCount())
	{
		PrintNetLog(0xffffffff,"(%d)CNetConnection::Create:���ݶ��в��ǿյģ�",GetID());
	}

	if(m_DataQueue.GetBufferSize()<RecvQueueSize)
	{
		m_DataQueue.Create(RecvQueueSize);
	}

	if(SendQueueSize)
	{
		PrintNetLog(0,"���÷��Ͷ��У���СΪ%u",SendQueueSize);
		m_UseSendBuffer=true;
		if(m_SendBuffer.GetBufferSize()<SendQueueSize)
		{
			m_SendBuffer.Create(SendQueueSize);
		}
	}
	else
	{
		m_UseSendBuffer=false;
	}
	
	
	
	m_CurProtocol=m_Socket.GetProtocol();
	InterlockedExchange((LPLONG)&(m_WantClose),FALSE);	
	return TRUE;
}

void CNetConnection::Destory()
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
	
	COverLappedObject * pOverLappedObject;
	while(m_DataQueue.PopFront(pOverLappedObject))
	{			
		GetServer()->DeleteOverLappedObject(pOverLappedObject);		
	}	

	InterlockedExchange((LPLONG)&(m_WantClose),FALSE);
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
void CNetConnection::Disconnect()
{
	//PrintNetLog(0xffffffff,"(%d)Connection�ر�",GetID());

	//PrintNetLog(0xffffffff,"%s���ӹر�",GetName());

	
	m_Socket.Close();
	
	InterlockedExchange((LPLONG)&(m_WantClose),FALSE);
	m_SendQueryCount=0;


	COverLappedObject * pOverLappedObject;
	while(m_DataQueue.PopFront(pOverLappedObject))
	{			
		GetServer()->DeleteOverLappedObject(pOverLappedObject);		
	}	
	
	while(m_SendBuffer.PopFront(pOverLappedObject))
	{			
		GetServer()->DeleteOverLappedObject(pOverLappedObject);		
	}	

	OnDisconnection();
}
void CNetConnection::QueryDisconnect()
{
	InterlockedExchange((LPLONG)&(m_WantClose),TRUE);
}



BOOL CNetConnection::StartWork()
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

BOOL CNetConnection::Send(LPCVOID pData,UINT Size)
{
	if(m_UseSendBuffer)
	{
		UINT FreeSize=(m_SendBuffer.GetBufferSize()-m_SendBuffer.GetObjectCount())*MAX_DATA_PACKET_SIZE;
		if(FreeSize<Size)
		{
			int FlushCount=(Size/MAX_DATA_PACKET_SIZE)*2;
			DoBufferSend(FlushCount);			
		}
		void * Pos=m_SendBuffer.GetLastObjectPos();
		if(Pos)
		{
			//�Ƚ����һ��������
			COverLappedObject ** ppOverLappedObject=m_SendBuffer.GetObject(Pos);
			if(ppOverLappedObject)
			{
				if((*ppOverLappedObject)->GetDataBuff()->GetFreeSize())
				{
					UINT PushSize=(*ppOverLappedObject)->GetDataBuff()->GetFreeSize();
					if(PushSize>Size)
						PushSize=Size;
					(*ppOverLappedObject)->GetDataBuff()->PushBack(pData,PushSize);
					pData=(char *)pData+PushSize;
					Size-=PushSize;
				}
			}
		}		
		while(Size)
		{
			int PacketSize=Size;
			if(PacketSize>MAX_DATA_PACKET_SIZE)
				PacketSize=MAX_DATA_PACKET_SIZE;

			COverLappedObject * pOverLappedObject=GetServer()->CreateOverLappedObject();
			if(pOverLappedObject==NULL)
			{
				PrintNetLog(0xffffffff,"Connection����Send��OverLappedObjectʧ�ܣ�");
				Disconnect();
				return FALSE;
			}

			pOverLappedObject->SetType(IO_SEND);
			pOverLappedObject->GetDataBuff()->SetUsedSize(0);
			
			if(!pOverLappedObject->GetDataBuff()->PushBack(pData,PacketSize))
			{
				GetServer()->DeleteOverLappedObject(pOverLappedObject);
				PrintNetLog(0xffffffff,"ConnectionҪ���͵����ݰ�����");
				Disconnect();
				return FALSE;
			}
			pData=(char *)pData+PacketSize;
			Size-=PacketSize;
			if(m_SendBuffer.PushBack(pOverLappedObject)==NULL)
			{
				PrintNetLog(0,"�쳣�����ͻ�������");
				Disconnect();
				return FALSE;
			}
		}
		return TRUE;
	}
	else
	{
		return QuerySend(pData,Size);
	}
}

BOOL CNetConnection::SendDirect(LPCVOID pData,UINT Size)
{
	return QuerySend(pData,Size);;
}

BOOL CNetConnection::QuerySend(LPCVOID pData,UINT Size)
{		
	//CAutoLock Lock(m_EasyCriticalSection);
	
	FUNCTION_BEGIN;
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
				Disconnect();
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
				Disconnect();
				return FALSE;
			}
			pData=(char *)pData+PacketSize;

			static DWORD NumberOfBytes;
			static DWORD Flag;

			NumberOfBytes=0;
			Flag=0;
			
			AtomicInc(&m_SendQueryCount);
			if(m_Socket.SendOverlapped(
				pOverLappedObject->GetDataBuff()->GetBuffer(),
				pOverLappedObject->GetDataBuff()->GetUsedSize(),
				NumberOfBytes,Flag,
				pOverLappedObject->GetOverlapped()))
			{
				
				continue;
			}
			AtomicDec(&m_SendQueryCount);
			PrintNetLog(0xffffffff,"����Send����ʧ�ܣ�");	
			GetServer()->DeleteOverLappedObject(pOverLappedObject);
			Disconnect();
			return FALSE;
		}
		return TRUE;
	}	

	FUNCTION_END;
	return FALSE;	
}

BOOL CNetConnection::QueryRecv()
{
	//CAutoLock Lock(m_EasyCriticalSection);
	if(m_IsRecvPaused)
	{
		return TRUE;
	}

	if(m_Socket.IsConnected())
	{
		if(m_DataQueue.GetObjectCount()>=m_DataQueue.GetBufferSize())
		{
			PrintNetDebugLog(0,"���ջ�����������ͣ����");
			m_IsRecvPaused=true;
			return TRUE;
		}
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
		COverLappedObject * pOverLappedObject;
		while(m_DataQueue.PopFront(pOverLappedObject))
		{			
			OnRecvData(*(pOverLappedObject->GetDataBuff()));
			GetServer()->DeleteOverLappedObject(pOverLappedObject);
			PacketCount++;
			if(PacketCount>=ProcessPacketLimit)
				break;
		}			
	}

	m_IsRecvPaused=false;

	//������ķ���
	if(m_UseSendBuffer)
	{
		PacketCount+=DoBufferSend(ProcessPacketLimit);
	}

	//����ر�
	if(m_WantClose)
	{		
		Disconnect();		
	}
	return PacketCount;
}

UINT CNetConnection::GetCurSendQueryCount()
{
	return m_SendQueryCount;
}


bool CNetConnection::StealFrom(CNameObject * pObject,UINT Param)
{
	PrintNetLog(0xffffffff,"(%d)ִ�������滻(%d)��",GetID(),pObject->GetID());
	if(pObject->IsKindOf(GET_CLASS_INFO(CNetConnection)))
	{
		Destory();

		COverLappedObject * pOverLappedObject;

		if(!CNameObject::StealFrom(pObject,Param))
			return false;
		CNetConnection * pConnection=(CNetConnection *)pObject;
		if(!m_Socket.StealFrom(&(pConnection->m_Socket),Param))
			return false;
		
		m_pServer=pConnection->m_pServer;
		m_WantClose=pConnection->m_WantClose;
		m_CurProtocol=pConnection->m_CurProtocol;
		m_pIOCPEventRouter=pConnection->m_pIOCPEventRouter;
		pConnection->m_pIOCPEventRouter=NULL;
		if(m_pIOCPEventRouter)
			m_pIOCPEventRouter->SetEventHander(this);

		m_IsRecvPaused=pConnection->m_IsRecvPaused;

		m_SendQueryCount=pConnection->m_SendQueryCount;
		m_UseSendBuffer=pConnection->m_UseSendBuffer;
		if(m_UseSendBuffer)
		{
			m_SendBuffer.Create(pConnection->m_SendBuffer.GetBufferSize());
			while(pConnection->m_SendBuffer.PopFront(pOverLappedObject))
			{
				m_SendBuffer.PushBack(pOverLappedObject);
			}		
		}

		m_DataQueue.Create(pConnection->m_DataQueue.GetBufferSize());
		
		while(pConnection->m_DataQueue.PopFront(pOverLappedObject))
		{
			m_DataQueue.PushBack(pOverLappedObject);
		}		

		return true;
		
	}
	return false;
}

int CNetConnection::DoBufferSend(int ProcessPacketLimit)
{
	int ProcessCount=0;

	COverLappedObject * pOverLappedObject;
	while(m_SendBuffer.PopFront(pOverLappedObject))
	{			
		pOverLappedObject->SetType(IO_SEND);	
		pOverLappedObject->SetIOCPEventRouter(m_pIOCPEventRouter);
		pOverLappedObject->SetParentID(GetID());

		static DWORD NumberOfBytes;
		static DWORD Flag;

		NumberOfBytes=0;
		Flag=0;

		AtomicInc(&m_SendQueryCount);
		if(!m_Socket.SendOverlapped(
			pOverLappedObject->GetDataBuff()->GetBuffer(),
			pOverLappedObject->GetDataBuff()->GetUsedSize(),
			NumberOfBytes,Flag,
			pOverLappedObject->GetOverlapped()))
		{

			AtomicDec(&m_SendQueryCount);
			PrintNetLog(0xffffffff,"����Send����ʧ�ܣ�");	
			GetServer()->DeleteOverLappedObject(pOverLappedObject);
			Disconnect();
			break;
		}		

		ProcessCount++;
		if(ProcessCount>=ProcessPacketLimit)
			break;
	}

	return ProcessCount;

}