/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DBTransaction.cpp                                        */
/*      ��������:  2009��07��06��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
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
