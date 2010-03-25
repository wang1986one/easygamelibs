/****************************************************************************/
/*                                                                          */
/*      �ļ���:    MySQLDatabase.h                                          */
/*      ��������:  2009��07��06��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once
#include "dbinterfaces.h"


//#pragma comment(lib,"mysqlclient.lib")

	



namespace DBLib
{

enum DB_MYSQL_ERROR_INFO
{
	DBERR_MYSQL_MYSQLINITFAIL=DBERR_MAX,
	DBERR_MYSQL_CONNECTFAIL,
	DBERR_MYSQL_WANTCONNECT,
	DBERR_MYSQL_COMMITFAIL,
	DBERR_MYSQL_ROLLBACKFAIL,
	DBERR_MYSQL_ENABLETRANSACTIONFAIL,
	DBERR_MYSQL_MAX,
};


class CMySQLDatabase :
	public IDatabase
{
protected:	

	DECLARE_CLASS_INFO(CMySQLDatabase)
public:
	CMySQLDatabase(void);
	virtual ~CMySQLDatabase(void);	

	virtual IDBConnection * CreateConnection();
	virtual int DeleteConnection(IDBConnection * pDBConnection);	
	
};



}