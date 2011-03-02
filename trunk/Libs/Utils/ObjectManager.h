/****************************************************************************/
/*                                                                          */
/*      �ļ���:    ObjectManager.h                                          */
/*      ��������:  2009��07��06��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
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