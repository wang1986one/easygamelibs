#pragma once

enum SIMPLE_FILE_SERVER_STATUS
{
	SFSS_WORKING,
	SFSS_INITIALIZING,
	SFSS_BUSY,
	SFSS_TERMINATING,
};

class CMainThread :
	public CServerThread,public CStaticObject<CMainThread>
{
protected:	
	CTestService * m_pService;
	//CSimpleFileService *	m_pSimpleFileService;
	int						m_Status;
public:
	CMainThread(void);
	virtual ~CMainThread(void);

	virtual BOOL OnStart();

	virtual int Update(int ProcessPacketLimit=DEFAULT_SERVER_PROCESS_PACKET_LIMIT);
	
	virtual void OnBeginTerminate();
	virtual BOOL OnTerminating();	

	virtual void OnTerminate();

	

	virtual int GetClientCount();
	
	virtual LPCTSTR GetConfigFileName();

	void SetServerStatus(int Status);
	int GetServerStatus();
	
protected:
	

};


inline void CMainThread::SetServerStatus(int Status)
{
	m_Status=Status;
}
inline int CMainThread::GetServerStatus()
{
	return m_Status;
}