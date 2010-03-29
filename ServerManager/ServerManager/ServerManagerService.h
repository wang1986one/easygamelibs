/****************************************************************************/
/*                                                                          */
/*      文件名:    ServerManagerService.h                                   */
/*      创建日期:  2010年02月09日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once

class CServerManagerService :
	public CNetService
{
protected:
	CIDStorage<CServerManagerClient>			m_ClientPool;

	CEasyArray<SERVICE_INFO>					m_ProcessInfoList;
	CEasyArray<DWORD>							m_ProcessIDList;
	CEasyTimer									m_ProcessInfoFetchTimer;

	CEasyArray<SERVICE_INFO>					m_ServiceInfoList;
	UINT64										m_RecentTime;
	
	CEasyArray<NET_ADAPTER_INFO>				m_NetAdapterInfos;

	DECLARE_CLASS_INFO_STATIC(CServerManagerService);
public:
	CServerManagerService(void);
	~CServerManagerService(void);

	BOOL Init(CNetServer * pServer);
	virtual void Destory();

	virtual CBaseTCPConnection * CreateConnection(CIPAddress& RemoteAddress);
	virtual BOOL DeleteConnection(CBaseTCPConnection * pConnection);

	virtual int Update(int ProcessPacketLimit=DEFAULT_SERVER_PROCESS_PACKET_LIMIT);

	UINT GetClientCount();

	CEasyArray<SERVICE_INFO>& GetServiceInfoList();
	CEasyArray<SERVICE_INFO>& GetProcessInfoList();
	CEasyArray<NET_ADAPTER_INFO>& GetNetAdapterInfo();

	bool StartupService(UINT ServiceIndex);
	bool ShutdownService(UINT ServiceIndex);

	bool StartupProcess(SERVICE_INFO& ServiceInfo);
	bool ShutdownProcess(SERVICE_INFO& ServiceInfo);

protected:
	BOOL DoListen(CIPAddress& ListenAddress);
	void FetchProcessInfo();
	void InitNetAdapterInfo();
	void UpdateNetAdapterInfo(UINT64 TimeSpan);
	
};


inline UINT CServerManagerService::GetClientCount()
{
	return m_ClientPool.GetObjectCount();
}

inline CEasyArray<SERVICE_INFO>& CServerManagerService::GetServiceInfoList()
{
	return m_ServiceInfoList;
}

inline CEasyArray<SERVICE_INFO>& CServerManagerService::GetProcessInfoList()
{
	return m_ProcessInfoList;
}

inline CEasyArray<NET_ADAPTER_INFO>& CServerManagerService::GetNetAdapterInfo()
{
	return m_NetAdapterInfos;
}