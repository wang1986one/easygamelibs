#include "StdAfx.h"


IMPLEMENT_CLASS_INFO_STATIC(CEasyNetLinkService,CNetService);

CEasyNetLinkService::CEasyNetLinkService(void)
{
	m_pManager=NULL;
	m_NeedReallocConnectionID=FALSE;
	m_MaxPacketSize=0;	
}

CEasyNetLinkService::~CEasyNetLinkService(void)
{
}


CBaseTCPConnection * CEasyNetLinkService::CreateConnection(CIPAddress& RemoteAddress)
{
	if(m_IPList.size())
	{
		bool IsMatch=false;
		for(size_t i=0;i<m_IPList.size();i++)
		{
			if(m_IPList.at(i).IsMatch(RemoteAddress))
			{
				IsMatch=true;
				break;
			}
		}
		if(!IsMatch)
		{
			PrintNetLog(0xffffff,"连接[%s]不在IP列表中，被拒绝",RemoteAddress.GetIPString());
			return NULL;
		}
	}
	CEasyNetLinkConnection * pConnection=NULL;
	if(m_pManager)
	{
		pConnection=m_pManager->CreateAcceptConnection(GetReportID());
		if(pConnection)
		{
			pConnection->SetMaxPacketSize(m_MaxPacketSize);
			pConnection->EnableReallocConnectionID(NeedReallocConnectionID());

		}
	}
	return pConnection;
}

BOOL CEasyNetLinkService::DeleteConnection(CBaseTCPConnection * pConnection)
{
	if(m_pManager)
	{
		m_pManager->DeleteConnection((CEasyNetLinkConnection *)pConnection);
		SAFE_RELEASE(pConnection);
	}
	return TRUE;
}

void CEasyNetLinkService::PrintInfo(UINT LogChannel)
{
	CLogManager::GetInstance()->PrintLog(LogChannel,ILogPrinter::LOG_LEVEL_NORMAL,0,
		"ServiceID=[%s] ListenAddress=%s:%u",
		CClassifiedID(m_ReportID).ToStr(),
		GetLocalAddress().GetIPString(),
		GetLocalAddress().GetPort());
}