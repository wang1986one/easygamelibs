#pragma once



class CServerApp :
	public CWinApp,public CNTService
{
protected:
public:
	CServerApp(void);
	~CServerApp(void);

	virtual BOOL InitInstance();

	virtual int ExitInstance();

	

	virtual void Run(DWORD argc, LPTSTR * argv);
	
	virtual void Stop();
public:

	virtual int Run();
	virtual BOOL OnStartUp();
	virtual void OnShutDown();

protected:
	void GetProgamVersion();

	//DECLARE_MESSAGE_MAP()

};
