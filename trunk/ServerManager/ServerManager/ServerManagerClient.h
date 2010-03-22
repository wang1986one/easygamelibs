#pragma once

class CServerManagerService;



class CServerManagerClient :
	public CNetConnection,
	public IFileCacheEventHandler
{
protected:
	CServerManagerService *	m_pManager;	
	CEasyBuffer				m_AssembleBuffer;	
	bool					m_WantDelete;

	CEasyString				m_TransferFilePath;
	CEasyBuffer				m_TransferBuffer;
	CEasyBuffer				m_PackProp;
	UINT					m_OriginSize;
	UINT					m_TransferLeftSize;
	CEasyTime				m_CurTransferFileLastWriteTime;

	CScriptExecutor			m_ScriptExecutor;
	bool					m_IsInScriptExecute;

	

	DECLARE_CLASS_INFO_STATIC(CServerManagerClient);
public:
	CServerManagerClient(void);
	~CServerManagerClient(void);

	BOOL Init(UINT ID,CServerManagerService * pManager);
	virtual void Destory();

	virtual void OnConnection(BOOL IsSucceed);
	virtual void OnDisconnection();

	virtual void OnRecvData(const CEasyBuffer& DataBuffer);

	virtual int Update(int ProcessPacketLimit=DEFAULT_SERVER_PROCESS_PACKET_LIMIT);

	void OnMsg(CSmartStruct& Msg);
	void SendMsg(CSmartStruct& Msg);
	void SendMsg(UINT MsgID);
	bool WantDelete();

	virtual void OnGetFile(FILE_CACHE * pFileCache);
	void OnScriptExcute(int ErrorCode,UINT Param);

protected:

	UINT PackServiceInfo(CEasyArray<SERVICE_INFO>& ServiceList,UINT StartIndex,CSmartStruct& Packet,WORD MemberID);
	void PackNetAdapterInfo(NET_ADAPTER_INFO& NetAdapterInfo,CSmartStruct& Packet,WORD MemberI);
	bool SendFileList(UINT ServiceIndex,CEasyString& WorkDir,CEasyString& SearchDir);
	
};

inline bool CServerManagerClient::WantDelete()
{
	return m_WantDelete;
}
