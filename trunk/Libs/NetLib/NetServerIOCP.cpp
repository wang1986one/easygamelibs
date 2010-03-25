/****************************************************************************/
/*                                                                          */
/*      �ļ���:    NetServerIOCP.cpp                                        */
/*      ��������:  2009��07��06��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#include "StdAfx.h"

IMPLEMENT_CLASS_INFO_STATIC(CNetServer,CBaseServer);

CNetServer::CNetServer(void):CBaseServer()
{
	CNetSocket::NetStartup();
	m_hIOCP=NULL;	
	m_pIOCPThreads=NULL;	
	m_IOCPThreadCount=0;
	m_IOCPObjectPoolSize=MAX_EVENT_OBJECT;
	m_EventRouterPoolSiz=DEFAULT_EVENT_ROUTER_COUNT;
	m_IOCPThreadNumPerCPU=DEFAULT_THREAD_NUMBER_PER_CPU;	
}

CNetServer::~CNetServer(void)
{
	ShutDown();	
	CNetSocket::NetCleanup();
}

BOOL CNetServer::StartUp(int EventObjectPoolSize,int ThreadNumberPerCPU,int EventRouterPoolSiz)
{
	m_IOCPObjectPoolSize=EventObjectPoolSize;
	m_EventRouterPoolSiz=EventRouterPoolSiz;
	m_IOCPThreadNumPerCPU=ThreadNumberPerCPU;
	return Start();	
}

void CNetServer::ShutDown(DWORD Milliseconds)
{	
	SafeTerminate(Milliseconds);	
}

BOOL CNetServer::OnStart()
{
	if(m_hIOCP!=NULL)
		return FALSE;

	CNetSocket::NetStartup();

	m_OverLappedObjectPool.Create(m_IOCPObjectPoolSize);
	m_EventRouterPool.Create(m_EventRouterPoolSiz);

	m_hIOCP = CreateIoCompletionPort( INVALID_HANDLE_VALUE, NULL, 0, 0 );
	if( m_hIOCP == NULL )
	{
		PrintNetLog(0xffffffff,"(%d)������ɶ˿�ʧ��(%d)��",GetID(),GetLastError());		
		return FALSE;
	}

	SYSTEM_INFO	si;
	GetSystemInfo( &si );
	m_IOCPThreadCount = si.dwNumberOfProcessors * m_IOCPThreadNumPerCPU;

	m_pIOCPThreads = new CIOCPThread[m_IOCPThreadCount];

	for( int i = 0;i < m_IOCPThreadCount;i ++ )
	{
		m_pIOCPThreads[i].SetIOCPHandle(m_hIOCP);
		m_pIOCPThreads[i].Start();
	}	
	if(!OnStartUp())
	{		
		return FALSE;
	}
	return TRUE;
}

BOOL CNetServer::OnRun()
{
	if(Update()==0)
	{
		DoSleep(1);
	}
	return TRUE;
}

void CNetServer::OnTerminate()
{	
	if( m_pIOCPThreads )
	{
		for( int i = 0;i < m_IOCPThreadCount;i ++ )
		{			
			m_pIOCPThreads[i].SafeTerminate();
		}
		delete[] m_pIOCPThreads;
		m_pIOCPThreads = NULL;
	}		

	OnShutDown();

	CloseHandle( m_hIOCP );
	m_hIOCP = NULL;	
	if(m_OverLappedObjectPool.GetObjectCount())
	{		
		PrintNetLog(0xffffffff,"(%d)�رգ���ʼͳ��OverLappedObjectʹ��״����",GetID());
		PrintObjectStatus();
	}

	m_OverLappedObjectPool.Destory();	
	m_EventRouterPool.Destory();

	CNetSocket::NetCleanup();
}

COverLappedObject * CNetServer::CreateOverLappedObject()
{
	

	COverLappedObject * pOverLappedObject=NULL;

	pOverLappedObject=m_OverLappedObjectPool.NewObject();
	if(pOverLappedObject)
	{
		if(pOverLappedObject->GetParentID())
		{
			PrintImportantLog(0,"������δ�ͷŵ�OverLappedObject");
		}
		pOverLappedObject->Create(this);
		
		return pOverLappedObject;
	}
	PrintNetLog(0xffffffff,"(%d)Server�޷�����COverLappedObject��",GetID());



	return NULL;
}

BOOL CNetServer::DeleteOverLappedObject(COverLappedObject * pOverLappedObject)
{	
	pOverLappedObject->Destory();
	if(!m_OverLappedObjectPool.DeleteObject(pOverLappedObject->GetID()))
	{
		PrintNetLog(0xffffffff,"(%d)Server�޷�ɾ��COverLappedObject(%d)��",GetID(),pOverLappedObject->GetID());
		return FALSE;
	}	
	
	return TRUE;
}

CIOCPEventRouter * CNetServer::CreateEventRouter()
{

	CIOCPEventRouter * pEventRouter=NULL;
	UINT ID=0;	
	
	ID=m_EventRouterPool.NewObject(&pEventRouter);

	if(pEventRouter)
	{
		pEventRouter->SetID(ID);
		return pEventRouter;
	}
	PrintNetLog(0xffffffff,"(%d)Server�޷�����CIOCPEventRouter��",GetID());
	return NULL;
}

BOOL CNetServer::DeleteEventRouter(CIOCPEventRouter * pEventRouter)
{

	pEventRouter->Destory();
	if(!m_EventRouterPool.DeleteObject(pEventRouter->GetID()))
	{
		PrintNetLog(0xffffffff,"(%d)Server�޷�ɾ��CIOCPEventRouter(%d)��",GetID(),pEventRouter->GetID());
		return FALSE;
	}

	return TRUE;
}


BOOL CNetServer::BindSocket(SOCKET Socket)
{
	if(m_hIOCP == NULL)
	{
		PrintNetLog(0xffffffff,"(%d)��ɶ˿�û�г�ʼ��,�޷���Socket��",GetID());		
		return FALSE;
	}
	if(Socket == INVALID_SOCKET)
	{
		PrintNetLog(0xffffffff,"(%d)Socketû�г�ʼ��,�޷���Socket��",GetID());		
		return FALSE;
	}
	HANDLE hPort = CreateIoCompletionPort((HANDLE)Socket, m_hIOCP, 0, 0 );
	return TRUE;
}

BOOL CNetServer::BindFile(HANDLE FileHandle)
{
	if(m_hIOCP == NULL)
	{
		PrintNetLog(0xffffffff,"(%d)��ɶ˿�û�г�ʼ��,�޷���FileHandle��",GetID());		
		return FALSE;
	}
	if(FileHandle == INVALID_HANDLE_VALUE)
	{
		PrintNetLog(0xffffffff,"(%d)FileHandleû�г�ʼ��,�޷���FileHandle��",GetID());		
		return FALSE;
	}
	HANDLE hPort = CreateIoCompletionPort(FileHandle, m_hIOCP, 0, 0 );
	return TRUE;
}

BOOL CNetServer::OnStartUp()
{
	return TRUE;
}
void CNetServer::OnShutDown()
{
	
}

int CNetServer::Update(int ProcessPacketLimit)
{
	int ProcessCount=0;	
	return ProcessCount;
}


void CNetServer::PrintObjectStatus()
{

	int AcceptCount=0;
	int RecvCount=0;
	int SendCount=0;
	int OtherCount=0;
	
	LPVOID Pos=m_OverLappedObjectPool.GetFirstObjectPos();
	while(Pos)
	{
		COverLappedObject * pObject=m_OverLappedObjectPool.GetNext(Pos);
		switch(pObject->GetType())
		{
		case IO_RECV:
			RecvCount++;			
			break;
		case IO_SEND:
			SendCount++;			
			break;
		case IO_ACCEPT:
			AcceptCount++;			
			break;
		default:
			OtherCount++;			
		}
	}

	UINT UsedCount,FreeCount;

	m_OverLappedObjectPool.Verfy(UsedCount,FreeCount);

	PrintNetLog(0,"��%d(%u,%u,%u)��OverLapped����ʹ����,����Accept=%d,Recv=%d,Send=%d,Other=%d",
		m_OverLappedObjectPool.GetObjectCount(),
		UsedCount,FreeCount,UsedCount+FreeCount,
		AcceptCount,
		RecvCount,
		SendCount,
		OtherCount);
	PrintNetLog(0,"��%d��EventRouterʹ����",m_EventRouterPool.GetObjectCount());
}