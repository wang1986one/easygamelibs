#pragma once

class CServerManagerClientView;

class CServerConnection :
	public CNetConnection
{
protected:
	CServerManagerClientView	*m_pView;
	CEasyBuffer					m_AssembleBuffer;
	CEasyString					m_ServerAddress;
	UINT						m_ServerPort;
	CEasyTimer					m_ConnectTimer;
	CEasyTimer					m_ServiceInfoFetchTimer;
	
public:
	CServerConnection(CServerManagerClientView * pView,LPCTSTR szServerAddress,UINT ServerPort);
	~CServerConnection(void);

	virtual void OnConnection(BOOL IsSucceed);
	virtual void OnDisconnection();

	virtual void OnRecvData(const CEasyBuffer& DataBuffer);

	virtual int Update(int ProcessPacketLimit=DEFAULT_SERVER_PROCESS_PACKET_LIMIT);

	LPCTSTR GetServerAddress();
	UINT GetServerPort();

	void PrintLog(LPCTSTR szFormat,...);
	

	void OnMsg(CSmartStruct& Msg);

	void SendMsg(CSmartStruct& Msg);
	void SendMsg(UINT MsgID);	

	void QueryServiceList();
	void QueryStartupService(UINT ServiceIndex);
	void QueryShutDownService(UINT ServiceIndex);
	void QueryBrowseWorkDir(UINT ServiceIndex,LPCTSTR Dir);

	void QueryStartDownload(UINT ServiceIndex,LPCTSTR SourceFilePath,LPCTSTR TargetFilePath);
	void QueryDownloadData(UINT ServiceIndex);

	void QueryStartUpload(UINT ServiceIndex,LPCTSTR SourceFilePath,LPCTSTR TargetFilePath,time_t FileTime,UINT TotalSize,UINT OrgTotalSize,LPVOID pPackProps,UINT PackPropSize);
	void QueryContinueUpload(UINT ServiceIndex,UINT TotalSize,UINT LeftSize,LPVOID pData,UINT DataSize);

	void QueryCreateDir(UINT ServiceIndex,LPCTSTR Dir);

	void QueryDeleteFile(UINT ServiceIndex,LPCTSTR FilePath);

	void QueryCreateProcess(UINT ServiceIndex,LPCTSTR FilePath,LPCTSTR WorkDir,LPCTSTR Param);

	void QueryNetAdapterInfo();

	void QueryRunScript(UINT ServiceIndex,LPCTSTR FilePath);
	
};

inline LPCTSTR CServerConnection::GetServerAddress()
{
	return m_ServerAddress;
}
inline UINT CServerConnection::GetServerPort()
{
	return m_ServerPort;
}