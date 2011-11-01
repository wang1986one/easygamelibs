/****************************************************************************/
/*                                                                          */
/*      �ļ���:    EasyList.h                                               */
/*      ��������:  2009��10��15��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once

template<class T>
class CEasyList
{
protected:
	struct StorageNode
	{
		StorageNode * pPrev;
		StorageNode * pNext;
		T Object;
	};
	StorageNode *	m_pObjectListHead;
	StorageNode *	m_pObjectListTail;
	UINT			m_ObjectCount;
public:
	CEasyList()
	{
		m_pObjectListHead=NULL;
		m_pObjectListTail=NULL;
		m_ObjectCount=0;
	}
	~CEasyList()
	{
		Clear();
	}
	void Clear()
	{
		while(m_pObjectListHead)
		{
			StorageNode * pNode=m_pObjectListHead;
			m_pObjectListHead=m_pObjectListHead->pNext;
			delete pNode;
		}
		m_pObjectListHead=NULL;
		m_pObjectListTail=NULL;
		m_ObjectCount=0;
	}
	UINT GetObjectCount()
	{
		return m_ObjectCount;
	}
	LPVOID GetFirstObjectPos()
	{
		return m_pObjectListHead;
	}
	LPVOID GetLastObjectPos()
	{
		return m_pObjectListTail;
	}
	T * GetObject(LPVOID Pos)
	{
		StorageNode * pNode=(StorageNode *)Pos;
		if(pNode)
			return &(pNode->Object);
		else
			return NULL;
	}
	T * GetNext(LPVOID& Pos)
	{
		if(Pos)
		{
			StorageNode * pNode=(StorageNode *)Pos;
			Pos=pNode->pNext;
			return &(pNode->Object);
		}
		return NULL;
	}
	T * GetPrev(LPVOID Pos)
	{
		if(Pos)
		{
			StorageNode * pNode=(StorageNode *)Pos;
			Pos=pNode->pPrev;
			return &(pNode->Object);
		}
		return NULL;
	}
	BOOL MoveToBefore(LPVOID Pos,LPVOID Target)
	{
		if(Pos&&Pos!=Target&&m_ObjectCount>1)
		{
			StorageNode * pNode=(StorageNode *)Pos;
			Pick(pNode);
			InsertNodeBefore(pNode,Target);
			return TRUE;
		}
		return FALSE;
	}
	BOOL MoveToAfter(LPVOID Pos,LPVOID Target)
	{
		if(Pos&&Pos!=Target&&m_ObjectCount>1)
		{
			StorageNode * pNode=(StorageNode *)Pos;
			Pick(pNode);
			InsertNodeAfter(pNode,Target);
			return TRUE;
		}
		return FALSE;
	}

	LPVOID InsertAfter(LPVOID Pos=NULL)
	{
		StorageNode * pNode=new StorageNode;
		InsertNodeAfter(pNode,Pos);
		return pNode;
	}	

	LPVOID InsertAfter(const T& Object,LPVOID Pos=NULL)
	{
		StorageNode * pNode=new StorageNode;		
		pNode->Object=Object;
		InsertNodeAfter(pNode,Pos);
		return pNode;
	}
	
	LPVOID InsertBefore(LPVOID Pos=NULL)
	{
		StorageNode * pNode=new StorageNode;		
		InsertNodeBefore(pNode,Pos);
		return pNode;
	}
	LPVOID InsertBefore(const T& Object,LPVOID Pos=NULL)
	{
		StorageNode * pNode=new StorageNode;
		pNode->Object=Object;
		InsertNodeBefore(pNode,Pos);
		return pNode;
	}
	LPVOID InsertSorted(const T& Object)
	{
		StorageNode * pNode=new StorageNode;
		if(pNode)
		{	
			pNode->Object=Object;
			StorageNode * pHead=m_pObjectListHead;
			while(pHead&&pHead->Object<Object)
			{
				pHead=pHead->pNext;
			}
			if(pHead)
			{
				InsertNodeBefore(pNode,pHead);
			}
			else
			{
				InsertNodeAfter(pNode,pHead);
			}
			return pNode;
		}
		return NULL;
	}
	
	
	BOOL DeleteObject(LPVOID Pos)
	{
		if(Pos)
		{
			StorageNode * pNode=(StorageNode *)Pos;
			Pick(pNode);
			delete pNode;
			return TRUE;
		}
		return FALSE;
	}
	
	LPVOID PushFront()
	{
		StorageNode * pNode=new StorageNode;
		if(pNode)
		{				
			InsertNodeBefore(pNode,m_pObjectListHead);			
			return pNode;
		}
		return NULL;
	}

	LPVOID PushFront(const T& Object)
	{
		StorageNode * pNode=(StorageNode *)PushFront();
		if(pNode)
		{			
			pNode->Object=Object;
			return pNode;
		}
		return NULL;
	}

	LPVOID PushBack()
	{
		StorageNode * pNode=new StorageNode;
		if(pNode)
		{				
			InsertNodeAfter(pNode,m_pObjectListTail);			
			return pNode;
		}
		return NULL;
	}

	LPVOID PushBack(const T& Object)
	{
		StorageNode * pNode=(StorageNode *)PushBack();
		if(pNode)
		{			
			pNode->Object=Object;
			return pNode;
		}
		return NULL;
	}

	BOOL PopFront(T& Object)
	{
		if(m_pObjectListHead)
		{
			Object=m_pObjectListHead->Object;
			DeleteObject(m_pObjectListHead);
			return TRUE;
		}
		return FALSE;
	}	

	BOOL PopBack(T& Object)
	{
		if(m_pObjectListTail)
		{
			Object=m_pObjectListTail->Object;
			DeleteObject(m_pObjectListTail);
			return TRUE;
		}
		return FALSE;
	}
	LPVOID Find(const T& Object)
	{
		StorageNode * pNode=m_pObjectListHead;
		while(pNode)
		{
			if(pNode->Object==Object)
				return pNode;
			pNode=pNode->pNext;
		}
		return NULL;
	}
	
protected:
	void Pick(StorageNode * pNode)
	{
		if(pNode==m_pObjectListHead)
			m_pObjectListHead=pNode->pNext;
		if(pNode==m_pObjectListTail)
			m_pObjectListTail=pNode->pPrev;

		if(pNode->pPrev)
			pNode->pPrev->pNext=pNode->pNext;
		if(pNode->pNext)
			pNode->pNext->pPrev=pNode->pPrev;
		m_ObjectCount--;
	}
	void InsertNodeBefore(StorageNode * pNode,LPVOID Pos)
	{		
		StorageNode * pBefore=(StorageNode *)Pos;	

		if(pBefore==NULL)
			pBefore=m_pObjectListHead;

		if(pBefore)
			pNode->pPrev=pBefore->pPrev;
		else
			pNode->pPrev=NULL;

		pNode->pNext=pBefore;
		if(pBefore)
		{
			if(pBefore->pPrev)
				pBefore->pPrev->pNext=pNode;
			pBefore->pPrev=pNode;
		}
		if(m_pObjectListHead==NULL)
		{
			m_pObjectListHead=pNode;
			m_pObjectListTail=pNode;
		}
		else if(m_pObjectListHead==pBefore)
		{
			m_pObjectListHead=pNode;
		}
		m_ObjectCount++;		
	}
	void InsertNodeAfter(StorageNode * pNode,LPVOID Pos)
	{		
		StorageNode * pAfter=(StorageNode *)Pos;	

		if(pAfter==NULL)
			pAfter=m_pObjectListTail;

		if(pAfter)
			pNode->pNext=pAfter->pNext;
		else
			pNode->pNext=NULL;

		pNode->pPrev=pAfter;

		if(pAfter)
		{
			if(pAfter->pNext)
				pAfter->pNext->pPrev=pNode;
			pAfter->pNext=pNode;
		}

		if(m_pObjectListTail==NULL)
		{
			m_pObjectListHead=pNode;
			m_pObjectListTail=pNode;
		}
		else if(m_pObjectListTail==pAfter)
		{
			m_pObjectListTail=pNode;
		}
		m_ObjectCount++;
	}
};