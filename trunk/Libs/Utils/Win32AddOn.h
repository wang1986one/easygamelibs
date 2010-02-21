#pragma once



/////////////////////////////////////////////////////////////////////////////
// Win32 libraries

#ifndef _AFX

	#ifndef _UNICODE
		#ifdef _DEBUG
			#pragma comment(lib, "nafxcwd.lib")
		#else
			#pragma comment(lib, "nafxcw.lib")
		#endif
	#else
		#ifdef _DEBUG
			#pragma comment(lib, "uafxcwd.lib")
		#else
			#pragma comment(lib, "uafxcw.lib")
		#endif
	#endif


	#ifdef _DLL
		#if !defined(_AFX_NO_DEBUG_CRT) && defined(_DEBUG)
			#pragma comment(lib, "msvcrtd.lib")
		#else
			#pragma comment(lib, "msvcrt.lib")
		#endif
	#else
		#ifdef _MT
			#if !defined(_AFX_NO_DEBUG_CRT) && defined(_DEBUG)
				#pragma comment(lib, "libcmtd.lib")
			#else
				#pragma comment(lib, "libcmt.lib")
			#endif
		#else
			#if !defined(_AFX_NO_DEBUG_CRT) && defined(_DEBUG)
				#pragma comment(lib, "libcd.lib")
			#else
				#pragma comment(lib, "libc.lib")
			#endif
		#endif
	#endif

	#pragma comment(lib, "kernel32.lib")
	#pragma comment(lib, "user32.lib")
	#pragma comment(lib, "gdi32.lib")
	#pragma comment(lib, "msimg32.lib")
	#pragma comment(lib, "comdlg32.lib")
	#pragma comment(lib, "winspool.lib")
	#pragma comment(lib, "advapi32.lib")
	#pragma comment(lib, "shell32.lib")
	#pragma comment(lib, "comctl32.lib")
	#pragma comment(lib, "shlwapi.lib")

	// force inclusion of NOLIB.OBJ for /disallowlib directives
	#pragma comment(linker, "/include:__afxForceEXCLUDE")

	// force inclusion of DLLMODUL.OBJ for _USRDLL
	#ifdef _USRDLL
	#pragma comment(linker, "/include:__afxForceUSRDLL")
	#endif
#endif


#include "windows.h"
#include "Winsvc.h"
#include "shlobj.h"
#include "tchar.h"
#include <time.h>

#pragma warning (push)
#pragma warning (disable : 4311)
#include "atomic_ops\include\atomic_ops.h"
#pragma warning (pop)

#pragma warning (push)
#pragma warning (disable : 4267)	// type conversion
#pragma warning (disable : 4311)	// pointer truncation
#pragma warning (disable : 4244)	// type conversion

#include "PugXML.h"
using namespace pug;

#pragma warning (pop)

typedef double DOUBLE;

typedef unsigned __int64 ULONG64;
typedef unsigned __int64 UINTG64;

typedef __int64 LONG64;
typedef __int64 INTG64;

inline unsigned int AtomicInc(volatile unsigned int * pVal)
{
	return AO_int_fetch_and_add1_read(pVal)+1;
}



inline unsigned int AtomicDec(volatile unsigned int * pVal)
{
	return AO_int_fetch_and_sub1_read(pVal)-1;
}

//inline unsigned int AtomicRead(volatile unsigned int * pVal)
//{
//	return AO_int_load(pVal);
//}
//
//inline void AtomicWrite(volatile unsigned int * pVal,unsigned int NewVal)
//{
//	AO_int_store(pVal,NewVal);
//}

inline unsigned int AtomicAdd(volatile unsigned int * pVal,int AddVal)
{
	return AO_int_fetch_and_add(pVal,AddVal);
}

inline unsigned int AtomicSub(volatile unsigned int * pVal,int SubVal)
{
	return AO_int_fetch_and_add(pVal,-SubVal);
}

inline int AtomicCompareAndSet(volatile unsigned int * pVal,unsigned int CompValue,unsigned int NewVal)
{
	return InterlockedCompareExchange((volatile LONG *)pVal,NewVal,CompValue)!=*pVal;
}



//inline unsigned __int64 AtomicIncAndRead(volatile unsigned __int64 * pVal)
//{
//	return InterlockedIncrement64(pVal);
//}
//
//inline unsigned __int64 AtomicDecAndRead(volatile unsigned __int64 * pVal)
//{
//	return InterlockedDecrement64(pVal);
//}

inline size_t GetEnvVar(LPCTSTR pszVarName,LPTSTR pszValue,size_t nBufferLen)
{
	size_t ValueLen=0;
#ifdef _UNICODE
	_wgetenv_s(&ValueLen,pszValue,nBufferLen,pszVarName);
#else	
	getenv_s(&ValueLen,pszValue,nBufferLen,pszVarName);
#endif
	return ValueLen;
}

inline bool SetEnvVar(LPCTSTR pszVarName,LPTSTR pszValue)
{
#ifdef _UNICODE
	if(_wputenv_s(pszVarName,pszValue)==0)
		return true;
#else	
	if(_putenv_s(pszVarName,pszValue)==0)
		return true;
#endif
	return false;
}

#include "EasyCriticalSectionWin.h"
#include "EasyReadWriteLockWin.h"