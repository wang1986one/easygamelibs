#pragma once

class CAutoReadLock
{
protected:
	CEasyReadWriteLock * m_pLock;
public:
	CAutoReadLock()
	{
		m_pLock=NULL;
	}
	CAutoReadLock(CEasyReadWriteLock& ReadWriteLock)
	{
		m_pLock=&ReadWriteLock;
		m_pLock->LockRead();
	}
	void Lock(CEasyReadWriteLock& ReadWriteLock)
	{
		m_pLock=&ReadWriteLock;
		m_pLock->LockRead();
	}
	BOOL TryLock(CEasyReadWriteLock& ReadWriteLock)
	{
		if(ReadWriteLock.TryLockRead())
		{
			m_pLock=&ReadWriteLock;
			return TRUE;
		}
		return FALSE;
	}
	~CAutoReadLock()
	{
		if(m_pLock)
			m_pLock->Unlock();
	}
};
