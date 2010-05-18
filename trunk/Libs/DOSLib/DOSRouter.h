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
	

	BOOL RouterMessage(OBJECT_ID SenderID,OBJECT_ID ReceiverID,WORD MsgID,LPCVOID pData,UINT DataSize);

	BOOL RouterMessage(CDOSMessagePacket * pPacket);

	UINT GetRouterID();
	CDOSServer * GetServer();
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