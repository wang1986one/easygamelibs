#pragma once


class CEasyCriticalSection
{
protected:
	pthread_mutex_t		m_Mutex;
public:
	CEasyCriticalSection(void)
	{
		pthread_mutexattr_t MutexAttr;
		pthread_mutexattr_init(&MutexAttr);
		pthread_mutexattr_settype(&MutexAttr,PTHREAD_MUTEX_RECURSIVE);
		pthread_mutex_init(&m_Mutex,&MutexAttr);
		pthread_mutexattr_destroy(&MutexAttr);
	}
	~CEasyCriticalSection(void)
	{
		pthread_mutex_destroy(&m_Mutex);
	}
	void Lock()
	{
		pthread_mutex_lock(&m_Mutex);
	}
	void Unlock()
	{
		pthread_mutex_unlock(&m_Mutex);
	}

	BOOL TryLock()
	{
		if(pthread_mutex_trylock(&m_Mutex)==0)
			return TRUE;
		else
			return FALSE;
	}
};
