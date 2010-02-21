#include "StdAfx.h"

IMPLEMENT_CLASS_INFO_STATIC(CIOCPEventRouter,CNameObject);

BOOL CIOCPEventRouter::OnIOCPEvent(int EventID,COverLappedObject * pOverLappedObject)
{
	CAutoReadLock Lock(m_EasyReadWriteLock);

	if(pOverLappedObject->GetRouterSession()==m_SessionID)
	{
		if(m_pIOCPEventHandler!=NULL)
		{
			
			return m_pIOCPEventHandler->OnIOCPEvent(EventID,pOverLappedObject);			
			
		}
		else
		{
			PrintNetLog(0xffffffff,"EventRouter没有EventHandler,Overlapped(%u)被忽略",
				pOverLappedObject->GetParentID());
		}
	}
	else
	{
		PrintNetLog(0xffffffff,"EventRouter收到不是本Session的Overlapped(%u)，忽略",
			pOverLappedObject->GetParentID());
	}
	pOverLappedObject->Release();
	return FALSE;
}