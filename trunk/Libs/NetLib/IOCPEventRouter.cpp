/****************************************************************************/
/*                                                                          */
/*      �ļ���:    IOCPEventRouter.cpp                                      */
/*      ��������:  2009��09��11��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
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
			PrintNetLog(0xffffffff,"EventRouterû��EventHandler,Overlapped(%u)������",
				pOverLappedObject->GetParentID());
		}
	}
	else
	{
		PrintNetLog(0xffffffff,"EventRouter�յ����Ǳ�Session��Overlapped(%u)������",
			pOverLappedObject->GetParentID());
	}
	pOverLappedObject->Release();
	return FALSE;
}