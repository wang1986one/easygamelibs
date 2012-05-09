/****************************************************************************/
/*                                                                          */
/*      文件名:    stdafx.h                                                 */
/*      创建日期:  2012年04月22日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 从 Windows 头中排除极少使用的资料
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的

// 关闭 MFC 对某些常见但经常可放心忽略的警告消息的隐藏
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展


#include <afxdisp.h>        // MFC 自动化类



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                     // MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT

#define CONFIG_FILE_NAME							"Config.xml"
#define TYPE_DEF_FILE_NAME							"TypeDef.xml"

#define PROTOCOL_FILE_TEMPLE_FILE_NAME				"ProtocolFile.tmpl"
#define INTERFACE_TEMPLE_FILE_NAME					"Interface.tmpl"

#define PARAM_TEMPLE_FILE_NAME						"Param.tmpl"
#define PARAM_DESCRIPTION_TEMPLE_FILE_NAME			"ParamDescript.tmpl"

#define METHOD_HEADER_TEMPLE_FILE_NAME				"MethodHeader.tmpl"
#define METHOD_HEADER_PURE_TAIL_TEMPLE_FILE_NAME	"MethodHeaderPureTail.tmpl"
#define METHOD_CALLER_SOURCE_TEMPLE_FILE_NAME		"MethodCallerSource.tmpl"
#define MSG_PACK_METHOD_HEADER_TEMPLE_FILE_NAME 	"MsgPackMethodHeader.tmpl"
#define MSG_PACK_METHOD_SOURCE_TEMPLE_FILE_NAME 	"MsgPackMethodSource.tmpl"
#define MSG_HANDLER_METHOD_HEADER_TEMPLE_FILE_NAME 	"MsgHandlerMethodHeader.tmpl"
#define MSG_HANDLER_METHOD_SOURCE_TEMPLE_FILE_NAME 	"MsgHandlerMethodSource.tmpl"

#define MSG_MAP_INIT_TEMPLE_FILE_NAME				"MsgMapInit.tmpl"
#define METHOD_SST_ID_ENUM_DEFINE_TEMPLE_FILE_NAME	"MethodSSTIDEnumDefine.tmpl"


#define MSG_CALLER_HEADER_TEMPLE_FILE_NAME			"MsgCallerHeader.tmpl"
#define MSG_CALLER_SOURCE_TEMPLE_FILE_NAME			"MsgCallerSource.tmpl"


#define MSG_HANDLER_HEADER_TEMPLE_FILE_NAME			"MsgHandlerHeader.tmpl"
#define MSG_HANDLER_SOURCE_TEMPLE_FILE_NAME			"MsgHandlerSource.tmpl"



#pragma warning (push)
#pragma warning (disable : 4267)	// type conversion
#pragma warning (disable : 4311)	// pointer truncation
#pragma warning (disable : 4244)	// type conversion

#include "PugXML.h"
using namespace pug;

#pragma warning (pop)

#include "resource.h"

#include <vector>
#include <fstream>

using namespace std;

struct PARAM_TYPE
{
	CString Name;
	CString CType;
	bool	IsFixedLength;
	CString	ConstructOperation;
	CString EvaluateOperation;
	bool	NeedNULLCheck;
	CString	NULLCheckOperation;
	CString	SizeCaculateOperation;
	CString	PackOperation;
	CString	UnpackOperation;	
	CString	ReferenceDefine;
};

struct METHOD_PARAM
{
	CString		Name;
	UINT		ID;
	int			Type;
	int			Length;
	CString		DefaultValue;
	bool		CanNull;
	bool		IsReference;
	CString		Description;
};

struct INTERFACE_METHOD
{
	CString					Name;
	vector<METHOD_PARAM>	ParamList;	
	CString					ID;
	UINT					ParamIDSeed;
	CString					Description;
};

struct EXPORT_CONFIG
{
	bool		IsExport;
	CString		ExportExt;
};

struct INTERFACE_CONFIG
{
	EXPORT_CONFIG		ProtocolFileExportConfig;
	EXPORT_CONFIG		InterfaceExportConfig;
	EXPORT_CONFIG		CallHeaderExportConfig;
	EXPORT_CONFIG		CallSourceExportConfig;
	EXPORT_CONFIG		HandlerHeaderExportConfig;
	EXPORT_CONFIG		HandlerSourceExportConfig;
	INTERFACE_CONFIG()
	{
		ProtocolFileExportConfig.IsExport=true;
		ProtocolFileExportConfig.ExportExt="h";
		InterfaceExportConfig.IsExport=true;
		InterfaceExportConfig.ExportExt="h";
		CallHeaderExportConfig.IsExport=true;
		CallHeaderExportConfig.ExportExt="h";
		CallSourceExportConfig.IsExport=true;
		CallSourceExportConfig.ExportExt="cpp";
		HandlerHeaderExportConfig.IsExport=true;
		HandlerHeaderExportConfig.ExportExt="h";
		HandlerSourceExportConfig.IsExport=true;
		HandlerSourceExportConfig.ExportExt="cpp";
	}
};

struct CALLER_INTERFACE
{
	CString						Name;
	vector<INTERFACE_METHOD>	MethodList;
	int							Type;
	CString						ID;
	CString						Description;
};

enum INTERFACE_TYPE
{
	INTERFACE_CALL,
	INTERFACE_CALL_BACK,
};

#include "DlgTypeEditor.h"
#include "DlgConfig.h"
#include "CallInterfaceMakerDlg.h"


#include "CallInterfaceMakerDlg.h"
#include "CallInterfaceMaker.h"



#include "DlgParamEditor.h"
#include "DlgMethodEditor.h"
#include "DlgInterfaceEdtor.h"


#include "DlgTextEditor.h"




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


