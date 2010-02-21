#pragma once

class CNetService;

class CIOCPListenThread :
	public CEasyThread
{
protected:
	SOCKET			m_ListenSocket;
	CNetService*	m_pServer;
public:
	CIOCPListenThread(void);
	virtual ~CIOCPListenThread(void);

	bool Init(CNetService * pService,SOCKET ListenSocket);
protected:	
	virtual BOOL OnStart();
	virtual BOOL OnRun();
	virtual void OnTerminate();

};


