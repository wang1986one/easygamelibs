/****************************************************************************/
/*                                                                          */
/*      �ļ���:    ThreadSafeStaticMap.h                                    */
/*      ��������:  2009��10��15��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
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

	T * GetPrevObject(LPVOID& Pos,KEY& Key)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CStaticMap<KEY,T,StorageMode>::GetPrevObject(Pos,Key);
	}
	
	T * GetSortedNextObject(LPVOID& Pos,KEY& Key)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CStaticMap<KEY,T,StorageMode>::GetSortedNextObject(Pos,Key);
	}

	T * GetSortedPrevObject(LPVOID& Pos,KEY& Key)
	{
		CAutoLock Lock(m_EasyCriticalSection);
		return CStaticMap<KEY,T,StorageMode>::GetSortedPrevObject(Pos,Key);
	}
	
};