#pragma once


class CWinServiceController :
	public CNameObject
{
protected:
	SC_HANDLE		m_hSC;
	SC_HANDLE		m_hService;
	CEasyString		m_ServiceName;

	DECLARE_CLASS_INFO_STATIC(CWinServiceController)
public:
	enum SERVICE_STATUS_TYPE
	{
		SS_NONE=0,
		SS_STOPPED=SERVICE_STOPPED,
		SS_START_PENDING=SERVICE_START_PENDING,
		SS_STOP_PENDING=SERVICE_STOP_PENDING,
		SS_RUNNING=SERVICE_RUNNING,
		SS_CONTINUE_PENDING=SERVICE_CONTINUE_PENDING,
		SS_PAUSE_PENDING=SERVICE_PAUSE_PENDING,
		SS_PAUSED=SERVICE_PAUSED,
	};

	CWinServiceController(void);
	~CWinServiceController(void);

	bool OpenService(LPCTSTR szServiceName);
	void CloseService();

	bool StartupService(LPCTSTR * pStartArgs,UINT StartArgCount,UINT WaitTimeOut=0);
	bool ShutdownService(UINT WaitTimeOut=0);
	bool PauseService(UINT WaitTimeOut=0);
	bool ResumeService(UINT WaitTimeOut=0);

	bool WaitForStatus(int Status,UINT WaitTimeOut);

	int GetServiceStatus();
};
