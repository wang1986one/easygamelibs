/****************************************************************************/
/*                                                                          */
/*      �ļ���:    PerformanceStatistician.h                                */
/*      ��������:  2010��02��09��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once
#define MAX_PERFORMANCE_FUNCTION_NUM	2048

extern TCHAR PERFORMANCE_STATISTICIAN_INSTANCE[];

class CPerformanceStatistician 
	:public CStaticObject2< CPerformanceStatistician,PERFORMANCE_STATISTICIAN_INSTANCE >
{
protected:
	volatile UINT			m_PerformanceCounts[MAX_PERFORMANCE_FUNCTION_NUM];
	LPCTSTR					m_PerformanceFunctionNames[MAX_PERFORMANCE_FUNCTION_NUM];
	volatile UINT			m_PerformanceFunctionCalls[MAX_PERFORMANCE_FUNCTION_NUM];
	volatile int			m_FunctionCount;
	CEasyCriticalSection	m_EasyCriticalSection;
	
public:
	CPerformanceStatistician(void);
	~CPerformanceStatistician(void);

	void PrintPerformanceStat(int LogChannel);
	void PrintPerformanceStatUnit(int LogChannel);

	void ResetPerformanceStat();
	

	void StartPerformanceCount(UINT Index);
	void AddPerformanceCount(UINT Index,UINT PerformanceCount);

	int GetFunctionIndex(int OldIndex,LPCTSTR FunctionName);

	
};


inline void CPerformanceStatistician::StartPerformanceCount(UINT Index)
{
	if(Index<MAX_PERFORMANCE_FUNCTION_NUM)
	{		

		AtomicInc(m_PerformanceFunctionCalls+Index);
	}
}

inline void CPerformanceStatistician::AddPerformanceCount(UINT Index,UINT PerformanceCount)
{
	if(Index<MAX_PERFORMANCE_FUNCTION_NUM)
	{
		AtomicAdd(m_PerformanceCounts+Index,PerformanceCount);
	}
}

inline int CPerformanceStatistician::GetFunctionIndex(int OldIndex,LPCTSTR FunctionName)
{
	if(OldIndex==0)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		if(OldIndex==0)
		{
			m_FunctionCount++;
			m_PerformanceFunctionNames[m_FunctionCount]=FunctionName;
			return m_FunctionCount;
		}
		
	}
	return OldIndex;
}

