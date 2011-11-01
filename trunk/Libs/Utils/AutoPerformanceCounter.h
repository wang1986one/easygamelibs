/****************************************************************************/
/*                                                                          */
/*      �ļ���:    AutoPerformanceCounter.h                                 */
/*      ��������:  2010��02��09��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once

inline __int64 QueryPerformanceCounterFast()
{
	__asm{RDTSC}
}

class CAutoPerformanceCounter
{
protected:	
	CPerformanceStatistician *	m_pPS;
	int							m_Index;
	LARGE_INTEGER				m_PerformanceStart;
	LARGE_INTEGER				m_PerformanceEnd;
public:
	inline CAutoPerformanceCounter(CPerformanceStatistician * pPS,int Index)
	{		
		m_pPS=pPS;
		m_Index=Index;
		//m_PerformanceStart.QuadPart=QueryPerformanceCounterFast();
		QueryPerformanceCounter(&m_PerformanceStart);		
		m_pPS->StartPerformanceCount(m_Index);
	}
	inline ~CAutoPerformanceCounter(void)
	{
		//m_PerformanceEnd.QuadPart=QueryPerformanceCounterFast();
		QueryPerformanceCounter(&m_PerformanceEnd);
		m_pPS->AddPerformanceCount(m_Index,(UINT)((m_PerformanceEnd.QuadPart-m_PerformanceStart.QuadPart)/PERFORMANCE_UNIT));
	}
};


#define BLOCK_BEGIN(Name)			{static int s_BlockIndex=0;static CPerformanceStatistician * s_pPS=NULL;if(!s_BlockIndex) {s_pPS=CPerformanceStatistician::GetInstance();s_BlockIndex=s_pPS->GetFunctionIndex(s_BlockIndex,Name);} CAutoPerformanceCounter tmpPerformanceCounter(s_pPS,s_BlockIndex);
#define BLOCK_END					}