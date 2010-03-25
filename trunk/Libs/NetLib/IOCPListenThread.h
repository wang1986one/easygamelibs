/****************************************************************************/
/*                                                                          */
/*      �ļ���:    IOCPListenThread.h                                       */
/*      ��������:  2009��07��06��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once

class CNetService;

class CIOCPListenThread :
	public CEasyThread
{
protected:
	SOCKET			m_ListenSocket;
	CNetService*	m_pServer;
public:
	CIOCPListenThread(void);
	virtual ~CIOCPListenThread(void);

	bool Init(CNetService * pService,SOCKET ListenSocket);
protected:	
	virtual BOOL OnStart();
	virtual BOOL OnRun();
	virtual void OnTerminate();

};


