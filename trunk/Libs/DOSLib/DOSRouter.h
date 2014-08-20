/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DOSRouter.h                                              */
/*      ��������:  2009��07��06��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once


class CDOSServer;

class CDOSRouter :
	public CEasyNetLinkManager,public CEasyThread
{
protected:
	
	CDOSServer										*m_pServer;
	CThreadSafeIDStorage<CDOSMessagePacket *>		m_MsgQueue;
	UINT											m_MsgProcessLimit;

	volatile UINT									m_RouteInMsgCount;
	volatile UINT									m_RouteInMsgFlow;
	volatile UINT									m_RouteOutMsgCount;
	volatile UINT									m_RouteOutMsgFlow;
	
	CThreadPerformanceCounter						m_ThreadPerformanceCounter;

	DECLARE_CLASS_INFO(CDOSRouter);
public:
	CDOSRouter(void);
	virtual ~CDOSRouter(void);

	BOOL Init(CDOSServer * pServer);

	virtual BOOL OnStart();
	virtual void OnClose();
	virtual BOOL OnRun();

	virtual CEasyNetLinkConnection * OnCreateConnection(UINT ID);
	virtual void OnLinkStart(CEasyNetLinkConnection * pConnection);
	virtual void OnLinkEnd(CEasyNetLinkConnection * pConnection);	
	

	BOOL RouterMessage(OBJECT_ID SenderID,OBJECT_ID ReceiverID,MSG_ID_TYPE MsgID,WORD MsgFlag,LPCVOID pData,UINT DataSize);

	BOOL RouterMessage(CDOSMessagePacket * pPacket);

	UINT GetRouterID();
	CDOSServer * GetServer();

	UINT GetInMsgCount();
	UINT GetInMsgFlow();
	UINT GetOutMsgCount();
	UINT GetOutMsgFlow();
	UINT GetMsgQueueLen();
	float GetCycleTime();
	float GetCPUUsedRate();
	void ResetStatData();
protected:
	int DoMessageRoute(int ProcessPacketLimit=DEFAULT_SERVER_PROCESS_PACKET_LIMIT);	
	BOOL IsSameRouter(OBJECT_ID * pReceiverIDs,int Count);
	int GetGroupCount(OBJECT_ID * pReceiverIDs,int Count);	

	BOOL DispatchMessage(CDOSMessagePacket * pPacket,OBJECT_ID * pReceiverIDs,int Count);
};

inline CDOSServer * CDOSRouter::GetServer()
{
	return m_pServer;
}

inline UINT CDOSRouter::GetInMsgCount()
{
	return m_RouteInMsgCount;
}
inline UINT CDOSRouter::GetInMsgFlow()
{
	return m_RouteInMsgFlow;
}
inline UINT CDOSRouter::GetOutMsgCount()
{
	return m_RouteOutMsgCount;
}
inline UINT CDOSRouter::GetOutMsgFlow()
{
	return m_RouteOutMsgFlow;
}
inline UINT CDOSRouter::GetMsgQueueLen()
{
	return m_MsgQueue.GetObjectCount();
}
inline float CDOSRouter::GetCycleTime()
{
	return m_ThreadPerformanceCounter.GetCycleTime();
}
inline float CDOSRouter::GetCPUUsedRate()
{
	return m_ThreadPerformanceCounter.GetCPUUsedRate();
}
inline void CDOSRouter::ResetStatData()
{
	m_RouteInMsgCount=0;
	m_RouteInMsgFlow=0;
	m_RouteOutMsgCount=0;
	m_RouteOutMsgFlow=0;	
}