#include "StdAfx.h"

IMPLEMENT_CLASS_INFO_STATIC(CBaseTCPConnection,CNameObject);
CBaseTCPConnection::CBaseTCPConnection(void)
{
}

CBaseTCPConnection::~CBaseTCPConnection(void)
{
}

UINT CBaseTCPConnection::GetCurSendQueryCount()
{
	return 0;
}