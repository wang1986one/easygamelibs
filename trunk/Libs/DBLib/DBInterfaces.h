/****************************************************************************/
/*                                                                          */
/*      文件名:    DBInterfaces.h                                           */
/*      创建日期:  2009年09月11日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once

//数据类型
#include "DBTypes.h"

namespace DBLib
{

class IDatabase;
class IDBConnection;
class IDBRecordSet;
class IDBParameterSet;
class CDBValue;

class IDatabase
	:public CNameObject
{
protected:
	DECLARE_CLASS_INFO_STATIC(IDatabase)
public:
	IDatabase();	
	virtual ~IDatabase();

	virtual IDBConnection * CreateConnection()=0;
	virtual int DeleteConnection(IDBConnection * pDBConnection)=0;	
};

class IDBConnection
	:public CNameObject
{
protected:
	DECLARE_CLASS_INFO_STATIC(IDBConnection)
public:
	IDBConnection();
	virtual ~IDBConnection();

	virtual IDBRecordSet * CreateRecordSet(int RecordSetType)=0;
	virtual IDBParameterSet * CreateParameterSet(int RecordSetType)=0;

	virtual int Connect(LPCTSTR ConnectStr)=0;
	virtual int Disconnect()=0;
	virtual BOOL IsConnected()=0;
	virtual int ExecuteSQL(LPCSTR SQLStr,int StrLen=0,IDBParameterSet * pParamSet=NULL)=0;	
	virtual int GetResults(IDBRecordSet * pDBRecordset)=0;
	virtual int NextResults(IDBRecordSet * pDBRecordset)=0;	
	virtual int GetAffectedRowCount()=0;
	virtual int EnableTransaction(BOOL IsEnable)=0;
	virtual int Commit()=0;
	virtual int RollBack()=0;	
	virtual UINT GetLastDatabaseErrorCode()=0;
	virtual LPCTSTR GetLastDatabaseErrorString()=0;
	virtual int TranslateString(LPCTSTR szSource,int SrcLen,LPTSTR szTarget,int MaxLen)=0;

};

class IDBRecordSet
	:public CNameObject
{
protected:
	DECLARE_CLASS_INFO_STATIC(IDBRecordSet)
public:
	IDBRecordSet();
	virtual ~IDBRecordSet();


	virtual int GetRecordCount()=0;
	virtual int GetColumnCount()=0;
	virtual LPCTSTR GetColumnName(int Index)=0;
	virtual int GetIndexByColumnName(LPCTSTR Name)=0;
	virtual DB_COLUMN_INFO * GetColumnInfo(int Index)=0;


	virtual CDBValue& GetField(int Index)=0;
	virtual CDBValue& GetField(LPCTSTR Name)=0;
	virtual int MoveFirst()=0;
	virtual int MoveLast()=0;
	virtual int MoveNext()=0;
	virtual int MovePrevious()=0;
	virtual int MoveTo(int Index)=0;	
	virtual bool IsEOF()=0;
	virtual bool IsBOF()=0;

	virtual bool Close()=0;

};

class IDBParameterSet
	:public CNameObject
{
protected:
	DECLARE_CLASS_INFO_STATIC(IDBParameterSet)
public:
	IDBParameterSet();
	virtual ~IDBParameterSet();


	virtual int GetCount()=0;	
	
	virtual DB_PARAM_INFO * GetParamInfo(int Index)=0;

	virtual CDBValue& GetParam(int Index)=0;	

	virtual bool AddParam(DB_PARAM_INFO& ParamInfo,CDBValue Value)=0;
	virtual bool AddParam(DB_PARAM_INFO& ParamInfo)=0;
	virtual bool SetParam(int Index,DB_PARAM_INFO& ParamInfo,CDBValue Value)=0;
	virtual bool SetParam(int Index,DB_PARAM_INFO& ParamInfo)=0;

};

}