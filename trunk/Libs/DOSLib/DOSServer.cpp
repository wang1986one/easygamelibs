#include "StdAfx.h"

IMPLEMENT_CLASS_INFO_STATIC(CDOSServer,CNetServer);

CDOSServer::CDOSServer(void):CNetServer()
{
	FUNCTION_BEGIN;
	m_pDOSObjectProxyService=NULL;
	m_pDOSRouterService=NULL;
	m_pObjectManager=NULL;	
	FUNCTION_END;
}

CDOSServer::~CDOSServer(void)
{
	FUNCTION_BEGIN;
	ShutDown();
	FUNCTION_END;
}



BOOL CDOSServer::OnStartUp()
{
	FUNCTION_BEGIN;
	if(!m_MemoryPool.Create(m_ServerConfig.MemoryPoolBlockSize,m_ServerConfig.MemoryPoolLeveSize,m_ServerConfig.MemoryPoolLevelCount,true))
	{
		PrintDOSLog(0xffff,"初始化内存池失败！");
		return FALSE;
	}
	if(m_ServerConfig.ObjectProxyServiceListenAddress.GetPort())
	{
		m_pDOSObjectProxyService=new CDOSObjectProxyService();
		if(!m_pDOSObjectProxyService->Init(this))
		{
			PrintDOSLog(0xffff,"代理服务启动失败！");
			return FALSE;
		}
		//m_pDOSObjectProxyService->WaitForWorking(DEFAULT_THREAD_STARTUP_TIME);
		PrintDOSLog(0xffff,"代理服务启动！");
	}

	m_pDOSRouterService=new CDOSRouter();	
	if(!m_pDOSRouterService->Init(this))
	{
		PrintDOSLog(0xffff,"路由服务启动失败！");
		return FALSE;
	}
	//m_pDOSRouterService->WaitForWorking(DEFAULT_THREAD_STARTUP_TIME);
	PrintDOSLog(0xffff,"路由服务启动！");

	m_pObjectManager=new CDOSObjectManager();

	m_pObjectManager->SetServer(this);
	if(!m_pObjectManager->Initialize())
	{
		return FALSE;
	}


	PrintDOSLog(0xffff,"对象管理器启动！");

	PrintDOSLog(0xffff,"服务器(%d)启动！",m_ServerConfig.RouterID);

	return TRUE;
	FUNCTION_END;
	return FALSE;
}

void CDOSServer::OnShutDown()
{
	FUNCTION_BEGIN;
	m_MemoryPool.Verfy();

	if(m_pDOSObjectProxyService)
	{
		m_pDOSObjectProxyService->SafeTerminate();
	}

	if(m_pDOSRouterService)
	{
		m_pDOSRouterService->SafeTerminate();
	}

	SAFE_RELEASE(m_pObjectManager);

	if(m_pDOSObjectProxyService)
	{
		delete m_pDOSObjectProxyService;
		m_pDOSObjectProxyService=NULL;
	}

	if(m_pDOSRouterService)
	{
		delete m_pDOSRouterService;
		m_pDOSRouterService=NULL;
	}

	m_MemoryPool.Destory();

	FUNCTION_END;
}
