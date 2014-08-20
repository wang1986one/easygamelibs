/****************************************************************************/
/*                                                                          */
/*      文件名:    DOSDefines.h                                             */
/*      创建日期:  2009年10月23日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once

#define LOG_DOS_CHANNEL					1301
#define LOG_DOS_OBJECT_STATE_CHANNEL	1302
#define THREAD_CPU_COUNT_TIME			(5000)
//#define DOS_PROXY_KEEP_ALIVE_TIME		(10000)
//#define DOS_PROXY_KEEP_ALIVE_MAX_COUNT	(20)

enum MSG_COMPRESS_TYPE
{
	MSG_COMPRESS_NONE,
	MSG_COMPRESS_LZO,
	MSG_COMPRESS_ZIP_FAST,
	MSG_COMPRESS_ZIP_NORMAL,
	MSG_COMPRESS_ZIP_SLOW,
};


struct DOS_CONFIG
{
	UINT										RouterID;	
	CEasyString									RouterLinkConfigFileName;	
	UINT										MaxRouterSendMsgQueue;
	UINT										RouterMsgProcessLimit;
	

	CIPAddress									ObjectProxyServiceListenAddress;
	UINT										MaxProxyConnection;	
	UINT										MaxProxyMsgQueue;
	UINT										MaxProxyConnectionMsgQueue;
	UINT										MaxProxyGlobalMsgMap;
	UINT										MaxProxyMsgMap;
	UINT										MaxProxyMsgSize;
	UINT										ProxySendBufferSize;
	UINT										ProxySendDelay;
	UINT										ProxySendQueryLimit;
	UINT										ProxyMsgCompressType;
	UINT										ProxyMsgMinCompressSize;
	UINT										ProxyUnacceptConnectionKeepTime;
	bool										ProxyUseServerInitiativeKeepAlive;
	UINT										ProxyKeepAliveTime;
	UINT										ProxyKeepAliveCount;

	UINT										MemoryPoolBlockSize;
	UINT										MemoryPoolLeveSize;
	UINT										MemoryPoolLevelCount;

	UINT										ObjectGroupCount;
	UINT										MaxGroupObjectCount;
	UINT										MaxObjectMsgQueue;	
	UINT										ObjectAliveTestTime;
	UINT										ObjectAliveCheckTime;
	UINT										ObjectKeepAliveCount;
	bool										StatObjectCPUCost;

	DOS_CONFIG()
	{
		RouterID=0;	
		MaxRouterSendMsgQueue=0;
		RouterMsgProcessLimit=0;
		MaxProxyConnection=0;	
		MaxProxyMsgQueue=0;
		MaxProxyConnectionMsgQueue=0;
		MaxProxyGlobalMsgMap=0;
		MaxProxyMsgMap=0;
		MaxProxyMsgSize=0;
		ProxySendBufferSize=0;
		ProxySendDelay=0;
		ProxySendQueryLimit=0;
		ProxyMsgCompressType=MSG_COMPRESS_NONE;
		ProxyMsgMinCompressSize=0;
		ProxyUnacceptConnectionKeepTime=30*1000;
		ProxyUseServerInitiativeKeepAlive=false;
		ProxyKeepAliveTime=30000;
		ProxyKeepAliveCount=10;
		MemoryPoolBlockSize=0;
		MemoryPoolLeveSize=0;
		MemoryPoolLevelCount=0;
		ObjectGroupCount=0;
		MaxGroupObjectCount=0;
		MaxObjectMsgQueue=0;
		ObjectAliveTestTime=20*1000;
		ObjectAliveCheckTime=1000;
		ObjectKeepAliveCount=5;
		StatObjectCPUCost=false;
	}
};

class CDOSBaseObject;

struct DOS_OBJECT_REGISTER_INFO
{
	OBJECT_ID			ObjectID;
	int					Weight;
	int					ObjectGroupIndex;
	UINT				MsgQueueSize;
	UINT				MsgProcessLimit;
	CDOSBaseObject *	pObject;
	UINT				Param;

	DOS_OBJECT_REGISTER_INFO()
	{
		ObjectID=0;
		Weight=1;
		ObjectGroupIndex=-1;
		MsgQueueSize=0;
		MsgProcessLimit=0;
		pObject=NULL;
		Param=0;
	}
};

extern UINT DistinctObjectID(OBJECT_ID * pObjectIDs,UINT Count);

inline BOOL PrintDOSLog(DWORD Color,LPCTSTR Format,...)
{
	va_list vl;
	va_start(vl,Format);
	BOOL ret=CLogManager::GetInstance()->PrintLogVL(LOG_DOS_CHANNEL,ILogPrinter::LOG_LEVEL_NORMAL,Color,Format,vl);
	va_end(vl);
	return ret;
}


inline BOOL PrintDOSDebugLog(DWORD Color,LPCTSTR Format,...)
{
	va_list vl;
	va_start(vl,Format);
	BOOL ret=CLogManager::GetInstance()->PrintLogVL(LOG_DOS_CHANNEL,ILogPrinter::LOG_LEVEL_DEBUG,Color,Format,vl);
	va_end(vl);
	return ret;
}

inline BOOL PrintDOSObjectStatLog(DWORD Color,LPCTSTR Format,...)
{
	va_list vl;
	va_start(vl,Format);
	BOOL ret=CLogManager::GetInstance()->PrintLogVL(LOG_DOS_OBJECT_STATE_CHANNEL,ILogPrinter::LOG_LEVEL_NORMAL,Color,Format,vl);
	va_end(vl);
	return ret;
}