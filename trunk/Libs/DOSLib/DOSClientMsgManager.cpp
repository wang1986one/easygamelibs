/****************************************************************************/
/*                                                                          */
/*      文件名:    DOSClientMsgManager.cpp                                  */
/*      创建日期:  2009年07月06日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#include "StdAfx.h"


IMPLEMENT_CLASS_INFO(CDOSClientMsgManager,CNameObject);

CDOSClientMsgManager::CDOSClientMsgManager(void):CNameObject()
{

}

CDOSClientMsgManager::~CDOSClientMsgManager(void)
{
	FUNCTION_BEGIN;
	for(int i=0;i<(int)m_MsgMap.size();i++)
	{
		for(int j=0;j<(int)m_MsgMap[i].size();j++)
		{
			PrintDOSLog(0xff0000,"未注销的客户端消息处理ID=%d,Obj=%p,Proc=%p",i,m_MsgMap[i][j].pObject,m_MsgMap[i][j].pProc);
		}
	}
	FUNCTION_END;
}


BOOL CDOSClientMsgManager::DealMsg(CDOSSimpleMessage * pMessage)
{
	FUNCTION_BEGIN;
	WORD MsgIndex=pMessage->GetCmdID();

	for(int i=0;i<(int)m_MsgMap[MsgIndex].size();i++)
	{
		CNameObject * pObject=m_MsgMap[MsgIndex][i].pObject;
		CMSG_PROC pProc=m_MsgMap[MsgIndex][i].pProc;
//		assert(!IsBadReadPtr(pObject,sizeof(CDOSClientMsgCatcher)));
		if((pObject->*pProc)(pMessage))
			return true;
	}
	FUNCTION_END;
	return false;
}



void CDOSClientMsgManager::RegisterMsgProc(BOOL IsRegister,WORD MsgID,CNameObject * ProcObject,CMSG_PROC MsgProc)
{
	FUNCTION_BEGIN;
	WORD MsgIndex=MsgID;

	if(IsRegister)
	{
		CMSG_PROC_ENTRY entry;
		entry.pObject=ProcObject;
		entry.pProc=MsgProc;

		for(int i=0;i<(int)m_MsgMap[MsgIndex].size();i++)
		{
			if(m_MsgMap[MsgIndex][i].pObject==ProcObject)
			{
				m_MsgMap[MsgIndex][i]=entry;
				PrintDOSDebugLog(0x00ffffff,"重新注册消息处理%d,%p,%p",MsgID,ProcObject,MsgProc);
				return;
			}
		}
		m_MsgMap[MsgIndex].insert(m_MsgMap[MsgIndex].begin(),entry);
		PrintDOSDebugLog(0x00ffffff,"注册消息处理%d,%p,%p",MsgID,ProcObject,MsgProc);
	}
	else
	{
		for(int i=(int)m_MsgMap[MsgIndex].size()-1;i>=0;i--)
		{
			if(m_MsgMap[MsgIndex][i].pObject==ProcObject&&m_MsgMap[MsgIndex][i].pProc==MsgProc)
			{
				m_MsgMap[MsgIndex].erase(m_MsgMap[MsgIndex].begin()+i);
				PrintDOSDebugLog(0x00ffffff,"注销消息处理%d,%p,%p",MsgID,ProcObject,MsgProc);
				return;
			}
		}
	}
	FUNCTION_END;
}

