/****************************************************************************/
/*                                                                          */
/*      文件名:    StaticList.h                                             */
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
class CStaticList
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
	typedef StorageNode * LPSTORAGENODE;
	volatile LPSTORAGENODE	m_pObjectBuff;
	volatile LPSTORAGENODE	m_pFreeList;
	volatile LPSTORAGENODE	m_pObjectList;
	volatile LPSTORAGENODE	m_pObjectListTail;
	volatile UINT			m_ObjectCount;
	volatile UINT			m_ObjectBuffSize;
	
public:
	CStaticList(void)		
	{		
		m_pObjectBuff=NULL;
		m_pFreeList=NULL;
		m_pObjectList=NULL;
		m_pObjectListTail=NULL;
		m_ObjectCount=0;
		m_ObjectBuffSize=0;		
	}	
	virtual ~CStaticList(void)
	{
		Destory();
	}
	BOOL Create(int Size)
	{
		Destory();
		m_ObjectBuffSize=Size;
		m_pObjectBuff=new StorageNode[m_ObjectBuffSize];
		for(UINT i=0;i<m_ObjectBuffSize;i++)
		{			
			i==0?m_pObjectBuff[i].pPrev=NULL:m_pObjectBuff[i].pPrev=&(m_pObjectBuff[i-1]);
			i==m_ObjectBuffSize-1?m_pObjectBuff[i].pNext=NULL:m_pObjectBuff[i].pNext=&(m_pObjectBuff[i+1]);
			m_pObjectBuff[i].IsFree=true;
			m_pObjectBuff[i].InitObject();
		}
		m_pFreeList=m_pObjectBuff;
		m_pObjectList=NULL;
		m_pObjectListTail=NULL;
		m_ObjectCount=0;	
		return TRUE;
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
		m_pFreeList=NULL;
		m_pObjectList=NULL;
		m_pObjectListTail=NULL;
		m_ObjectCount=0;
		m_ObjectBuffSize=0;
	}

	void Clear()
	{
		for(UINT i=0;i<m_ObjectBuffSize;i++)
		{			
			i==0?m_pObjectBuff[i].pPrev=NULL:m_pObjectBuff[i].pPrev=&(m_pObjectBuff[i-1]);
			i==m_ObjectBuffSize-1?m_pObjectBuff[i].pNext=NULL:m_pObjectBuff[i].pNext=&(m_pObjectBuff[i+1]);
			m_pObjectBuff[i].IsFree=true;
			m_pObjectBuff[i].DeleteObject();
		}
		m_pFreeList=m_pObjectBuff;
		m_pObjectList=NULL;
		m_pObjectListTail=NULL;
		m_ObjectCount=0;	
	}

	LPVOID PushFront()
	{
		if(m_pFreeList)
		{			
			StorageNode * pNode;

			pNode=m_pFreeList;
			m_pFreeList=pNode->pNext;
			if(m_pFreeList)
				m_pFreeList->pPrev=NULL;
			pNode->pPrev=NULL;
			pNode->pNext=m_pObjectList;
			if(m_pObjectList)
				m_pObjectList->pPrev=pNode;
			else
				m_pObjectListTail=pNode;
			m_pObjectList=pNode;
			pNode->IsFree=false;			
			pNode->NewObject();
			m_ObjectCount++;
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
		if(m_pFreeList)
		{			
			StorageNode * pNode;

			pNode=m_pFreeList;
			m_pFreeList=pNode->pNext;
			if(m_pFreeList)
				m_pFreeList->pPrev=NULL;
			pNode->pPrev=m_pObjectListTail;
			pNode->pNext=NULL;
			if(m_pObjectListTail)
				m_pObjectListTail->pNext=pNode;
			else
				m_pObjectList=pNode;
			m_pObjectListTail=pNode;
			pNode->IsFree=false;
			pNode->NewObject();
			m_ObjectCount++;
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
		if(m_pObjectList)
		{
			Object=m_pObjectList->GetObjectRef();
			DeleteNode(m_pObjectList);
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

	BOOL DeleteObject(LPVOID Pos)
	{		
		if(Pos)
		{		
			DeleteNode((StorageNode *)Pos);
			return TRUE;			
		}
		return FALSE;
	}	

	UINT GetObjectCount()
	{
		return m_ObjectCount;
	}

	LPVOID GetFirstObjectPos()
	{
		return m_pObjectList;
	}

	LPVOID GetLastObjectPos()
	{
		return m_pObjectListTail;
	}
	BOOL GetHead(T& Object)
	{
		if(m_pObjectList)
		{
			Object=m_pObjectList->GetObjectRef();
			return TRUE;
		}
		return FALSE;
	}
	BOOL GetTail(T& Object)
	{
		if(m_pObjectListTail)
		{
			Object=m_pObjectListTail->GetObjectRef();
			return TRUE;
		}
		return FALSE;
	}

	T * GetNext(LPVOID& Pos)
	{
		if(Pos)
		{
			StorageNode * pNode=(StorageNode *)Pos;
			Pos=pNode->pNext;
			return pNode->GetObjectPointer();
		}
		return NULL;
	}

	T * GetPrev(LPVOID& Pos)
	{
		if(Pos)
		{
			StorageNode * pNode=(StorageNode *)Pos;
			Pos=pNode->pPrev;
			return pNode->GetObjectPointer();
		}
		return NULL;
	}

	T * GetObject(LPVOID Pos)
	{
		if(Pos)
		{
			StorageNode * pNode=(StorageNode *)Pos;
			return pNode->GetObjectPointer();
		}
		return NULL;
	}




protected:
	void DeleteNode(StorageNode * pNode)
	{	
		pNode->IsFree=true;		
		pNode->DeleteObject();
		if(pNode->pPrev)
		{
			pNode->pPrev->pNext=pNode->pNext;
			if(pNode->pNext)
				pNode->pNext->pPrev=pNode->pPrev;
			else
				m_pObjectListTail=pNode->pPrev;
		}
		else
		{
			m_pObjectList=pNode->pNext;
			if(m_pObjectList)
				m_pObjectList->pPrev=NULL;
			else
				m_pObjectListTail=NULL;
		}
		pNode->pPrev=NULL;
		pNode->pNext=m_pFreeList;
		if(m_pFreeList)
			m_pFreeList->pPrev=pNode;
		m_pFreeList=pNode;
		m_ObjectCount--;
	}


};
