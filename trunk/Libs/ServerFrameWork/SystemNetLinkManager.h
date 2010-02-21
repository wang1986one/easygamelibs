#pragma once

class CSystemNetLinkManager :
	public CEasyNetLinkManager
{
protected:
	IBaseServer *		m_pServer;
public:
	CSystemNetLinkManager(void);
	~CSystemNetLinkManager(void);

	void SetServerThread(IBaseServer * pServer)
	{
		m_pServer=pServer;
	}


	virtual CEasyNetLinkConnection * OnCreateConnection(UINT ReportID);
	

	void SendLogMsg(LPCTSTR LogMsg);
};
