#pragma once

template<class T>
class CEasyList
{
protected:
	struct OBJ_NODE
	{
		OBJ_NODE * pPrev;
		OBJ_NODE * pNext;
		T Object;
	};
	OBJ_NODE *	m_pHead;
	OBJ_NODE *	m_pTail;
	UINT		m_Count;
public:
	CEasyList()
	{
		m_pHead=NULL;
		m_pTail=NULL;
		m_Count=0;
	}
	~CEasyList()
	{
		Clear();
	}
	void Clear()
	{
		while(m_pHead)
		{
			OBJ_NODE * pNode=m_pHead;
			m_pHead=m_pHead->pNext;
			delete pNode;
		}
		m_pHead=NULL;
		m_pTail=NULL;
		m_Count=0;
	}
	UINT GetCount()
	{
		return m_Count;
	}
	LPVOID GetHead()
	{
		return m_pHead;
	}
	LPVOID GetTail()
	{
		return m_pTail;
	}
	T& GetObject(LPVOID Pos)
	{
		OBJ_NODE * pNode=(OBJ_NODE *)Pos;
		return pNode->Object;
	}
	LPVOID GetPrev(LPVOID Pos)
	{
		OBJ_NODE * pNode=(OBJ_NODE *)Pos;
		return pNode->pPrev;
	}
	T& GetPrevObject(LPVOID& Pos)
	{
		OBJ_NODE * pNode=(OBJ_NODE *)Pos;
		Pos=pNode->pPrev;
		return pNode->Object;
	}	
	LPVOID GetNext(LPVOID Pos)
	{
		OBJ_NODE * pNode=(OBJ_NODE *)Pos;
		return pNode->pNext;
	}
	T& GetNextObject(LPVOID& Pos)
	{
		OBJ_NODE * pNode=(OBJ_NODE *)Pos;
		Pos=pNode->pNext;
		return pNode->Object;
	}	
	void InsertBefore(T& Object,LPVOID Pos=NULL)
	{
		OBJ_NODE * pNode=new OBJ_NODE;		
		pNode->Object=Object;
		InsertBefore(pNode,Pos);
	}
	T& InsertBefore(LPVOID Pos=NULL)
	{
		OBJ_NODE * pNode=new OBJ_NODE;		
		InsertBefore(pNode,Pos);
		return pNode->Object;
	}
	void InsertAfter(T& Object,LPVOID Pos=NULL)
	{
		OBJ_NODE * pNode=new OBJ_NODE;		
		pNode->Object=Object;
		InsertAfter(pNode,Pos);
	}
	T& InsertAfter(LPVOID Pos=NULL)
	{
		OBJ_NODE * pNode=new OBJ_NODE;		
		InsertAfter(pNode,Pos);
		return pNode->Object;
	}	
	void Delete(LPVOID Pos)
	{
		OBJ_NODE * pNode=(OBJ_NODE *)Pos;
		Pick(pNode);
		delete pNode;
	}
	void MoveToBefore(LPVOID Pos,LPVOID Target)
	{
		if(Pos!=Target)
		{
			OBJ_NODE * pNode=(OBJ_NODE *)Pos;
			Pick(pNode);
			InsertBefore(pNode,Target);
		}
	}
	void MoveToAfter(LPVOID Pos,LPVOID Target)
	{
		if(Pos!=Target)
		{
			OBJ_NODE * pNode=(OBJ_NODE *)Pos;
			Pick(pNode);
			InsertAfter(pNode,Target);
		}
	}
	LPVOID Find(T& Object)
	{
		OBJ_NODE * pNode=m_pHead;
		while(pNode)
		{
			if(pNode->Object==Object)
				return pNode;
			pNode=pNode->pNext;
		}
		return NULL;
	}
	
protected:
	void Pick(OBJ_NODE * pNode)
	{
		if(pNode==m_pHead)
			m_pHead=pNode->pNext;
		if(pNode==m_pTail)
			m_pTail=pNode->pPrev;

		if(pNode->pPrev)
			pNode->pPrev->pNext=pNode->pNext;
		if(pNode->pNext)
			pNode->pNext->pPrev=pNode->pPrev;
		m_Count--;
	}
	void InsertBefore(OBJ_NODE * pNode,LPVOID Pos)
	{		
		OBJ_NODE * pBefore=(OBJ_NODE *)Pos;	

		if(pBefore==NULL)
			pBefore=m_pHead;

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
		if(m_pHead==NULL)
		{
			m_pHead=pNode;
			m_pTail=pNode;
		}
		else if(m_pHead==pBefore)
		{
			m_pHead=pNode;
		}
		m_Count++;		
	}
	void InsertAfter(OBJ_NODE * pNode,LPVOID Pos)
	{		
		OBJ_NODE * pAfter=(OBJ_NODE *)Pos;	

		if(pAfter==NULL)
			pAfter=m_pTail;

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

		if(m_pTail==NULL)
		{
			m_pHead=pNode;
			m_pTail=pNode;
		}
		else if(m_pTail==pAfter)
		{
			m_pTail=pNode;
		}
		m_Count++;
	}
};