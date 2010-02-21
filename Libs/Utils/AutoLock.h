#pragma once

class CAutoLock	
{
protected:
	CEasyCriticalSection * m_pSection;
public:
	CAutoLock(CEasyCriticalSection& Section)
	{
		m_pSection=&Section;
		m_pSection->Lock();
	}
	~CAutoLock()
	{
		m_pSection->Unlock();
	}
};

class CAutoLockEx
{
protected:
	CEasyCriticalSection * m_pSection;
public:
	CAutoLockEx()
	{
		m_pSection=NULL;
	}
	CAutoLockEx(CEasyCriticalSection& Section)
	{
		m_pSection=&Section;
		m_pSection->Lock();
	}
	void Lock(CEasyCriticalSection& Section)
	{
		m_pSection=&Section;
		m_pSection->Lock();
	}
	BOOL TryLock(CEasyCriticalSection& Section)
	{
		if(Section.TryLock())
		{
			m_pSection=&Section;
			return TRUE;
		}
		return FALSE;
	}
	~CAutoLockEx()
	{
		if(m_pSection)
			m_pSection->Unlock();
	}
};