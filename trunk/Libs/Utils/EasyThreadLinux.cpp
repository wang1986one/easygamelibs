/****************************************************************************/
/*                                                                          */
/*      �ļ���:    EasyThreadLinux.cpp                                      */
/*      ��������:  2009��09��11��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#include "StdAfx.h"

IMPLEMENT_CLASS_INFO_STATIC(CEasyThread,CNameObject);

CEasyThread::CEasyThread():CNameObject()
{
	m_ThreadID=0;	
	m_Status=THREAD_STATUS_TERMINATED;
	m_WantTerminate=FALSE;
}

CEasyThread::~CEasyThread()
{
	SafeTerminate(DEFAULT_THREAD_TERMINATE_TIME);	
}


BOOL CEasyThread::Start(BOOL IsSuspended,DWORD StartWaitTime)
{	
	int Flag = 0;	

	if(IsWorking())
		return FALSE;	


	m_Status=THREAD_STATUS_STARTING;
	m_WantTerminate=FALSE;

	if(pthread_create(&m_ThreadID,NULL,ThreadProc,this)!=0)
	{
		return FALSE;
	}	
	pthread_detach(m_ThreadID);	
	WaitForWorking(StartWaitTime);

	return TRUE;
}


BOOL CEasyThread::Resume()
{
	return FALSE;
}


BOOL CEasyThread::Suspend()
{
	return FALSE;
}


void CEasyThread::Terminate()
{
	m_WantTerminate=TRUE;
	if(m_Status!=THREAD_STATUS_TERMINATED)
		m_Status=THREAD_STATUS_ENDING
}


void CEasyThread::ForceTerminate()
{
	if(m_ThreadID)
	{
		pthread_cancel(m_ThreadID);
		OnTerminate();	
		m_ThreadID=0;
	}
	
}

void CEasyThread::SafeTerminate(DWORD Milliseconds)
{	
	Terminate();
	if(!WaitForTerminate(Milliseconds))
		ForceTerminate();	
	m_ThreadID=0;
}

BOOL CEasyThread::WaitForWorking(DWORD Milliseconds)
{	
	if(Milliseconds==INFINITE)
	{	
		while(m_Status==THREAD_STATUS_STARTING)
		{
			DoSleep(10);
		}
		return TRUE;
	}
	else
	{		
		CEasyTimer Timer;
		Timer.SetTimeOut(Milliseconds);
		while(!Timer.IsTimeOut())
		{			
			if(m_Status!=THREAD_STATUS_STARTING)
				return TRUE;
			DoSleep(10);
		}
		return FALSE;
	}
}

BOOL CEasyThread::WaitForTerminate(DWORD Milliseconds)
{
	if(Milliseconds==INFINITE)
	{	
		while(m_Status!=THREAD_STATUS_TERMINATED)
		{
			DoSleep(10);
		}
		return TRUE;
	}
	else
	{		
		CEasyTimer Timer;
		Timer.SetTimeOut(Milliseconds);
		while(!Timer.IsTimeOut())
		{			
			if(m_Status==THREAD_STATUS_TERMINATED)
				return TRUE;
			DoSleep(10);
		}
		return FALSE;
	}
}

BOOL CEasyThread::OnStart()
{
	return TRUE;
}

void CEasyThread::Execute()
{
	while((!m_WantTerminate)&&(OnRun()))
	{
	}	
}

BOOL CEasyThread::OnRun()
{
	return TRUE;
}

void CEasyThread::OnTerminate()
{
	
}


LPVOID CEasyThread::ThreadProc(LPVOID pParam)
{
#ifdef USE_THREAD_EXCEPTION_CATCH
	__try
	{
#endif
	
		CEasyThread * pThread=(CEasyThread *)pParam;

		pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);
		
		if(pThread->OnStart())
		{
			pThread->m_Status=THREAD_STATUS_WORKING=);
			pThread->Execute();
		}
		pThread->m_Status=THREAD_STATUS_ENDING;
		pThread->OnTerminate();
		pThread->m_Status=THREAD_STATUS_TERMINATED;				
		
		return NULL;
#ifdef USE_THREAD_EXCEPTION_CATCH
	}__except(CExceptionParser::ExceptionHander(GetExceptionInformation()))
	{
		exit(0);
	}	
	return 0;
#endif
}