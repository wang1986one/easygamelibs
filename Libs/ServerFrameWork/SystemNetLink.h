#pragma once

class CSystemNetLink :
	public CEasyNetLinkConnection
{
protected:
	IBaseServer *		m_pServer;
	BOOL				m_IsLinkLog;
public:
	CSystemNetLink(IBaseServer * pServer);
	~CSystemNetLink(void);

	void SendMsg(WORD Msg,LPCVOID pData=NULL,int DataLen=0);


	virtual void OnLinkStart();
	virtual void OnLinkEnd();
	virtual void OnData(const CEasyBuffer& DataBuffer);

	BOOL IsLinkLog()
	{
		return m_IsLinkLog;
	}
};
