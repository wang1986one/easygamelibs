/****************************************************************************/
/*                                                                          */
/*      文件名:    DOSDefines.cpp                                           */
/*      创建日期:  2009年07月06日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#include "stdafx.h"

int OBJECT_ID::Compare(const void * Value1,const void * Value2)
{
	if(((OBJECT_ID *)Value1)->ID==((OBJECT_ID *)Value2)->ID)
		return 0;
	else if(((OBJECT_ID *)Value1)->ID<((OBJECT_ID *)Value2)->ID)
		return -1;
	else
		return 1;
}