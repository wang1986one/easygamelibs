#pragma once
#include <vector>

#if defined(USE_CRT_DETAIL_NEW) && defined(_DEBUG)
#undef new
#include <set>
#define new new( __FILE__, __LINE__ )
#else
#include <set>
#endif


template < class T>
class CIndexSet
{
protected:
	std::vector<T>	m_ObjectArray;
	std::set<T>		m_ObjectSet;
public:
	CIndexSet()
	{
	}
	~CIndexSet()
	{
		Clear();
	}
	void Clear()
	{
		m_ObjectArray.clear();
		m_ObjectSet.clear();
	}
	BOOL Add(T Object)
	{
		if(m_ObjectSet.insert(Object).second)
		{
			m_ObjectArray.push_back(Object);
			return TRUE;
		}
		return FALSE;
	}
	BOOL AddFront(T Object)
	{
		if(m_ObjectSet.insert(Object).second)
		{
			m_ObjectArray.insert(m_ObjectArray.begin(),Object);
			return TRUE;
		}
		return FALSE;
	}
	void AddForce(T Object)
	{
		if(!Add(Object))
		{
			m_ObjectArray.push_back(Object);
		}
	}
	UINT GetObjectCount()
	{
		return (UINT)m_ObjectArray.size();
	}
	T& GetObject(int Index)
	{
		return m_ObjectArray[Index];
	}
	T& operator[](int Index)
	{
		return m_ObjectArray[Index];
	}
	int GetIndex(T Object)
	{
		for(int i=0;i<(int)m_ObjectArray.size();i++)
		{
			if(m_ObjectArray[i]==Object)
			{
				return i;
			}
		}
		return -1;
	}
	BOOL DeleteObject(int Index)
	{
		if(Index>=0&&Index<(int)m_ObjectArray.size())
		{
			m_ObjectSet.erase(m_ObjectArray[Index]);
			m_ObjectArray.erase(m_ObjectArray.begin()+Index);
			return TRUE;
		}
		return FALSE;
	}

};