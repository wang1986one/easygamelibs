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
// �ض�����Ŀ�İ����ļ�

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // �� Windows ͷ���ų�����ʹ�õ�����
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // ĳЩ CString ���캯��������ʽ��

// �ر� MFC ��ĳЩ�����������ɷ��ĺ��Եľ�����Ϣ������
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ


#include <afxdisp.h>        // MFC �Զ�����



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC �� Internet Explorer 4 �����ؼ���֧��
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                     // MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <vector>
#include <fstream>
using namespace std;

#define MAX_LOG_MSG_LEN				10240
#define CONNECT_WAIT_TIME			5000
#define CONNECT_TIME				60000
#define SERVICE_INFO_FETCH_TIME		5000
#define MIN_SERVER_DIR_REFRESH_TIME	3000
#define SETTING_FILE_NAME			"./Setting.xml"


#include "../../Libs/Utils/Utils.h"
#include "../../Libs/NetLib/NetLib.h"
#include "../../Libs/zlib/zlib.h"
#include "../ServerManager/Protocol.h"


struct SERVER_INFO
{
	CEasyString			ServerAddress;
	UINT				ServerPort;
	int					ServerStatus;
};

enum SERVER_CONNECTION_STATUS
{
	SCS_NO_CONNECTION,
	SCS_DISCONNECTED,
	SCS_CONNECTING,
};

struct TRANSFER_FILE_INFO
{
	CEasyString	SourceFilePath;
	CEasyString	TargetFilePath;
};



#include "DialogItemSorter.h"
#include "ServerConnection.h"

#include "Resource.h"

#include "DlgInput.h"
#include "DlgEditBox.h"

#include "DlgTrannferProgress.h"
#include "DlgServerManage.h"
#include "DlgWorkDirBowser.h"


#include "ServerManagerClientView.h"
#include "ServerManagerClientDoc.h"
#include "MainFrm.h"

#include "ServerManagerClient.h"





#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


