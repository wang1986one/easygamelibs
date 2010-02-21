#include "StdAfx.h"



CMemoryAllocatee::CMemoryAllocatee(void)
{	
	m_IsWorking=true;
}

CMemoryAllocatee::~CMemoryAllocatee(void)
{
	m_IsWorking=false;


	if(m_AllocInfos.size())
	{

		std::map<void *,ALLOC_INFO>::iterator itr;
		for(itr=m_AllocInfos.begin();itr!=m_AllocInfos.end();itr++)
		{
			if(itr->second.FileName)
			{
				PrintImportantLog(0,"Memory Leak Size=%u,Position=%s:%d",
					itr->second.AllocSize,itr->second.FileName,itr->second.Line);
			}
			else
			{
				PrintImportantLog(0,"Memory Leak Size=%u",
					itr->second.AllocSize);
			}

		}
	}
}

void * CMemoryAllocatee::Alloc(int Size,const char * FileName,int Line)
{
	void * pMemBlock=::malloc(Size);
	if(m_IsWorking)
	{
		ALLOC_INFO& AllocInfo=m_AllocInfos[pMemBlock];
		AllocInfo.AllocSize=Size;
		AllocInfo.FileName=FileName;
		AllocInfo.Line=Line;
	}
	return pMemBlock;
}

void * CMemoryAllocatee::Realloc(void * pBlock,int Size,const char * FileName,int Line)
{
	if(m_IsWorking)
	{
		m_AllocInfos.erase(pBlock);
	}
	void * pMemBlock=::realloc(pBlock,Size);
	if(m_IsWorking)
	{
		ALLOC_INFO& AllocInfo=m_AllocInfos[pMemBlock];
		AllocInfo.AllocSize=Size;
		AllocInfo.FileName=FileName;
		AllocInfo.Line=Line;
	}
	return pMemBlock;
}

void CMemoryAllocatee::Free(void * pBlock)
{
	if(m_IsWorking)
	{
		m_AllocInfos.erase(pBlock);
	}
	free(pBlock);
}

CMemoryAllocatee * CMemoryAllocatee::GetInstance()
{
	return &m_MemoryAllocatee;
}


#pragma init_seg(lib)

bool CMemoryAllocatee::m_IsWorking=false;
CMemoryAllocatee CMemoryAllocatee::m_MemoryAllocatee;
