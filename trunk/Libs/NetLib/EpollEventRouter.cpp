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
			PrintNetLog(0xffffffff,"EventRouterû��EventHandler,Event(%u)������",EventID);
		}
	}
	else
	{
		PrintNetLog(0xffffffff,"���Ǳ���Session��Event,Event(%u)������",EventID);
	}
	
	return FALSE;
}