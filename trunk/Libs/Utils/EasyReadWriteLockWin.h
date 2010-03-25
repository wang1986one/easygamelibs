/****************************************************************************/
/*                                                                          */
/*      �ļ���:    EasyReadWriteLockWin.h                                   */
/*      ��������:  2009��09��11��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once
#include "AutoLock.h"

class CEasyReadWriteLock
{
protected:
	enum LOCK_LEVEL
	{
		LL_NONE,
		LL_READ,
		LL_WRITE,
	};
	CEasyCriticalSection	m_ReadLock;
	CEasyCriticalSection	m_WriteLock;	
	volatile UINT			m_LockLevel;
	volatile UINT			m_ReadCount;
	HANDLE					m_UnlockEvent;

public:
	CEasyReadWriteLock()
	{		
		m_LockLevel=LL_NONE;
		m_ReadCount=0;
		m_UnlockEvent=CreateEvent(NULL,TRUE,FALSE,NULL);
	}
	~CEasyReadWriteLock()
	{
		CAutoLockEx Lock1(m_ReadLock);
		CAutoLockEx Lock2(m_WriteLock);
		CloseHandle(m_UnlockEvent);
	}
	void LockRead()
	{
		CAutoLockEx Lock1(m_ReadLock);
		if(m_LockLevel == LL_WRITE)
		{
			WaitForSingleObject( m_UnlockEvent,INFINITE);
			CAutoLockEx Lock2(m_WriteLock);
			m_LockLevel = LL_READ;
			m_ReadCount ++;
			ResetEvent(m_UnlockEvent);
		}
		else
		{
			CAutoLockEx Lock2(m_WriteLock);
			m_LockLevel = LL_READ;
			m_ReadCount ++;
		}

	}
	BOOL TryLockRead()
	{
		CAutoLockEx Lock1;
		if(Lock1.TryLock(m_ReadLock))
		{
			if(m_LockLevel == LL_WRITE)
			{
				if(WaitForSingleObject( m_UnlockEvent,0) == WAIT_OBJECT_0)
				{
					CAutoLockEx Lock2;
					if(Lock2.TryLock(m_WriteLock))
					{
						m_LockLevel = LL_READ;
						m_ReadCount ++;
						ResetEvent(m_UnlockEvent);
						return TRUE;
					}
					
				}
				
			}
			else
			{
				CAutoLockEx Lock2;
				if(Lock2.TryLock(m_WriteLock))
				{
					m_LockLevel = LL_READ;
					m_ReadCount ++;
					return TRUE;
				}
			}
		}
		return FALSE;
		
	}	
	void LockWrite()
	{
		CAutoLockEx Lock1(m_ReadLock);

		if(m_LockLevel == LL_NONE)
		{
			CAutoLockEx Lock2(m_WriteLock);
			m_LockLevel = LL_WRITE;
		}
		else
		{
			WaitForSingleObject( m_UnlockEvent,INFINITE);
			CAutoLockEx Lock2(m_WriteLock);
			m_LockLevel = LL_WRITE;
			ResetEvent(m_UnlockEvent);
		}
	}
	BOOL TryLockWrite()
	{
		CAutoLockEx Lock1;

		if(Lock1.TryLock(m_ReadLock))
		{
			if(m_LockLevel == LL_NONE)
			{
				CAutoLockEx Lock2;
				if(Lock2.TryLock(m_WriteLock))
				{
					m_LockLevel = LL_WRITE;
					return TRUE;
				}
			}
			else
			{
				if(WaitForSingleObject( m_UnlockEvent,0) == WAIT_OBJECT_0)
				{
					CAutoLockEx Lock2;
					if(Lock2.TryLock(m_WriteLock))
					{
						m_LockLevel = LL_WRITE;
						ResetEvent(m_UnlockEvent);
						return TRUE;
					}
				}
			}
		}
		return FALSE;
	}
	void Unlock()
	{
		CAutoLock Lock(m_WriteLock);

		if ( m_LockLevel == LL_READ )
		{
			m_ReadCount --;
			if ( m_ReadCount == 0 ) 
			{
				m_LockLevel = LL_NONE;		
				SetEvent (m_UnlockEvent);

			}
		}
		else if ( m_LockLevel == LL_WRITE )
		{
			m_LockLevel = LL_NONE;
			SetEvent (m_UnlockEvent);

		}

	}
};