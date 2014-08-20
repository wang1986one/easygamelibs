/****************************************************************************/
/*                                                                          */
/*      �ļ���:    NetServerEpoll.cpp                                       */
/*      ��������:  2009��09��11��                                           */
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
	m_hEpoll=INVALID_HANDLE_VALUE;
	m_pEpollThreads=NULL;
	m_EpollThreadCount=0;
	m_EventObjectPoolSize=MAX_EVENT_OBJECT;
	m_EventObjectPoolGrowSize=DEFAULT_EVENT_OBJECT_POOL_GROW_SIZE;
	m_EventObjectPoolGrowLimit=DEFAULT_EVENT_OBJECT_POOL_GROW_LIMIT;
	m_EventRouterPoolSize=DEFAULT_EVENT_ROUTER_COUNT;
	m_EventRouterPoolGrowSize=DEFAULT_EVENT_ROUTER_POOL_GROW_SIZE;
	m_EventRouterPoolGrowLimit=DEFAULT_EVENT_ROUTER_POOL_GROW_LIMIT;
	m_EpollThreadNumPerCPU=DEFAULT_THREAD_NUMBER_PER_CPU;
}

CNetServer::~CNetServer(void)
{
	ShutDown();
}

BOOL CNetServer::StartUp(int EventObjectPoolSize,
						 int ThreadNumberPerCPU,
						 int EventRouterPoolSiz,
						 int EventObjectPoolGrowSize,
						 int EventObjectPoolGrowLimit,
						 int EventRouterPoolGrowSize,
						 int EventRouterPoolGrowlimit)
{
	m_EventObjectPoolSize=EventObjectPoolSize;
	m_EventRouterPoolSize=EventRouterPoolSiz;
	m_EpollThreadNumPerCPU=ThreadNumberPerCPU;
	m_EventObjectPoolGrowSize=EventObjectPoolGrowSize;
	m_EventObjectPoolGrowLimit=EventObjectPoolGrowLimit;
	m_EventRouterPoolGrowSize=EventRouterPoolGrowSize;
	m_EventRouterPoolGrowLimit=EventRouterPoolGrowlimit;
	return Start();
}

void CNetServer::ShutDown(DWORD Milliseconds)
{
	SafeTerminate(Milliseconds);
}

BOOL CNetServer::OnStart()
{
	if(!CBaseServer::OnStart())
		return FALSE;

	if(m_hEpoll!=INVALID_HANDLE_VALUE)
		return FALSE;

	m_EpollEventObjectPool.Create(m_EventObjectPoolSize,m_EventObjectPoolGrowSize,m_EventObjectPoolGrowLimit);
	m_EventRouterPool.Create(m_EventRouterPoolSize,m_EventRouterPoolGrowSize,m_EventRouterPoolGrowLimit);

	m_hEpoll = epoll_create( m_EventRouterPoolSize );
	if( m_hEpoll == INVALID_HANDLE_VALUE )
	{
		PrintNetLog(0xffffffff,"(%d)����Epollʧ��(%d)��",GetID(),GetLastError());
		return FALSE;
	}

	m_EpollThreadCount = GetSystemCPUCount() * m_EpollThreadNumPerCPU;

	m_pEpollThreads = new CEpollThread[m_EpollThreadCount](this);

	for( int i = 0;i < m_EpollThreadCount;i ++ )
	{
		m_pEpollThreads[i].SetEpollHandle(m_hEpoll);
		m_pEpollThreads[i].Start();
	}
	if(!OnStartUp())
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CNetServer::OnRun()
{
	if(!CBaseServer::OnRun())
		return FALSE;

	if(Update()==0)
	{
		DoSleep(1);
	}
	return TRUE;
}

void CNetServer::OnTerminate()
{
	if( m_pEpollThreads )
	{
		for( int i = 0;i < m_EpollThreadCount;i ++ )
		{
			m_pEpollThreads[i].SafeTerminate();
		}
		delete[] m_pEpollThreads;
		m_pEpollThreads = NULL;
	}

	OnShutDown();

	if(m_hEpoll != INVALID_HANDLE_VALUE)
	{
		close( m_hEpoll );
		m_hEpoll = INVALID_HANDLE_VALUE;
	}

	if(m_EpollEventObjectPool.GetObjectCount())
	{
		PrintNetLog(0xffffffff,"(%d)�رգ���ʼͳ��Objectʹ��״����",GetID());
		PrintObjectStatus();
	}

	m_EpollEventObjectPool.Destory();
	m_EventRouterPool.Destory();
}

CEpollEventObject * CNetServer::CreateEventObject()
{


	CEpollEventObject * pEpollEventObject=NULL;

	pEpollEventObject=m_EpollEventObjectPool.NewObject();
	if(pEpollEventObject)
	{
		if(pEpollEventObject->GetParentID())
		{
			PrintImportantLog(0,"������δ�ͷŵ�EpollEventObject");
		}
		pEpollEventObject->Create(this);

		return pEpollEventObject;
	}
	PrintNetLog(0xffffffff,"(%d)Server�޷�����EpollEventObject��",GetID());



	return NULL;
}

BOOL CNetServer::DeleteEventObject(CEpollEventObject * pEpollEventObject)
{
	pEpollEventObject->Destory();
	if(!m_EpollEventObjectPool.DeleteObject(pEpollEventObject->GetID()))
	{
		PrintNetLog(0xffffffff,"(%d)Server�޷�ɾ��EpollEventObject(%d)��",GetID(),pEpollEventObject->GetID());
		return FALSE;
	}

	return TRUE;
}

CEpollEventRouter * CNetServer::CreateEventRouter()
{

	CEpollEventRouter * pEventRouter=NULL;
	UINT ID=0;

	ID=m_EventRouterPool.NewObject(&pEventRouter);

	if(pEventRouter)
	{
		pEventRouter->SetID(ID);
		return pEventRouter;
	}
	PrintNetLog(0xffffffff,"(%d)Server�޷�����EpollEventRouter��",GetID());
	return NULL;
}

CEpollEventRouter * CNetServer::GetEventRouter(UINT ID)
{
	return m_EventRouterPool.GetObject(ID);
}

BOOL CNetServer::DeleteEventRouter(CEpollEventRouter * pEventRouter)
{

	pEventRouter->Destory();
	if(!m_EventRouterPool.DeleteObject(pEventRouter->GetID()))
	{
		PrintNetLog(0xffffffff,"(%d)Server�޷�ɾ��EpollEventRouter(%d)��",GetID(),pEventRouter->GetID());
		return FALSE;
	}

	return TRUE;
}


BOOL CNetServer::BindSocket(SOCKET Socket,CEpollEventRouter * pEpollEventRouter)
{
	if(m_hEpoll == INVALID_HANDLE_VALUE)
	{
		PrintNetLog(0xffffffff,"(%d)Epollû�г�ʼ��,�޷���Socket��",GetID());
		return FALSE;
	}
	if(Socket == INVALID_SOCKET)
	{
		PrintNetLog(0xffffffff,"(%d)Socketû�г�ʼ��,�޷���Socket��",GetID());
		return FALSE;
	}

	epoll_event EpollEvent;
	ZeroMemory(&EpollEvent,sizeof(EpollEvent));
	ULONG64_CONVERTER Param64;
	Param64.LowPart=(DWORD)pEpollEventRouter->GetID();
	Param64.HighPart=(DWORD)(pEpollEventRouter->GetSessionID());
	EpollEvent.data.u64=Param64.QuadPart;
	EpollEvent.events=EPOLLIN|EPOLLOUT|EPOLLERR|EPOLLHUP|EPOLLET;
	if(epoll_ctl(m_hEpoll,EPOLL_CTL_ADD,Socket,&EpollEvent)==0)
		return TRUE;
	else
		return FALSE;
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

	LPVOID Pos=m_EpollEventObjectPool.GetFirstObjectPos();
	while(Pos)
	{
		CEpollEventObject * pObject=m_EpollEventObjectPool.GetNext(Pos);
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

	UINT UsedCount=0,FreeCount=0;

	m_EpollEventObjectPool.Verfy(UsedCount,FreeCount);

	PrintNetLog(0,"��%d(%u,%u,%u)��EpollEventObject����ʹ����,����Accept=%d,Recv=%d,Send=%d,Other=%d",
		m_EpollEventObjectPool.GetObjectCount(),
		UsedCount,FreeCount,UsedCount+FreeCount,
		AcceptCount,
		RecvCount,
		SendCount,
		OtherCount);
	PrintNetLog(0,"��%d��EventRouterʹ����",m_EventRouterPool.GetObjectCount());
}
