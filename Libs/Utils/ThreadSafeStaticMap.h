/****************************************************************************/
/*                                                                          */
/*      文件名:    ThreadSafeStaticMap.h                                    */
/*      创建日期:  2009年10月15日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once


template < class KEY,class T ,int StorageMode=EDSM_STATIC>
class CThreadSafeStaticMap:public CStaticMap<KEY,T,StorageMode>
{
protected:
	
	CEasyCriticalSection		m_EasyCriticalSection;
public:
	CThreadSafeStaticMap()
	{
		
	}
	~CThreadSafeStaticMap()
	{
		
	}
	bool Create(UINT Size,UINT GrowSize=0,UINT GrowLimit=0)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CStaticMap<KEY,T,StorageMode>::Create(Size,GrowSize,GrowLimit);
	}
	void Destory()
	{
		CAutoLock Lock(m_EasyCriticalSection);
		CStaticMap<KEY,T,StorageMode>::Destory();
	}
	void Clear()
	{
		CAutoLock Lock(m_EasyCriticalSection);
		CStaticMap<KEY,T,StorageMode>::Clear();
	}
	
	UINT New(const KEY& Key,T** ppValue)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CStaticMap<KEY,T,StorageMode>::New(Key,ppValue);
	}
	UINT Insert(const KEY& Key,const T& Value)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CStaticMap<KEY,T,StorageMode>::Insert(Key,Value);
	}
	BOOL DeleteByID(UINT ID)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CStaticMap<KEY,T,StorageMode>::DeleteByID(ID);
	}
	BOOL Delete(const KEY& Key)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CStaticMap<KEY,T,StorageMode>::Delete(Key);
	}
	T * Find(const KEY& Key)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CStaticMap<KEY,T,StorageMode>::Find(Key);
	}
	T* GetObject(LPVOID Pos)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CStaticMap<KEY,T,StorageMode>::GetObject(Pos);
	}
	T* GetObject(UINT ID)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CStaticMap<KEY,T,StorageMode>::GetObject(ID);
	}
	
	LPVOID GetObjectPosByID(UINT ID)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CStaticMap<KEY,T,StorageMode>::GetObjectPosByID(ID);
	}

	T * GetNextObject(LPVOID& Pos,KEY& Key)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CStaticMap<KEY,T,StorageMode>::GetNextObject(Pos,Key);
	}
	T * GetNextObject(LPVOID& Pos)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CStaticMap<KEY,T,StorageMode>::GetNextObject(Pos);
	}

	T * GetPrevObject(LPVOID& Pos,KEY& Key)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CStaticMap<KEY,T,StorageMode>::GetPrevObject(Pos,Key);
	}
	T * GetPrevObject(LPVOID& Pos)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CStaticMap<KEY,T,StorageMode>::GetPrevObject(Pos);
	}
	
	T * GetSortedNextObject(LPVOID& Pos,KEY& Key)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CStaticMap<KEY,T,StorageMode>::GetSortedNextObject(Pos,Key);
	}
	T * GetSortedNextObject(LPVOID& Pos)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CStaticMap<KEY,T,StorageMode>::GetSortedNextObject(Pos);
	}

	T * GetSortedPrevObject(LPVOID& Pos,KEY& Key)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CStaticMap<KEY,T,StorageMode>::GetSortedPrevObject(Pos,Key);
	}
	T * GetSortedPrevObject(LPVOID& Pos)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CStaticMap<KEY,T,StorageMode>::GetSortedPrevObject(Pos);
	}
	
};