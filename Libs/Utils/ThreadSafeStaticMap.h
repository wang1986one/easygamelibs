#pragma once

//#include "math.h"

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
	bool Create(UINT Size)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CStaticMap::Create(Size);
	}
	void Destory()
	{
		CAutoLock Lock(m_EasyCriticalSection);
		CStaticMap::Destory();
	}
	void Clear()
	{
		CAutoLock Lock(m_EasyCriticalSection);
		CStaticMap::Clear();
	}
	
	UINT New(const KEY& Key,T** ppValue)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CStaticMap::New(Key,ppValue);
	}
	UINT Insert(const KEY& Key,const T& Value)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CStaticMap::Insert(Key,Value);
	}
	BOOL DeleteByID(UINT ID)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CStaticMap::DeleteByID(ID);
	}
	BOOL Delete(const KEY& Key)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CStaticMap::Delete(Key);
	}
	T * Find(const KEY& Key)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CStaticMap::Find(Key);
	}
	T* GetObject(LPVOID Pos)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CStaticMap::GetObject(Pos);
	}
	T* GetObject(UINT ID)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CStaticMap::GetObject(ID);
	}
	
	LPVOID GetObjectPosByID(UINT ID)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CStaticMap::GetObjectPosByID(ID);
	}

	T * GetNextObject(LPVOID& Pos,KEY& Key)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CStaticMap::GetNextObject(Pos,Key);
	}

	T * GetPrevObject(LPVOID& Pos,KEY& Key)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CStaticMap::GetPrevObject(Pos,Key);
	}
	
	T * GetSortedNextObject(LPVOID& Pos,KEY& Key)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CStaticMap::GetSortedNextObject(Pos,Key);
	}

	T * GetSortedPrevObject(LPVOID& Pos,KEY& Key)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CStaticMap::GetSortedPrevObject(Pos,Key);
	}
	
};