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
#include <vector>
#include <map>

#define LOG_DOS_CHANNEL				1301
#define THREAD_CPU_COUNT_TIME		(5000)

typedef WORD	ROUTE_ID_TYPE;

struct OBJECT_ID
{
	union
	{
		UINT64	ID;
		struct
		{
			WORD ObjectIndex;
			WORD GroupIndex;
			WORD ObjectTypeID;
			WORD RouterID;
		};
	};
	OBJECT_ID()
	{
		ID=0;
	}	
	OBJECT_ID(unsigned __int64 Value)
	{
		ID=Value;
	}
	bool operator==(const OBJECT_ID& ObjectID)
	{
		return ID==ObjectID.ID;
	}
	bool operator!=(const OBJECT_ID& ObjectID)
	{
		return ID!=ObjectID.ID;
	}
	static int Compare(const void * Value1,const void * Value2);
};

inline int OBJECT_ID::Compare(const void * Value1,const void * Value2)
{
	if(((OBJECT_ID *)Value1)->ID==((OBJECT_ID *)Value2)->ID)
		return 0;
	else if(((OBJECT_ID *)Value1)->ID<((OBJECT_ID *)Value2)->ID)
		return -1;
	else
		return 1;
}

#define BROAD_CAST_ROUTER_ID		(0xFFFF)
#define BROAD_CAST_OBJECT_TYPE_ID	(0xFFFF)
#define BROAD_CAST_GROUP_INDEX		(0xFFFF)
#define BROAD_CAST_OBJECT_INDEX		(0xFFFF)

enum DOS_OBJECT_TYPE
{
	DOT_UNKNOW,
	DOT_PROXY_OBJECT,
	//DOT_MAIN_CONTROL_OBJECT,
	//DOT_MAIN_NORMAL_OBJECT,
	DOT_NORMAL_OBJECT,
	DOT_MAX,
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

	UINT										MemoryPoolBlockSize;
	UINT										MemoryPoolLeveSize;
	UINT										MemoryPoolLevelCount;

	UINT										ObjectGroupCount;
	UINT										MaxGroupObjectCount;
	UINT										MaxObjectMsgQueue;

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
		MemoryPoolBlockSize=0;
		MemoryPoolLeveSize=0;
		MemoryPoolLevelCount=0;
		ObjectGroupCount=0;
		MaxGroupObjectCount=0;
		MaxObjectMsgQueue=0;		
	}
};

class CDOSBaseObject;

struct DOS_OBJECT_REGISTER_INFO
{
	OBJECT_ID			ObjectID;
	int					Weight;
	UINT				MsgQueueSize;
	UINT				MsgProcessLimit;
	CDOSBaseObject *	pObject;
	UINT				Param;

	DOS_OBJECT_REGISTER_INFO()
	{
		ObjectID=0;
		Weight=1;
		MsgQueueSize=0;
		MsgProcessLimit=0;
		pObject=NULL;
		Param=0;
	}
};

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