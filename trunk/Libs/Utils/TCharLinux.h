/****************************************************************************/
/*                                                                          */
/*      �ļ���:    TCharLinux.h                                             */
/*      ��������:  2009��07��06��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once



#ifdef UNICODE


#else



#define _tcscmp			strcmp
#define _tcsncmp		strncmp
#define _tcsicmp		strcasecmp
#define _tcsnicmp		strncasecmp
#define _tcslen			strlen


#define _tcscpy			strcpy
#define _tcscpy_s		strcpy_s
#define _tcsncpy		strncpy
#define _tcsncpy_s		strncpy_s

#define _tcscat_s		strcat_s
#define _tcsncat_s		strncat_s

#define _tcschr			strchr

#define _tstoi			atoi
#define _tcstol			strtol
#define _tcstod			strtod
#define _tcstoul		strtoul
#define _tstoi64		atoll
#define _tcstoi64		strtoll
#define _tcstoui64		strtoull
#define _tstof			atof
#define _tcstok_s		strtok_s
#define _itot_s			_itoa_s
#define _ltot_s			_ltoa_s
#define _istalnum		isalnum

#define _tcsftime		strftime


#define _tcprintf		printf
#define _stprintf_s		sprintf_s
#define _vstprintf_s	vsprintf_s

#define _tfopen_s		fopen_s

#endif

