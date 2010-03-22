#include "StdAfx.h"


CNetLinkManager::CNetLinkManager(void)
{
	FUNCTION_BEGIN;
	m_pServerThread=NULL;
	FUNCTION_END
}

CNetLinkManager::~CNetLinkManager(void)
{
}


CEasyNetLinkConnection * CNetLinkManager::OnCreateConnection(UINT ReportID)
{
	FUNCTION_BEGIN;
	return new CNetLink(m_pServerThread);	
	FUNCTION_END
	return NULL;
}


BOOL CNetLinkManager::AcceptConnection(CEasyNetLinkConnection * pConnection)
{
	FUNCTION_BEGIN;
	if(!CEasyNetLinkManager::AcceptConnection(pConnection))
		return FALSE;


	return TRUE;
	FUNCTION_END
	return FALSE;
}

BOOL CNetLinkManager::DeleteConnection(CEasyNetLinkConnection * pConnection)
{
	FUNCTION_BEGIN;
	if(!CEasyNetLinkManager::DeleteConnection(pConnection))
		return FALSE;

	return TRUE;
	FUNCTION_END
	return FALSE;
}
