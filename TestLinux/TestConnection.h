#pragma once

class CTestService;

class CTestConnection :
	public CNetConnection
{
protected:
	CTestService		*m_pService;
public:
	CTestConnection(void);
	~CTestConnection(void);

	void SetService(CTestService * pService)
	{
		m_pService=pService;
	}

	virtual void OnConnection(BOOL IsSucceed);
	virtual void OnDisconnection();

	virtual void OnRecvData(const CEasyBuffer& DataBuffer);

	virtual int Update(int ProcessPacketLimit=DEFAULT_SERVER_PROCESS_PACKET_LIMIT);
};
