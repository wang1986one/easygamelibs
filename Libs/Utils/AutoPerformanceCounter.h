#pragma once

class CAutoPerformanceCounter
{
protected:	
	int				m_Index;
	LARGE_INTEGER	m_PerformanceStart;
	LARGE_INTEGER	m_PerformanceEnd;
public:
	inline CAutoPerformanceCounter(int Index)
	{		
		m_Index=Index;
		QueryPerformanceCounter(&m_PerformanceStart);		
		CPerformanceStatistician::GetInstance()->StartPerformanceCount(m_Index);
	}
	inline ~CAutoPerformanceCounter(void)
	{
		QueryPerformanceCounter(&m_PerformanceEnd);
		CPerformanceStatistician::GetInstance()->
			AddPerformanceCount(m_Index,m_PerformanceEnd.QuadPart-m_PerformanceStart.QuadPart);
	}
};
