/****************************************************************************/
/*                                                                          */
/*      �ļ���:    NetLink.h                                                */
/*      ��������:  2009��02��09��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
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
