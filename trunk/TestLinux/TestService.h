#pragma once

class CTestService :
	public CNetService
{
protected:
	CIDStorage<CTestConnection>		m_ConnectionPool;
public:
	CTestService(void);
	~CTestService(void);

	bool Init();

	virtual int Update(int ProcessPacketLimit=DEFAULT_SERVER_PROCESS_PACKET_LIMIT);

	virtual CBaseTCPConnection * CreateConnection(CIPAddress& RemoteAddress);
	virtual BOOL DeleteConnection(CBaseTCPConnection * pConnection);

	UINT GetConnectionCount()
	{
		return m_ConnectionPool.GetObjectCount();
	}
};
