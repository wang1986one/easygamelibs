/****************************************************************************/
/*                                                                          */
/*      文件名:    AutoPerformanceCounter.h                                 */
/*      创建日期:  2010年02月09日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
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
