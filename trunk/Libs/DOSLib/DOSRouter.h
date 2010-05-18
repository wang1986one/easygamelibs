/****************************************************************************/
/*                                                                          */
/*      文件名:    DOSRouter.h                                              */
/*      创建日期:  2009年07月06日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
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