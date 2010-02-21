#pragma once

#ifdef UNICODE

typedef wchar_t TCHAR;

#else

typedef char TCHAR;

#define _T(x)			x
#define _tcscmp			strcmp
#define _tcsncmp		strncmp
#define _tcslen			strlen
#define _istalnum		isalnum
#define _tcscpy			strcpy
#define _tcsncpy		strncpy
#define _tcsncpy_s		strncpy_s
#define _tcstol			strtol
#define _tcstod			strtod
#define _tcstok_s		strtok_s
#define _itot_s			_itoa_s
#define _ltot_s			_ltoa_s
#define _stprintf_s		sprintf_s
#define _tcsftime		strftime
#endif

typedef TCHAR * LPTSTR;
typedef const TCHAR * LPCTSTR;