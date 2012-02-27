#pragma once

#ifdef WIN32

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// 从 Windows 标头中排除不常使用的资料
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

#include "../Libs/ServerFrameWork/ServerFrameWork.h"
//#include "../Libs/Utils/Utils.h"
//#include "../Libs/NetLib/NetLib.h"

#define FUNCTION_BEGIN
#define FUNCTION_END

#define MAX_SERVER_TERMINATE_WAIT_TIME	30000
#define CONFIG_FILE_NAME				"Config.XML"

#define WORK_THREAD_NUM					2




#include <iostream>
#include <stdio.h>
using namespace std;

#include "TestConnection.h"
#include "TestService.h"


#include "MainThread.h"