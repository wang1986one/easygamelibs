/****************************************************************************/
/*                                                                          */
/*      �ļ���:    EpollEventRouter.cpp                                     */
/*      ��������:  2009��09��11��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
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