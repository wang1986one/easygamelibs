#pragma once

#ifdef WIN32

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// �� Windows ��ͷ���ų�����ʹ�õ�����
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