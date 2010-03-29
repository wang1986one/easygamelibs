/****************************************************************************/
/*                                                                          */
/*      �ļ���:    NetLinkManager.h                                         */
/*      ��������:  2009��02��09��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
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
