#include "StdAfx.h"

CMainThread::CMainThread(void)
{
	FUNCTION_BEGIN;
	//m_pSimpleFileService=NULL;
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

    Log("%lld",CEasyTimerEx::GetTime());

	m_pService=new CTestService();

	m_pService->SetServer(this);
	m_pService->Init();

	//CLinuxFileAccessor File;

	//int FileOpenMode=IFileAccessor::modeAppend|IFileAccessor::modeWrite|
	//	IFileAccessor::shareShareRead|IFileAccessor::osWriteThrough;

	//File.Open("Test.txt",FileOpenMode);

	//File.Write("Test\r\n",6);

	//if(!CMainConfig::GetInstance()->LoadConfig(MakeModuleFullPath(NULL,CONFIG_FILE_NAME)))
	//{
	//	return FALSE;
	//}

	//

	//m_pSimpleFileService=new CSimpleFileService();
	//if(!m_pSimpleFileService->Init(this))
	//{
	//	return FALSE;
	//}

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
	//Process+=m_pSimpleFileService->Update(ProcessPacketLimit);

	Process+=m_pService->Update(ProcessPacketLimit);

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
	SAFE_RELEASE(m_pService);
	//SAFE_RELEASE(m_pSimpleFileService);
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
	if(m_pService)
		return m_pService->GetConnectionCount();
	//if(m_pSimpleFileService)
	//	return m_pSimpleFileService->GetClientCount();
	FUNCTION_END;
	return 0;
}

LPCTSTR CMainThread::GetConfigFileName()
{
	return CONFIG_FILE_NAME;
}
