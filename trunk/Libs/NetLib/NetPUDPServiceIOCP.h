/****************************************************************************/
/*                                                                          */
/*      �ļ���:    NetPUDPServiceIOCP.h                                     */
/*      ��������:  2009��07��06��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once


class CNetServer;

class CNetPUDPService :	
	public CBaseService,public IIOCPEventHandler
{
protected:
	volatile BOOL							m_WantClose;
	CNetServer *							m_pServer;
	int										m_ParallelRecvCount;	
	CIOCPEventRouter *						m_pIOCPEventRouter;
	

	DECLARE_CLASS_INFO_STATIC(CNetPUDPService);
public:
	CNetPUDPService();
	virtual ~CNetPUDPService(void);
	

	void SetServer(CNetServer * pServer);

	CNetServer * GetServer();

	virtual BOOL OnIOCPEvent(int EventID,COverLappedObject * pOverLappedObject);
	

	virtual BOOL Create(int ParallelRecvCount=DEFAULT_PARALLEL_RECV);
	virtual void Destory();
	
	BOOL StartListen(const CIPAddress& Address);
	void Close();	

	virtual void OnStartUp();
	virtual void OnClose();

	BOOL QueryUDPSend(const CIPAddress& IPAddress,LPCVOID pData,int Size);

	virtual void OnRecvData(const CIPAddress& IPAddress,const CEasyBuffer& DataBuffer);


	
protected:	
	BOOL QueryUDPRecv();
	
};


inline CNetServer * CNetPUDPService::GetServer()
{
	return m_pServer;
}

