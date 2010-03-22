#include "StdAfx.h"

CMainThread::CMainThread(void)
{
	FUNCTION_BEGIN;
	m_pServerManagerService=NULL;
	m_pFileCache=NULL;
	m_Status=SFSS_INITIALIZING;
	FUNCTION_END;
}

CMainThread::~CMainThread(void)
{
}

BOOL CMainThread::OnStart()
{
	FUNCTION_BEGIN;
	if(!CServerThread::OnStart())
		return FALSE;	
	

	if(!CMainConfig::GetInstance()->LoadConfig(MakeModuleFullPath(NULL,CONFIG_FILE_NAME)))
	{
		return FALSE;
	}

	

	m_pServerManagerService=new CServerManagerService();	
	if(!m_pServerManagerService->Init(this))
	{
		return FALSE;
	}

	m_pFileCache=new CFileCache;
	if(!m_pFileCache->Create(CMainConfig::GetInstance()->GetFileCacheSize(),
		CMainConfig::GetInstance()->GetFileCacheQueryQueueSize()))
	{
		Log("FileCache³õÊ¼»¯Ê§°Ü");
		return FALSE;
	}

	return TRUE;
	FUNCTION_END;
	return FALSE;
}

int CMainThread::Update(int ProcessPacketLimit)
{
	FUNCTION_BEGIN;
	ProcessPacketLimit=1024;
	int Process=0;
	Process+=CServerThread::Update(ProcessPacketLimit);	
	Process+=m_pServerManagerService->Update(ProcessPacketLimit);
	Process+=m_pFileCache->Update(ProcessPacketLimit);
	
	return Process;
	FUNCTION_END;
	return 0;
}

void CMainThread::OnBeginTerminate()
{
	FUNCTION_BEGIN;
	m_Status=SFSS_TERMINATING;
	FUNCTION_END;
}

BOOL CMainThread::OnTerminating()
{
	FUNCTION_BEGIN;
	SAFE_DELETE(m_pFileCache);
	SAFE_RELEASE(m_pServerManagerService);
	FUNCTION_END;
	return false;
}

void CMainThread::OnTerminate()
{
	FUNCTION_BEGIN;
	CServerThread::OnTerminate();	
	FUNCTION_END;
}


int CMainThread::GetClientCount()
{
	FUNCTION_BEGIN;
	if(m_pServerManagerService)
		return m_pServerManagerService->GetClientCount();
	FUNCTION_END;
	return 0;
}

LPCTSTR CMainThread::GetConfigFileName()
{
	return CONFIG_FILE_NAME;
}