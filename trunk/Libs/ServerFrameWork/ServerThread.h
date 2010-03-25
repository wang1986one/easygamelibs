/****************************************************************************/
/*                                                                          */
/*      文件名:    ServerThread.h                                           */
/*      创建日期:  2009年07月06日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once

#define SERVER_ENDING_TIME	3*60*1000

class CServerThread : 
	public CNetServer,
	public IBaseServer
{
protected:		
	CExpressionCaculator		m_CommandExecutor;
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
	
	


	CSystemNetLinkManager * GetSysNetLinkManager()
	{
		return m_pSysNetLinkManager;
	}

	static int StartLog(INT_PTR FnParam,CVariableList* pVarList,CBolan* pResult,CBolan* pParams,int ParamCount);
	static int StopLog(INT_PTR FnParam,CVariableList* pVarList,CBolan* pResult,CBolan* pParams,int ParamCount);
	static int TestLog(INT_PTR FnParam,CVariableList* pVarList,CBolan* pResult,CBolan* pParams,int ParamCount);	
	static int RebuildUDPControlPort(INT_PTR FnParam,CVariableList* pVarList,CBolan* pResult,CBolan* pParams,int ParamCount);
};
