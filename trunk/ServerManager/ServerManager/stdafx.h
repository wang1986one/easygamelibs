/****************************************************************************/
/*                                                                          */
/*      �ļ���:    stdafx.h                                                 */
/*      ��������:  2010��02��09��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// ��Ŀ�ض��İ����ļ�

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// �� Windows ��ͷ���ų�����ʹ�õ�����
#endif

// ���������ʹ��������ָ����ƽ̨֮ǰ��ƽ̨�����޸�����Ķ��塣
// �йز�ͬƽ̨����Ӧֵ��������Ϣ����ο� MSDN��
#ifndef WINVER				// ����ʹ�� Windows 95 �� Windows NT 4 ����߰汾���ض����ܡ�
#define WINVER 0x0500		//Ϊ Windows98 �� Windows 2000 �����°汾�ı�Ϊ�ʵ���ֵ��
#endif

#ifndef _WIN32_WINNT		// ����ʹ�� Windows NT 4 ����߰汾���ض����ܡ�
#define _WIN32_WINNT 0x0500		//Ϊ Windows98 �� Windows 2000 �����°汾�ı�Ϊ�ʵ���ֵ��
#endif

#ifndef _WIN32_WINDOWS		// ����ʹ�� Windows 98 ����߰汾���ض����ܡ�
#define _WIN32_WINDOWS 0x0500 //Ϊ Windows Me �����°汾�ı�Ϊ�ʵ���ֵ��
#endif

#ifdef WIN32

#ifndef _WIN32_IE			// ����ʹ�� IE 4.0 ����߰汾���ض����ܡ�
#define _WIN32_IE 0x0500	//Ϊ IE 5.0 �����°汾�ı�Ϊ�ʵ���ֵ��
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// ĳЩ CString ���캯��������ʽ��

// �ر� MFC ��ĳЩ��������������ȫ���Եľ�����Ϣ������
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC ���ĺͱ�׼���
#include <afxext.h>         // MFC ��չ
#include <afxdisp.h>        // MFC �Զ�����

#include <afxdtctl.h>		// Internet Explorer 4 �����ؼ��� MFC ֧��
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// Windows �����ؼ��� MFC ֧��
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


