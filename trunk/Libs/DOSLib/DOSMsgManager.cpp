/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DOSMsgManager.cpp                                        */
/*      ��������:  2009��09��11��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
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
			PrintDOSLog(0xff0000,"δע���Ŀͻ�����Ϣ����ID=%u,Obj=%p,Proc=%p",
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
		PrintDOSLog(0xff0000,"��Ϣ��������ʼ��ʧ�ܣ�");
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
		PrintDOSDebugLog(0xff0000,"��Ϣ%uδ�ҵ���Ӧ�Ĵ�����",pMessage->GetCmdID());
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
			PrintDOSDebugLog(0x00ffffff,"ע����Ϣ%u�Ĵ�����ʱ������ע�ắ��%p,%p",
				MsgID,pExistEntry->pObject,pExistEntry->pProc);
		}		
		m_MsgMap.Insert(MsgID,entry);
		PrintDOSDebugLog(0x00ffffff,"ע����Ϣ����%u,%p,%p",MsgID,ProcObject,MsgProc);
	}
	else
	{
		m_MsgMap.Delete(MsgID);		
		PrintDOSDebugLog(0x00ffffff,"ע����Ϣ����%u,%p,%p",MsgID,ProcObject,MsgProc);
		
	}
	
	FUNCTION_END;
}

