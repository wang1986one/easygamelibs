#include "StdAfx.h"

IMPLEMENT_CLASS_INFO_STATIC(CEpollEventRouter,CNameObject);

BOOL CEpollEventRouter::OnEpollEvent(UINT EventID,UINT SessionID)
{	
	CAutoReadLock Lock(m_EasyReadWriteLock);
	if(m_SessionID==SessionID)
	{
		if(m_pEpollEventHandler!=NULL)
		{

			return m_pEpollEventHandler->OnEpollEvent(EventID);			

		}
		else
		{
			PrintNetLog(0xffffffff,"EventRouter没有EventHandler,Event(%u)被忽略",EventID);
		}
	}
	else
	{
		PrintNetLog(0xffffffff,"不是本次Session的Event,Event(%u)被忽略",EventID);
	}
	
	return FALSE;
}