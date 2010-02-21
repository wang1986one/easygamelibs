// stdafx.h : 标准系统包含文件的包含文件，
// 或是常用但不常更改的项目特定的包含文件
//

#pragma once
#ifdef WIN32


#define WIN32_LEAN_AND_MEAN		// 从 Windows 头中排除极少使用的资料

//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>

#define USE_CRT_DETAIL_NEW

#endif

// TODO: 在此处引用程序要求的附加头文件


#include "../Utils/Utils.h"
#include "NetLib.h"


#define FUNCTION_BEGIN				//static int s_FunctionIndex=0;if(!s_FunctionIndex) s_FunctionIndex=CPerformanceStatistician::GetInstance()->GetFunctionIndex(); CAutoPerformanceCounter tmpPerformanceCounter(s_FunctionIndex,__FUNCTION__);
#define FUNCTION_END