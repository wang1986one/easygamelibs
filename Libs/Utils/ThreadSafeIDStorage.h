/****************************************************************************/
/*                                                                          */
/*      文件名:    ThreadSafeIDStorage.h                                    */
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
class CThreadSafeIDStorage:public CIDStorage<T,StorageMode>
{
protected:
	CEasyCriticalSection		m_EasyCriticalSection;
public:
	CThreadSafeIDStorage()
	{
		m_pObjectBuff=NULL;
		m_pFreeListHead=NULL;
		m_pFreeListTail=NULL;
		m_pObjectListHead=NULL;
		m_pObjectListTail=NULL;
		m_ObjectCount=0;
		m_ObjectBuffSize=0;		
	}
	~CThreadSafeIDStorage()
	{
		
	}
	bool Create(UINT Size)
	{	
		CAutoLock Lock(m_EasyCriticalSection);
		return CIDStorage::Create(Size);
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

	UINT AddObject(T& Object)
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
	bool DeleteObject(UINT ID)
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
	BOOL MoveBeforeTo(LPVOID Pos,LPVOID Before)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CIDStorage::MoveBeforeTo(Pos,Before);
	}
	BOOL MoveAfterTo(LPVOID Pos,LPVOID After)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CIDStorage::MoveAfterTo(Pos,After);
	}
	void Verfy(UINT& UsedCount,UINT& FreeCount)
	{
		CAutoLock Lock(m_EasyCriticalSection);

		UsedCount=0;
		FreeCount=0;

		StorageNode * pNode=m_pObjectListHead;
		while(pNode&&UsedCount<m_ObjectBuffSize)
		{
			pNode=pNode->pNext;
			UsedCount++;
		}

		pNode=m_pFreeListHead;
		while(pNode&&FreeCount<m_ObjectBuffSize)
		{
			pNode=pNode->pNext;
			FreeCount++;
		}
	}
};
