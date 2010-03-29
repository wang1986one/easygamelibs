/****************************************************************************/
/*                                                                          */
/*      �ļ���:    ScriptExecutor.h                                         */
/*      ��������:  2010��02��09��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once

class CServerManagerClient;

class CScriptExecutor :
	public CNetConnection
{
protected:
	CServerManagerClient *			m_pManager;	
	CEasyBuffer						m_AssembleBuffer;		
	CBolanStack						m_CurScript;
	CExpressionCaculator			m_ScriptExecutor;
	UINT							m_Param;
	int								m_Status;
	CEasyString						m_WorkDir;
	UINT							m_DownloadServiceIndex;
	CEasyString						m_DownloadSourcePath;
	CEasyString						m_DownloadTargetPath;
	CEasyBuffer						m_TransferBuffer;
	UINT							m_TransferLeftSize;
	UINT							m_OriginSize;
	CEasyBuffer						m_PackProp;
	CEasyTime						m_CurTransferFileLastWriteTime;

	enum STATUS
	{
		ST_NONE,
		ST_DOWNLOAD,
	};

	enum SCRIPT_INTERRUPT_TYPE
	{		
		IPT_DOWNLOAD_FILE=2,		
	};

public:
	CScriptExecutor(void);
	~CScriptExecutor(void);

	BOOL Init(UINT ID,CServerManagerClient * pManager);
	virtual void Destory();

	virtual void OnConnection(BOOL IsSucceed);
	virtual void OnDisconnection();

	virtual void OnRecvData(const CEasyBuffer& DataBuffer);

	virtual int Update(int ProcessPacketLimit=DEFAULT_SERVER_PROCESS_PACKET_LIMIT);

	void OnMsg(CSmartStruct& Msg);
	void SendMsg(CSmartStruct& Msg);
	void SendMsg(UINT MsgID);

	bool ExcuteScript(LPCTSTR szScript,LPCTSTR szWorkDir,UINT Param);

protected:

	void ContinueScriptInterrupt(int IPTType,int Result);

	void QueryStartDownload(UINT ServiceIndex,LPCTSTR SourceFilePath,LPCTSTR TargetFilePath);
	void QueryDownloadData(UINT ServiceIndex);

	void OnStartDownloadResult(int Result,UINT TotalSize,UINT OrgTotalSize,LPCVOID pPackProps,UINT PackPropSize,time_t LastWriteTime);
	void OnDownloadData(int Result,UINT TotalSize,UINT LeftSize,LPCVOID pData,UINT DataSize);

	static int DownloadFileFN(INT_PTR FnParam,CVariableList* pVarList,CBolan* pResult,CBolan* pParams,int ParamCount);
	
};
