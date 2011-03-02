/****************************************************************************/
/*                                                                          */
/*      文件名:    EasyArray.h                                              */
/*      创建日期:  2010年02月09日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once

#pragma warning (push)
#pragma warning (disable : 4345)



template < class T >
class CEasyArray
{
protected:
	T *		m_pBuffer;
	UINT	m_BufferSize;
	UINT	m_ArrayLength;
	UINT	m_GrowSize;
public:
	CEasyArray()
	{
		m_pBuffer=NULL;
		m_BufferSize=0;
		m_ArrayLength=0;
		m_GrowSize=1;
	}
	CEasyArray(const CEasyArray& Array)
	{
		m_pBuffer=NULL;
		m_BufferSize=0;
		m_ArrayLength=0;
		m_GrowSize=1;
		*this=Array;		
	}
	CEasyArray(UINT Size,UINT GrowSize=1)
	{
		m_BufferSize=Size;
		m_ArrayLength=0;
		m_GrowSize=GrowSize;
		if(m_BufferSize)
		{
			m_pBuffer=(T *)malloc(sizeof(T)*m_BufferSize);
			ConstructObjects(m_pBuffer,m_BufferSize);
		}
	}
	~CEasyArray()
	{
		Clear();
		m_GrowSize=1;
	}

	void Clear()
	{
		if(m_pBuffer)
		{			
			DestructObjects(m_pBuffer,m_ArrayLength);
			free(m_pBuffer);
			m_pBuffer=NULL;
		}
		m_BufferSize=0;
		m_ArrayLength=0;
	}
	void Empty()
	{
		if(m_pBuffer)
		{			
			DestructObjects(m_pBuffer,m_ArrayLength);			
			m_ArrayLength=0;
		}		
	}
	void Create(UINT Size,UINT GrowSize=1)
	{
		Clear();		
		m_BufferSize=Size;
		m_ArrayLength=0;
		m_GrowSize=GrowSize;
		if(m_BufferSize)
		{
			m_pBuffer=(T *)malloc(sizeof(T)*m_BufferSize);
			ConstructObjects(m_pBuffer,m_BufferSize);
		}
	}

	void Add(const T& Value)
	{
		if(m_ArrayLength>=m_BufferSize)
		{
			ResizeBuffer(m_BufferSize+m_GrowSize);
		}
		ConstructObjects(m_pBuffer+m_ArrayLength,1);
		m_pBuffer[m_ArrayLength]=Value;
		m_ArrayLength++;
	}
	void Add(const CEasyArray<T>& Array)
	{
		if(m_ArrayLength+Array.GetCount()>=m_BufferSize)
		{
			ResizeBuffer(m_BufferSize+Array.GetCount()+m_GrowSize);
		}
		ConstructObjects(m_pBuffer+m_ArrayLength,Array.GetCount());
		for(UINT i=0;i<Array.GetCount();i++)
		{
			m_pBuffer[m_ArrayLength]=Array.GetObjectConst(i);
			m_ArrayLength++;
		}
	}
	bool Insert(UINT BeforeIndex,const T& Value)
	{
		if(BeforeIndex<=m_ArrayLength)
		{
			if(m_ArrayLength>=m_BufferSize)
			{
				ResizeBuffer(m_BufferSize+m_GrowSize);
			}
			if(m_ArrayLength-BeforeIndex)
				memmove(m_pBuffer+BeforeIndex+1,m_pBuffer+BeforeIndex,sizeof(T)*(m_ArrayLength-BeforeIndex));
			ConstructObjects(m_pBuffer+BeforeIndex,1);
			m_pBuffer[BeforeIndex]=Value;
			m_ArrayLength++;
			return true;
		}
		return false;
	}
	bool Delete(UINT Index)
	{
		if(Index<m_ArrayLength)
		{
			DestructObjects(m_pBuffer+Index,1);
			if(m_ArrayLength-Index-1)
				memmove(m_pBuffer+Index,m_pBuffer+Index+1,sizeof(T)*(m_ArrayLength-Index-1));				
			m_ArrayLength--;

			if(m_BufferSize-m_ArrayLength>=m_GrowSize)
			{
				ShrinkBuffer(m_BufferSize-m_GrowSize);
			}			
			return true;
		}
		return false;
	}
	UINT GetCount() const
	{
		return m_ArrayLength;
	}
	bool SetCount(UINT NewLength)
	{
		if(NewLength<=m_BufferSize)
		{
			m_ArrayLength=NewLength;
			return true;
		}
		return false;
	}
	UINT GetBufferLength()
	{
		return m_BufferSize;
	}
	T * GetBuffer()
	{
		return m_pBuffer;
	}
	T * GetObject(UINT Index)
	{
		if(Index<m_ArrayLength)
		{
			return m_pBuffer+Index;
		}
		return NULL;
	}
	const T& GetObjectConst(UINT Index) const
	{
#ifdef _DEBUG
		assert(Index<m_ArrayLength);
#endif
		return m_pBuffer[Index];
	}
	void Resize(UINT NewSize)
	{
		if(NewSize)
		{
			ResizeBuffer(NewSize);
			if(NewSize>m_ArrayLength)
			{
				ConstructObjects(m_pBuffer+m_ArrayLength,NewSize-m_ArrayLength);
			}
			m_ArrayLength=NewSize;
		}
		else
		{
			Clear();
		}
	}
	void Reserve(UINT NewSize)
	{		
		ResizeBuffer(NewSize);		
	}
	const CEasyArray& operator=(const CEasyArray& Array)
	{
		if(m_BufferSize<Array.m_ArrayLength)
			Create(Array.m_ArrayLength,m_GrowSize);
		m_ArrayLength=Array.m_ArrayLength;
		for(UINT i=0;i<m_ArrayLength;i++)
		{
			m_pBuffer[i]=Array.m_pBuffer[i];
		}
		return *this;
	}
	T& operator[](UINT Index)
	{
#ifdef _DEBUG
		assert(Index<m_ArrayLength);
#endif
		return m_pBuffer[Index];
	}
	

protected:
	void ConstructObjects(T * pObjects,UINT ObjectCount)
	{
		for(UINT i=0;i<ObjectCount;i++)
		{
			void * pPoint=pObjects+i;

#if defined(USE_CRT_DETAIL_NEW) && defined(_DEBUG)
#undef new
#endif

			::new(pPoint) T();

#if defined(USE_CRT_DETAIL_NEW) && defined(_DEBUG)
#define new new( __FILE__, __LINE__ )
#endif
		}
	}
	void DestructObjects(T * pObjects,UINT ObjectCount)
	{
		for(UINT i=0;i<ObjectCount;i++)
		{
			pObjects[i].~T();
		}
	}
	void ResizeBuffer(UINT NewSize)
	{
		T * pNewBuffer=(T *)malloc(sizeof(T)*NewSize);
		UINT CopySize;
		if(NewSize>m_BufferSize)
			CopySize=m_BufferSize;
		else
			CopySize=NewSize;
		memcpy(pNewBuffer,m_pBuffer,sizeof(T)*CopySize);
		if(NewSize<m_BufferSize)
		{
			DestructObjects(m_pBuffer+NewSize,m_BufferSize-NewSize);
		}
		m_BufferSize=NewSize;
		free(m_pBuffer);
		m_pBuffer=pNewBuffer;
	}	
	void ShrinkBuffer(UINT NewSize)
	{		
		if(NewSize==0)
		{
			Clear();
			return;
		}
		if(NewSize>=m_BufferSize)
		{
			assert(NewSize<m_BufferSize);
			return;
		}
		T * pNewBuffer=(T *)malloc(sizeof(T)*NewSize);				
		memcpy(pNewBuffer,m_pBuffer,sizeof(T)*m_ArrayLength);
		m_BufferSize=NewSize;
		free(m_pBuffer);
		m_pBuffer=pNewBuffer;
	}
};

#pragma warning (pop)