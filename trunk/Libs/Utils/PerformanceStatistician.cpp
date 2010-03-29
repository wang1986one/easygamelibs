/****************************************************************************/
/*                                                                          */
/*      �ļ���:    PerformanceStatistician.cpp                              */
/*      ��������:  2010��02��09��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      ��������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ������������κ���ҵ�ͷ���ҵ������������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#include "StdAfx.h"
#include <vector>
#include <algorithm>

TCHAR PERFORMANCE_STATISTICIAN_INSTANCE[]="PerformanceStatisticianInstance";

CPerformanceStatistician::CPerformanceStatistician(void)
{
	ZeroMemory((void *)m_PerformanceCounts,sizeof(m_PerformanceCounts));
	ZeroMemory(m_PerformanceFunctionNames,sizeof(m_PerformanceFunctionNames));
	ZeroMemory((void *)m_PerformanceFunctionCalls,sizeof(m_PerformanceFunctionCalls));
	m_FunctionCount=0;
}

CPerformanceStatistician::~CPerformanceStatistician(void)
{
}

void CPerformanceStatistician::PrintPerformanceStat(int LogChannel)
{
	struct PERFORMANCE_INFO
	{
		UINT PerformanceCount;
		LPCTSTR PerformanceFunctionName;
		int PerformanceFunctionCall;

		static bool Sort(const PERFORMANCE_INFO& p1,const PERFORMANCE_INFO& p2)
		{
			return p1.PerformanceCount>p2.PerformanceCount;
		}
	};
	std::vector<PERFORMANCE_INFO> PerformanceList;
	CLogManager::GetInstance()->PrintLog(LogChannel,ILogPrinter::LOG_LEVEL_NORMAL,0,"����ͳ�ƿ�ʼ:");
	for(int i=0;i<MAX_PERFORMANCE_FUNCTION_NUM;i++)
	{
		if(m_PerformanceFunctionNames[i])
		{
			PERFORMANCE_INFO Info;
			Info.PerformanceCount=m_PerformanceCounts[i];
			Info.PerformanceFunctionName=m_PerformanceFunctionNames[i];
			Info.PerformanceFunctionCall=m_PerformanceFunctionCalls[i];
			PerformanceList.push_back(Info);
			
		}
	}
	std::sort(PerformanceList.begin(),PerformanceList.end(),PERFORMANCE_INFO::Sort);
	for(size_t i=0;i<PerformanceList.size();i++)
	{
		if(PerformanceList[i].PerformanceCount==0)
			break;
		CLogManager::GetInstance()->PrintLog(LogChannel,ILogPrinter::LOG_LEVEL_NORMAL,0,"    %80hs:   %u:%d",
			PerformanceList[i].PerformanceFunctionName,
			PerformanceList[i].PerformanceCount,
			PerformanceList[i].PerformanceFunctionCall);
	}
}

void CPerformanceStatistician::PrintPerformanceStatUnit(int LogChannel)
{
	struct PERFORMANCE_INFO
	{
		UINT PerformanceCount;
		LPCTSTR PerformanceFunctionName;		

		static bool Sort(const PERFORMANCE_INFO& p1,const PERFORMANCE_INFO& p2)
		{
			return p1.PerformanceCount>p2.PerformanceCount;
		}
	};
	std::vector<PERFORMANCE_INFO> PerformanceList;
	CLogManager::GetInstance()->PrintLog(LogChannel,ILogPrinter::LOG_LEVEL_NORMAL,0,"����ͳ�ƿ�ʼ(��λ):");
	for(int i=0;i<MAX_PERFORMANCE_FUNCTION_NUM;i++)
	{
		if(m_PerformanceFunctionNames[i]&&m_PerformanceFunctionCalls[i])
		{
			PERFORMANCE_INFO Info;
			Info.PerformanceCount=m_PerformanceCounts[i]/m_PerformanceFunctionCalls[i];
			Info.PerformanceFunctionName=m_PerformanceFunctionNames[i];			
			PerformanceList.push_back(Info);

		}
	}
	std::sort(PerformanceList.begin(),PerformanceList.end(),PERFORMANCE_INFO::Sort);
	for(size_t i=0;i<PerformanceList.size();i++)
	{
		CLogManager::GetInstance()->PrintLog(LogChannel,ILogPrinter::LOG_LEVEL_NORMAL,0,"    %80hs:   %u",
			PerformanceList[i].PerformanceFunctionName,
			PerformanceList[i].PerformanceCount);
	}
}

void CPerformanceStatistician::ResetPerformanceStat()
{
	ZeroMemory((void *)m_PerformanceCounts,sizeof(m_PerformanceCounts));
	//ZeroMemory(m_PerformanceFunctionNames,sizeof(m_PerformanceFunctionNames));
	ZeroMemory((void *)m_PerformanceFunctionCalls,sizeof(m_PerformanceFunctionCalls));
}




