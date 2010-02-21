#pragma once


class CGuardThread :
	public CEasyThread
{
protected:
	UINT					m_RecentKeepAliveTime;
	UINT					m_KeepAliveTime;
	UINT					m_LostAliveCount;
	UINT					m_MaxLostAliveCount;
	UINT					m_TargetThreadID;
	LPCTSTR					m_RecentSourceFileName;
	int						m_RecentSourceLine;
	CEasyCriticalSection	m_EasyCriticalSection;

	DECLARE_CLASS_INFO_STATIC(CGuardThread)
public:
	CGuardThread(void);
	~CGuardThread(void);

	void SetKeepAliveTime(UINT Time,UINT MaxLostAliveCount)
	{
		m_KeepAliveTime=Time;
		m_MaxLostAliveCount=MaxLostAliveCount;
	}
	void SetTargetThreadID(UINT ThreadID)
	{
		m_TargetThreadID=ThreadID;
	}

	void MakeKeepAlive()
	{
		CAutoLock Lock(m_EasyCriticalSection);
		m_RecentKeepAliveTime=CEasyTimer::GetTime();
		m_LostAliveCount=0;
	}

	void ReportRecentFunction(LPCTSTR SourceFileName,int Line)
	{
		m_RecentSourceFileName=SourceFileName;
		m_RecentSourceLine=Line;
	}

protected:
	virtual BOOL OnStart();
	virtual BOOL OnRun();
	virtual void OnTerminate();
};
