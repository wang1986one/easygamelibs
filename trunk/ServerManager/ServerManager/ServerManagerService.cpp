#include "StdAfx.h"

IMPLEMENT_CLASS_INFO_STATIC(CServerManagerService,CNetService);

CServerManagerService::CServerManagerService(void)
{
	FUNCTION_BEGIN;
	m_RecentTime=0;
	FUNCTION_END;
}

CServerManagerService::~CServerManagerService(void)
{
	FUNCTION_BEGIN;
	Destory();
	FUNCTION_END;
}


BOOL CServerManagerService::Init(CNetServer * pServer)
{
	FUNCTION_BEGIN;
	SetServer(pServer);
	if(!m_ClientPool.Create(CMainConfig::GetInstance()->GetMaxClient()))
	{
		Log("创建%u大小的客户端对象池失败",CMainConfig::GetInstance()->GetMaxClient());
		return FALSE;
	}	
	Log("创建%u大小的客户端对象池成功",CMainConfig::GetInstance()->GetMaxClient());
	if(!DoListen(CMainConfig::GetInstance()->GetListenAddress()))
	{
		return FALSE;
	}
	SERVICE_INFO SelfInfo;
	strcpy_s(SelfInfo.Name,MAX_PATH,"ServerManager");
	GetModuleFileName(NULL,SelfInfo.ImageFilePath,MAX_PATH);
	strncpy_0(SelfInfo.WorkDir,MAX_PATH,GetModulePath(NULL),MAX_PATH);
	m_ServiceInfoList.Add(SelfInfo);

	for(UINT i=0;i<CMainConfig::GetInstance()->GetServiceInfoList().GetCount();i++)
	{
		m_ServiceInfoList.Add(CMainConfig::GetInstance()->GetServiceInfoList()[i]);
	}
	

	m_ProcessInfoList.Create(CMainConfig::GetInstance()->GetMaxProcessCount());
	m_ProcessIDList.Create(CMainConfig::GetInstance()->GetMaxProcessCount());

	m_ProcessInfoFetchTimer.SaveTime();

	LARGE_INTEGER PerformanceCount;
	LARGE_INTEGER PerformanceFrequency;

	QueryPerformanceCounter(&PerformanceCount);
	QueryPerformanceFrequency(&PerformanceFrequency);

	m_RecentTime=PerformanceCount.QuadPart/(PerformanceFrequency.QuadPart/10000000LL);


	InitNetAdapterInfo();
	
	return TRUE;
	FUNCTION_END;
	return FALSE;
}

void CServerManagerService::Destory()
{
	FUNCTION_BEGIN;
	m_ClientPool.Destory();
	CNetService::Destory();
	FUNCTION_END;
}

CBaseTCPConnection * CServerManagerService::CreateConnection(CIPAddress& RemoteAddress)
{
	FUNCTION_BEGIN;
	CServerManagerClient * pClient=NULL;
	UINT ID=m_ClientPool.NewObject(&pClient);
	if(pClient)
	{
		if(pClient->Init(ID,this))
		{
			return pClient;
		}
		else
		{			
			LogDebug("初始化客户端对象失败");			
		}
		m_ClientPool.DeleteObject(ID);
	}
	else
	{
		LogDebug("创建客户端对象失败");
	}

	FUNCTION_END;
	return NULL;
}

BOOL CServerManagerService::DeleteConnection(CBaseTCPConnection * pConnection)
{
	FUNCTION_BEGIN;
	
	CServerManagerClient * pClient=(CServerManagerClient *)pConnection;
	pClient->Destory();
	m_ClientPool.DeleteObject(pClient->GetID());
	return TRUE;
	
	FUNCTION_END;
	return FALSE;
}


int CServerManagerService::Update(int ProcessPacketLimit)
{
	FUNCTION_BEGIN;
	int Process=CNetService::Update(ProcessPacketLimit);

	vector<CServerManagerClient *> DeleteList;
	void * Pos=m_ClientPool.GetFirstObjectPos();
	while(Pos)
	{
		CServerManagerClient * pClient=m_ClientPool.GetNext(Pos);
		Process+=pClient->Update(ProcessPacketLimit);

		if(pClient->WantDelete())
		{
			DeleteList.push_back(pClient);
		}
	}

	for(size_t i=0;i<DeleteList.size();i++)
	{
		DeleteConnection(DeleteList[i]);
	}

	if(m_ProcessInfoFetchTimer.IsTimeOut(CMainConfig::GetInstance()->GetProcessInfoFetchTime()))
	{
		m_ProcessInfoFetchTimer.SaveTime();
		FetchProcessInfo();
	}

	return Process;
	FUNCTION_END;
	return 0;
}

bool CServerManagerService::StartupService(UINT ServiceIndex)
{
	if(ServiceIndex>0&&ServiceIndex<m_ServiceInfoList.GetCount())
	{
		if(m_ServiceInfoList[ServiceIndex].Status==SS_STOP)
		{
			return StartupProcess(m_ServiceInfoList[ServiceIndex]);
		}
	}
	return false;
}

bool CServerManagerService::ShutdownService(UINT ServiceIndex)
{
	if(ServiceIndex>0&&ServiceIndex<m_ServiceInfoList.GetCount())
	{
		if(m_ServiceInfoList[ServiceIndex].Status==SS_RUNING)
		{
			return ShutdownProcess(m_ServiceInfoList[ServiceIndex]);
		}
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CServerManagerService::DoListen(CIPAddress& ListenAddress)
{
	FUNCTION_BEGIN;
	Close();
	if(!StartListen(ListenAddress))
	{
		Log("SimpleFileService侦听于%s:%u失败",
			ListenAddress.GetIPString(),ListenAddress.GetPort());
		return FALSE;
	}
	Log("SimpleFileService侦听于%s:%u成功",
		ListenAddress.GetIPString(),ListenAddress.GetPort());
	return TRUE;
	FUNCTION_END;
	return FALSE;
}

void CServerManagerService::FetchProcessInfo()
{	
	DWORD NeedSize;

	LARGE_INTEGER PerformanceCount;
	LARGE_INTEGER PerformanceFrequency;

	QueryPerformanceCounter(&PerformanceCount);
	QueryPerformanceFrequency(&PerformanceFrequency);

	UINT64 SystemTime=PerformanceCount.QuadPart/(PerformanceFrequency.QuadPart/10000000LL);
	UINT64 SystemUseTime=SystemTime-m_RecentTime;

	if(EnumProcesses(m_ProcessIDList.GetBuffer(),sizeof(DWORD)*m_ProcessIDList.GetBufferLength(),&NeedSize))
	{
		UINT ProcessCount=NeedSize/sizeof(DWORD);
		if(ProcessCount>m_ProcessIDList.GetBufferLength())
			ProcessCount=m_ProcessIDList.GetBufferLength();
		m_ProcessIDList.SetCount(ProcessCount);
		m_ProcessInfoList.SetCount(ProcessCount);
		for(UINT i=0;i<m_ProcessIDList.GetCount();i++)
		{			
			m_ProcessInfoList[i].ProcessID=m_ProcessIDList[i];
			HANDLE hProcess=OpenProcess(PROCESS_QUERY_INFORMATION|PROCESS_VM_READ,FALSE,m_ProcessIDList[i]);
			if(hProcess!=NULL)
			{
				HMODULE hModule;

				if(EnumProcessModules(hProcess,&hModule,sizeof(hModule),&NeedSize))
				{
					char ImageFilePath[MAX_PATH];
					if(GetModuleFileNameEx(hProcess,hModule,ImageFilePath,MAX_PATH))
					{
						GetLongPathName(ImageFilePath,m_ProcessInfoList[i].ImageFilePath,MAX_PATH);
					}
				}	

				FILETIME CreateTime,ExitTime,KernelTime,UserTime;
				if(GetProcessTimes(hProcess,&CreateTime,&ExitTime,&KernelTime,&UserTime))
				{
					ULONG64_CONVERTER Converter;
					Converter.LowPart=KernelTime.dwLowDateTime;
					Converter.HighPart=KernelTime.dwHighDateTime;
					UINT64 UseTime=Converter.QuadPart;
					Converter.LowPart=UserTime.dwLowDateTime;
					Converter.HighPart=UserTime.dwHighDateTime;
					UseTime+=Converter.QuadPart;
					if(m_ProcessInfoList[i].RecentTime&&m_RecentTime)
					{
						m_ProcessInfoList[i].CPUUsed=(float)(UseTime-m_ProcessInfoList[i].RecentTime)/(float)SystemUseTime;
					}
					m_ProcessInfoList[i].RecentTime=UseTime;
				}

				PROCESS_MEMORY_COUNTERS MemoryInfo;
				if(GetProcessMemoryInfo(hProcess,&MemoryInfo,sizeof(MemoryInfo)))
				{
					m_ProcessInfoList[i].MemoryUsed=MemoryInfo.WorkingSetSize;
					m_ProcessInfoList[i].VirtualMemoryUsed=MemoryInfo.PagefileUsage;
				}

				CloseHandle(hProcess);
			}
		}		
	}
	m_RecentTime=SystemTime;

	for(UINT j=0;j<m_ServiceInfoList.GetCount();j++)
	{
		m_ServiceInfoList[j].ProcessID=0;
		m_ServiceInfoList[j].CPUUsed=0;
		m_ServiceInfoList[j].MemoryUsed=0;
		m_ServiceInfoList[j].VirtualMemoryUsed=0;
		for(UINT i=0;i<m_ProcessInfoList.GetCount();i++)
		{
			if(_strnicmp(m_ServiceInfoList[j].ImageFilePath,m_ProcessInfoList[i].ImageFilePath,MAX_PATH)==0)
			{
				m_ServiceInfoList[j].ProcessID=m_ProcessInfoList[i].ProcessID;
				m_ServiceInfoList[j].CPUUsed=m_ProcessInfoList[i].CPUUsed;
				m_ServiceInfoList[j].MemoryUsed=m_ProcessInfoList[i].MemoryUsed;
				m_ServiceInfoList[j].VirtualMemoryUsed=m_ProcessInfoList[i].VirtualMemoryUsed;
			}
		}
		if(m_ServiceInfoList[j].ProcessID==0)
		{
			m_ServiceInfoList[j].Status=SS_STOP;
		}
		else
		{
			m_ServiceInfoList[j].Status=SS_RUNING;
		}

		CFileSearcher Searcher;

		if(Searcher.FindFirst(m_ServiceInfoList[j].ImageFilePath))
		{
			if(Searcher.FindNext())
			{
				m_ServiceInfoList[j].ImageFileTime=Searcher.GetLastWriteTime();
			}
		}
	}

	UpdateNetAdapterInfo(SystemUseTime);
	
}

bool CServerManagerService::StartupProcess(SERVICE_INFO& ServiceInfo)
{
	STARTUPINFO StartupInfo;
	ZeroMemory( &StartupInfo, sizeof(StartupInfo) );
	StartupInfo.cb = sizeof(StartupInfo);

	PROCESS_INFORMATION ProcessInfo;

	ZeroMemory( &ProcessInfo, sizeof(ProcessInfo) );

	CEasyString CommandLine;

	CommandLine.Format("\"%s\" %s",
		ServiceInfo.ImageFilePath,ServiceInfo.StartupParam);

	
	if(CreateProcess(ServiceInfo.ImageFilePath,CommandLine,NULL,NULL,FALSE,0,NULL,ServiceInfo.WorkDir,&StartupInfo,&ProcessInfo))
	{
		ServiceInfo.Status=SS_RUNING;
		CloseHandle( ProcessInfo.hProcess );
		CloseHandle( ProcessInfo.hThread );
		return true;
	}
	return false;
}

bool CServerManagerService::ShutdownProcess(SERVICE_INFO& ServiceInfo)
{
	HANDLE hProcess=OpenProcess(PROCESS_TERMINATE ,FALSE,ServiceInfo.ProcessID);
	if(hProcess!=NULL)
	{
		bool Ret=TerminateProcess(hProcess,0);
		CloseHandle(hProcess);
		return Ret;
	}
	return false;
}


void CServerManagerService::InitNetAdapterInfo()
{
	ULONG IfTableLen=0;
	GetIfTable(NULL,&IfTableLen,TRUE);

	CEasyBuffer Buffer1(IfTableLen);

	MIB_IFTABLE * pIfTable=(MIB_IFTABLE *)Buffer1.GetBuffer();

	if(GetIfTable(pIfTable,&IfTableLen,TRUE)!=NO_ERROR)
	{
		Log("获取网络适配器信息失败");
		return;
	}


	ULONG AdressTableLen=0;
	GetAdaptersAddresses(AF_INET,0,NULL,NULL,&AdressTableLen);

	CEasyBuffer Buffer2(AdressTableLen);	

	IP_ADAPTER_ADDRESSES  * pAddressTable=(IP_ADAPTER_ADDRESSES  *)Buffer2.GetBuffer();

	if(GetAdaptersAddresses(AF_INET,0,NULL,pAddressTable,&AdressTableLen)!=NO_ERROR)
	{
		Log("获取网络适配器IP地址信息失败");
		return;
	}


	m_NetAdapterInfos.Resize(pIfTable->dwNumEntries);
	for(UINT i=0;i<pIfTable->dwNumEntries;i++)
	{
		m_NetAdapterInfos[i].Index=pIfTable->table[i].dwIndex;
		m_NetAdapterInfos[i].RecvBytes=pIfTable->table[i].dwInOctets;
		m_NetAdapterInfos[i].RecvFlux=0;
		m_NetAdapterInfos[i].SendBytes=pIfTable->table[i].dwOutOctets;
		m_NetAdapterInfos[i].SendFlux=0;
		m_NetAdapterInfos[i].IPAddress=0;

		IP_ADAPTER_ADDRESSES * pAddress=pAddressTable;
		while(pAddress)
		{
			if(pAddress->IfIndex==pIfTable->table[i].dwIndex)
			{
				if(pAddress->FirstUnicastAddress)
				{
					m_NetAdapterInfos[i].IPAddress=((sockaddr_in *)pAddress->FirstUnicastAddress->Address.lpSockaddr)->sin_addr.s_addr;
				}
				break;
			}
			pAddress=pAddress->Next;
		}

	}

	
}

void CServerManagerService::UpdateNetAdapterInfo(UINT64 TimeSpan)
{
	ULONG IfTableLen=0;
	GetIfTable(NULL,&IfTableLen,TRUE);

	CEasyBuffer Buffer(IfTableLen);
	

	MIB_IFTABLE * pIfTable=(MIB_IFTABLE *)Buffer.GetBuffer();

	if(GetIfTable(pIfTable,&IfTableLen,TRUE)!=NO_ERROR)
	{		
		Log("获取网络适配器信息失败");
		return;
	}

	for(UINT i=0;i<pIfTable->dwNumEntries;i++)
	{
		for(UINT j=0;j<m_NetAdapterInfos.GetCount();j++)
		{
			if(pIfTable->table[i].dwIndex==m_NetAdapterInfos[j].Index)
			{
				m_NetAdapterInfos[i].RecvFlux=(float)(pIfTable->table[i].dwInOctets-m_NetAdapterInfos[i].RecvBytes)*10000000.0f/TimeSpan;
				m_NetAdapterInfos[i].SendFlux=(float)(pIfTable->table[i].dwOutOctets-m_NetAdapterInfos[i].SendBytes)*10000000.0f/TimeSpan;
				m_NetAdapterInfos[i].RecvBytes=pIfTable->table[i].dwInOctets;
				m_NetAdapterInfos[i].SendBytes=pIfTable->table[i].dwOutOctets;
			}
		}
	}
}