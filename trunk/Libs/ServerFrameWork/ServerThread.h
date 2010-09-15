/****************************************************************************/
/*                                                                          */
/*      �ļ���:    ServerThread.h                                           */
/*      ��������:  2009��07��06��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once

#define SERVER_ENDING_TIME	3*60*1000

class CServerThread : 
	public CNetServer,
	public IBaseServer
{
protected:	
	CESVariableList				m_ESVariableList;
	CESFactionList				m_ESFactionList;
	CESBolanStack				m_Script;
	CESThread					m_ESThread;
	CEasyScriptExecutor			m_ScriptExecutor;


	CSystemNetLinkManager		*m_pSysNetLinkManager;
	CSystemControlPort			*m_pUDPSystemControlPort;
	CSmartStruct				m_ServerStatus;
	

	UINT						m_CycleCount;
	

	CEasyTimer					m_CountTimer;

	DECLARE_CLASS_INFO_STATIC(CServerThread);
public:
	CServerThread();
	virtual ~CServerThread(void);
	
public:	
	

	virtual CNetServer * GetServer()
	{
		return this;
	}

	virtual BOOL PrintConsoleLog(LPCTSTR szLogMsg);

	virtual void ExecCommand(LPCTSTR szCommand);

	virtual BOOL SetServerStatus(WORD StatusID,const CSmartValue& Value);
	virtual void SetServerStatusFormat(WORD StatusID,LPCTSTR szStatusName,int FormatType=SSFT_DEFAULT);
	virtual CSmartValue GetServerStatus(WORD StatusID)
	{
		return m_ServerStatus.GetMember(StatusID);
	}
	virtual CSmartStruct& GetAllServerStatus()
	{
		return m_ServerStatus;
	}

protected:

	virtual void Execute();
	
	virtual BOOL OnRun();	


	virtual int Update(int ProcessPacketLimit=DEFAULT_SERVER_PROCESS_PACKET_LIMIT);

	virtual BOOL OnStart();	
	virtual void OnTerminate();

	virtual BOOL OnTerminating();
	virtual void OnBeginTerminate();

	virtual int GetClientCount();
	virtual LPCTSTR GetConfigFileName();
	
	virtual void DoServerStat();


	CSystemNetLinkManager * GetSysNetLinkManager()
	{
		return m_pSysNetLinkManager;
	}

	int StartLog(CESVariableList* pVarList,ES_BOLAN* pResult,ES_BOLAN* pParams,int ParamCount);
	int StopLog(CESVariableList* pVarList,ES_BOLAN* pResult,ES_BOLAN* pParams,int ParamCount);
	int TestLog(CESVariableList* pVarList,ES_BOLAN* pResult,ES_BOLAN* pParams,int ParamCount);	
	int RebuildUDPControlPort(CESVariableList* pVarList,ES_BOLAN* pResult,ES_BOLAN* pParams,int ParamCount);
};
