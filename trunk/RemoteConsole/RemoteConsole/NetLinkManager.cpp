/****************************************************************************/
/*                                                                          */
/*      文件名:    NetLinkManager.cpp                                       */
/*      创建日期:  2009年02月09日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
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
