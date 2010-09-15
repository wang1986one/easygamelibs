/****************************************************************************/
/*                                                                          */
/*      �ļ���:    ServerThread.cpp                                         */
/*      ��������:  2010��02��09��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#include "StdAfx.h"


IMPLEMENT_CLASS_INFO_STATIC(CServerThread,CNetServer);

CServerThread::CServerThread()
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

	

	LogFileName.Format("%s/Log/%s.NetLib",(LPCTSTR)ModulePath,g_ProgramName);
	pLog=new CServerLogPrinter(this,CServerLogPrinter::LOM_FILE,
		CSystemConfig::GetInstance()->GetLogLevel(),LogFileName);
	CLogManager::GetInstance()->AddChannel(LOG_NET_CHANNEL,pLog);
	SAFE_RELEASE(pLog);

	LogFileName.Format("%s/Log/%s.DBLib",(LPCTSTR)ModulePath,g_ProgramName);
	pLog=new CServerLogPrinter(this,CServerLogPrinter::LOM_FILE,
		CSystemConfig::GetInstance()->GetLogLevel(),LogFileName);
	CLogManager::GetInstance()->AddChannel(LOG_DB_ERROR_CHANNEL,pLog);
	SAFE_RELEASE(pLog);	



	Log("��ʼ����������,��ǰ�汾:%u.%u.%u.%u",
		g_ProgramVersion[3],
		g_ProgramVersion[2],
		g_ProgramVersion[1],
		g_ProgramVersion[0]);

	m_ESVariableList.Create(128);
	m_ESFactionList.Create(128);
	CESFunctionLib::GetInstance()->AddFunction(&m_ESFactionList);
	m_ESThread.SetVariableList(&m_ESVariableList);
	m_ESThread.SetFactionList(&m_ESFactionList);
	m_ESThread.SetScript(&m_Script);


	m_ESFactionList.AddFaction("StartLog",3,this,(LPSCRIPT_FACTION)&CServerThread::StartLog);
	m_ESFactionList.AddFaction("StopLog",2,this,(LPSCRIPT_FACTION)&CServerThread::StopLog);
	m_ESFactionList.AddFaction("TestLog",1,this,(LPSCRIPT_FACTION)&CServerThread::TestLog);
	m_ESFactionList.AddFaction("RebuildUDPControlPort",0,this,(LPSCRIPT_FACTION)&CServerThread::RebuildUDPControlPort);

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
	SetServerStatusFormat(SC_SST_SS_CLIENT_COUNT,"�ͻ�������");
	SetServerStatusFormat(SC_SST_SS_CYCLE_TIME,"ѭ��ʱ��(����)");
	SetServerStatusFormat(SC_SST_SS_TCP_RECV_FLOW,"TCP��������(Byte/S)",SSFT_FLOW);
	SetServerStatusFormat(SC_SST_SS_TCP_SEND_FLOW,"TCP��������(Byte/S)",SSFT_FLOW);
	SetServerStatusFormat(SC_SST_SS_UDP_RECV_FLOW,"UDP��������(Byte/S)",SSFT_FLOW);
	SetServerStatusFormat(SC_SST_SS_UDP_SEND_FLOW,"UDP��������(Byte/S)",SSFT_FLOW);
	SetServerStatusFormat(SC_SST_SS_TCP_RECV_COUNT,"TCP���մ���(��/S)");
	SetServerStatusFormat(SC_SST_SS_TCP_SEND_COUNT,"TCP���ʹ���(��/S)");
	SetServerStatusFormat(SC_SST_SS_UDP_RECV_COUNT,"UDP���մ���(��/S)");
	SetServerStatusFormat(SC_SST_SS_UDP_SEND_COUNT,"UDP���ʹ���(��/S)");
	SetServerStatusFormat(SC_SST_SS_PROGRAM_VERSION,"�������汾");

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
		m_CountTimer.SaveTime();
		DoServerStat();
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
	int RetCode;
	ES_BOLAN Result;

	Log("ִ������:%s",szCommand);

	RetCode=m_ESThread.PushScript(szCommand);
	if(RetCode)
	{
		Log("�����������:Line=%d,%s",
			m_ESThread.GetLastLine(),
			ESGetErrorMsg(RetCode));
	}
	RetCode=m_ScriptExecutor.ExecScript(m_ESThread);
	if(RetCode)
	{
		Log("�����������:Line=%d,%s",
			m_ESThread.GetLastLine(),
			ESGetErrorMsg(RetCode));
	}
	else
	{
		Log("ִ��������:%s",
			(LPCTSTR)BolanToString(m_ESThread.GetResult()));
	}
	FUNCTION_END;
}

BOOL CServerThread::SetServerStatus(WORD StatusID,const CSmartValue& Value)
{
	FUNCTION_BEGIN;
	if(m_ServerStatus.IDToIndex(StatusID)==CSmartStruct::INVALID_MEMBER_ID)
	{
		m_ServerStatus.AddMember(StatusID,Value);
	}
	else
	{
		m_ServerStatus.GetMember(StatusID).SetValue(Value);
	}
	FUNCTION_END;
	return FALSE;
}

void CServerThread::SetServerStatusFormat(WORD StatusID,LPCTSTR szStatusName,int FormatType)
{
	FUNCTION_BEGIN;
	CControlPanel::GetInstance()->SetServerStatusFormat(StatusID,szStatusName,FormatType);
	FUNCTION_END;
}

int CServerThread::StartLog(CESVariableList* pVarList,ES_BOLAN* pResult,ES_BOLAN* pParams,int ParamCount)
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

int CServerThread::StopLog(CESVariableList* pVarList,ES_BOLAN* pResult,ES_BOLAN* pParams,int ParamCount)
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

int CServerThread::TestLog(CESVariableList* pVarList,ES_BOLAN* pResult,ES_BOLAN* pParams,int ParamCount)
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

int CServerThread::RebuildUDPControlPort(CESVariableList* pVarList,ES_BOLAN* pResult,ES_BOLAN* pParams,int ParamCount)
{
	FUNCTION_BEGIN;
	
	if(!m_pUDPSystemControlPort->Init(this))
	{
		Log("�ؽ�UDPϵͳ���ƶ˿�ʧ��");
	}
	else
	{
		Log("�ؽ�UDPϵͳ���ƶ˿ڳɹ�");
	}
	
	FUNCTION_END;
	return 0;
}

void CServerThread::DoServerStat()
{
	FUNCTION_BEGIN;
	int ClientCount=GetClientCount();
	float CycleTime=(float)SERVER_INFO_COUNT_TIME/m_CycleCount;
	float TCPRecvFlow=(float)m_TCPRecvBytes*1000/1024/SERVER_INFO_COUNT_TIME;
	float TCPSendFlow=(float)m_TCPSendBytes*1000/1024/SERVER_INFO_COUNT_TIME;
	float UDPRecvFlow=(float)m_UDPRecvBytes*1000/1024/SERVER_INFO_COUNT_TIME;
	float UDPSendFlow=(float)m_UDPSendBytes*1000/1024/SERVER_INFO_COUNT_TIME;

	float TCPRecvCount=(float)m_TCPRecvCount*1000/SERVER_INFO_COUNT_TIME;
	float TCPSendCount=(float)m_TCPSendCount*1000/SERVER_INFO_COUNT_TIME;
	float UDPRecvCount=(float)m_UDPRecvCount*1000/SERVER_INFO_COUNT_TIME;
	float UDPSendCount=(float)m_UDPSendCount*1000/SERVER_INFO_COUNT_TIME;


	LogServerInfo("CycleTime=%g,"
		"TCPRecvFlow=%s,TCPSendFlow=%s,UDPRecvFlow=%s,UDPSendFlow=%s,"
		"TCPRecvCount=%g,TCPSendCount=%g,UDPRecvCount=%g,UDPSendCount=%g,"
		"ClientCount=%d",
		CycleTime,
		(LPCTSTR)FormatNumberWordsFloat(TCPRecvFlow,true),
		(LPCTSTR)FormatNumberWordsFloat(TCPSendFlow,true),
		(LPCTSTR)FormatNumberWordsFloat(UDPRecvFlow,true),
		(LPCTSTR)FormatNumberWordsFloat(UDPSendFlow,true),
		TCPRecvCount,
		TCPSendCount,
		UDPRecvCount,
		UDPSendCount,
		ClientCount);

	SetServerStatus(SC_SST_SS_CLIENT_COUNT,CSmartValue(ClientCount));
	SetServerStatus(SC_SST_SS_CYCLE_TIME,CSmartValue(CycleTime));
	SetServerStatus(SC_SST_SS_TCP_RECV_FLOW,CSmartValue(TCPRecvFlow));
	SetServerStatus(SC_SST_SS_TCP_SEND_FLOW,CSmartValue(TCPSendFlow));
	SetServerStatus(SC_SST_SS_UDP_RECV_FLOW,CSmartValue(UDPRecvFlow));
	SetServerStatus(SC_SST_SS_UDP_SEND_FLOW,CSmartValue(UDPSendFlow));
	SetServerStatus(SC_SST_SS_TCP_RECV_COUNT,CSmartValue(TCPRecvCount));
	SetServerStatus(SC_SST_SS_TCP_SEND_COUNT,CSmartValue(TCPSendCount));
	SetServerStatus(SC_SST_SS_UDP_RECV_COUNT,CSmartValue(UDPRecvCount));
	SetServerStatus(SC_SST_SS_UDP_SEND_COUNT,CSmartValue(UDPSendCount));

	CControlPanel::GetInstance()->SetServerStatus(m_ServerStatus.GetData(),m_ServerStatus.GetDataLen());

	m_CycleCount=0;
	ResetFluxStat();

	if(CSystemConfig::GetInstance()->IsLogServerObjectUse())
	{
		PrintObjectStatus();
	}		
	FUNCTION_END;
}