#include "StdAfx.h"


IMPLEMENT_CLASS_INFO_STATIC(CNetPUDPService,CBaseService);

CNetPUDPService::CNetPUDPService()
{
	m_WantClose=FALSE;
	m_pServer=NULL;	
	m_ParallelRecvCount=0;	
	m_pIOCPEventRouter=NULL;	
	
	
}

CNetPUDPService::~CNetPUDPService(void)
{
	Destory();
}

BOOL CNetPUDPService::OnIOCPEvent(int EventID,COverLappedObject * pOverLappedObject)
{		
	if(IsWorking())
	{
		if(pOverLappedObject->GetType()==IO_RECV)
		{			
			if(!QueryUDPRecv())
			{
				PrintNetLog(0xffffffff,"PUDPService�޷����������UDPRecv����");
				Close();
			}

			if(EventID==IOE_PACKET)
			{
				GetServer()->AddUDPRecvBytes(pOverLappedObject->GetDataBuff()->GetUsedSize());
				OnRecvData(pOverLappedObject->GetAddress(),*pOverLappedObject->GetDataBuff());
				GetServer()->DeleteOverLappedObject(pOverLappedObject);
				return TRUE;
			}
			else
			{
				PrintNetLog(0xffffffff,"����ʧ�ܣ�");
			}				
		}
		else if(pOverLappedObject->GetType()==IO_SEND)
		{
			if(EventID==IOE_PACKET)
			{
				GetServer()->AddUDPSendBytes(pOverLappedObject->GetDataBuff()->GetUsedSize());
				GetServer()->DeleteOverLappedObject(pOverLappedObject);
				return TRUE;
			}
			else
				PrintNetLog(0xffffffff,"����ʧ�ܣ�");
		}			
		else
			PrintNetLog(0xffffffff,"PUDPService�յ��Ƿ�IOCP����");
	}
	else
		PrintNetLog(0xffffffff,"PUDPServiceδ���ã�IOCP�������ԣ�");
	GetServer()->DeleteOverLappedObject(pOverLappedObject);
	return FALSE;
}

BOOL CNetPUDPService::Create(int ParallelRecvCount)
{	
	if(GetServer()==NULL)
		return FALSE;

	if(m_pIOCPEventRouter==NULL)
	{
		m_pIOCPEventRouter=GetServer()->CreateEventRouter();
		m_pIOCPEventRouter->Init(this);
	}

	m_ParallelRecvCount=ParallelRecvCount;	
	m_Socket.MakeSocket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);	
	InterlockedExchange((LPLONG)&(m_WantClose),FALSE);
	
	return TRUE;
}
void CNetPUDPService::Destory()
{
	

	if(m_pIOCPEventRouter)
	{
		m_pIOCPEventRouter->SetEventHander(NULL);
		GetServer()->DeleteEventRouter(m_pIOCPEventRouter);
		m_pIOCPEventRouter=NULL;
	}

	m_Socket.Close();	
	
	InterlockedExchange((LPLONG)&(m_WantClose),FALSE);
}


BOOL CNetPUDPService::StartListen(const CIPAddress& Address)
{
	

	if(m_Socket.GetState()==CNetSocket::SS_UNINITED)
	{
		if(!Create())
			return FALSE;
	}
	if(!m_Socket.Listen(Address))
	{
		PrintNetLog(0xffffffff,"(%d)PUDPService����ʧ�ܣ�",GetID());
		return FALSE;
	}


	
	
	if(!GetServer()->BindSocket(m_Socket.GetSocket()))
	{
		PrintNetLog(0xffffffff,"(%d)PUDPService��IOCPʧ�ܣ�",GetID());
		Close();
		return FALSE;
	}	
	
	
			
	for(int i=0;i<m_ParallelRecvCount;i++)
	{
		if(!QueryUDPRecv())
		{
			PrintNetLog(0xffffffff,"(%d)Service����UDPRecv����ʧ�ܣ�",GetID());
			Close();
			return FALSE;
		}
	}
	
	OnStartUp();
	return TRUE;
}
void CNetPUDPService::Close()
{	

	m_Socket.Close();
	
	InterlockedExchange((LPLONG)&(m_WantClose),FALSE);	

	OnClose();	
}


void CNetPUDPService::OnStartUp()
{
}

void CNetPUDPService::OnClose()
{
}


BOOL CNetPUDPService::QueryUDPRecv()
{	

	COverLappedObject * pOverLappedObject=GetServer()->CreateOverLappedObject();
	if(pOverLappedObject==NULL)
	{
		PrintNetLog(0xffffffff,"(%d)PUDPService������UDPRecv��OverLappedObjectʧ�ܣ�",GetID());
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

	if(m_Socket.RecvFromOverlapped(pOverLappedObject->GetAddress(),
		pOverLappedObject->GetAddressLen(),
		pOverLappedObject->GetDataBuff()->GetBuffer(),
		pOverLappedObject->GetDataBuff()->GetBufferSize(),
		NumberOfBytes,Flag,
		pOverLappedObject->GetOverlapped()))
	{
		return TRUE;
	}
	PrintNetLog(0xffffffff,"(%d)PUDPService����UDPRecv����ʧ�ܣ�",GetID());	
	GetServer()->DeleteOverLappedObject(pOverLappedObject);
	return FALSE;
}

BOOL CNetPUDPService::QueryUDPSend(const CIPAddress& IPAddress,LPCVOID pData,int Size)
{		
	COverLappedObject * pOverLappedObject=GetServer()->CreateOverLappedObject();
	if(pOverLappedObject==NULL)
	{
		PrintNetLog(0xffffffff,"(%d)PUDPService����UDPSend��OverLappedObjectʧ�ܣ�",GetID());
		return FALSE;
	}

	pOverLappedObject->SetAddress(IPAddress.GetSockAddr());

	pOverLappedObject->SetType(IO_SEND);	
	pOverLappedObject->SetIOCPEventRouter(m_pIOCPEventRouter);
	pOverLappedObject->GetDataBuff()->SetUsedSize(0);
	pOverLappedObject->SetParentID(GetID());

	if(!pOverLappedObject->GetDataBuff()->PushBack(pData,Size))
	{
		GetServer()->DeleteOverLappedObject(pOverLappedObject);
		PrintNetLog(0xffffffff,"(%d)PUDPServiceҪ���͵����ݰ�����",GetID());
		return FALSE;
	}
	

	static DWORD NumberOfBytes;
	static DWORD Flag;

	NumberOfBytes=0;
	Flag=0;

	if(m_Socket.SendToOverlapped(pOverLappedObject->GetAddress(),
		pOverLappedObject->GetDataBuff()->GetBuffer(),
		pOverLappedObject->GetDataBuff()->GetUsedSize(),
		NumberOfBytes,Flag,
		pOverLappedObject->GetOverlapped()))
	{				
		return TRUE;
	}
	PrintNetLog(0xffffffff,"(%d)PUDPService����UDPSend����ʧ�ܣ�",GetID());	
	GetServer()->DeleteOverLappedObject(pOverLappedObject);
	return FALSE;
}

void CNetPUDPService::OnRecvData(const CIPAddress& IPAddress,const CEasyBuffer& DataBuffer)
{
}

