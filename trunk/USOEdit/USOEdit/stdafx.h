/****************************************************************************/
/*                                                                          */
/*      �ļ���:    stdafx.h                                                 */
/*      ��������:  2011��10��07��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
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

#ifndef _WIN32_IE			// ����ʹ�� IE 4.0 ����߰汾���ض����ܡ�
#define _WIN32_IE 0x0500	//Ϊ IE 5.0 �����°汾�ı�Ϊ�ʵ���ֵ��
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// ĳЩ CString ���캯��������ʽ��

// �ر� MFC ��ĳЩ��������������ȫ���Եľ�����Ϣ������
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC ���ĺͱ�׼���
#include <afxext.h>         // MFC ��չ
#include <afxcview.h>
#include <afxdisp.h>        // MFC �Զ�����

#include <afxdtctl.h>		// Internet Explorer 4 �����ؼ��� MFC ֧��
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// Windows �����ؼ��� MFC ֧��
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