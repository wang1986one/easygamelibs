/****************************************************************************/
/*                                                                          */
/*      �ļ���:    BaseServer.h                                             */
/*      ��������:  2009��09��11��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once

class CBaseServer :
	protected CEasyThread
{
protected:
	volatile UINT		m_TCPRecvBytes;
	volatile UINT		m_TCPSendBytes;
	volatile UINT		m_UDPRecvBytes;
	volatile UINT		m_UDPSendBytes;

	volatile UINT		m_TCPRecvCount;
	volatile UINT		m_TCPSendCount;
	volatile UINT		m_UDPRecvCount;
	volatile UINT		m_UDPSendCount;

	DECLARE_CLASS_INFO_STATIC(CBaseServer);
public:
	CBaseServer(void);
	~CBaseServer(void);

	void AddTCPRecvBytes(int Count)
	{
		AtomicAdd(&m_TCPRecvBytes,Count);
		AtomicInc(&m_TCPRecvCount);
	}
	void AddTCPSendBytes(int Count)
	{
		AtomicAdd(&m_TCPSendBytes,Count);
		AtomicInc(&m_TCPSendCount);
	}
	void AddUDPRecvBytes(int Count)
	{
		AtomicAdd(&m_UDPRecvBytes,Count);
		AtomicInc(&m_UDPRecvCount);
	}
	void AddUDPSendBytes(int Count)
	{
		AtomicAdd(&m_UDPSendBytes,Count);
		AtomicInc(&m_UDPSendCount);
	}
	void ResetFluxStat()
	{
		m_TCPRecvBytes=0;
		m_TCPSendBytes=0;
		m_UDPRecvBytes=0;
		m_UDPSendBytes=0;

		m_TCPRecvCount=0;
		m_TCPSendCount=0;
		m_UDPRecvCount=0;
		m_UDPSendCount=0;
	}

	virtual BOOL StartUp(int EventObjectPoolSize=MAX_EVENT_OBJECT,int ThreadNumberPerCPU=DEFAULT_THREAD_NUMBER_PER_CPU,int EventRouterPoolSiz=DEFAULT_EVENT_ROUTER_COUNT)=0;
	virtual void ShutDown(DWORD Milliseconds=DEFAULT_THREAD_TERMINATE_TIME)=0;

protected:
	virtual BOOL OnStartUp()=0;
	virtual void OnShutDown()=0;
	virtual int Update(int ProcessPacketLimit=DEFAULT_SERVER_PROCESS_PACKET_LIMIT)=0;

};
