/****************************************************************************/
/*                                                                          */
/*      �ļ���:    SystemNetLinkManager.h                                   */
/*      ��������:  2009��07��06��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
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
