#include "StdAfx.h"
#include "GuardThread.h"

IMPLEMENT_CLASS_INFO_STATIC(CGuardThread,CEasyThread);

#define DEFAULT_GUARD_KEEPALIVE_TIME	(20*1000)
#define DEFAULT_MAX_LOST_ALIVE_COUNT	(5)

CGuardThread::CGuardThread(void)
{
	m_KeepAliveTime=DEFAULT_GUARD_KEEPALIVE_TIME;
	m_MaxLostAliveCount=DEFAULT_MAX_LOST_ALIVE_COUNT;
	m_TargetThreadID=0;
	m_RecentKeepAliveTime=CEasyTimer::GetTime();
	m_LostAliveCount=0;
	m_RecentSourceFileName=NULL;
	m_RecentSourceLine=0;
}

CGuardThread::~CGuardThread(void)
{
}


BOOL CGuardThread::OnStart()
{
	return TRUE;
}

BOOL CGuardThread::OnRun()
{
	{
		CAutoLock Lock(m_EasyCriticalSection);
		UINT CurTime=CEasyTimer::GetTime();
		if(GetTimeToTime(m_RecentKeepAliveTime,CurTime)>m_KeepAliveTime)
		{
			m_RecentKeepAliveTime=CurTime;
			m_LostAliveCount++;
			PrintImportantLog(0,"检测到线程[%u]失去响应,第%d次",m_TargetThreadID,m_LostAliveCount);
			if(m_LostAliveCount>m_MaxLostAliveCount)
			{
				PrintImportantLog(0,"检测到线程[%u]死锁，做Dump并关闭",m_TargetThreadID);
				PrintImportantLog(0,"最近函数位置[%s,%d]",m_RecentSourceFileName,m_RecentSourceLine);
#ifdef WIN32
				CExceptionParser::GetInstance()->WriteDump(NULL);
#endif
				exit(0);
			}
		}
	}
	DoSleep(100);
	return TRUE;
}

void CGuardThread::OnTerminate()
{

}
