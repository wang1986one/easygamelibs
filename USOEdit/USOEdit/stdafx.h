/****************************************************************************/
/*                                                                          */
/*      文件名:    stdafx.h                                                 */
/*      创建日期:  2011年10月07日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
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

#ifndef _WIN32_IE			// 允许使用 IE 4.0 或更高版本的特定功能。
#define _WIN32_IE 0x0500	//为 IE 5.0 及更新版本改变为适当的值。
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// 某些 CString 构造函数将是显式的

// 关闭 MFC 对某些常见但经常被安全忽略的警告消息的隐藏
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 核心和标准组件
#include <afxext.h>         // MFC 扩展
#include <afxcview.h>
#include <afxdisp.h>        // MFC 自动化类

#include <afxdtctl.h>		// Internet Explorer 4 公共控件的 MFC 支持
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// Windows 公共控件的 MFC 支持
#endif // _AFX_NO_AFXCMN_SUPPORT

#define USE_CRT_DETAIL_NEW

#include "..\..\Libs\Utils\Utils.h"
#include "..\..\Libs\D3DLib\D3DLib.h"
#include "..\..\Libs\D3DGUI\D3DGUIHeaders.h"
#include "..\..\Libs\D3DLib\D3DGUIObjectCreator.h"
#include "..\..\Libs\MFCExtensionLib\MFCExtensionLib.h"

#include "..\..\Libs\StormLib\src\StormLib.h"

#include "resource.h"

#define SETTING_FILE_NAME	_T("USOEditSetting.xml")
#define FRAME_RATE_STAT_TIME	2000

#define USO_OBJECT_SEC	0x20000000



enum USO_OBJECT_TYPE
{
	USO_OBJECT_SUBMESH_AGENT=USO_OBJECT_SEC|0x01,
};

enum CAMERA_MODE
{
	CAMERA_MODE_SCENE,
	CAMERA_MODE_OBJECT,
};

enum CAMERA_PROJECT_MODE
{
	CAMERA_PROJECT_MODE_PERSPECTIVE,
	CAMERA_PROJECT_MODE_ORTHO,
};

struct CAMERA_SETTING
{
	int			CameraMode;
	int			CameraProjectMode;
	FLOAT		CameraNearFace;
	FLOAT		CameraFarFace;
	FLOAT		CameraMoveStep;
	FLOAT		CameraRotateStep;
};

struct VIEWPORT_SETTING
{
	int			ViewportWidth;
	int			ViewportHeight;
};


#include "MPQFileAccessor.h"
#include "MPQFileSystem.h"

#include "MemFileAccessor.h"

#include "USOObject.h"
#include "TextureViewRect.h"

#include "DlgPathListSetting.h"
#include "DlgRenderPerformance.h"
#include "DlgCameraSetting.h"
#include "DlgLightSetting.h"

#include "DlgMapView.h"
#include "DlgImport.h"
#include "DlgWOWM2Import.h"
#include "DlgWOWCharImport.h"
#include "DlgWOWCreatureImport.h"
#include "DlgWOWItemImport.h"
#include "DlgWOWWMOImport.h"
#include "DlgWOWADTImport.h"


#include "DlgColorConfig.h"
#include "DlgFXEditor.h"
#include "DlgFXSelect.h"
#include "DlgWOWCharEquipmentSetting.h"
#include "DlgWOWCharSetting.h"
#include "DlgTextureViewer.h"
#include "DlgFileViewer.h"
#include "DlgAniSetting.h"
#include "DlgTrianglesViewer.h"
#include "DlgAniFrameViewer.h"
#include "DlgBoneViewer.h"
#include "DlgAniSequenceViewer.h"
#include "DlgParticleEmitterParamViewer.h"
#include "DlgSenceTreeView.h"
#include "DlgHeightTest.h"
#include "DlgM2CameraViewer.h"
#include "DlgAttachmentViewer.h"
#include "DlgM2SkinView.h"

#include "D3DViewPort.h"
#include "D3DView.h"
#include "USOTreeView.h"
#include "USOPropertyView.h"
#include "ControlPanelView.h"

#include "USORightView.h"
#include "USOLeftView.h"
#include "D3DView.h"

#include "USOEditDoc.h"

#include "ChildFrm.h"
#include "MainFrm.h"

#include "USOEdit.h"


#define FUNCTION_BEGIN				//{static int s_FunctionIndex=0;if(!s_FunctionIndex) s_FunctionIndex=CPerformanceStatistician::GetInstance()->GetFunctionIndex(); CAutoPerformanceCounter tmpPerformanceCounter(s_FunctionIndex,__FUNCTION__);
#define FUNCTION_END				//}