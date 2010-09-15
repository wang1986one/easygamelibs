/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DBTransationManager.cpp                                  */
/*      ��������:  2009��09��11��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#include "StdAfx.h"


namespace DBLib
{

IMPLEMENT_CLASS_INFO_STATIC(CDBTransationManager,CNameObject);
CDBTransationManager::CDBTransationManager(void)
{
	m_pDatabase=NULL;
	m_ExecTimes=0;
	m_RealExecTime=0;
	m_AvgExecTime=0;
	m_ExecTimesPerSec=0;
}

CDBTransationManager::~CDBTransationManager(void)
{
	Destory();
}

void CDBTransationManager::Destory()
{
	for(int i=0;i<(int)m_WorkThreads.size();i++)
	{
		if(m_WorkThreads[i])
			m_WorkThreads[i]->SafeTerminate(DEFAULT_THREAD_TERMINATE_TIME);
		SAFE_RELEASE(m_WorkThreads[i]);
	}
	m_WorkThreads.clear();
}

bool CDBTransationManager::Init(IDatabase * pDatabase,LPCTSTR szConnectStr,int ThreadCount,int QueueSize,UINT Flag)
{
	if(pDatabase==NULL)
		return false;
	Destory();
	m_pDatabase=pDatabase;
	m_Flag=Flag;

	for(int i=0;i<ThreadCount;i++)
	{
		IDBConnection * pConnection=m_pDatabase->CreateConnection();
		if(pConnection->Connect(szConnectStr)!=DBERR_SUCCEED)
		{
			PrintDBLog(0xff,"[%u]���ݿ��޷����ӣ�����ʼ������",GetID());
		}
		CDBTransationWorkThread * pThread=new CDBTransationWorkThread(this);
		if(!pThread->Init(pConnection,szConnectStr,QueueSize))
		{
			SAFE_RELEASE(pThread);
			return false;
		}
		m_WorkThreads.push_back(pThread);

	}


	m_PerformanceCountTimer.SaveTime();

	PrintDBLog(0xff,"[%u]һ��������%d�������߳�",GetID(),ThreadCount);

	return true;
}

CDBTransationWorkThread * CDBTransationManager::AllocWorkThread()
{
	CDBTransationWorkThread * pThread=NULL;
	UINT Len=0xffffffff;
	for(int i=0;i<(int)m_WorkThreads.size();i++)
	{
		if(m_WorkThreads[i]->GetQueueLen()<Len)
		{
			pThread=m_WorkThreads[i];
			Len=m_WorkThreads[i]->GetQueueLen();
		}
	}
	return pThread;
}

bool CDBTransationManager::AddTransaction(CDBTransaction * pDBTansaction)
{
	CDBTransationWorkThread * pThread=AllocWorkThread();
	if(pThread)
	{
		return pThread->AddTransaction(pDBTansaction);
	}
	else
		return false;
}



int CDBTransationManager::Update(int ProcessLimit)
{
	int ProcessCount=0;
	while(ProcessCount<ProcessLimit)
	{
		int Count=0;
		for(int i=0;i<(int)m_WorkThreads.size();i++)
		{
			CDBTransaction * pDBTansaction=m_WorkThreads[i]->PopFinishTransaction();
			if(pDBTansaction)
			{
				pDBTansaction->OnFinish();
				SAFE_RELEASE(pDBTansaction);
				ProcessCount++;
				Count++;
			}
		}
		if(Count==0)
			break;
	}

	if(m_PerformanceCountTimer.IsTimeOut(PERFORMANCE_COUNT_TIME))
	{
		m_PerformanceCountTimer.SaveTime();
		m_ExecTimesPerSec=(float)m_ExecTimes*1000/PERFORMANCE_COUNT_TIME;
		if(m_ExecTimes)
			m_AvgExecTime=(float)m_RealExecTime/m_ExecTimes;
		else
			m_AvgExecTime=0;
		m_RealExecTime=0;
		m_ExecTimes=0;
		if(m_Flag&DBTM_FLAG_LOG_PERFORMANCE)
		{
			PrintDBLog(0xff,"[%u]ƽ��ִ��ʱ��=%g����,ÿ��ִ�д���%g",GetID(),m_AvgExecTime,m_ExecTimesPerSec);
		}
	}

	return ProcessCount;
}

bool CDBTransationManager::IsIdle()
{
	int Len=0;
	for(int i=0;i<(int)m_WorkThreads.size();i++)
	{
		Len+=m_WorkThreads[i]->GetQueueLen()+m_WorkThreads[i]->GetFinishQueueLen();
	}
	return Len==0;
}

}
