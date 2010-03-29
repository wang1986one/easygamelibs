/****************************************************************************/
/*                                                                          */
/*      文件名:    NetLinkManager.h                                         */
/*      创建日期:  2009年02月09日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
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
