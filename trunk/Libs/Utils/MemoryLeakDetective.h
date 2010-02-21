#pragma once





#ifdef  USE_USER_NEW


	#ifndef _DEBUG

		#include "MemoryAllocatee.h"

		//NEW_OFF
		//#undef new
		//#undef delete
		//#undef malloc
		//#undef calloc
		//#undef realloc
		//#undef free


		//#include "stdlib.h"           // Required for malloc() and free()

		//
		// C++ style memory allocation.

		//inline void* operator new( size_t size)
		//{
		//	return CMemoryAllocatee::GetInstance()->Alloc(size,NULL,0);
		//}

		inline void* operator new( size_t size, const char *file, int line )
		{
			return CMemoryAllocatee::GetInstance()->Alloc(size,file,line);
		}


		//inline void operator delete( void* address)
		//{
		//	CMemoryAllocatee::GetInstance()->Free(address);
		//}

		inline void operator delete( void* address, const char *file, int line  )
		{
			CMemoryAllocatee::GetInstance()->Free(address);
		}


		//inline void* operator new[]( size_t size)
		//{
		//	return CMemoryAllocatee::GetInstance()->Alloc(size,NULL,0);
		//}

		inline void* operator new[]( size_t size, const char *file, int line  )
		{
			return CMemoryAllocatee::GetInstance()->Alloc(size,file,line);
		}

		//inline void operator delete[]( void* address)
		//{
		//	CMemoryAllocatee::GetInstance()->Free(address);
		//}

		inline void operator delete[]( void* address , const char *file, int line  )
		{
			CMemoryAllocatee::GetInstance()->Free(address);
		}



		#define malloc(sz)       CMemoryAllocatee::GetInstance()->Alloc(sz,__FILE__,__LINE__)
		#define calloc(num, sz)  CMemoryAllocatee::GetInstance()->Alloc(sz*num,__FILE__,__LINE__)
		#define realloc(ptr, sz) CMemoryAllocatee::GetInstance()->Realloc(ptr,sz,__FILE__,__LINE__)
		#define free(ptr)        CMemoryAllocatee::GetInstance()->Free(ptr)

		#define new              new( __FILE__, __LINE__ )

	#endif

#endif



#ifdef USE_CRT_DETAIL_NEW
	#ifdef _DEBUG
		#include "CrtDebugNew.h"
	#endif
#endif

