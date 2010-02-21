#pragma once


class CEpollThread :
	public CEasyThread
{
protected:
	UINT			m_hEpoll;	
	
	DECLARE_CLASS_INFO_STATIC(CEpollThread)
public:
	CEpollThread(void);
	virtual ~CEpollThread(void);

	void SetEpollHandle(UINT hEpoll);
protected:	
	virtual BOOL OnStart();
	virtual BOOL OnRun();
	virtual void OnTerminate();
};

inline void CEpollThread::SetEpollHandle(UINT hEpoll)
{
	m_hEpoll=hEpoll;
}