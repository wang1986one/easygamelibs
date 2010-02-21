#pragma once
#include <map>



class CMemoryAllocatee
{
protected:
	struct ALLOC_INFO
	{
		int				AllocSize;
		const char *	FileName;
		int				Line;
	};

	static bool				m_IsWorking;
	static CMemoryAllocatee m_MemoryAllocatee;

	std::map<void *,ALLOC_INFO>	m_AllocInfos;
public:
	CMemoryAllocatee(void);
	~CMemoryAllocatee(void);

	void * Alloc(int Size,const char * FileName,int Line);
	void * Realloc(void * pBlock,int Size,const char * FileName,int Line);
	void Free(void * pBlock);

	static CMemoryAllocatee * GetInstance();
};
