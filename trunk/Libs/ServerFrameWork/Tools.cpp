/****************************************************************************/
/*                                                                          */
/*      �ļ���:    Tools.cpp                                                */
/*      ��������:  2009��07��06��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#include "StdAfx.h"


WORD g_ProgramVersion[4]={0,0,0,0};
#ifdef WIN32
int g_ServiceWorkMode=SWM_SERVICE;
#else
int g_ServiceWorkMode=SWM_APPLICATION;
#endif