/****************************************************************************/
/*                                                                          */
/*      �ļ���:    stdafx.h                                                 */
/*      ��������:  2009��07��06��                                           */
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
//#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// ĳЩ CString ���캯����Ϊ��ʽ��

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// �� Windows ͷ���ų�����ʹ�õ�����
#endif

#include "afx.h"
//#include "afxwin.h"         // MFC ��������ͱ�׼���

//#define USE_CRT_DETAIL_NEW
// TODO: �ڴ˴����ó���Ҫ��ĸ���ͷ�ļ�
#include "..\Utils\Utils.h"
#include "..\D3DLib\D3DLib.h"

#include "DShowLib.h"

#define USE_CRT_DETAIL_NEW