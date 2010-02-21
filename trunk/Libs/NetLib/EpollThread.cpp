#include "StdAfx.h"



IMPLEMENT_CLASS_INFO_STATIC(CEpollThread,CEasyThread);

CEpollThread::CEpollThread(void):CEasyThread()
{
	m_hEpoll=NULL;
}

CEpollThread::~CEpollThread(void)
{
}

BOOL CEpollThread::OnStart()
{
	PrintNetLog(0xff,"Epoll�����߳�����");
	return TRUE;
}

BOOL CEpollThread::OnRun()
{

	epoll_event Events[DEFAULT_EPOLL_EVENT_WAIT_NUM];

	int EventCount=epoll_wait(m_hEpoll,Events,DEFAULT_EPOLL_EVENT_WAIT_NUM,100);

	if(EventCount>0)
	{
		for(int i=0;i<EventCount;i++)
		{
			ULONG64_CONVERTER Param64;

			Param64.QuadPart=Events[i].data.u64;	

			CEpollEventRouter * pEpollEventRouter=(CEpollEventRouter *)Param64.LowPart;
			if(pEpollEventRouter)
			{
				pEpollEventRouter->OnEpollEvent(Events[i].events,Param64.HighPart);
			}
			else
			{
				PrintNetLog(0,"Epoll�¼��޷��ҵ�������");
			}
		}
	}
	else if(EventCount<0)
	{
		PrintNetLog(0,"epoll_wait��������%d",errno);
	}


	return TRUE;
}

void CEpollThread::OnTerminate()
{
	PrintNetLog(0xff,"Epoll�����߳��˳�");
}
