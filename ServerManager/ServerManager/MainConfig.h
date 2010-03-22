#pragma once



class CMainConfig :
	public CStaticObject<CMainConfig>
{
protected:
	CIPAddress					m_ListenAddress;
	UINT						m_MaxClient;
	
	CEasyArray<SERVICE_INFO>	m_ServiceInfoList;
	UINT						m_ProcessInfoFetchTime;
	UINT						m_MaxProcessCount;

	UINT						m_FileCacheSize;
	UINT						m_FileCacheQueryQueueSize;
	
public:
	CMainConfig(void);
	~CMainConfig(void);

	bool LoadConfig(LPCTSTR FileName);	
	
	CIPAddress& GetListenAddress()
	{
		return m_ListenAddress;
	}	
	UINT GetMaxClient()
	{
		return m_MaxClient;
	}
	
	CEasyArray<SERVICE_INFO>& GetServiceInfoList()
	{
		return m_ServiceInfoList;
	}

	UINT GetProcessInfoFetchTime()
	{
		return m_ProcessInfoFetchTime;
	}
	UINT GetMaxProcessCount()
	{
		return m_MaxProcessCount;
	}
	UINT GetFileCacheSize()
	{
		return m_FileCacheSize;
	}
	UINT GetFileCacheQueryQueueSize()
	{
		return m_FileCacheQueryQueueSize;
	}
};
