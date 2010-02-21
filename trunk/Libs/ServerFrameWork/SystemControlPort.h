#pragma once

class CSystemControlPort :
	public CNetService
{
protected:
	IBaseServer * m_pServer;
public:
	CSystemControlPort(void);
	~CSystemControlPort(void);

	bool Init(IBaseServer * pServer);

	virtual void OnStartUp();
	virtual void OnClose();

	virtual void OnRecvData(const CIPAddress& IPAddress,const CEasyBuffer& DataBuffer);

	void SendMsg(const CIPAddress& Address,WORD Msg,LPCVOID pData=NULL,int DataLen=0);
};
