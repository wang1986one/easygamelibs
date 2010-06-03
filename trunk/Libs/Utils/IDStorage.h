/****************************************************************************/
/*                                                                          */
/*      �ļ���:    IDStorage.h                                              */
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
class CIDStorage
{
protected:
	template < int Mode,class OBJECT_TYPE>
	struct _StorageNode
	{
		UINT ID;
		_StorageNode * pPrev;
		_StorageNode * pNext;
		bool IsFree;	
		void InitObject()
		{
		}
		OBJECT_TYPE& GetObjectRef()
		{
			return Object;
		}
		OBJECT_TYPE * GetObjectPointer()
		{
			return &Object;
		}
		void NewObject()
		{

		}
		void DeleteObject()
		{

		}
		void FinalReleaseObject()
		{

		}
	protected:
		OBJECT_TYPE Object;
	};

	template < class OBJECT_TYPE>
	struct _StorageNode<EDSM_NEW_ONCE,OBJECT_TYPE>
	{
		UINT ID;
		_StorageNode * pPrev;
		_StorageNode * pNext;
		bool IsFree;				
		void InitObject()
		{
			Object=NULL;
		}
		OBJECT_TYPE& GetObjectRef()
		{
			return *pObject;
		}
		OBJECT_TYPE * GetObjectPointer()
		{
			return pObject;
		}
		void NewObject()
		{
			if(pObject==NULL)
				pObject=new OBJECT_TYPE;
		}
		void DeleteObject()
		{
			
		}
		void FinalReleaseObject()
		{
			SAFE_DELETE(pObject);
		}
	protected:
		OBJECT_TYPE * pObject;
	};


	template < class OBJECT_TYPE>
	struct _StorageNode<EDSM_NEW_EVERY_TIME,OBJECT_TYPE>
	{
		UINT ID;
		_StorageNode * pPrev;
		_StorageNode * pNext;
		bool IsFree;				
		void InitObject()
		{
			Object=NULL;
		}
		OBJECT_TYPE& GetObjectRef()
		{
			return *pObject;
		}
		OBJECT_TYPE * GetObjectPointer()
		{
			return pObject;
		}
		void NewObject()
		{
			if(pObject==NULL)
				pObject=new OBJECT_TYPE;
		}
		void DeleteObject()
		{
			SAFE_DELETE(pObject);
		}
		void FinalReleaseObject()
		{
			SAFE_DELETE(pObject);
		}
	protected:
		OBJECT_TYPE * pObject;
	};

	typedef _StorageNode<StorageMode,T> StorageNode;
	StorageNode * m_pObjectBuff;
	StorageNode * m_pFreeListHead;
	StorageNode * m_pFreeListTail;
	StorageNode * m_pObjectListHead;
	StorageNode * m_pObjectListTail;
	UINT m_ObjectCount;
	UINT m_ObjectBuffSize;	
public:
	CIDStorage()
	{
		m_pObjectBuff=NULL;
		m_pFreeListHead=NULL;
		m_pFreeListTail=NULL;
		m_pObjectListHead=NULL;
		m_pObjectListTail=NULL;
		m_ObjectCount=0;
		m_ObjectBuffSize=0;		
	}
	~CIDStorage()
	{
		Destory();
	}
	bool Create(UINT Size)
	{		
		Destory();
		m_ObjectBuffSize=Size;
		m_pObjectBuff=new StorageNode[m_ObjectBuffSize];
		for(UINT i=0;i<m_ObjectBuffSize;i++)
		{
			m_pObjectBuff[i].ID=i+1;			
			if(i==0)
			{
				m_pFreeListHead=m_pObjectBuff;
				m_pObjectBuff[i].pPrev=NULL;
			}
			else
			{
				m_pObjectBuff[i].pPrev=&(m_pObjectBuff[i-1]);
			}
			if(i==m_ObjectBuffSize-1)
			{
				m_pFreeListTail=m_pObjectBuff+i;
				m_pObjectBuff[i].pNext=NULL;
			}
			else
			{
				m_pObjectBuff[i].pNext=&(m_pObjectBuff[i+1]);
			}				
			m_pObjectBuff[i].IsFree=true;
			m_pObjectBuff[i].InitObject();
		}
		
		m_pObjectListHead=NULL;
		m_pObjectListTail=NULL;
		m_ObjectCount=0;		
		return true;

	}
	UINT GetBufferSize()
	{
		return m_ObjectBuffSize;
	}
	BOOL IsCreated()
	{
		return m_pObjectBuff!=NULL;
	}
	void Destory()
	{
		for(UINT i=0;i<m_ObjectBuffSize;i++)
		{			
			m_pObjectBuff[i].FinalReleaseObject();
		}
		SAFE_DELETE_ARRAY(m_pObjectBuff);
		m_pFreeListHead=NULL;
		m_pFreeListTail=NULL;
		m_pObjectListHead=NULL;
		m_pObjectListTail=NULL;
		m_ObjectCount=0;
		m_ObjectBuffSize=0;		
	}
	void Clear()
	{
		for(UINT i=0;i<m_ObjectBuffSize;i++)
		{
			m_pObjectBuff[i].ID=i+1;			
			if(i==0)
			{
				m_pFreeListHead=m_pObjectBuff;
				m_pObjectBuff[i].pPrev=NULL;
			}
			else
			{
				m_pObjectBuff[i].pPrev=&(m_pObjectBuff[i-1]);
			}
			if(i==m_ObjectBuffSize-1)
			{
				m_pFreeListTail=m_pObjectBuff+i;
				m_pObjectBuff[i].pNext=NULL;
			}
			else
			{
				m_pObjectBuff[i].pNext=&(m_pObjectBuff[i+1]);
			}
			m_pObjectBuff[i].IsFree=true;
			m_pObjectBuff[i].DeleteObject();
		}		
		m_pObjectListHead=NULL;
		m_pObjectListTail=NULL;
		m_ObjectCount=0;	
	}
	UINT NewObject(T** ppObject)
	{
		StorageNode * pNode=NewNode();
		if(pNode)
		{				
			InsertNodeAfter(pNode,m_pObjectListTail);			
			*ppObject=pNode->GetObjectPointer();
			return pNode->ID;
		}
		*ppObject=NULL;
		return 0;
	}

	T* NewObject()
	{
		StorageNode * pNode=NewNode();
		if(pNode)
		{				
			InsertNodeAfter(pNode,m_pObjectListTail);			
			pNode->GetObjectRef().SetID(pNode->ID);
			return pNode->GetObjectPointer();
		}

		return NULL;
	}	

	UINT AddObject(const T& Object)
	{
		UINT ID;
		T * pObject;
		ID=NewObject(&pObject);
		if(ID)
		{
			*pObject=Object;
			return ID;
		}
		return 0;
	}
	LPVOID InsertAfter(LPVOID Pos)
	{
		StorageNode * pNode=NewNode();
		if(pNode)
		{				
			InsertNodeAfter(pNode,(StorageNode *)Pos);			
			return pNode;
		}
		return NULL;
	}
	LPVOID InsertAfter(const T& Object,LPVOID Pos)
	{
		StorageNode * pNode=(StorageNode *)InsertAfter(Pos);
		if(pNode)
		{		
			pNode->GetObjectRef()=Object;
			return pNode;
		}
		return NULL;
	}
	LPVOID InsertBefore(LPVOID Pos)
	{
		StorageNode * pNode=NewNode();
		if(pNode)
		{				
			InsertNodeBefore(pNode,(StorageNode *)Pos);			
			return pNode;
		}
		return NULL;
	}
	LPVOID InsertBefore(const T& Object,LPVOID Pos)
	{
		StorageNode * pNode=(StorageNode *)InsertNodeBefore(Pos);
		if(pNode)
		{		
			pNode->GetObjectRef()=Object;
			return pNode;
		}
		return NULL;
	}
	LPVOID InsertSorted(const T& Object)
	{
		StorageNode * pNode=NewNode();
		if(pNode)
		{	
			pNode->GetObjectRef()=Object;
			StorageNode * pHead=m_pObjectListHead;
			while(pHead&&pHead->GetObjectRef()<Object)
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
	LPVOID GetObjectPos(UINT ID)
	{
		if(ID>0&&ID<=m_ObjectBuffSize)
		{
			ID--;
			if(!m_pObjectBuff[ID].IsFree)
				return &(m_pObjectBuff[ID]);
		}
		return NULL;
	}
	T* GetObject(LPVOID Pos)
	{
		StorageNode * pNode=(StorageNode *)Pos;	
		if(Pos)
			return pNode->GetObjectPointer();
		else
			return NULL;
	}	
	T* GetObject(UINT ID)
	{		
		return GetObject(GetObjectPos(ID));
	}
	UINT GetObjectID(LPVOID Pos)
	{
		StorageNode * pNode=(StorageNode *)Pos;	
		if(Pos)
			return pNode->ID;
		else
			return 0;
	}
	
	bool DeleteObject(UINT ID)
	{
		StorageNode * pNode=(StorageNode *)GetObjectPos(ID);
		if(pNode)
		{
			DeleteNode(pNode);
			return true;
		}
		return false;;
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

	T* GetNext(LPVOID& Pos)
	{
		if(Pos)
		{
			StorageNode * pNode=(StorageNode *)Pos;
			Pos=pNode->pNext;
			return pNode->GetObjectPointer();
		}
		return NULL;
	}

	T* GetPrev(LPVOID& Pos)
	{
		if(Pos)
		{
			StorageNode * pNode=(StorageNode *)Pos;
			Pos=pNode->pPrev;
			return pNode->GetObjectPointer();
		}
		return NULL;
	}
	BOOL MoveBeforeTo(LPVOID Pos,LPVOID Before)
	{
		if(Pos&&Pos!=Before&&m_ObjectCount>1)
		{
			StorageNode * pNode=(StorageNode *)Pos;
			StorageNode * pBefore=(StorageNode *)Before;
			PickNode(pNode);
			InsertNodeBefore(pNode,pBefore);
			return TRUE;
		}
		return FALSE;
	}
	BOOL MoveAfterTo(LPVOID Pos,LPVOID After)
	{
		if(Pos&&Pos!=After&&m_ObjectCount>1)
		{
			StorageNode * pNode=(StorageNode *)Pos;
			StorageNode * pAfter=(StorageNode *)After;
			PickNode(pNode);
			InsertNodeAfter(pNode,pAfter);
			return TRUE;
		}
		return FALSE;
	}

	LPVOID PushFront()
	{
		StorageNode * pNode=NewNode();
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
			pNode->GetObjectRef()=Object;
			return pNode;
		}
		return NULL;
	}

	LPVOID PushBack()
	{
		StorageNode * pNode=NewNode();
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
			pNode->GetObjectRef()=Object;
			return pNode;
		}
		return NULL;
	}

	BOOL PopFront(T& Object)
	{
		if(m_pObjectListHead)
		{
			Object=m_pObjectListHead->GetObjectRef();
			DeleteNode(m_pObjectListHead);
			return TRUE;
		}
		return FALSE;
	}	

	BOOL PopBack(T& Object)
	{
		if(m_pObjectListTail)
		{
			Object=m_pObjectListTail->GetObjectRef();
			DeleteNode(m_pObjectListTail);
			return TRUE;
		}
		return FALSE;
	}

protected:
	StorageNode * NewNode()
	{
		if(m_pFreeListHead)
		{			
			StorageNode * pNode;			

			pNode=m_pFreeListHead;
			if(m_pFreeListHead==m_pFreeListTail)
			{
				m_pFreeListHead=NULL;
				m_pFreeListTail=NULL;
			}
			else
			{
				m_pFreeListHead=pNode->pNext;
				if(m_pFreeListHead)
					m_pFreeListHead->pPrev=NULL;	
			}


			pNode->pPrev=NULL;
			pNode->pNext=NULL;			
			pNode->IsFree=false;				
			pNode->NewObject();
			m_ObjectCount++;
			return pNode;
		}
		return NULL;
	}
	void InsertNodeBefore(StorageNode * pNode,StorageNode * pBefore)
	{
		if(pBefore==NULL)
			pBefore=m_pObjectListHead;
		if(pBefore)
		{
			pNode->pPrev=pBefore->pPrev;
			pNode->pNext=pBefore;
			if(pBefore->pPrev)
				pBefore->pPrev->pNext=pNode;
			pBefore->pPrev=pNode;
		}
		else
		{
			m_pObjectListHead=pNode;
			m_pObjectListTail=pNode;
		}
		if(pBefore==m_pObjectListHead)
		{
			m_pObjectListHead=pNode;
		}
	}
	void InsertNodeAfter(StorageNode * pNode,StorageNode * pAfter)
	{
		if(pAfter==NULL)
			pAfter=m_pObjectListTail;
		if(pAfter)
		{
			pNode->pPrev=pAfter;
			pNode->pNext=pAfter->pNext;
			if(pAfter->pNext)
				pAfter->pNext->pPrev=pNode;
			pAfter->pNext=pNode;
		}
		else
		{
			m_pObjectListHead=pNode;
			m_pObjectListTail=pNode;
		}
		if(pAfter==m_pObjectListTail)
		{
			m_pObjectListTail=pNode;
		}
	}
	void PickNode(StorageNode * pNode)
	{
		if(pNode->pPrev)
			pNode->pPrev->pNext=pNode->pNext;
		if(pNode->pNext)
			pNode->pNext->pPrev=pNode->pPrev;
		if(pNode==m_pObjectListHead)
			m_pObjectListHead=pNode->pNext;
		if(pNode==m_pObjectListTail)
			m_pObjectListTail=pNode->pPrev;
		pNode->pPrev=NULL;
		pNode->pNext=NULL; 
	}
	void DeleteNode(StorageNode * pNode)
	{	
		PickNode(pNode);
		pNode->IsFree=true;		
		pNode->DeleteObject();
		pNode->pPrev=m_pFreeListTail;
		pNode->pNext=NULL;
		if(m_pFreeListTail)
		{
			m_pFreeListTail->pNext=pNode;
			m_pFreeListTail=pNode;
		}
		else
		{			
			m_pFreeListHead=pNode;
			m_pFreeListTail=pNode;
		}
		m_ObjectCount--;
	}

	void Verfy(UINT& UsedCount,UINT& FreeCount)
	{
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