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
		return CIDStorage::Create(Size,GrowSize,GrowLimit);
	}
	void Destory()
	{
		CAutoLock Lock(m_EasyCriticalSection);
		CIDStorage::Destory();
	}
	void Clear()
	{
		CAutoLock Lock(m_EasyCriticalSection);
		CIDStorage::Clear();
	}
	UINT NewObject(T** ppObject)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CIDStorage::NewObject(ppObject);
	}	

	T* NewObject()
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CIDStorage::NewObject();
	}	
	LPVOID InsertAfter(LPVOID Pos=NULL)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CIDStorage::InsertAfter(Pos);
	}
	LPVOID InsertAfter(const T& Object,LPVOID Pos=NULL)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CIDStorage::InsertAfter(Object,Pos);
	}
	LPVOID InsertBefore(LPVOID Pos=NULL)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CIDStorage::InsertBefore(Pos);
	}
	LPVOID InsertBefore(const T& Object,LPVOID Pos=NULL)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CIDStorage::InsertBefore(Object,Pos);
	}
	LPVOID InsertSorted(const T& Object)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CIDStorage::InsertSorted(Object);
	}
	UINT AddObject(const T& Object)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CIDStorage::AddObject(Object);
	}		
	LPVOID GetObjectPos(UINT ID)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CIDStorage::GetObjectPos(ID);
	}
	T* GetObject(LPVOID Pos)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CIDStorage::GetObject(Pos);
	}	
	T* GetObject(UINT ID)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CIDStorage::GetObject(ID);
	}
	UINT GetObjectID(LPVOID Pos)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CIDStorage::GetObjectID(Pos);
	}
	BOOL DeleteObject(UINT ID)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CIDStorage::DeleteObject(ID);
	}	
	T* GetNext(LPVOID& Pos)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CIDStorage::GetNext(Pos);
	}

	T* GetPrev(LPVOID& Pos)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CIDStorage::GetPrev(Pos);
	}
	BOOL MoveBeforeTo(LPVOID Pos,LPVOID Target)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CIDStorage::MoveBeforeTo(Pos,Target);
	}
	BOOL MoveAfterTo(LPVOID Pos,LPVOID Target)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CIDStorage::MoveAfterTo(Pos,Target);
	}
	BOOL MoveSorted(LPVOID Pos)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CIDStorage::MoveSorted(Pos);
	}
	LPVOID PushFront()
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CIDStorage::PushFront();
	}

	LPVOID PushFront(const T& Object)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CIDStorage::PushFront(Object);
	}

	LPVOID PushBack()
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CIDStorage::PushBack();
	}

	LPVOID PushBack(const T& Object)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CIDStorage::PushBack(Object);
	}

	BOOL PopFront(T& Object)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CIDStorage::PopFront(Object);
	}

	BOOL PopBack(T& Object)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CIDStorage::PopBack(Object);
	}
	LPVOID Find(const T& Object)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CIDStorage::Find(Object);		
	}

	void Verfy(UINT& UsedCount,UINT& FreeCount)
	{
		CAutoLock Lock(m_EasyCriticalSection);

		CIDStorage::Verfy(UsedCount,FreeCount);
	}
};
