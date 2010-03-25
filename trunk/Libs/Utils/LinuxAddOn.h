/****************************************************************************/
/*                                                                          */
/*      文件名:    LinuxAddOn.h                                             */
/*      创建日期:  2009年10月23日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once


#define   _FILE_OFFSET_BITS   64
#define   _LARGEFILE64_SOURCE
#define   __USE_LARGEFILE64

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <limits.h>
#include <utility>
#include <assert.h>
#include <ctype.h>
#include <wctype.h>
#include <wchar.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>
#include "atomic_ops/include/atomic_ops.h"
#include "TCharLinux.h"



#define CLOCKS_PER_MILLISEC		(CLOCKS_PER_SEC/1000)
#define INVALID_HANDLE_VALUE	(-1)


#define MAX_PATH        PATH_MAX

#define TRUE			1
#define FALSE			0
#define INFINITE		0xffffffff
#define __int64			long long


typedef signed char			INT8;
typedef unsigned char		UINT8;
typedef signed short		INT16;
typedef unsigned short		UINT16;
typedef signed int			INT32;
typedef unsigned long		UINT32;


typedef long long LONG64;
typedef long long INT64;
typedef unsigned long long ULONG64;
typedef unsigned long long UINT64;

typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned int DWORD;
typedef unsigned int UINT;
typedef long LONG;
typedef unsigned long ULONG;
typedef unsigned int WPARAM;
typedef unsigned long LPARAM;
typedef unsigned int  LRESULT;
typedef int BOOL;
typedef float FLOAT;
typedef double DOUBLE;
typedef char * LPSTR;
typedef const char * LPCSTR;
typedef wchar_t * LPWSTR;
typedef const wchar_t * LPCWSTR;
typedef void * LPVOID;
typedef const void * LPCVOID;
typedef void * HMODULE;
typedef void * HANDLE;
typedef int INT_PTR, *PINT_PTR;
typedef unsigned int UINT_PTR, *PUINT_PTR;
typedef unsigned long DWORD_PTR, *PDWORD_PTR;

#define _stricmp		strcasecmp
#define _strnicmp		strncasecmp
#define _wcsicmp		wcscasecmp
#define _atoi64			atoll
#define GetLastError()	errno


#define MAKEWORD(a, b)      ((WORD)(((BYTE)(((DWORD_PTR)(a)) & 0xff)) | ((WORD)((BYTE)(((DWORD_PTR)(b)) & 0xff))) << 8))
#define MAKELONG(a, b)      ((LONG)(((WORD)(((DWORD_PTR)(a)) & 0xffff)) | ((DWORD)((WORD)(((DWORD_PTR)(b)) & 0xffff))) << 16))
#define LOWORD(l)           ((WORD)(((DWORD_PTR)(l)) & 0xffff))
#define HIWORD(l)           ((WORD)((((DWORD_PTR)(l)) >> 16) & 0xffff))
#define LOBYTE(w)           ((BYTE)(((DWORD_PTR)(w)) & 0xff))
#define HIBYTE(w)           ((BYTE)((((DWORD_PTR)(w)) >> 8) & 0xff))

inline int fopen_s(FILE ** _File, const char * _Filename, const char * _Mode)
{
	*_File=fopen(_Filename,_Mode);
	return 0;
}

inline size_t fread_s(void * _DstBuf, size_t _DstSize, size_t _ElementSize, size_t _Count, FILE * _File)
{
	return fread(_DstBuf,_ElementSize,_Count,_File);
}
inline int strcpy_s(char * _Dst, size_t _SizeInBytes, const char * _Src)
{
	strcpy(_Dst,_Src);
	return 0;
}

inline int wcscpy_s(wchar_t * _Dst, size_t _SizeInWords, const wchar_t * _Src)
{
	wcscpy(_Dst,_Src);
	return 0;
}

inline int strncpy_s(char * _Dst, size_t _SizeInBytes, const char * _Src, size_t _MaxCount)
{
	strncpy(_Dst,_Src,_MaxCount);
	return 0;
}

inline int wcsncpy_s(wchar_t * _Dst, size_t _SizeInWords, const wchar_t * _Src, size_t _MaxCount)
{
	wcsncpy(_Dst,_Src,_MaxCount);
	return 0;
}

inline int strcat_s(char * _Dst, size_t _SizeInBytes, const char * _Src)
{
	strcat(_Dst,_Src);
	return 0;
}

inline int wcscat_s(wchar_t * _Dst, size_t _SizeInWords, const wchar_t * _Src)
{
	wcscat(_Dst,_Src);
	return 0;
}

inline char *  strtok_s(char * _Str, const char * _Delim, char ** _Context)
{
	if(_Str==NULL)
		_Str=*_Context;
	char * Ret=strtok(_Str,_Delim);
	*_Context=Ret+1;
	return Ret;
}

inline int _itoa_s(int _Value, char * _DstBuf, size_t _Size, int _Radix)
{
	if(_Radix==16)
	{
		sprintf(_DstBuf,"%X",_Value);
	}
	else
	{
		sprintf(_DstBuf,"%d",_Value);
	}
	return 0;
}

inline int _ltoa_s(long _Val, char * _DstBuf, size_t _Size, int _Radix)
{
	if(_Radix==16)
	{
		sprintf(_DstBuf,"%lX",_Val);
	}
	else
	{
		sprintf(_DstBuf,"%ld",_Val);
	}
	return 0;
}

#define sprintf_s(_DstBuf, _SizeInBytes, _Format, ...)	sprintf(_DstBuf,_Format,__VA_ARGS__)
#define _vscprintf(_Format, _ArgList)		vsnprintf(NULL,0,_Format,_ArgList)
#define _vscwprintf(_Format, _ArgList)		vswprintf(NULL,0,_Format,_ArgList)

inline int vsprintf_s(char * _DstBuf, size_t _SizeInBytes, const char * _Format, va_list _ArgList)
{
	return vsprintf(_DstBuf,_Format,_ArgList);
}

inline int vswprintf_s(wchar_t * _DstBuf, size_t _SizeInBytes, const wchar_t * _Format, va_list _ArgList)
{
	return vswprintf(_DstBuf,_SizeInBytes,_Format,_ArgList);
}

inline int _strupr_s(char * _Str, size_t _Size)
{
	while(_Size&&(*_Str))
	{
		*_Str=toupper(*_Str);
		_Str++;
		_Size--;
	}
	return 0;
}

inline int _wcsupr_s(wchar_t * _Str, size_t _SizeInWords)
{
	while(_SizeInWords&&(*_Str))
	{
		*_Str=towupper(*_Str);
		_Str++;
		_SizeInWords--;
	}
	return 0;
}

inline int _strlwr_s(char * _Str, size_t _Size)
{
	while(_Size&&(*_Str))
	{
		*_Str=tolower(*_Str);
		_Str++;
		_Size--;
	}
	return 0;
}

inline int _wcslwr_s(wchar_t * _Str,  size_t _SizeInWords)
{
	while(_SizeInWords&&(*_Str))
	{
		*_Str=towlower(*_Str);
		_Str++;
		_SizeInWords--;
	}
	return 0;
}

inline int strncat_s(char * _Dst, size_t _SizeInBytes, const char * _Src, size_t _MaxCount)
{
	strncat(_Dst,_Src,_MaxCount);
	return 0;
}

inline int localtime_s(struct tm * _Tm, const time_t * _Time)
{
	*_Tm=*localtime(_Time);
	return 0;
}

inline unsigned int AtomicInc(volatile unsigned int * pVal)
{
	return AO_int_fetch_and_add1_read(pVal)+1;
}



inline unsigned int AtomicDec(volatile unsigned int * pVal)
{
	return AO_int_fetch_and_sub1_read(pVal)+1;
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
	return AO_int_compare_and_swap(pVal,CompValue,NewVal)
}


#define ZeroMemory(Destination,Length) memset((Destination),0,(Length))

#include <iterator>
#include "pugxml_linux.h"
using namespace pug;




#include "EasyCriticalSectionLinux.h"
#include "EasyReadWriteLockLinux.h"

typedef struct tagRECT
{
	LONG    left;
	LONG    top;
	LONG    right;
	LONG    bottom;
} RECT, *PRECT,  *LPRECT;

typedef const tagRECT * LPCRECT;

typedef struct tagPOINT
{
	LONG  x;
	LONG  y;
} POINT, *PPOINT,  *LPPOINT;

typedef const tagPOINT * LPCPOINT;

typedef struct tagSIZE
{
	LONG        cx;
	LONG        cy;
} SIZE, *PSIZE, *LPSIZE;

typedef const tagSIZE * LPCSIZE;
