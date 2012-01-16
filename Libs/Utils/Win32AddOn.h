/****************************************************************************/
/*                                                                          */
/*      文件名:    Win32AddOn.h                                             */
/*      创建日期:  2010年02月09日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once



/////////////////////////////////////////////////////////////////////////////
// Win32 libraries

//#ifndef _AFX
//
//	#ifndef _UNICODE
//		#ifdef _DEBUG
//			#pragma comment(lib, "nafxcwd.lib")
//		#else
//			#pragma comment(lib, "nafxcw.lib")
//		#endif
//	#else
//		#ifdef _DEBUG
//			#pragma comment(lib, "uafxcwd.lib")
//		#else
//			#pragma comment(lib, "uafxcw.lib")
//		#endif
//	#endif
//
//
//	#ifdef _DLL
//		#if !defined(_AFX_NO_DEBUG_CRT) && defined(_DEBUG)
//			#pragma comment(lib, "msvcrtd.lib")
//		#else
//			#pragma comment(lib, "msvcrt.lib")
//		#endif
//	#else
//		#ifdef _MT
//			#if !defined(_AFX_NO_DEBUG_CRT) && defined(_DEBUG)
//				#pragma comment(lib, "libcmtd.lib")
//			#else
//				#pragma comment(lib, "libcmt.lib")
//			#endif
//		#else
//			#if !defined(_AFX_NO_DEBUG_CRT) && defined(_DEBUG)
//				#pragma comment(lib, "libcd.lib")
//			#else
//				#pragma comment(lib, "libc.lib")
//			#endif
//		#endif
//	#endif
//
//	#pragma comment(lib, "kernel32.lib")
//	#pragma comment(lib, "user32.lib")
//	#pragma comment(lib, "gdi32.lib")
//	#pragma comment(lib, "msimg32.lib")
//	#pragma comment(lib, "comdlg32.lib")
//	#pragma comment(lib, "winspool.lib")
//	#pragma comment(lib, "advapi32.lib")
//	#pragma comment(lib, "shell32.lib")
//	#pragma comment(lib, "comctl32.lib")
//	#pragma comment(lib, "shlwapi.lib")
//
//	// force inclusion of NOLIB.OBJ for /disallowlib directives
//	#pragma comment(linker, "/include:__afxForceEXCLUDE")
//
//	// force inclusion of DLLMODUL.OBJ for _USRDLL
//	#ifdef _USRDLL
//	#pragma comment(linker, "/include:__afxForceUSRDLL")
//	#endif
//#endif


#include "windows.h"
#include "Winsvc.h"
#include "shlobj.h"
#include "tchar.h"
#include <time.h>
#include <conio.h>


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

inline unsigned int AtomicAnd(volatile unsigned int * pVal,int AndVal)
{
	return _InterlockedAnd((volatile LONG *)pVal,AndVal);
}

inline unsigned int AtomicOr(volatile unsigned int * pVal,int AndVal)
{
	return _InterlockedOr((volatile LONG *)pVal,AndVal);
}

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
#include "AutoLock.h"
#include "EasyReadWriteLockWin.h"