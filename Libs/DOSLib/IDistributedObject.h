/****************************************************************************/
/*                                                                          */
/*      文件名:    IDistributedObject.h                                     */
/*      创建日期:  2009年09月11日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once

class IDistributedObject;

class IDistributedObjectOperator
{
public:
	virtual UINT GetRouterID()=0;
	virtual OBJECT_ID GetObjectID()=0;
	virtual BOOL SendMessage(OBJECT_ID ReceiverID,WORD MsgID,LPCVOID pData=0,UINT DataSize=0)=0;
	virtual BOOL SendMessageMulti(OBJECT_ID * pReceiverIDList,UINT ReceiverCount,bool IsSorted,WORD MsgID,LPCVOID pData=0,UINT DataSize=0)=0;

	virtual CDOSMessagePacket * NewMessagePacket(UINT DataSize,UINT ReceiverCount)=0;
	virtual BOOL ReleaseMessagePacket(CDOSMessagePacket * pPacket)=0;
	virtual BOOL SendMessagePacket(CDOSMessagePacket * pPacket)=0;

	virtual BOOL InitMsgProc(int MaxMsgCount)=0;
	virtual void RegisterMsgProc(WORD CmdID,DOS_MSG_PROC MsgProc)=0;
	virtual void UnregisterMsgProc(WORD CmdID,DOS_MSG_PROC MsgProc)=0;

	virtual BOOL RegisterMsgMap(OBJECT_ID ProxyObjectID,WORD * pCmdIDList,int CmdCount)=0;
	virtual BOOL UnregisterMsgMap(OBJECT_ID ProxyObjectID,WORD * pCmdIDList,int CmdCount)=0;
	virtual BOOL RegisterGlobalMsgMap(WORD ProxyRouterID,WORD * pCmdIDList,int CmdCount)=0;
	virtual BOOL UnregisterGlobalMsgMap(WORD ProxyRouterID,WORD * pCmdIDList,int CmdCount)=0;

	virtual void PrintLog(int Level,DWORD Color,LPCTSTR Format,va_list vl)=0;

	virtual BOOL RegisterObject(IDistributedObject * pObject,OBJECT_ID ObjectID,int Weight=1,bool IsSynchro=false)=0;
	virtual void Release()=0;
};

class IDistributedObject:public CNameObject
{
public:
	virtual BOOL Initialize(IDistributedObjectOperator * pOperator)=0;
	virtual void Destory()=0;

	virtual BOOL OnPreTranslateMessage(CDOSMessage * pMessage)=0;
	virtual int Update(int ProcessPacketLimit=DEFAULT_SERVER_PROCESS_PACKET_LIMIT)=0;
};

class IDistributedObjectManager
{
public:
	virtual BOOL RegisterObject(IDistributedObject * pObject,OBJECT_ID ObjectID,int Weight=1)=0;
};