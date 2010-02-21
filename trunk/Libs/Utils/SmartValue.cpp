#include "StdAfx.h"



int CSmartValue::GetTypeFromData(LPCVOID pData,UINT DataSize)
{
	if(pData==NULL||DataSize<sizeof(BYTE))
		return VT_UNKNOWN;

	return *((BYTE *)pData);
}
