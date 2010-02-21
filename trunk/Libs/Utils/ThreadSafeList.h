#pragma once

template < class T ,int StorageMode=EDSM_STATIC>
class CThreadSafeList:public CStaticList<T,StorageMode>
{
protected:	
	CEasyCriticalSection	m_EasyCriticalSection;
public:
	CThreadSafeList(void)
		:m_EasyCriticalSection()
	{
		
	}	
	virtual ~CThreadSafeList(void)
	{
		
	}
	BOOL Create(int Size)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CStaticList::Create(Size);
	}
	
	void Destory()
	{
		CAutoLock Lock(m_EasyCriticalSection);
		CStaticList::Destory();
	}

	void Clear()
	{
		CAutoLock Lock(m_EasyCriticalSection);
		CStaticList::Clear();
	}

	T * PushFront()
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CStaticList::PushFront();
	}

	BOOL PushFront(T& Object)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CStaticList::PushFront(Object);
	}

	T* PushBack()
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CStaticList::PushBack();
	}

	BOOL PushBack(T& Object)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CStaticList::PushBack(Object);
	}

	BOOL PopFront(T& Object)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CStaticList::PopFront(Object);
	}

	BOOL PopBack(T& Object)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CStaticList::PopBack(Object);
	}

	BOOL DeleteObject(LPVOID Pos)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CStaticList::DeleteObject(Pos);
	}
	
	BOOL GetHead(T& Object)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CStaticList::GetHead(Object);
	}
	BOOL GetTail(T& Object)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CStaticList::GetTail(Object);
	}

	T * GetNext(LPVOID& Pos)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CStaticList::GetNext(Pos);
	}

	T * GetPrev(LPVOID& Pos)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CStaticList::GetPrev(Pos);
	}
};
