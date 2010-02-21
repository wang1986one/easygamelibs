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

bool CDBParameterSet::AddParam(DB_PARAM_INFO& ParamInfo,CDBValue Value)
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

bool CDBParameterSet::SetParam(int Index,DB_PARAM_INFO& ParamInfo,CDBValue Value)
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