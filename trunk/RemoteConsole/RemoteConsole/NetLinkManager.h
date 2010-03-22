#pragma once

class CNetLinkManager :
	public CEasyNetLinkManager
{
protected:
	CServerThread *		m_pServerThread;
public:
	CNetLinkManager(void);
	~CNetLinkManager(void);

	void SetServerThread(CServerThread * pServerThread)
	{
		m_pServerThread=pServerThread;
	}


	virtual CEasyNetLinkConnection * OnCreateConnection(UINT ReportID);

	virtual BOOL AcceptConnection(CEasyNetLinkConnection * pConnection);
	virtual BOOL DeleteConnection(CEasyNetLinkConnection * pConnection);

	
};
