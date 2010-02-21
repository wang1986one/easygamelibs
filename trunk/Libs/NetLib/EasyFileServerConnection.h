#pragma once

class CEasyFileServerConnection :
	public CNetConnection
{
protected:

	DECLARE_CLASS_INFO_STATIC(CEasyFileServerConnection);
public:
	CEasyFileServerConnection(void);
	virtual ~CEasyFileServerConnection(void);

	virtual int Update(int ProcessPacketLimit=DEFAULT_SERVER_PROCESS_PACKET_LIMIT);
	virtual void OnRecvData(const CEasyBuffer& DataBuffer);
	virtual void OnConnection(BOOL IsSucceed);
	virtual void OnDisconnection();
};
