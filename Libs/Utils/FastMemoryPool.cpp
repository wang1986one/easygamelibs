#include "StdAfx.h"

IMPLEMENT_CLASS_INFO_STATIC(CFastMemoryPool,CNameObject);

CFastMemoryPool::CFastMemoryPool(void):CNameObject()
{
	m_pBlockLevels=NULL;
	m_BlockLevelCount=0;
	m_BlockSize=0;
	m_LevelSize=0;
	m_BufferSize=0;
	m_pBuffer=NULL;
}

CFastMemoryPool::~CFastMemoryPool(void)
{
	Destory();
}

BOOL CFastMemoryPool::Create(UINT BlockSize,UINT LevelSize,UINT MaxLevel,bool IsThreadLock)
{
	Destory();

	m_BlockSize=BlockSize;
	m_LevelSize=LevelSize;
	m_IsThreadLock=IsThreadLock;

	UINT NodeAditionSize=sizeof(BlockNode)+sizeof(UINT);

	//计算最多可划分多少个级别
	UINT Block=BlockSize;
	m_BlockLevelCount=0;
	while(Block+NodeAditionSize<LevelSize&&MaxLevel)
	{
		m_BlockLevelCount++;
		Block=Block<<1;
		MaxLevel--;
	}
	
	m_BufferSize=m_LevelSize*m_BlockLevelCount;
	m_pBuffer=new char[m_BufferSize];
	m_pBlockLevels=new BlockList[m_BlockLevelCount];

	for(UINT i=0;i<m_BlockLevelCount;i++)
	{
		char * pLevelBuffer=m_pBuffer+i*m_LevelSize;		
		m_pBlockLevels[i].AvailableSize=m_BlockSize<<i;
		m_pBlockLevels[i].BlockSize=m_pBlockLevels[i].AvailableSize+NodeAditionSize;
		m_pBlockLevels[i].BlockCount=m_LevelSize/m_pBlockLevels[i].BlockSize;
		m_pBlockLevels[i].UsedCount=0;
		m_pBlockLevels[i].pBlocks=(BlockNode *)pLevelBuffer;
		m_pBlockLevels[i].pFreeList=m_pBlockLevels[i].pBlocks;
		
		for(UINT j=0;j<m_pBlockLevels[i].BlockCount;j++)
		{
			BlockNode * pBlocks=(BlockNode *)((char *)m_pBlockLevels[i].pBlocks+j*m_pBlockLevels[i].BlockSize);
			BlockNode * pPrevBlocks=(BlockNode *)((char *)m_pBlockLevels[i].pBlocks+(j-1)*m_pBlockLevels[i].BlockSize);
			BlockNode * pNextBlocks=(BlockNode *)((char *)m_pBlockLevels[i].pBlocks+(j+1)*m_pBlockLevels[i].BlockSize);

			pBlocks->pBlockList=m_pBlockLevels+i;
			pBlocks->AllocSize=0;
			pBlocks->Flag=BF_FREE;
#ifdef LOG_MEM_CALL_STACK
			pBlocks->RecentCallInfo=0;
			ZeroMemory(pBlocks->CallInfo,sizeof(pBlocks->CallInfo));			
#endif
			if(j==0)
			{
				pBlocks->pPrev=NULL;
			}
			else
			{
				pBlocks->pPrev=pPrevBlocks;
			}
			if(j==m_pBlockLevels[i].BlockCount-1)
			{
				pBlocks->pNext=NULL;
			}
			else
			{
				pBlocks->pNext=pNextBlocks;
			}
			
		}
	}
	return TRUE;
}

void CFastMemoryPool::Destory()
{
#ifdef _DEBUG
	for(UINT i=0;i<m_BlockLevelCount;i++)
	{
		if(m_pBlockLevels[i].UsedCount)
			PrintImportantLog(0xff0000,"还有%d个内存块未释放！",m_pBlockLevels[i].UsedCount);
	}
#endif
	SAFE_DELETE_ARRAY(m_pBlockLevels);
	SAFE_DELETE_ARRAY(m_pBuffer);
	m_BlockLevelCount=0;
	m_BlockSize=0;
	m_LevelSize=0;
	m_BufferSize=0;
}

void CFastMemoryPool::Clear()
{
	for(UINT i=0;i<m_BlockLevelCount;i++)
	{
		m_pBlockLevels[i].UsedCount=0;
		m_pBlockLevels[i].pFreeList=m_pBlockLevels[i].pBlocks;
		
		for(UINT j=0;j<m_pBlockLevels[i].BlockCount;j++)
		{
			BlockNode * pBlocks=(BlockNode *)((char *)m_pBlockLevels[i].pBlocks+j*m_pBlockLevels[i].BlockSize);
			BlockNode * pPrevBlocks=(BlockNode *)((char *)m_pBlockLevels[i].pBlocks+(j-1)*m_pBlockLevels[i].BlockSize);
			BlockNode * pNextBlocks=(BlockNode *)((char *)m_pBlockLevels[i].pBlocks+(j+1)*m_pBlockLevels[i].BlockSize);

			pBlocks->Flag=BF_FREE;

			if(j==0)
			{
				pBlocks->pPrev=NULL;
			}
			else
			{
				pBlocks->pPrev=pPrevBlocks;
			}
			if(j==m_pBlockLevels[i].BlockCount-1)
			{
				pBlocks->pNext=NULL;
			}
			else
			{
				pBlocks->pNext=pNextBlocks;
			}
		}
	}
}

void CFastMemoryPool::Verfy()
{
	for(UINT i=0;i<m_BlockLevelCount;i++)
	{
		for(UINT j=0;j<m_pBlockLevels[i].BlockCount;j++)
		{
			BlockNode * pNode=(BlockNode *)((char *)m_pBlockLevels[i].pBlocks+j*m_pBlockLevels[i].BlockSize);
			

			if(pNode->Flag==BF_USED)
			{
				if(*((UINT *)(((BYTE *)pNode)+sizeof(BlockNode)+pNode->AllocSize))!=BF_TAIL)
				{
					PrintImportantLog(0,"CFastMemoryPool::FreeBlock:内存块%p尾部已被破坏");			
					assert(false);
				}
			}
			else if(pNode->Flag!=BF_FREE)
			{
				PrintImportantLog(0,"CFastMemoryPool::FreeBlock:内存块%p头部已被破坏");			
				assert(false);

			}			
		}
	}
}

LPVOID CFastMemoryPool::Alloc(UINT Size)
{
	if(m_pBuffer&&Size)
	{			
		UINT Level=0;
		for(;Level<m_BlockLevelCount;Level++)
		{
			if(m_pBlockLevels[Level].AvailableSize>=Size)
				break;
		}		
		if(Level<m_BlockLevelCount)
		{
			LPVOID pMem=AllocBlock(m_pBlockLevels+Level,Size);
			if(pMem)
				return pMem;
		}

		return new char[Size];
	}
	else
		return NULL;
}

BOOL CFastMemoryPool::Free(LPVOID pMem)
{
	if(m_pBuffer)
	{
		if((UINT)((char *)pMem-m_pBuffer)<m_BufferSize)
		{
			return FreeBlock((BlockNode *)((char *)pMem-sizeof(BlockNode)));
		}
		delete[] (char *)pMem;
		return TRUE;
	}
	else
		return FALSE;
}

#ifdef LOG_MEM_CALL_STACK
void CFastMemoryPool::LogMemUse(LPVOID pMem,int RefCount)
{
	if(m_pBuffer)
	{
		if((UINT)((char *)pMem-m_pBuffer)<m_BufferSize)
		{
			BlockNode * pNode=(BlockNode *)((char *)pMem-sizeof(BlockNode));

			pNode->CallInfo[pNode->RecentCallInfo].StackDepth=CExceptionParser::GetInstance()->GetCallStack((DWORD64 *)pNode->CallInfo[pNode->RecentCallInfo].CallStack,MAX_CALL_STACK);
			pNode->CallInfo[pNode->RecentCallInfo].RefCount=RefCount;
			pNode->RecentCallInfo++;
			if(pNode->RecentCallInfo>=MAX_CALL_INFO)
				pNode->RecentCallInfo=0;
			
		}		
	}
}
#endif

LPVOID CFastMemoryPool::AllocBlock(BlockList * pBlockList,UINT AllocSize)
{
	CAutoLockEx ThreadLock;
	if(m_IsThreadLock)
		ThreadLock.Lock(m_EasyCriticalSection);

	if(pBlockList)
	{
		if(pBlockList->pFreeList)
		{
			volatile BlockNode * pNode;

			pNode=pBlockList->pFreeList;
			pBlockList->pFreeList=pNode->pNext;
			if(pBlockList->pFreeList)
				pBlockList->pFreeList->pPrev=NULL;
			pNode->Flag=BF_USED;
			pNode->AllocSize=AllocSize;
			*((UINT *)(((BYTE *)pNode)+sizeof(BlockNode)+pNode->AllocSize))=BF_TAIL;
			pBlockList->UsedCount++;

			return (char *)pNode+sizeof(BlockNode);
		}
	}
	return NULL;
}

BOOL CFastMemoryPool::FreeBlock(BlockNode * pNode)
{
	CAutoLockEx ThreadLock;
	if(m_IsThreadLock)
		ThreadLock.Lock(m_EasyCriticalSection);

	if(pNode)
	{
		if(pNode->Flag!=BF_USED)
		{
			PrintImportantLog(0,"CFastMemoryPool::FreeBlock:内存块%p头部已被破坏");			
#ifdef LOG_MEM_CALL_STACK
			PrintCallStackLog(pNode);
#endif
			assert(false);

		}
		if(*((UINT *)(((BYTE *)pNode)+sizeof(BlockNode)+pNode->AllocSize))!=BF_TAIL)
		{
			PrintImportantLog(0,"CFastMemoryPool::FreeBlock:内存块%p尾部已被破坏");			
#ifdef LOG_MEM_CALL_STACK
			PrintCallStackLog(pNode);
#endif
			assert(false);
		}
		BlockList * pBlockList=pNode->pBlockList;

		pNode->AllocSize=0;
		pNode->Flag=BF_FREE;
		pNode->pPrev=NULL;
		pNode->pNext=pBlockList->pFreeList;
		if(pBlockList->pFreeList)
			pBlockList->pFreeList->pPrev=pNode;
		pBlockList->pFreeList=pNode;
		pBlockList->UsedCount--;
		return TRUE;
		
	}
	return FALSE;
}

#ifdef LOG_MEM_CALL_STACK
void CFastMemoryPool::PrintCallStackLog(BlockNode * pNode)
{
	PrintImportantLog(0,"当前记录号:%u",pNode->RecentCallInfo);
	CExceptionParser::ADDRESS_INFO AddressInfo;
	CExceptionParser::GetInstance()->SymInit();
	for(UINT j=0;j<MAX_CALL_INFO;j++)
	{
		PrintImportantLog(0,"记录号",j);
		for(UINT i=0;i<pNode->CallInfo[j].StackDepth;i++)
		{
			CExceptionParser::GetInstance()->GetAddressInfo(pNode->CallInfo[j].CallStack[i],&AddressInfo);
			PrintImportantLog(0,"%ll08X,%s:%d",AddressInfo.Address,AddressInfo.CppFileName,AddressInfo.LineNumber);
		}
	}
}
#endif