/****************************************************************************/
/*                                                                          */
/*      文件名:    NetLink.h                                                */
/*      创建日期:  2009年02月09日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once

class CRemoteConsoleView;

class CNetLink :
	public CEasyNetLinkConnection
{
protected:
	CServerThread *			m_pServerThread;
	CRemoteConsoleView *	m_pView;
	BOOL					m_IsLinkLog;
public:
	CNetLink(CServerThread * pServerThread);
	~CNetLink(void);

	void SendMsg(WORD Msg,LPCVOID pData=NULL,int DataLen=0);


	virtual void OnLinkStart();
	virtual void OnLinkEnd();
	virtual void OnData(const CEasyBuffer& DataBuffer);

	BOOL IsLinkLog()
	{
		return m_IsLinkLog;
	}

	void ForceDisconnect()
	{
		m_pView=NULL;
		SetKeepConnect(FALSE);
		Disconnect();
	}
};
