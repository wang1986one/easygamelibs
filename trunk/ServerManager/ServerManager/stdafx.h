/****************************************************************************/
/*                                                                          */
/*      文件名:    stdafx.h                                                 */
/*      创建日期:  2010年02月09日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 项目特定的包含文件

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// 从 Windows 标头中排除不常使用的资料
#endif

// 如果您必须使用下列所指定的平台之前的平台，则修改下面的定义。
// 有关不同平台的相应值的最新信息，请参考 MSDN。
#ifndef WINVER				// 允许使用 Windows 95 和 Windows NT 4 或更高版本的特定功能。
#define WINVER 0x0500		//为 Windows98 和 Windows 2000 及更新版本改变为适当的值。
#endif

#ifndef _WIN32_WINNT		// 允许使用 Windows NT 4 或更高版本的特定功能。
#define _WIN32_WINNT 0x0500		//为 Windows98 和 Windows 2000 及更新版本改变为适当的值。
#endif

#ifndef _WIN32_WINDOWS		// 允许使用 Windows 98 或更高版本的特定功能。
#define _WIN32_WINDOWS 0x0500 //为 Windows Me 及更新版本改变为适当的值。
#endif

#ifdef WIN32

#ifndef _WIN32_IE			// 允许使用 IE 4.0 或更高版本的特定功能。
#define _WIN32_IE 0x0500	//为 IE 5.0 及更新版本改变为适当的值。
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// 某些 CString 构造函数将是显式的

// 关闭 MFC 对某些常见但经常被安全忽略的警告消息的隐藏
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 核心和标准组件
#include <afxext.h>         // MFC 扩展
#include <afxdisp.h>        // MFC 自动化类

#include <afxdtctl.h>		// Internet Explorer 4 公共控件的 MFC 支持
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// Windows 公共控件的 MFC 支持
#endif // _AFX_NO_AFXCMN_SUPPORT

#endif

#include "../../Libs/ServerFrameWork/ServerFrameWork.h"
#include "../../Libs/zlib/zlib.h"
#include "Psapi.h"
#include "Iphlpapi.h"
#pragma comment(lib,"Psapi.lib")
#pragma comment(lib,"Iphlpapi.lib")

#define MAX_SERVER_TERMINATE_WAIT_TIME	300000
#define CONFIG_FILE_NAME				"Config.XML"

#define WORK_THREAD_NUM					2
#define ASSEMBEL_BUFFER_SIZE			(MAX_DATA_PACKET_SIZE*2)
#define PACK_NEED_SIZE					4096
#define DOWNLOAD_CONNECT_TIME			30000


#include "Protocol.h"

struct SERVICE_INFO
{
	char		Name[MAX_PATH];
	char		ImageFilePath[MAX_PATH];
	char		WorkDir[MAX_PATH];
	char		StartupParam[MAX_PATH];
	DWORD		ProcessID;
	UINT64		RecentTime;
	BYTE		Status;
	float		CPUUsed;
	UINT		MemoryUsed;
	UINT		VirtualMemoryUsed;	
	CEasyTime	ImageFileTime;
	SERVICE_INFO()
	{
		Name[0]=0;
		ImageFilePath[0]=0;
		WorkDir[0]=0;
		StartupParam[0]=0;
		ProcessID=0;
		Status=SS_STOP;
		RecentTime=0;
		CPUUsed=0;
		MemoryUsed=0;
		VirtualMemoryUsed=0;
	}
};

struct FILE_INFO
{
	CEasyString			FileName;
	DWORD				FileAttr;
	ULONG64				FileSize;
	time_t				FileCreateTime;
	time_t				FileLastAccessTime;
	time_t				FileLastWriteTime;
};


struct NET_ADAPTER_INFO
{
	UINT	Index;
	DWORD	IPAddress;
	UINT64	SendBytes;
	float	SendFlux;
	UINT64	RecvBytes;
	float	RecvFlux;
};


#include "MainConfig.h"

#include "FileCache.h"

#include "ScriptExecutor.h"
#include "ServerManagerClient.h"
#include "ServerManagerService.h"



#include "MainThread.h"

#include "MainApp.h"

#define FUNCTION_BEGIN				//static int s_FunctionIndex=0;if(!s_FunctionIndex) s_FunctionIndex=CPerformanceStatistician::GetInstance()->GetFunctionIndex(); CAutoPerformanceCounter tmpPerformanceCounter(s_FunctionIndex,__FUNCTION__);
#define FUNCTION_END


