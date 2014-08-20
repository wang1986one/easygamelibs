/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DBLib.h                                                  */
/*      ��������:  2009��07��06��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once

#define DBLIB_VERSION	"2.0.0.0"

#define LOG_DB_ERROR_CHANNEL	1202
//typedef unsigned long ulong;

inline BOOL PrintDBLog(DWORD Color,LPCSTR Format,...)
{
	va_list vl;
	va_start(vl,Format);
	BOOL ret=CLogManager::GetInstance()->PrintLogVL(LOG_DB_ERROR_CHANNEL,ILogPrinter::LOG_LEVEL_NORMAL,Color,Format,vl);
	va_end(vl);
	return ret;
}

inline BOOL PrintDBDebugLog(DWORD Color,LPCSTR Format,...)
{
	va_list vl;
	va_start(vl,Format);
	BOOL ret=CLogManager::GetInstance()->PrintLogVL(LOG_DB_ERROR_CHANNEL,ILogPrinter::LOG_LEVEL_DEBUG,Color,Format,vl);
	va_end(vl);
	return ret;
}

#include "DBValue.h"

#include "DBInterfaces.h"

#include "DBParameterSet.h"
#include "DBStaticRecordSet.h"
#include "DBFakeRecordSet.h"


#ifdef WIN32

#include <sql.h>
#include <sqlext.h>
#include <WinSock2.h>
#include <MSWSock.h>




#include "ODBCRecordSet.h"
#include "ODBCConnection.h"
#include "ODBCDataBase.h"

#else



#endif


#include "MySQL/include/mysql.h"


#include "MySQLDynamicRecordSet.h"
#include "MySQLRecordSet.h"
#include "MySQLConnection.h"
#include "MySQLDatabase.h"

#include "DBTransaction.h"
#include "DBTransationWorkThread.h"
#include "DBTransationManager.h"

using namespace DBLib;


