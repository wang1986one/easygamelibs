/****************************************************************************/
/*                                                                          */
/*      �ļ���:    IDistributedObject.h                                     */
/*      ��������:  2009��09��11��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once

#define MAKE_MSG_ID(InterfaceID,MethodID,IsAck)		((((DWORD)InterfaceID&0x7FFF)<<16)|(((DWORD)MethodID)&0xFFFF)|(IsAck?0x80000000:0))
#define GET_INTERFACE_ID(MsgID)						(((DWORD)MsgID&0x7FFF)>>16)
#define GET_METHOD_ID(MsgID)						((DWORD)MsgID&0xFFFF)
#define IS_ACK_MSG(MsgID)							((DWORD)MsgID&0x7FFFFFFF)




#define DECLARE_DOS_MSG_MAP(ClassName)		\
	protected:\
	OBJECT_ID& GetCurMsgSenderID()\
	{\
		return m_CurMsgSenderID;\
	}\
	CDOSMessage * GetCurMsg()\
	{\
		return m_pCurHandleMsg;\
	}\
	void InitDOSMsgMap();\
	BOOL HandleMsg(MSG_ID_TYPE MsgID,CSmartStruct& MsgPacket);\
	virtual BOOL OnMessage(CDOSMessage * pMessage);


#define START_DOS_MSG_MAP(ClassName)	\
	void ClassName::InitDOSMsgMap()\
	{

#define DOS_MSG_MAP(MsgHandlerClassName) \
		MsgHandlerClassName::InitMsgMap(m_MsgFnMap);

#define END_DOS_MSG_MAP(ClassName) \
	}\
	BOOL ClassName::HandleMsg(MSG_ID_TYPE MsgID,CSmartStruct& MsgPacket)\
	{\
		DOS_MSG_HANDLE_INFO * pHandleInfo=m_MsgFnMap.Find(MsgID);\
		if(pHandleInfo)\
		{\
			return ((pHandleInfo->pObject)->*(pHandleInfo->pFN))(MsgPacket);\
		}\
		return COMMON_RESULT_MSG_HANDLER_NOT_FIND;\
	}\
	BOOL ClassName::OnMessage(CDOSMessage * pMessage)\
	{\
		m_CurMsgSenderID=pMessage->GetSenderID();\
		m_pCurHandleMsg=pMessage;\
		return HandleMsg(pMessage->GetMsgID(),pMessage->GetDataPacket());\
	}

#define INIT_DOS_MSG_MAP	InitDOSMsgMap();



#define DECLARE_DOS_MSG_MAP_CLIENT(ClassName) \
	protected:\
	CDOSSimpleMessage * GetCurMsg()\
	{\
		return m_pCurHandleMsg;\
	}\
	void InitDOSMsgMap();\
	BOOL HandleMsg(MSG_ID_TYPE MsgID,CSmartStruct& MsgPacket);\
	virtual BOOL OnDOSMessage(CDOSSimpleMessage * pMessage);

#define START_DOS_MSG_MAP_CLIENT(ClassName)	\
	void ClassName::InitDOSMsgMap()\
	{

#define DOS_MSG_MAP_CLIENT(MsgHandlerClassName) \
		MsgHandlerClassName::InitMsgMap(m_MsgFnMap);

#define END_DOS_MSG_MAP_CLIENT(ClassName) \
	}\
	BOOL ClassName::HandleMsg(MSG_ID_TYPE MsgID,CSmartStruct& MsgPacket)\
	{\
		DOS_MSG_HANDLE_INFO * pHandleInfo=m_MsgFnMap.Find(MsgID);\
		if(pHandleInfo)\
		{\
			return ((pHandleInfo->pObject)->*(pHandleInfo->pFN))(MsgPacket);\
		}\
		return COMMON_RESULT_MSG_HANDLER_NOT_FIND;\
	}\
	BOOL ClassName::OnDOSMessage(CDOSSimpleMessage * pMessage)\
	{\
		m_pCurHandleMsg=pMessage;\
		return HandleMsg(pMessage->GetMsgID(),pMessage->GetDataPacket());\
	}


class CBaseMsgHandler
{
public:
	CBaseMsgHandler()
	{

	}
	virtual ~CBaseMsgHandler()
	{

	}

};

typedef int (CBaseMsgHandler::*DOS_MSG_HANDLE_FN)(CSmartStruct& MsgPacket);

struct DOS_MSG_HANDLE_INFO
{
	CBaseMsgHandler *	pObject;
	DOS_MSG_HANDLE_FN	pFN;
};

enum COMMON_RESULT_CODE
{
	COMMON_RESULT_SUCCEED=0,	
	COMMON_RESULT_MSG_PACK_ERROR=189001,
	COMMON_RESULT_MSG_UNPACK_ERROR=189002,
	COMMON_RESULT_MSG_ALLOC_ERROR=189101,
	COMMON_RESULT_MSG_SEND_ERROR=189102,
	COMMON_RESULT_MSG_HANDLER_NOT_FIND=189103,
	COMMON_RESULT_FAILED=-1,
};

class IDistributedObject;

struct DOS_OBJECT_REGISTER_INFO_EX
{
	OBJECT_ID				ObjectID;
	int						Weight;
	UINT					MsgQueueSize;
	UINT					MsgProcessLimit;
	IDistributedObject *	pObject;
	UINT					Param;

	DOS_OBJECT_REGISTER_INFO_EX()
	{
		ObjectID=0;
		Weight=1;
		MsgQueueSize=0;
		MsgProcessLimit=0;
		pObject=NULL;
		Param=0;
	}
};

class IDistributedObjectOperator
{
public:
	virtual UINT GetRouterID()=0;
	virtual OBJECT_ID GetObjectID()=0;
	virtual BOOL SendMessage(OBJECT_ID ReceiverID,MSG_ID_TYPE MsgID,WORD MsgFlag=0,LPCVOID pData=0,UINT DataSize=0)=0;
	virtual BOOL SendMessageMulti(OBJECT_ID * pReceiverIDList,UINT ReceiverCount,bool IsSorted,MSG_ID_TYPE MsgID,WORD MsgFlag=0,LPCVOID pData=0,UINT DataSize=0)=0;

	virtual CDOSMessagePacket * NewMessagePacket(UINT DataSize,UINT ReceiverCount)=0;
	virtual BOOL ReleaseMessagePacket(CDOSMessagePacket * pPacket)=0;
	virtual BOOL SendMessagePacket(CDOSMessagePacket * pPacket)=0;

	virtual BOOL RegisterMsgMap(OBJECT_ID ProxyObjectID,MSG_ID_TYPE * pMsgIDList,int CmdCount)=0;
	virtual BOOL UnregisterMsgMap(OBJECT_ID ProxyObjectID,MSG_ID_TYPE * pMsgIDList,int CmdCount)=0;
	virtual BOOL RegisterGlobalMsgMap(ROUTE_ID_TYPE ProxyRouterID,MSG_ID_TYPE * pMsgIDList,int CmdCount)=0;
	virtual BOOL UnregisterGlobalMsgMap(ROUTE_ID_TYPE ProxyRouterID,MSG_ID_TYPE * pMsgIDList,int CmdCount)=0;

	virtual BOOL AddConcernedObject(OBJECT_ID ObjectID,bool NeedTest)=0;
	virtual BOOL DeleteConcernedObject(OBJECT_ID ObjectID)=0;

	virtual BOOL FindObject(UINT ObjectType)=0;
	virtual BOOL ReportObject(OBJECT_ID TargetID,const CSmartStruct& ObjectInfo)=0;

	virtual BOOL RegisterObject(DOS_OBJECT_REGISTER_INFO_EX& ObjectRegisterInfo)=0;
	virtual void Release()=0;
};

class IDistributedObject:public CNameObject
{
protected:
	OBJECT_ID									m_CurMsgSenderID;
	CDOSMessage *								m_pCurHandleMsg;
	CEasyMap<MSG_ID_TYPE,DOS_MSG_HANDLE_INFO>	m_MsgFnMap;
public:
	virtual BOOL Initialize(IDistributedObjectOperator * pOperator)=0;
	virtual void Destory()=0;

	virtual BOOL OnPreTranslateMessage(CDOSMessage * pMessage){return FALSE;}
	virtual BOOL OnMessage(CDOSMessage * pMessage){return FALSE;}
	virtual BOOL OnSystemMessage(CDOSMessage * pMessage){return FALSE;}
	virtual void OnConcernedObjectLost(OBJECT_ID ObjectID){}
	virtual BOOL OnFindObject(OBJECT_ID CallerID){return FALSE;}
	virtual void OnObjectReport(OBJECT_ID ObjectID,const CSmartStruct& ObjectInfo){}
	virtual int Update(int ProcessPacketLimit=DEFAULT_SERVER_PROCESS_PACKET_LIMIT){return 0;}

};

class IDistributedObjectManager
{
public:
	virtual BOOL RegisterObject( DOS_OBJECT_REGISTER_INFO_EX& ObjectRegisterInfo)=0;
};

