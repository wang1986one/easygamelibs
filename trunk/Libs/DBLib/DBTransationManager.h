#pragma once

namespace DBLib
{

#define DEFAULT_TRANS_THREAD	4
#define DEFAULT_PROCESS_LIMIT	32
#define PERFORMANCE_COUNT_TIME	(5*60*1000)



class CDBTransationManager
	:public CNameObject
{
public:
	enum FLAG
	{
		DBTM_FLAG_LOG_PERFORMANCE=1,
	};
protected:
	std::vector<CDBTransationWorkThread *>	m_WorkThreads;
	IDatabase *								m_pDatabase;
	UINT									m_Flag;

	CEasyTimer								m_PerformanceCountTimer;
	volatile UINT							m_ExecTimes;
	volatile UINT							m_RealExecTime;

	float									m_AvgExecTime;
	float									m_ExecTimesPerSec;

	
	
	DECLARE_CLASS_INFO_STATIC(CDBTransationManager)
public:
	CDBTransationManager(void);
	~CDBTransationManager(void);

	void Destory();

	bool Init(IDatabase * pDatabase,LPCTSTR szConnectStr,int ThreadCount=DEFAULT_TRANS_THREAD,int QueueSize=DEFAULT_TRANS_QUEUE,UINT Flag=0);

	void ModifyFlag(UINT Add,UINT Remove)
	{
		m_Flag|=Add;
		m_Flag&=~Remove;
	}

	CDBTransationWorkThread * AllocWorkThread();
	
	bool AddTransaction(CDBTransaction * pDBTansaction);

	int Update(int ProcessLimit=DEFAULT_PROCESS_LIMIT);

	bool IsIdle();

	void AddExecTime(UINT ExecTime)
	{
		AtomicInc(&m_ExecTimes);
		AtomicAdd(&m_RealExecTime,ExecTime);
	}

	float GetAvgExecTime()
	{
		return m_AvgExecTime;
	}
	float GetExecTimesPerSec()
	{
		return m_ExecTimesPerSec;
	}
};

}