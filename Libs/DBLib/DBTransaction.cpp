#include "StdAfx.h"


namespace DBLib
{

IMPLEMENT_CLASS_INFO_STATIC(CDBTransaction,CNameObject);
CDBTransaction::CDBTransaction(void)
{
	m_IsSucceed=false;
}

CDBTransaction::~CDBTransaction(void)
{
}

bool CDBTransaction::OnExecute(IDBConnection * pConnection)
{
	return false;
}

void CDBTransaction::OnFinish()
{
}

}
