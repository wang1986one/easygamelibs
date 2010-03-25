/****************************************************************************/
/*                                                                          */
/*      文件名:    DOSMsgManager.cpp                                        */
/*      创建日期:  2009年09月11日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#include "StdAfx.h"


IMPLEMENT_CLASS_INFO(CDOSMsgManager,CNameObject);

CDOSMsgManager::CDOSMsgManager(void):CNameObject()
{
	FUNCTION_BEGIN;
	FUNCTION_END;
}

CDOSMsgManager::~CDOSMsgManager(void)
{
	FUNCTION_BEGIN;
	void * Pos=m_MsgMap.GetFirstObjectPos();
	while(Pos)
	{
		WORD Key;
		DOS_MSG_PROC_ENTRY * pEntry=m_MsgMap.GetNextObject(Pos,Key);
		if(pEntry)
		{
			PrintDOSLog(0xff0000,"未注销的客户端消息处理ID=%u,Obj=%p,Proc=%p",
				Key,pEntry->pObject,pEntry->pProc);
		}
	}	
	FUNCTION_END;

}

BOOL CDOSMsgManager::Init(int MaxMsgCount)
{
	FUNCTION_BEGIN;	
	if(!m_MsgMap.Create(MaxMsgCount))
	{
		PrintDOSLog(0xff0000,"消息管理器初始化失败！");
		return FALSE;
	}
	return TRUE;
	FUNCTION_END;
	return FALSE;
}

BOOL CDOSMsgManager::DealMsg(CDOSMessage * pMessage)
{
	FUNCTION_BEGIN;
	DOS_MSG_PROC_ENTRY * pMsgProcEntry=m_MsgMap.Find(pMessage->GetCmdID());
	
	if(pMsgProcEntry)
	{
		CNameObject * pObject=pMsgProcEntry->pObject;
		DOS_MSG_PROC pProc=pMsgProcEntry->pProc;
		return (pObject->*pProc)(pMessage);
	}
	else
	{
		PrintDOSDebugLog(0xff0000,"消息%u未找到对应的处理函数",pMessage->GetCmdID());
	}
	
	FUNCTION_END;
	return FALSE;
}



void CDOSMsgManager::RegisterMsgProc(BOOL IsRegister,WORD MsgID,CNameObject * ProcObject,DOS_MSG_PROC MsgProc)
{
	FUNCTION_BEGIN;	
	if(IsRegister)
	{
		DOS_MSG_PROC_ENTRY entry;
		entry.pObject=ProcObject;
		entry.pProc=MsgProc;

		DOS_MSG_PROC_ENTRY * pExistEntry=m_MsgMap.Find(MsgID);
		if(pExistEntry)
		{
			PrintDOSDebugLog(0x00ffffff,"注册消息%u的处理函数时发现已注册函数%p,%p",
				MsgID,pExistEntry->pObject,pExistEntry->pProc);
		}		
		m_MsgMap.Insert(MsgID,entry);
		PrintDOSDebugLog(0x00ffffff,"注册消息处理%u,%p,%p",MsgID,ProcObject,MsgProc);
	}
	else
	{
		m_MsgMap.Delete(MsgID);		
		PrintDOSDebugLog(0x00ffffff,"注销消息处理%u,%p,%p",MsgID,ProcObject,MsgProc);
		
	}
	
	FUNCTION_END;
}

