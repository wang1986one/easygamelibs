#include "StdAfx.h"
#include ".\odbcdatabase.h"

namespace DBLib
{

IMPLEMENT_CLASS_INFO(CODBCDatabase,IDatabase);

CODBCDatabase::CODBCDatabase(void):IDatabase()
{
	m_hEnv=NULL;	
}

CODBCDatabase::~CODBCDatabase(void)
{
	if ( m_hEnv )
	{
		SQLFreeHandle( SQL_HANDLE_ENV, m_hEnv );
		m_hEnv = NULL;		
	}	
}

int CODBCDatabase::Init()
{
	if ( SQLAllocHandle( SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_hEnv ) != SQL_SUCCESS )
		return DBERR_SQLALLOCHANDLEFAIL;

	if ( SQLSetEnvAttr( m_hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER) SQL_OV_ODBC3, SQL_IS_INTEGER ) != SQL_SUCCESS )
		return DBERR_ODBC_SQLSETENVATTRFAIL;
	return DBERR_SUCCEED;
}

IDBConnection * CODBCDatabase::CreateConnection()
{
	CODBCConnection * pConnection=new CODBCConnection();	
	if(pConnection->Init(this)==DBERR_SUCCEED)
		return pConnection;
	pConnection->Release();	
	return NULL;
}

int CODBCDatabase::DeleteConnection(IDBConnection * pDBConnection)
{
	if(pDBConnection)
	{
		pDBConnection->Release();
		return DBERR_SUCCEED;
	}
	return DBERR_INVALID_CONNECTION;
}


}