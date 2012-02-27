/****************************************************************************/
/*                                                                          */
/*      文件名:    EasyNetLinkManager.cpp                                   */
/*      创建日期:  2009年07月06日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#include "StdAfx.h"





IMPLEMENT_CLASS_INFO_STATIC(CEasyNetLinkManager,CNameObject);

CEasyNetLinkManager::CEasyNetLinkManager(void)
{
	m_pServer=NULL;
}

CEasyNetLinkManager::~CEasyNetLinkManager(void)
{
	Destory();
}


BOOL CEasyNetLinkManager::Init(CNetServer * pServer,LPCTSTR ConfigFileName)
{
	xml_parser Parser;

	if(!Parser.parse_file(ConfigFileName,pug::parse_trim_attribute))
	{
		return FALSE;
	}
	xml_node Config=Parser.document();
	if(!Config.moveto_child("EasyLink"))
		return FALSE;

	return Init(pServer,Config);
}

BOOL CEasyNetLinkManager::Init(CNetServer * pServer,xml_node& Config)
{
	m_pServer=pServer;
	if(m_pServer==NULL)
		return FALSE;

	if(!Config.has_attribute("ServerID"))
	{
		return FALSE;
	}
	CClassifiedID ServerID=(LPCTSTR)Config.attribute("ServerID").getvalue();

	UINT ReallocIDRange=2048;
	if(Config.has_attribute("ReallocIDRange"))
	{
		ReallocIDRange=Config.attribute("ReallocIDRange");
	}

	if(!m_ConnectionIDPool.Create(ReallocIDRange))
	{
		PrintNetLog(0xffffff,"CEasyNetLinkManager::Init 创建[%u]大小的ConnectionIDPool失败",
			ReallocIDRange);
		return FALSE;
	}

	for(UINT i=0;i<Config.children();i++)
	{
		if(_strnicmp(Config.child(i).name(),"Services",8)==0)
		{
			xml_node Services=Config.child(i);
			BOOL NeedReallocConnectionID=FALSE;
			if(Services.has_attribute("ReallocConnectionID"))
				NeedReallocConnectionID=(bool)Services.attribute("ReallocConnectionID");

			CClassifiedID ReportID;
			if(Services.has_attribute("ReportID"))
			{
				ReportID=Services.attribute("ReportID").getvalue();
			}
			else
			{
				ReportID=ServerID;
			}

			for(UINT j=0;j<Services.children();j++)
			{
				if(_strnicmp(Services.child(j).name(),"Service",7)==0)
				{
					xml_node Service=Services.child(j);
					if(Service.has_attribute("ID")&&
						Service.has_attribute("Port"))
					{
						CClassifiedID ID=(LPCTSTR)Service.attribute("ID").getvalue();
						CIPAddress Address;
						UINT MaxPacketSize=MAX_DATA_PACKET_SIZE;
						bool IsUseListenThread=true;
						int ParallelAcceptCount=DEFAULT_PARALLEL_ACCEPT;
						UINT AcceptQueueSize=DEFAULT_SERVER_ACCEPT_QUEUE;
						UINT RecvQueueSize=DEFAULT_SERVER_RECV_DATA_QUEUE;
						UINT SendQueueSize=0;
						if(Service.has_attribute("IP"))
							Address.SetIP(Service.attribute("IP").getvalue());
						if(Service.has_attribute("MaxPacketSize"))
							MaxPacketSize=Service.attribute("MaxPacketSize");
						if(Service.has_attribute("IsUseListenThread"))
							IsUseListenThread=(bool)Service.attribute("IsUseListenThread");
						if(Service.has_attribute("ParallelAcceptCount"))
							ParallelAcceptCount=Service.attribute("ParallelAcceptCount");
						if(Service.has_attribute("AcceptQueueSize"))
							AcceptQueueSize=Service.attribute("AcceptQueueSize");
						if(Service.has_attribute("RecvQueueSize"))
							RecvQueueSize=Service.attribute("RecvQueueSize");
						if(Service.has_attribute("SendQueueSize"))
							SendQueueSize=Service.attribute("SendQueueSize");
						Address.SetPort(Service.attribute("Port"));
						CEasyNetLinkService * pService=AddService(ID,ReportID,Address,NeedReallocConnectionID,
							MaxPacketSize,IsUseListenThread,
							ParallelAcceptCount,AcceptQueueSize,RecvQueueSize,SendQueueSize);
						if(!pService)
						{
							PrintNetLog(0xffffff,"CEasyNetLinkManager::Init 创建Service[%s][%s:%u]失败",
								ID.ToStr(),Address.GetIPString(),Address.GetPort());
							return FALSE;
						}
						for(UINT k=0;k<Service.children();k++)
						{
							if(_strnicmp(Service.child(k).name(),"IPList",6)==0)
							{
								CEasyArray<CIPPattern> IPPatternList;
								xml_node IPList=Service.child(k);

								for(UINT l=0;l<IPList.children();l++)
								{
									if(_strnicmp(IPList.child(l).name(),"IP",2)==0)
									{
										xml_node IP=IPList.child(l);
										if(IP.has_attribute("IP"))
										{
											IPPatternList.Add(CIPPattern(IP.attribute("IP").getvalue()));
										}
									}
								}
								pService->SetIPList(IPPatternList);
							}
						}
					}
				}
			}
		}
		if(_strnicmp(Config.child(i).name(),"Connections",11)==0)
		{
			xml_node Connections=Config.child(i);

			CClassifiedID ReportID;
			if(Connections.has_attribute("ReportID"))
			{
				ReportID=Connections.attribute("ReportID").getvalue();
			}
			else
			{
				ReportID=ServerID;
			}

			for(UINT j=0;j<Connections.children();j++)
			{
				if(_strnicmp(Connections.child(j).name(),"Connection",10)==0)
				{
					xml_node Connection=Connections.child(j);
					if(Connection.has_attribute("IP")&&
						Connection.has_attribute("Port"))
					{
						CIPAddress Address;
						UINT MaxPacketSize=MAX_DATA_PACKET_SIZE;
						UINT RecvQueueSize=DEFAULT_SERVER_RECV_DATA_QUEUE;
						UINT SendQueueSize=0;
						Address.SetIP(Connection.attribute("IP").getvalue());
						Address.SetPort(Connection.attribute("Port"));
						if(Connection.has_attribute("MaxPacketSize"))
							MaxPacketSize=Connection.attribute("MaxPacketSize");
						if(Connection.has_attribute("RecvQueueSize"))
							RecvQueueSize=Connection.attribute("RecvQueueSize");
						if(Connection.has_attribute("SendQueueSize"))
							SendQueueSize=Connection.attribute("SendQueueSize");
						if(!AddConnection(ReportID,Address,MaxPacketSize,RecvQueueSize,SendQueueSize))
						{
							PrintNetLog(0xffffff,"CEasyNetLinkManager::Init 创建Connection[%s][%s:%u]失败",
								ReportID.ToStr(),Address.GetIPString(),Address.GetPort());
							return FALSE;
						}
					}
				}
			}
		}
	}
	return TRUE;
}

BOOL CEasyNetLinkManager::Init(CNetServer * pServer)
{
	m_pServer=pServer;
	if(m_pServer==NULL)
		return FALSE;
	return TRUE;
}

void CEasyNetLinkManager::Destory()
{
	for(int i=(int)m_ConnectionList.GetCount()-1;i>=0;i--)
	{
		SAFE_RELEASE(m_ConnectionList[i]);
	}

	for(int i=(int)m_ServiceList.GetCount()-1;i>=0;i--)
	{
		SAFE_RELEASE(m_ServiceList[i]);
	}
	m_ConnectionMap.Clear();
	m_ConnectionList.Clear();
	m_ServiceMap.Clear();
	m_ServiceList.Clear();
}

CEasyNetLinkService * CEasyNetLinkManager::AddService(UINT ID,UINT ReportID,const CIPAddress& ListenAddress,
													  BOOL NeedReallocConnectionID,UINT MaxPacketSize,bool IsUseListenThread,
													  int ParallelAcceptCount,UINT AcceptQueueSize,
													  UINT RecvQueueSize,UINT SendQueueSize)
{
	CEasyNetLinkService * pService=CreateLinkService(ID);
	if(pService)
	{
		pService->SetMaxPacketSize(MaxPacketSize);
		pService->SetReportID(ReportID);
		pService->EnableReallocConnectionID(NeedReallocConnectionID);
		if(pService->Create(IPPROTO_TCP,AcceptQueueSize,
			RecvQueueSize,SendQueueSize,
			ParallelAcceptCount,DEFAULT_PARALLEL_RECV,IsUseListenThread))
		{
			if(pService->StartListen(ListenAddress))
			{
				PrintNetLog(0xffffff,"CEasyNetLinkManager::AddService 创建Service[%s][%s:%u],%s,并发Accept=%d,Accept队列长度=%u,接收队列长度=%u,发送队列长度=%u",
					CClassifiedID(ID).ToStr(),
					ListenAddress.GetIPString(),
					ListenAddress.GetPort(),
					IsUseListenThread?"使用侦听线程":"使用IOCP侦听",
					ParallelAcceptCount,AcceptQueueSize,RecvQueueSize,SendQueueSize);
				return pService;
			}
		}
		DeleteLinkService(pService);
		SAFE_DELETE(pService);
	}
	return NULL;
}

BOOL CEasyNetLinkManager::AddConnection(UINT ReportID,const CIPAddress& ConnectionAddress,UINT MaxPacketSize,
										UINT RecvQueueSize,UINT SendQueueSize)
{
	CEasyNetLinkConnection * pConnection=CreateAcceptConnection(ReportID);
	if(pConnection)
	{
		pConnection->Create(RecvQueueSize,SendQueueSize);
		pConnection->SetMaxPacketSize(MaxPacketSize);
		pConnection->SetKeepConnect(TRUE);
		pConnection->SetRemoteAddress(ConnectionAddress);
		PrintNetLog(0xffffff,"CEasyNetLinkManager::AddConnection 创建Connection[%s][%s:%u],接收队列长度=%u,发送队列长度=%u",
			CClassifiedID(ReportID).ToStr(),
			ConnectionAddress.GetIPString(),
			ConnectionAddress.GetPort(),
			RecvQueueSize,SendQueueSize);
		return TRUE;
	}
	return FALSE;
}

int CEasyNetLinkManager::Update(int ProcessPacketLimit)
{
	int Process=0;
	for(int i=(int)m_ConnectionList.GetCount()-1;i>=0;i--)
	{
		Process+=m_ConnectionList[i]->Update(ProcessPacketLimit);
	}
	for(size_t i=0;i<m_ServiceList.GetCount();i++)
	{
		Process+=m_ServiceList[i]->Update(ProcessPacketLimit);
	}
	return Process;
}

CEasyNetLinkConnection * CEasyNetLinkManager::OnCreateConnection(UINT ID)
{
	return new CEasyNetLinkConnection();
}

CEasyNetLinkService * CEasyNetLinkManager::OnCreateService(UINT ID)
{
	return new CEasyNetLinkService();
}

CEasyNetLinkConnection * CEasyNetLinkManager::GetConnection(UINT ID)
{
	CEasyNetLinkConnection ** ppValue=m_ConnectionMap.Find(ID);
	if(ppValue)
	{
		return *ppValue;
	}
	return NULL;
}
LPVOID CEasyNetLinkManager::GetFirstConnectionPos()
{
	return m_ConnectionMap.GetFirstObjectPos();
}
CEasyNetLinkConnection *  CEasyNetLinkManager::GetNextConnection(LPVOID& Pos)
{
	UINT Key;
	CEasyNetLinkConnection ** ppValue=m_ConnectionMap.GetNextObject(Pos,Key);
	if(ppValue)
	{
		return *ppValue;
	}
	return NULL;
}

CEasyNetLinkService * CEasyNetLinkManager::GetService(UINT ID)
{
	CEasyNetLinkService ** ppValue=m_ServiceMap.Find(ID);
	if(ppValue)
	{
		return *ppValue;
	}
	return NULL;
}

UINT CEasyNetLinkManager::GetConnectionCount()
{
	return m_ConnectionList.GetCount();
}

CEasyNetLinkConnection * CEasyNetLinkManager::GetConnectionByIndex(UINT Index)
{
	if(Index<m_ConnectionList.GetCount())
	{
		return m_ConnectionList[Index];
	}
	return NULL;
}

CEasyNetLinkConnection * CEasyNetLinkManager::CreateAcceptConnection(UINT ReportID)
{
	CEasyNetLinkConnection * pConnection=new CEasyNetLinkConnection();
	if(pConnection)
	{
		pConnection->SetManager(this);
		pConnection->SetServer(m_pServer);
		pConnection->SetReportID(ReportID);
		pConnection->SetStatus(ENL_LINK_ACCEPTING);
		m_ConnectionList.Add(pConnection);
	}
	return pConnection;
}

CEasyNetLinkConnection * CEasyNetLinkManager::CreateConnection(UINT ID)
{
	CEasyNetLinkConnection * pConnection=OnCreateConnection(ID);
	if(pConnection)
	{
		pConnection->SetManager(this);
		pConnection->SetServer(m_pServer);
		pConnection->SetID(ID);
		pConnection->SetStatus(ENL_LINK_ACCEPTED);
		m_ConnectionList.Add(pConnection);
		m_ConnectionMap[ID]=pConnection;
	}
	return pConnection;
}



BOOL CEasyNetLinkManager::AcceptConnection(CEasyNetLinkConnection * pConnection)
{

	if(pConnection->NeedReallocConnectionID())
	{
		//需要重新分配ID
		int * Stay;
		UINT ID=m_ConnectionIDPool.NewObject(&Stay);
		if(ID==0)
		{
			PrintNetLog(0xffffffff,"CEasyNetLinkManager::AcceptConnection 无法给连接[%u]重新分配一个ID。",
				CClassifiedID(pConnection->GetID()).ToStr());
			return FALSE;
		}
		CClassifiedID ConnectionID=pConnection->GetID();
		ConnectionID.BigIndex()=ID;
		pConnection->SetID(ConnectionID);
	}
	else
	{
		CEasyNetLinkConnection * pExistConnection=GetConnection(pConnection->GetID());
		if(pExistConnection)
		{
			if(pExistConnection==pConnection)
			{
				OnLinkStart(pConnection);
				pConnection->OnLinkStart();
				return TRUE;
			}
			PrintNetLog(0xffffffff,"CEasyNetLinkManager::AcceptConnection 连接[%u]已经存在。",
				CClassifiedID(pConnection->GetID()).ToStr());
			return FALSE;
		}
	}

	//重新创建对象
	CEasyNetLinkConnection * pNewConnection=CreateConnection(pConnection->GetID());
	if(pNewConnection==NULL)
	{
		PrintNetLog(0xffffffff,"CEasyNetLinkManager::AcceptConnection 无法给连接[%s]创建合适的对象。",
			CClassifiedID(pConnection->GetID()).ToStr());

		return FALSE;
	}
	pConnection->PrepareSteal(pNewConnection);

	return TRUE;

}

BOOL CEasyNetLinkManager::DeleteConnection(CEasyNetLinkConnection * pConnection)
{
	if(pConnection->NeedReallocConnectionID())
	{
		CClassifiedID ConnectionID=pConnection->GetID();
		m_ConnectionIDPool.DeleteObject(ConnectionID.BigIndex());
	}

	m_ConnectionMap.Delete(pConnection->GetID());
	for(size_t i=0;i<m_ConnectionList.GetCount();i++)
	{
		if(m_ConnectionList[i]==pConnection)
		{
			m_ConnectionList.Delete(i);
			return TRUE;
		}
	}
	return FALSE;
}



CEasyNetLinkService * CEasyNetLinkManager::CreateLinkService(UINT ID)
{
	if(GetService(ID))
	{
		PrintNetLog(0xffffffff,"CEasyNetLinkManager::CreateService 服务[%u]已经存在。",ID);
		return NULL;
	}

	CEasyNetLinkService * pService=OnCreateService(ID);
	if(pService)
	{
		pService->SetID(ID);
		pService->SetManager(this);
		pService->SetServer(m_pServer);
		m_ServiceMap[ID]=pService;
		m_ServiceList.Add(pService);
	}
	return pService;
}

BOOL CEasyNetLinkManager::DeleteLinkService(CEasyNetLinkService * pService)
{
	if(m_ServiceMap.Delete(pService->GetID()))
	{
		for(size_t i=0;i<m_ServiceList.GetCount();i++)
		{
			if(m_ServiceList[i]==pService)
			{
				m_ServiceList.Delete(i);
				break;
			}
		}
		return TRUE;
	}
	return FALSE;
}

void CEasyNetLinkManager::OnLinkStart(CEasyNetLinkConnection * pConnection)
{
}

void CEasyNetLinkManager::OnLinkEnd(CEasyNetLinkConnection * pConnection)
{
}

void CEasyNetLinkManager::PrintLinkInfo(UINT LogChannel)
{
	CLogManager::GetInstance()->PrintLog(LogChannel,ILogPrinter::LOG_LEVEL_NORMAL,0,"Service List:");
	for(size_t i=0;i<m_ServiceList.GetCount();i++)
	{
		m_ServiceList[i]->PrintInfo(LogChannel);
	}
	CLogManager::GetInstance()->PrintLog(LogChannel,ILogPrinter::LOG_LEVEL_NORMAL,0,"Link List:");
	for(int i=(int)m_ConnectionList.GetCount()-1;i>=0;i--)
	{
		m_ConnectionList[i]->PrintInfo(LogChannel);
	}
}
