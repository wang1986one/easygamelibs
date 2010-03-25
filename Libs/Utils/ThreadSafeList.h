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
