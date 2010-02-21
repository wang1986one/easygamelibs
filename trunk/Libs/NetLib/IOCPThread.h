#pragma once


class CIOCPThread :
	public CEasyThread
{
protected:
	HANDLE			m_hIOCP;	
	
	DECLARE_CLASS_INFO_STATIC(CIOCPThread)
public:
	CIOCPThread(void);
	virtual ~CIOCPThread(void);

	void SetIOCPHandle(HANDLE hIOCP);
protected:	
	virtual BOOL OnStart();
	virtual BOOL OnRun();
	virtual void OnTerminate();
};

inline void CIOCPThread::SetIOCPHandle(HANDLE hIOCP)
{
	m_hIOCP=hIOCP;
}