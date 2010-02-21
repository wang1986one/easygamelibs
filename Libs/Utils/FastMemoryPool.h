#pragma once
#include "nameobject.h"

class CFastMemoryPool :
	public CNameObject
{
protected:
	enum BLOCK_FLAG
	{
		BF_FREE=0x12345678,
		BF_USED=0x9ABCDEF0,
		BF_TAIL=0xAABBCCDD,
	};

	enum
	{
		MAX_CALL_STACK=10,
		MAX_CALL_INFO=10,
	};
	struct CALL_INFO
	{
		DWORD64							CallStack[MAX_CALL_STACK];		
		UINT							StackDepth;
		int								RefCount;		
	};


	struct BlockList;
	struct BlockNode
	{	
		UINT					Flag;
		BlockList *				pBlockList;
		volatile BlockNode *	pPrev;
		volatile BlockNode *	pNext;
		UINT					AllocSize;	
#ifdef LOG_MEM_CALL_STACK
		CALL_INFO				CallInfo[MAX_CALL_INFO];
#endif
		UINT					RecentCallInfo;
	};
	struct BlockList
	{	
		UINT					AvailableSize;
		UINT					BlockSize;		
		UINT					BlockCount;
		volatile UINT			UsedCount;
		BlockNode *				pBlocks;
		volatile BlockNode *	pFreeList;		
	};
	

	BlockList *		m_pBlockLevels;
	UINT			m_BlockSize;
	UINT			m_LevelSize;
	UINT			m_BlockLevelCount;
	char *			m_pBuffer;	
	UINT			m_BufferSize;
	bool			m_IsThreadLock;

	CEasyCriticalSection	m_EasyCriticalSection;	

	DECLARE_CLASS_INFO_STATIC(CFastMemoryPool)
public:
	CFastMemoryPool(void);
	virtual ~CFastMemoryPool(void);

	BOOL Create(UINT BlockSize,UINT LevelSize,UINT MaxLevel=0,bool IsThreadLock=false);
	virtual void Destory();
	void Clear();
	void Verfy();

	LPVOID Alloc(UINT Size);
	BOOL Free(LPVOID pMem);
#ifdef LOG_MEM_CALL_STACK
	void LogMemUse(LPVOID pMem,int RefCount);
#else
	void LogMemUse(LPVOID pMem,int RefCount)
	{

	}
#endif
protected:
	LPVOID AllocBlock(BlockList * pBlockList,UINT AllocSize);
	BOOL FreeBlock(BlockNode * pNode);
#ifdef LOG_MEM_CALL_STACK
	void PrintCallStackLog(BlockNode * pNode);
#endif
};
