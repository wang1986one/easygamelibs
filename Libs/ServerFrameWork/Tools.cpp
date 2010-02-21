#include "StdAfx.h"


WORD g_ProgramVersion[4]={0,0,0,0};
#ifdef WIN32
int g_ServiceWorkMode=SWM_SERVICE;
#else
int g_ServiceWorkMode=SWM_APPLICATION;
#endif