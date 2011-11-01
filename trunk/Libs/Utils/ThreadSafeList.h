/****************************************************************************/
/*                                                                          */
/*      文件名:    ThreadSafeList.h                                         */
/*      创建日期:  2009年10月15日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once

template < class T >
class CThreadSafeList:public CEasyList<T>
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

	void Clear()
	{
		CAutoLock Lock(m_EasyCriticalSection);
		CEasyList::Clear();
	}

	T* GetObject(LPVOID Pos)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CEasyList::GetObject(Pos);
	}	

	T* GetNext(LPVOID& Pos)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CEasyList::GetNext(Pos);
	}

	T* GetPrev(LPVOID& Pos)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CEasyList::GetPrev(Pos);
	}
	BOOL MoveBeforeTo(LPVOID Pos,LPVOID Target)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CEasyList::MoveBeforeTo(Pos,Target);
	}
	BOOL MoveAfterTo(LPVOID Pos,LPVOID Target)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CEasyList::MoveAfterTo(Pos,Target);
	}
	
	LPVOID InsertAfter(LPVOID Pos=NULL)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CEasyList::InsertAfter(Pos);
	}
	LPVOID InsertAfter(const T& Object,LPVOID Pos=NULL)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CEasyList::InsertAfter(Object,Pos);
	}
	LPVOID InsertBefore(LPVOID Pos=NULL)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CEasyList::InsertBefore(Pos);
	}
	LPVOID InsertBefore(const T& Object,LPVOID Pos=NULL)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CEasyList::InsertBefore(Object,Pos);
	}
	LPVOID InsertSorted(const T& Object)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CEasyList::InsertSorted(Object);
	}	

	
	
	BOOL DeleteObject(LPVOID Pos)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CEasyList::DeleteObject(Pos);
	}	
	
	LPVOID PushFront()
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CEasyList::PushFront();
	}

	LPVOID PushFront(const T& Object)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CEasyList::PushFront(Object);
	}

	LPVOID PushBack()
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CEasyList::PushBack();
	}

	LPVOID PushBack(const T& Object)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CEasyList::PushBack(Object);
	}

	BOOL PopFront(T& Object)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CEasyList::PopFront(Object);
	}

	BOOL PopBack(T& Object)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CEasyList::PopBack(Object);
	}
	LPVOID Find(const T& Object)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CEasyList::Find(Object);		
	}
};
