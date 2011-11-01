/****************************************************************************/
/*                                                                          */
/*      文件名:    DBParameterSet.cpp                                       */
/*      创建日期:  2009年07月06日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#include "StdAfx.h"


namespace DBLib
{

IMPLEMENT_CLASS_INFO(CDBParameterSet,IDBParameterSet);

CDBParameterSet::CDBParameterSet(void)
{
	
}

CDBParameterSet::~CDBParameterSet(void)
{
}

void CDBParameterSet::Destory()
{
	m_Params.clear();
}

int CDBParameterSet::GetCount()
{
	return (int)m_Params.size();
}


DB_PARAM_INFO * CDBParameterSet::GetParamInfo(int Index)
{
	if((UINT)Index<m_Params.size())
	{
		return &(m_Params[Index].ParamInfo);
	}
	return NULL;
}

CDBValue& CDBParameterSet::GetParam(int Index)
{
	if((UINT)Index<m_Params.size())
	{
		return m_Params[Index].ParamValue;
	}
	return m_EmptyValue;
}

bool CDBParameterSet::AddParam(DB_PARAM_INFO& ParamInfo,const CDBValue& Value)
{
	m_Params.resize(m_Params.size()+1);
	m_Params.back().ParamInfo=ParamInfo;
	m_Params.back().ParamValue=Value;
	return true;
}

bool CDBParameterSet::AddParam(DB_PARAM_INFO& ParamInfo)
{
	m_Params.resize(m_Params.size()+1);
	m_Params.back().ParamInfo=ParamInfo;
	m_Params.back().ParamValue.SetNULLValue(ParamInfo.Type);
	return true;
}

bool CDBParameterSet::SetParam(int Index,DB_PARAM_INFO& ParamInfo,const CDBValue& Value)
{
	if((UINT)Index<m_Params.size())
	{
		m_Params[Index].ParamInfo=ParamInfo;
		m_Params[Index].ParamValue=Value;
		return true;
	}
	return false;
}

bool CDBParameterSet::SetParam(int Index,DB_PARAM_INFO& ParamInfo)
{
	if((UINT)Index<m_Params.size())
	{
		m_Params[Index].ParamInfo=ParamInfo;
		m_Params.back().ParamValue.SetNULLValue(ParamInfo.Type);
		return true;
	}
	return false;
}


}