/****************************************************************************/
/*                                                                          */
/*      �ļ���:    ThreadSafeIDStorage.h                                    */
/*      ��������:  2009��10��15��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once


template < class T ,int StorageMode=EDSM_STATIC>
class CThreadSafeIDStorage:public CIDStorage<T,StorageMode>
{
protected:
	CEasyCriticalSection		m_EasyCriticalSection;
public:
	CThreadSafeIDStorage()
	{
		
	}
	~CThreadSafeIDStorage()
	{
		
	}
	bool Create(UINT Size,UINT GrowSize=0,UINT GrowLimit=0)
	{	
		CAutoLock Lock(m_EasyCriticalSection);
		return CIDStorage<T,StorageMode>::Create(Size,GrowSize,GrowLimit);
	}
	void Destory()
	{
		CAutoLock Lock(m_EasyCriticalSection);
		CIDStorage<T,StorageMode>::Destory();
	}
	void Clear()
	{
		CAutoLock Lock(m_EasyCriticalSection);
		CIDStorage<T,StorageMode>::Clear();
	}
	UINT NewObject(T** ppObject)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CIDStorage<T,StorageMode>::NewObject(ppObject);
	}	

	T* NewObject()
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CIDStorage<T,StorageMode>::NewObject();
	}	
	LPVOID InsertAfter(LPVOID Pos=NULL)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CIDStorage<T,StorageMode>::InsertAfter(Pos);
	}
	LPVOID InsertAfter(const T& Object,LPVOID Pos=NULL)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CIDStorage<T,StorageMode>::InsertAfter(Object,Pos);
	}
	LPVOID InsertBefore(LPVOID Pos=NULL)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CIDStorage<T,StorageMode>::InsertBefore(Pos);
	}
	LPVOID InsertBefore(const T& Object,LPVOID Pos=NULL)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CIDStorage<T,StorageMode>::InsertBefore(Object,Pos);
	}
	LPVOID InsertSorted(const T& Object)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CIDStorage<T,StorageMode>::InsertSorted(Object);
	}
	UINT AddObject(const T& Object)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CIDStorage<T,StorageMode>::AddObject(Object);
	}		
	LPVOID GetObjectPos(UINT ID)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CIDStorage<T,StorageMode>::GetObjectPos(ID);
	}
	T* GetObject(LPVOID Pos)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CIDStorage<T,StorageMode>::GetObject(Pos);
	}	
	T* GetObject(UINT ID)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CIDStorage<T,StorageMode>::GetObject(ID);
	}
	UINT GetObjectID(LPVOID Pos)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CIDStorage<T,StorageMode>::GetObjectID(Pos);
	}
	BOOL DeleteObject(UINT ID)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CIDStorage<T,StorageMode>::DeleteObject(ID);
	}	
	T* GetNext(LPVOID& Pos)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CIDStorage<T,StorageMode>::GetNext(Pos);
	}

	T* GetPrev(LPVOID& Pos)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CIDStorage<T,StorageMode>::GetPrev(Pos);
	}
	BOOL MoveBeforeTo(LPVOID Pos,LPVOID Target)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CIDStorage<T,StorageMode>::MoveBeforeTo(Pos,Target);
	}
	BOOL MoveAfterTo(LPVOID Pos,LPVOID Target)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CIDStorage<T,StorageMode>::MoveAfterTo(Pos,Target);
	}
	BOOL MoveSorted(LPVOID Pos)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CIDStorage<T,StorageMode>::MoveSorted(Pos);
	}
	LPVOID PushFront()
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CIDStorage<T,StorageMode>::PushFront();
	}

	LPVOID PushFront(const T& Object)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CIDStorage<T,StorageMode>::PushFront(Object);
	}

	LPVOID PushBack()
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CIDStorage<T,StorageMode>::PushBack();
	}

	LPVOID PushBack(const T& Object)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CIDStorage<T,StorageMode>::PushBack(Object);
	}

	BOOL PopFront(T& Object)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CIDStorage<T,StorageMode>::PopFront(Object);
	}

	BOOL PopBack(T& Object)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CIDStorage<T,StorageMode>::PopBack(Object);
	}
	LPVOID Find(const T& Object)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CIDStorage<T,StorageMode>::Find(Object);		
	}

	void Verfy(UINT& UsedCount,UINT& FreeCount)
	{
		CAutoLock Lock(m_EasyCriticalSection);

		CIDStorage<T,StorageMode>::Verfy(UsedCount,FreeCount);
	}
};
