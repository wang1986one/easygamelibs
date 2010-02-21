#include "StdAfx.h"

IMPLEMENT_CLASS_INFO(CUSOObjectCreateFilter,CNameObject);

CUSOObjectCreateFilter::CUSOObjectCreateFilter(void)
{
}

CUSOObjectCreateFilter::~CUSOObjectCreateFilter(void)
{
}

bool CUSOObjectCreateFilter::OnObjectCreate(CNameObject * pObject)
{
	return true;
}

CNameObject * CUSOObjectCreateFilter::FindObject(CLASS_INFO * pClassInfo,LPCTSTR ObjectName)
{
	return NULL;
}
