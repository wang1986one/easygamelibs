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
// ���ǳ��õ��������ĵ���Ŀ�ض��İ����ļ�
//

#pragma once


#define WIN32_LEAN_AND_MEAN		// �� Windows ͷ���ų�����ʹ�õ�����
//#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// ĳЩ CEasyString ���캯����Ϊ��ʽ��

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// �� Windows ͷ���ų�����ʹ�õ�����
#endif

//#include "windows.h"
//#include "afxwin.h"         // MFC ��������ͱ�׼���

#define USE_CRT_DETAIL_NEW
// TODO: �ڴ˴����ó���Ҫ��ĸ���ͷ�ļ�
//#define D3D_DEBUG_INFO

#include "..\Utils\Utils.h"
#include "..\D3DGUI\D3DGUIHeaders.h"
#include "D3DLib.h"


#define FUNCTION_BEGIN				//{static int s_FunctionIndex=0;if(!s_FunctionIndex) s_FunctionIndex=CPerformanceStatistician::GetInstance()->GetFunctionIndex(s_FunctionIndex,__FUNCTION__); CAutoPerformanceCounter tmpPerformanceCounter(s_FunctionIndex);
#define FUNCTION_END				//}


#define BLOCK_BEGIN(BlockName)		//{static int s_FunctionIndex=0;if(!s_FunctionIndex) s_FunctionIndex=CPerformanceStatistician::GetInstance()->GetFunctionIndex(s_FunctionIndex,BlockName); CAutoPerformanceCounter tmpPerformanceCounter(s_FunctionIndex);
#define BLOCK_END					//}