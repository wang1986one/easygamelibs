/****************************************************************************/
/*                                                                          */
/*      文件名:    DOSClient.h                                              */
/*      创建日期:  2009年10月15日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once


class CDOSClient :	
	public CNetConnection,
	public IDistributedObjectOperator
{
protected:
	CEasyBuffer		m_AssembleBuffer;	
	CEasyBuffer		m_SendBuffer;

	DECLARE_CLASS_INFO(CDOSClient);
public:
	CDOSClient(void);
	virtual ~CDOSClient(void);	
	virtual void Destory();
	

	BOOL Start(UINT MaxMsgSize,const CIPAddress& Address,DWORD TimeOut=NO_CONNECTION_TIME_OUT);
	void Close();

	virtual UINT GetRouterID();
	virtual OBJECT_ID GetObjectID();
	virtual BOOL SendMessage(OBJECT_ID ReceiverID,MSG_ID_TYPE MsgID,WORD MsgFlag=0,LPCVOID pData=0,UINT DataSize=0);
	virtual BOOL SendMessageMulti(OBJECT_ID * pReceiverIDList,UINT ReceiverCount,bool IsSorted,MSG_ID_TYPE MsgID,WORD MsgFlag=0,LPCVOID pData=0,UINT DataSize=0);

	virtual CDOSMessagePacket * NewMessagePacket(UINT DataSize,UINT ReceiverCount);
	virtual BOOL ReleaseMessagePacket(CDOSMessagePacket * pPacket);
	virtual BOOL SendMessagePacket(CDOSMessagePacket * pPacket);

	virtual BOOL RegisterMsgMap(OBJECT_ID ProxyObjectID,MSG_ID_TYPE * pMsgIDList,int CmdCount);
	virtual BOOL UnregisterMsgMap(OBJECT_ID ProxyObjectID,MSG_ID_TYPE * pMsgIDList,int CmdCount);
	virtual BOOL RegisterGlobalMsgMap(ROUTE_ID_TYPE ProxyRouterID,MSG_ID_TYPE * pMsgIDList,int CmdCount);
	virtual BOOL UnregisterGlobalMsgMap(ROUTE_ID_TYPE ProxyRouterID,MSG_ID_TYPE * pMsgIDList,int CmdCount);

	virtual void PrintLog(int Level,DWORD Color,LPCTSTR Format,va_list vl);

	virtual BOOL RegisterObject(DOS_OBJECT_REGISTER_INFO_EX& ObjectRegisterInfo);
	virtual void Release();

protected:
	virtual void OnRecvData(const CEasyBuffer& DataBuffer);
	virtual BOOL OnDOSMessage(CDOSSimpleMessage * pMessage);

};
