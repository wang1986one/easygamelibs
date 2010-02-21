#pragma once

template < class T>
class CSmartPtr
{
protected:
	T * m_Pointer;
public:
	CSmartPtr()
	{
		m_Pointer=NULL;
	}
	CSmartPtr(const T * Pointer)
	{
		m_Pointer=(T *)Pointer;
	}
	~CSmartPtr()
	{
		SAFE_RELEASE(m_Pointer);
	}
	operator T*()
	{
		return m_Pointer;
	}
	T& operator *()
	{
		return *m_Pointer;
	}
	T** operator &()
	{
		return &m_Pointer;
	}
	T* operator->()
	{		
		return m_Pointer;
	}	
	bool operator==(const T* Pointer)
	{
		return m_Pointer == Pointer;
	}
	bool operator!=(const T* Pointer)
	{
		return m_Pointer != Pointer;
	}
	T* operator=(const T* Pointer)
	{
		SAFE_RELEASE(m_Pointer);
		m_Pointer=(T *)Pointer;		
		return m_Pointer;
	}
	void Detach()
	{
		m_Pointer=NULL;
	}
	
};

template < class T>
class CSmartPtrForDelete
{
protected:
	T * m_Pointer;
public:
	CSmartPtrForDelete()
	{
		m_Pointer=NULL;
	}
	CSmartPtrForDelete(const T * Pointer)
	{
		m_Pointer=(T *)Pointer;
	}
	~CSmartPtrForDelete()
	{
		SAFE_DELETE(m_Pointer);
	}
	operator T*()
	{
		return m_Pointer;
	}
	T& operator *()
	{
		return *m_Pointer;
	}
	T** operator &()
	{
		return &m_Pointer;
	}
	T* operator->()
	{		
		return m_Pointer;
	}	
	bool operator==(const T* Pointer)
	{
		return m_Pointer == Pointer;
	}
	bool operator!=(const T* Pointer)
	{
		return m_Pointer != Pointer;
	}
	T* operator=(const T* Pointer)
	{
		SAFE_DELETE(m_Pointer);
		m_Pointer=(T *)Pointer;		
		return m_Pointer;
	}

};

template < class T>
class CSmartPtrForArray
{
protected:
	T * m_Pointer;
public:
	CSmartPtrForArray()
	{
		m_Pointer=NULL;
	}
	CSmartPtrForArray(const T * Pointer)
	{
		m_Pointer=(T *)Pointer;
	}
	~CSmartPtrForArray()
	{
		SAFE_DELETE_ARRAY(m_Pointer);
	}
	operator T*()
	{
		return m_Pointer;
	}
	T& operator *()
	{
		return *m_Pointer;
	}
	T** operator &()
	{
		return &m_Pointer;
	}
	T* operator->()
	{		
		return m_Pointer;
	}	
	bool operator==(const T* Pointer)
	{
		return m_Pointer == Pointer;
	}
	bool operator!=(const T* Pointer)
	{
		return m_Pointer != Pointer;
	}
	T* operator=(const T* Pointer)
	{
		SAFE_DELETE_ARRAY(m_Pointer);
		m_Pointer=(T *)Pointer;		
		return m_Pointer;
	}
	

};