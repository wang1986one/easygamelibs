/****************************************************************************/
/*                                                                          */
/*      文件名:    ObjectManager.h                                          */
/*      创建日期:  2009年07月06日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once


template < class T>
class CObjectManager : public CNameObject
{
protected:
	CNameStorage<T,false,true>	m_ObjectStorage;
public:
	CObjectManager(int StorageSize)
	{
		m_ObjectStorage.Create(StorageSize);
	}
	~CObjectManager()
	{
		m_ObjectStorage.Destory();
	}
	bool Add(T * pObject,LPCTSTR ObjectName)
	{
		UINT ID=m_ObjectStorage.AddObject(pObject,ObjectName);
		if(ID)
		{
			pObject->SetID(ID);
			pObject->SetName(ObjectName);
			return true;
		}
		else
		{
			return false;
		}
	}

	T * Delete(UINT ID)
	{
		T * pObject=*(m_ObjectStorage.GetObject(ID));
		if(pObject)
		{
			m_ObjectStorage.DeleteObject(ID);		
			return pObject;
		}
		return NULL;
	}
	T * Delete(LPCTSTR ObjectName)
	{
		T * pObject=*(m_ObjectStorage.GetObject(ObjectName));
		if(pObject)
		{
			m_ObjectStorage.DeleteObject(pObject->GetID());		
			return pObject;
		}
		return NULL;
	}

	T * Get(UINT ID)
	{
		T ** ppObject=m_ObjectStorage.GetObject(ID);
		if(ppObject)
			return *ppObject;
		else
			return NULL;
	}

	T * Get(LPCTSTR ObjectName)
	{
		T ** ppObject=m_ObjectStorage.GetObject(ObjectName);
		if(ppObject)
			return *ppObject;
		else
			return NULL;
	}
	int GetCount()
	{
		return m_ObjectStorage.GetObjectCount();
	}
	LPVOID GetFirstPos()
	{
		return m_ObjectStorage.GetFirstObjectPos();
	}

	LPVOID GetLastPos()
	{
		return m_ObjectStorage.GetLastObjectPos();
	}

	T* GetNext(LPVOID& Pos)
	{		
		return m_ObjectStorage.GetNext(Pos);
	}

	T* GetPrev(LPVOID& Pos)
	{		
		return m_ObjectStorage.GetPrev(Pos);
	}

};