#include "StdAfx.h"


CThreadPerformanceCounter::CThreadPerformanceCounter(void)
{
	m_TreadHandle=NULL;
	m_CountIntervalTime=0;
	m_StartPerformanceCount=0;
	m_CycleCount=0;
	m_CPUCount=1;
	m_StartCPUUsedTime=0;
	m_PerformanceFrequency=1;
	m_CycleTime=0;
	m_CPUUsedRate=0;
}

CThreadPerformanceCounter::~CThreadPerformanceCounter(void)
{
}

void CThreadPerformanceCounter::Init(HANDLE ThreadHandle,UINT CountIntervalTime)
{
	m_TreadHandle=ThreadHandle;
	m_CountIntervalTime=CountIntervalTime;
	m_CountIntervalTimer.SaveTime();
	QueryPerformanceCounter((LARGE_INTEGER *)&m_StartPerformanceCount);
	QueryPerformanceFrequency((LARGE_INTEGER *)&m_PerformanceFrequency);
	SYSTEM_INFO	si;
	GetSystemInfo( &si );
	m_CPUCount=si.dwNumberOfProcessors;
	m_CycleCount=0;
	FILETIME CreationTime,ExitTime,KernelTime,UserTime;
	if(!GetThreadTimes(m_TreadHandle,
		&CreationTime,&ExitTime,
		&KernelTime,&UserTime))
	{
		PrintSystemLog(0,"获取线程CPU时间失败%d",GetLastError());
	}
	m_StartCPUUsedTime=((UINT64)KernelTime.dwLowDateTime)|(((UINT64)KernelTime.dwHighDateTime)<<32);
	m_StartCPUUsedTime+=((UINT64)UserTime.dwLowDateTime)|(((UINT64)UserTime.dwHighDateTime)<<32);
}

void CThreadPerformanceCounter::DoPerformanceCount()
{
	if(m_TreadHandle)
	{
		m_CycleCount++;
		if(m_CountIntervalTimer.IsTimeOut(m_CountIntervalTime))
		{
			m_CountIntervalTimer.SaveTime();

			UINT64 CurPerformanceCount;
			QueryPerformanceCounter((LARGE_INTEGER *)&CurPerformanceCount);
			float CPUTime=(float)(CurPerformanceCount-m_StartPerformanceCount)/m_PerformanceFrequency;		
			m_StartPerformanceCount=CurPerformanceCount;

			m_CycleTime=CPUTime*1000.0f/m_CycleCount;
			m_CycleCount=0;

			UINT64 CurCPUUsedTime;
			FILETIME CreationTime,ExitTime,KernelTime,UserTime;
			if(!GetThreadTimes(m_TreadHandle,
				&CreationTime,&ExitTime,
				&KernelTime,&UserTime))
			{
				PrintSystemLog(0,"获取线程CPU时间失败%d",GetLastError());
			}
			CurCPUUsedTime=((UINT64)KernelTime.dwLowDateTime)|(((UINT64)KernelTime.dwHighDateTime)<<32);
			CurCPUUsedTime+=((UINT64)UserTime.dwLowDateTime)|(((UINT64)UserTime.dwHighDateTime)<<32);

			m_CPUUsedRate=(float)(CurCPUUsedTime-m_StartCPUUsedTime)/(CPUTime*m_CPUCount*10000000.0f);

			m_StartCPUUsedTime=CurCPUUsedTime;
		}
	}
}