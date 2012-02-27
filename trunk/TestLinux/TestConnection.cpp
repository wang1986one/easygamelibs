#include "StdAfx.h"
#include "TestConnection.h"

CTestConnection::CTestConnection(void)
{
}

CTestConnection::~CTestConnection(void)
{
}


void CTestConnection::OnConnection(BOOL IsSucceed)
{
	printf("Connected at %s:%u ",
		GetLocalAddress().GetIPString(),GetLocalAddress().GetPort());
	printf("from %s:%u\n",
		GetRemoteAddress().GetIPString(),GetRemoteAddress().GetPort());
}
void CTestConnection::OnDisconnection()
{
	printf("\nDisconnected\n");
	m_pService->DeleteConnection(this);
}

void CTestConnection::OnRecvData(const CEasyBuffer& DataBuffer)
{
	Send(DataBuffer.GetBuffer(),DataBuffer.GetUsedSize());
	((CEasyBuffer&)DataBuffer).PushConstBack(0,1);
	printf("%s",DataBuffer.GetBuffer());
}

int CTestConnection::Update(int ProcessPacketLimit)
{
	int ProcessCount=CNetConnection::Update(ProcessPacketLimit);

	return ProcessCount;
}
