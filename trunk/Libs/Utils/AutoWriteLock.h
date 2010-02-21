#pragma once

class CAutoWriteLock
{
protected:
	CEasyReadWriteLock * m_pLock;
public:
	CAutoWriteLock()
	{
		m_pLock=NULL;
	}
	CAutoWriteLock(CEasyReadWriteLock& ReadWriteLock)
	{
		m_pLock=&ReadWriteLock;
		m_pLock->LockWrite();
	}
	void Lock(CEasyReadWriteLock& ReadWriteLock)
	{
		m_pLock=&ReadWriteLock;
		m_pLock->LockWrite();
	}
	BOOL TryLock(CEasyReadWriteLock& ReadWriteLock)
	{
		if(ReadWriteLock.TryLockWrite())
		{
			m_pLock=&ReadWriteLock;
			return TRUE;
		}
		return FALSE;
	}
	~CAutoWriteLock()
	{
		if(m_pLock)
			m_pLock->Unlock();
	}
};