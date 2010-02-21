#pragma once




class CServerApp
{
protected:
	volatile UINT		m_WantExist;
public:
	CServerApp(void);
	~CServerApp(void);

public:

	virtual BOOL OnStartUp();
	virtual void OnShutDown();

	virtual int Run();

protected:
	void GetProgamVersion();

};
