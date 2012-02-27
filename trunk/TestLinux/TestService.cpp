#include "StdAfx.h"

CTestService::CTestService(void)
{
	m_ConnectionPool.Create(1024);
}

CTestService::~CTestService(void)
{
}

bool CTestService::Init()
{
	Create();
	if(StartListen(CIPAddress((DWORD)0,12345)))
	{
		Log("Start Listen Succeed!");
	}
	else
	{
		Log("Start Listen Failed!");
	}
	return true;
}

int CTestService::Update(int ProcessPacketLimit)
{
	int ProcessCount=CNetService::Update(ProcessPacketLimit);

	LPVOID Pos=m_ConnectionPool.GetFirstObjectPos();
	while(Pos)
	{
		CTestConnection * pConnection=m_ConnectionPool.GetNext(Pos);
		ProcessCount+=pConnection->Update(ProcessPacketLimit);
	}

	return ProcessCount;
}

CBaseTCPConnection * CTestService::CreateConnection(CIPAddress& RemoteAddress)
{
	CTestConnection * pConnection=m_ConnectionPool.NewObject();
	if(pConnection)
	{
		pConnection->SetServer(GetServer());
		pConnection->SetService(this);
		return pConnection;
	}
	return NULL;
}

BOOL CTestService::DeleteConnection(CBaseTCPConnection * pConnection)
{
	pConnection->Destory();
	return m_ConnectionPool.DeleteObject(pConnection->GetID());
}