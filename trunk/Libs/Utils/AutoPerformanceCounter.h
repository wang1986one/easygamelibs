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

//inline __int64 QueryPerformanceCounterFast()
//{
//	__asm{RDTSC}
//}

class CAutoPerformanceCounter
{
protected:	
	CPerformanceStatistician *	m_pPS;
	int							m_Index;
	UINT64						m_PerformanceStart;
	UINT64						m_PerformanceEnd;
public:
	inline CAutoPerformanceCounter(CPerformanceStatistician * pPS,int Index)
	{		
		m_pPS=pPS;
		m_Index=Index;
		//m_PerformanceStart.QuadPart=QueryPerformanceCounterFast();
		//QueryPerformanceCounter(&m_PerformanceStart);		
		m_PerformanceStart=CEasyTimerEx::GetTime();
		m_pPS->StartPerformanceCount(m_Index);
	}
	inline ~CAutoPerformanceCounter(void)
	{
		//m_PerformanceEnd.QuadPart=QueryPerformanceCounterFast();
		//QueryPerformanceCounter(&m_PerformanceEnd);
		m_PerformanceEnd=CEasyTimerEx::GetTime();
		m_pPS->AddPerformanceCount(m_Index,(UINT)((m_PerformanceEnd-m_PerformanceStart)/PERFORMANCE_UNIT));
	}
};


#ifdef PERFORMANCE_STAT

#define BLOCK_BEGIN(Name)			{static int s_BlockIndex=0;static CPerformanceStatistician * s_pPS=NULL;if(!s_BlockIndex) {s_pPS=CPerformanceStatistician::GetInstance();s_BlockIndex=s_pPS->GetFunctionIndex(s_BlockIndex,Name);} CAutoPerformanceCounter tmpPerformanceCounter(s_pPS,s_BlockIndex);
#define BLOCK_END					}

#define FUNCTION_BEGIN				static int s_FunctionIndex=0;static CPerformanceStatistician * s_pPS=NULL;if(!s_FunctionIndex) {s_pPS=CPerformanceStatistician::GetInstance();s_FunctionIndex=s_pPS->GetFunctionIndex(s_FunctionIndex,__FUNCTION__);} CAutoPerformanceCounter tmpPerformanceCounter(s_pPS,s_FunctionIndex);
#define FUNCTION_END

#else

#define BLOCK_BEGIN(Name)
#define BLOCK_END

#define FUNCTION_BEGIN
#define FUNCTION_END

#endif