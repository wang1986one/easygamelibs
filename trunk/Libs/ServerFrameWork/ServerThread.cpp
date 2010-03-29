/****************************************************************************/
/*                                                                          */
/*      �ļ���:    ServerThread.cpp                                         */
/*      ��������:  2010��02��09��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      ��������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ������������κ���ҵ�ͷ���ҵ������������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#include "StdAfx.h"


IMPLEMENT_CLASS_INFO_STATIC(CServerThread,CNetServer);

CServerThread::CServerThread()
	:m_CommandExecutor(128,128)
{
	FUNCTION_BEGIN
	m_pSysNetLinkManager=NULL;
	m_pUDPSystemControlPort=NULL;
	FUNCTION_END;
}

CServerThread::~CServerThread(void)
{
	FUNCTION_BEGIN
	
	FUNCTION_END;
}

void CServerThread::Execute()
{
	FUNCTION_BEGIN;

	//װ��ϵͳ����
	CSystemConfig::GetInstance()->LoadConfig(MakeModuleFullPath(NULL,GetConfigFileName()));


#ifndef _DEBUG
	CMainGuardThread::GetInstance()->SetTargetThreadID(GetThreadID());
	CMainGuardThread::GetInstance()->SetKeepAliveTime(
		CSystemConfig::GetInstance()->GetGuardThreadKeepAliveTime(),
		CSystemConfig::GetInstance()->GetGuardThreadKeepAliveCount());
	CMainGuardThread::GetInstance()->Start();
#endif
	while((!m_WantTerminate)&&(OnRun()))
	{
#ifndef _DEBUG
		CMainGuardThread::GetInstance()->MakeKeepAlive();
#endif
	}	
#ifndef _DEBUG
	CMainGuardThread::GetInstance()->SafeTerminate();
#endif
	OnBeginTerminate();
	DWORD Time=CEasyTimer::GetTime();
	while(GetTimeToTime(Time,CEasyTimer::GetTime())<SERVER_ENDING_TIME&&OnTerminating())
	{
	}
	
	FUNCTION_END;
}


BOOL CServerThread::OnStart()
{
	FUNCTION_BEGIN;

	
	m_CycleCount=0;
	m_TCPRecvBytes=0;
	m_TCPSendBytes=0;
	m_UDPRecvBytes=0;
	m_UDPSendBytes=0;
	m_TCPRecvCount=0;
	m_TCPSendCount=0;
	m_UDPRecvCount=0;
	m_UDPSendCount=0;
	m_CountTimer.SaveTime();

	


	CEasyString LogFileName;
	CEasyString ModulePath=GetModulePath(NULL);

	CServerLogPrinter * pLog;



	LogFileName.Format("%s/Log/%s",(LPCTSTR)ModulePath,g_ProgramName);
	pLog=new CServerLogPrinter(this,CServerLogPrinter::LOM_CONSOLE|CServerLogPrinter::LOM_FILE,
		CSystemConfig::GetInstance()->GetLogLevel(),LogFileName);
	CLogManager::GetInstance()->AddChannel(SERVER_LOG_CHANNEL,pLog);
	SAFE_RELEASE(pLog);

	LogFileName.Format("%s/Log/%s.Status",(LPCTSTR)ModulePath,g_ProgramName);
	pLog=new CServerLogPrinter(this,CServerLogPrinter::LOM_FILE,
		CSystemConfig::GetInstance()->GetLogLevel(),LogFileName);
	CLogManager::GetInstance()->AddChannel(SERVER_STATUS_LOG_CHANNEL,pLog);
	SAFE_RELEASE(pLog);

	

	LogFileName.Format("%s/Log/NetLib",(LPCTSTR)ModulePath);
	pLog=new CServerLogPrinter(this,CServerLogPrinter::LOM_FILE,
		CSystemConfig::GetInstance()->GetLogLevel(),LogFileName);
	CLogManager::GetInstance()->AddChannel(LOG_NET_CHANNEL,pLog);
	SAFE_RELEASE(pLog);

	LogFileName.Format("%s/Log/DBLib",(LPCTSTR)ModulePath);
	pLog=new CServerLogPrinter(this,CServerLogPrinter::LOM_FILE,
		CSystemConfig::GetInstance()->GetLogLevel(),LogFileName);
	CLogManager::GetInstance()->AddChannel(LOG_DB_ERROR_CHANNEL,pLog);
	SAFE_RELEASE(pLog);	



	Log("��ʼ����������,��ǰ�汾:%u.%u.%u.%u",
		g_ProgramVersion[3],
		g_ProgramVersion[2],
		g_ProgramVersion[1],
		g_ProgramVersion[0]);


	


	m_CommandExecutor.AddFaction("StartLog",3,(INT_PTR)this,StartLog);
	m_CommandExecutor.AddFaction("StopLog",2,(INT_PTR)this,StopLog);
	m_CommandExecutor.AddFaction("TestLog",1,(INT_PTR)this,TestLog);
	m_CommandExecutor.AddFaction("RebuildUDPControlPort",0,(INT_PTR)this,RebuildUDPControlPort);

	if(!CNetServer::OnStart())
		return FALSE;

	//��ʼ��ϵͳ����
	m_pSysNetLinkManager=new CSystemNetLinkManager();
	m_pSysNetLinkManager->SetServerThread(this);

	xml_parser Parser;

	if(Parser.parse_file(MakeModuleFullPath(NULL,GetConfigFileName()),pug::parse_trim_attribute))
	{
		xml_node Config=Parser.document();
		if(Config.moveto_child("SystemLink"))
		{
			if(m_pSysNetLinkManager->Init(this,Config))
			{
				Log("��ʼ��ϵͳ���ӹ������ɹ�");
			}
			else
			{
				Log("��ʼ��ϵͳ���ӹ�����ʧ��");
			}
		}
		else
		{
			Log("���Ϸ���ϵͳ���������ļ�%s",GetConfigFileName());
		}
		
	}
	else
	{
		Log("δ�ҵ�ϵͳ���������ļ�%s",GetConfigFileName());
	}
	
	m_pUDPSystemControlPort=new CSystemControlPort();
	if(!m_pUDPSystemControlPort->Init(this))
	{
		Log("��ʼ��UDPϵͳ���ƶ˿�ʧ��");
	}
	

	m_ServerStatus.Create(SERVER_STATUS_BLOCK_SIZE);

	ULONG64_CONVERTER Version;
	memcpy(Version.Words,g_ProgramVersion,sizeof(ULONG64_CONVERTER));

	SetServerStatus(SC_SST_SS_PROGRAM_VERSION,CSmartValue(Version.QuadPart));

	Log("�������ɹ�����");
	
	FUNCTION_END;
	return TRUE;
}

void CServerThread::OnBeginTerminate()
{
	
}

void CServerThread::OnTerminate()
{
	FUNCTION_BEGIN;	
	
	SAFE_RELEASE(m_pSysNetLinkManager);
	SAFE_RELEASE(m_pUDPSystemControlPort);
	CNetServer::OnTerminate();
	Log("�������ر�");
	FUNCTION_END;
}

BOOL CServerThread::OnRun()
{
	FUNCTION_BEGIN;		

	CNetServer::OnRun();

	//���������ѭ��ʱ��
	m_CycleCount++;
	if(m_CountTimer.IsTimeOut(SERVER_INFO_COUNT_TIME))
	{			
		SERVER_INFO ServerInfo;
		ServerInfo.ClientCount=GetClientCount();
		ServerInfo.CycleTime=(double)SERVER_INFO_COUNT_TIME/m_CycleCount;
		ServerInfo.TCPRecvFlow=(double)m_TCPRecvBytes*1000/1024/SERVER_INFO_COUNT_TIME;
		ServerInfo.TCPSendFlow=(double)m_TCPSendBytes*1000/1024/SERVER_INFO_COUNT_TIME;
		ServerInfo.UDPRecvFlow=(double)m_UDPRecvBytes*1000/1024/SERVER_INFO_COUNT_TIME;
		ServerInfo.UDPSendFlow=(double)m_UDPSendBytes*1000/1024/SERVER_INFO_COUNT_TIME;

		ServerInfo.TCPRecvCount=(double)m_TCPRecvCount*1000/SERVER_INFO_COUNT_TIME;
		ServerInfo.TCPSendCount=(double)m_TCPSendCount*1000/SERVER_INFO_COUNT_TIME;
		ServerInfo.UDPRecvCount=(double)m_UDPRecvCount*1000/SERVER_INFO_COUNT_TIME;
		ServerInfo.UDPSendCount=(double)m_UDPSendCount*1000/SERVER_INFO_COUNT_TIME;

		CControlPanel::GetInstance()->SetServerInfo(ServerInfo);
		LogServerInfo("CycleTime=%06.6g,"
			"TCPRecvFlow=%06.6g,TCPSendFlow=%06.6g,UDPRecvFlow=%06.6g,UDPSendFlow=%06.6g,"
			"TCPRecvCount=%06.6g,TCPSendCount=%06.6g,UDPRecvCount=%06.6g,UDPSendCount=%06.6g,"
			"ClientCount=%04d",
			ServerInfo.CycleTime,
			ServerInfo.TCPRecvFlow,
			ServerInfo.TCPSendFlow,
			ServerInfo.UDPRecvFlow,
			ServerInfo.UDPSendFlow,
			ServerInfo.TCPRecvCount,
			ServerInfo.TCPSendCount,
			ServerInfo.UDPRecvCount,
			ServerInfo.UDPSendCount,
			ServerInfo.ClientCount);
		
		SetServerStatus(SC_SST_SS_CLIENT_COUNT,CSmartValue(ServerInfo.ClientCount));
		SetServerStatus(SC_SST_SS_CYCLE_TIME,CSmartValue(ServerInfo.CycleTime));
		SetServerStatus(SC_SST_SS_TCP_RECV_FLOW,CSmartValue(ServerInfo.TCPRecvFlow));
		SetServerStatus(SC_SST_SS_TCP_SEND_FLOW,CSmartValue(ServerInfo.TCPSendFlow));
		SetServerStatus(SC_SST_SS_UDP_RECV_FLOW,CSmartValue(ServerInfo.UDPRecvFlow));
		SetServerStatus(SC_SST_SS_UDP_SEND_FLOW,CSmartValue(ServerInfo.UDPSendFlow));
		SetServerStatus(SC_SST_SS_TCP_RECV_COUNT,CSmartValue(ServerInfo.TCPRecvCount));
		SetServerStatus(SC_SST_SS_TCP_SEND_COUNT,CSmartValue(ServerInfo.TCPSendCount));
		SetServerStatus(SC_SST_SS_UDP_RECV_COUNT,CSmartValue(ServerInfo.UDPRecvCount));
		SetServerStatus(SC_SST_SS_UDP_SEND_COUNT,CSmartValue(ServerInfo.UDPSendCount));

		m_CountTimer.SaveTime();
		m_CycleCount=0;
		ResetFluxStat();

		if(CSystemConfig::GetInstance()->IsLogServerObjectUse())
		{
			PrintObjectStatus();
		}		
	}

	//ִ�п���̨����
	PANEL_MSG * pCommand=CControlPanel::GetInstance()->GetCommand();
	if(pCommand)
	{
		
		ExecCommand(pCommand->Msg);
		CControlPanel::GetInstance()->ReleaseCommand(pCommand->ID);
	}

	FUNCTION_END;
	return TRUE;
}

int CServerThread::Update(int ProcessPacketLimit)
{
	FUNCTION_BEGIN;
	int Process=0;	
	Process+=CNetServer::Update(ProcessPacketLimit);		
	Process+=m_pSysNetLinkManager->Update(ProcessPacketLimit);
	Process+=m_pUDPSystemControlPort->Update(ProcessPacketLimit);
	return Process;
	FUNCTION_END;
	return 0;
}

BOOL CServerThread::OnTerminating()
{
	return FALSE;
}


int CServerThread::GetClientCount()
{
	return 0;
}

LPCTSTR CServerThread::GetConfigFileName()
{
	return SYSTEM_NET_LINK_CONFIG_FILE_NAME;
}

BOOL CServerThread::PrintConsoleLog(LPCTSTR szLogMsg)
{
	FUNCTION_BEGIN;
#ifdef WIN32
	CControlPanel::GetInstance()->PushMsg(szLogMsg);
#else
	printf("%s\n",szLogMsg);
#endif
	if(m_pSysNetLinkManager)
		m_pSysNetLinkManager->SendLogMsg(szLogMsg);
	FUNCTION_END;
	return FALSE;
}

void CServerThread::ExecCommand(LPCTSTR szCommand)
{
	FUNCTION_BEGIN;
	CBolan Result;
	int RetCode=m_CommandExecutor.ExecScript(szCommand,Result);
	if(RetCode)
	{
		Log("ִ������[%s]����[%d][%s]",szCommand,RetCode,
			m_CommandExecutor.GetErrorMsg(RetCode));
	}
	else
	{
		if(Result.ValueType==VALUE_TYPE_NUMBER)
			Log("ִ������[%s]���[%g]",szCommand,Result.value);
		else
			Log("ִ������[%s]���[%s]",szCommand,(LPCTSTR)Result.StrValue);
	}
	FUNCTION_END;
}

BOOL CServerThread::SetServerStatus(WORD StatusID,const CSmartValue& Value)
{
	FUNCTION_BEGIN;
	if(m_ServerStatus.IDToIndex(StatusID)==0xffffffff)
	{
		m_ServerStatus.AddMember(StatusID,Value);
	}
	else
	{
		m_ServerStatus.GetMember(StatusID)=Value;
	}
	FUNCTION_END;
	return FALSE;
}

int CServerThread::StartLog(INT_PTR FnParam,CVariableList* pVarList,CBolan* pResult,CBolan* pParams,int ParamCount)
{
	FUNCTION_BEGIN;
	CServerLogPrinter * pLog=NULL;
	if(_stricmp(pParams[0].StrValue,"Normal")==0)
	{
		pLog=(CServerLogPrinter *)CLogManager::GetInstance()->
			GetChannel(SERVER_LOG_CHANNEL);
	}	
	else if(_stricmp(pParams[0].StrValue,"Status")==0)
	{
		pLog=(CServerLogPrinter *)CLogManager::GetInstance()->
			GetChannel(SERVER_STATUS_LOG_CHANNEL);
	}
	else if(_stricmp(pParams[0].StrValue,"DB")==0)
	{		
		pLog=(CServerLogPrinter *)CLogManager::GetInstance()->
			GetChannel(LOG_DB_ERROR_CHANNEL);
	}
	else if(_stricmp(pParams[0].StrValue,"Net")==0)
	{
		pLog=(CServerLogPrinter *)CLogManager::GetInstance()->
			GetChannel(LOG_NET_CHANNEL);	
	}
	if(pLog)
	{
		int WitchMode=0;
		if(_stricmp(pParams[1].StrValue,"VS")==0)
		{
			WitchMode=CServerLogPrinter::LOM_VS;
		}
		else if(_stricmp(pParams[1].StrValue,"Console")==0)
		{
			WitchMode=CServerLogPrinter::LOM_CONSOLE;
		}
		else if(_stricmp(pParams[1].StrValue,"File")==0)
		{
			WitchMode=CServerLogPrinter::LOM_FILE;
		}
		if(WitchMode)
		{
			UINT Mode=pLog->GetLogMode();
			Mode|=WitchMode;

			int Level=pLog->GetLogLevel();

			CEasyString LogFileName;
			if(pParams[2].StrValue.IsEmpty())
			{
				LogFileName=pLog->GetLogFileName();
			}
			else
			{
				CEasyString ModulePath=GetModulePath(NULL);
				LogFileName.Format("%s/Log/%s",(LPCTSTR)ModulePath,(LPCTSTR)pParams[2].StrValue);
			}
			

			pLog->SetLogMode(Mode,Level,LogFileName);
			Log("Logģ��[%s],ģʽ[%s]������ѱ�����",
				(LPCTSTR)(pParams[0].StrValue),
				(LPCTSTR)(pParams[1].StrValue));
		}
	}
	FUNCTION_END;
	return 0;
}

int CServerThread::StopLog(INT_PTR FnParam,CVariableList* pVarList,CBolan* pResult,CBolan* pParams,int ParamCount)
{
	FUNCTION_BEGIN;
	CServerLogPrinter * pLog=NULL;
	if(_stricmp(pParams[0].StrValue,"Normal")==0)
	{
		pLog=(CServerLogPrinter *)CLogManager::GetInstance()->
			GetChannel(SERVER_LOG_CHANNEL);
	}	
	else if(_stricmp(pParams[0].StrValue,"Status")==0)
	{
		pLog=(CServerLogPrinter *)CLogManager::GetInstance()->
			GetChannel(SERVER_STATUS_LOG_CHANNEL);
	}
	else if(_stricmp(pParams[0].StrValue,"DB")==0)
	{
		pLog=(CServerLogPrinter *)CLogManager::GetInstance()->
			GetChannel(LOG_DB_ERROR_CHANNEL);
	}
	else if(_stricmp(pParams[0].StrValue,"Net")==0)
	{		
		pLog=(CServerLogPrinter *)CLogManager::GetInstance()->
			GetChannel(LOG_NET_CHANNEL);
	}
	if(pLog)
	{
		int WitchMode=0;
		if(_stricmp(pParams[1].StrValue,"VS")==0)
		{
			WitchMode=CServerLogPrinter::LOM_VS;
		}
		else if(_stricmp(pParams[1].StrValue,"Console")==0)
		{
			WitchMode=CServerLogPrinter::LOM_CONSOLE;
		}
		else if(_stricmp(pParams[1].StrValue,"File")==0)
		{
			WitchMode=CServerLogPrinter::LOM_FILE;
		}
		if(WitchMode)
		{
			UINT Mode=pLog->GetLogMode();
			Mode&=~WitchMode;
			int Level=pLog->GetLogLevel();
			pLog->SetLogMode(Mode,Level,NULL);
			Log("Logģ��[%s],ģʽ[%s]������ѱ��ر�",
				(LPCTSTR)(pParams[0].StrValue),
				(LPCTSTR)(pParams[1].StrValue));
		}
	}
	FUNCTION_END;
	return 0;
}

int CServerThread::TestLog(INT_PTR FnParam,CVariableList* pVarList,CBolan* pResult,CBolan* pParams,int ParamCount)
{
	FUNCTION_BEGIN;
	CServerLogPrinter * pLog=NULL;
	if(_stricmp(pParams[0].StrValue,"Normal")==0)
	{
		Log("Normal");
	}
	else if(_stricmp(pParams[0].StrValue,"Debug")==0)
	{
		LogDebug("Debug");
	}
	else if(_stricmp(pParams[0].StrValue,"Status")==0)
	{		
		LogServerInfo("Status");
	}
	else if(_stricmp(pParams[0].StrValue,"DB")==0)
	{
		PrintDBLog(0,"DB");
	}
	else if(_stricmp(pParams[0].StrValue,"Net")==0)
	{
		PrintNetLog(0,"Net");
	}
	
	FUNCTION_END;
	return 0;
}

int CServerThread::RebuildUDPControlPort(INT_PTR FnParam,CVariableList* pVarList,CBolan* pResult,CBolan* pParams,int ParamCount)
{
	FUNCTION_BEGIN;
	CServerThread * pServer=(CServerThread *)FnParam;
	if(pServer)
	{
		if(!pServer->m_pUDPSystemControlPort->Init(pServer))
		{
			Log("�ؽ�UDPϵͳ���ƶ˿�ʧ��");
		}
		else
		{
			Log("�ؽ�UDPϵͳ���ƶ˿ڳɹ�");
		}
	}
	else
	{
		Log("������ָ������쳣");
	}
	FUNCTION_END;
	return 0;
}