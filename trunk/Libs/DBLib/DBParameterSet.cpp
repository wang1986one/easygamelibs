/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DBParameterSet.cpp                                       */
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

IMPLEMENT_CLASS_INFO(CDBParameterSet,IDBParameterSet);

CDBParameterSet::CDBParameterSet(void)
{
	
}

CDBParameterSet::~CDBParameterSet(void)
{
}

void CDBParameterSet::Destory()
{
	m_Params.Clear();
}

int CDBParameterSet::GetCount()
{
	return (int)m_Params.GetCount();
}


DB_PARAM_INFO * CDBParameterSet::GetParamInfo(int Index)
{
	if((UINT)Index<m_Params.GetCount())
	{
		return &(m_Params[Index].ParamInfo);
	}
	return NULL;
}

CDBValue& CDBParameterSet::GetParam(int Index)
{
	if((UINT)Index<m_Params.GetCount())
	{
		return m_Params[Index].ParamValue;
	}
	return m_EmptyValue;
}

bool CDBParameterSet::AddParam(DB_PARAM_INFO& ParamInfo,const CDBValue& Value)
{
	m_Params.Resize(m_Params.GetCount()+1);
	m_Params[m_Params.GetCount()-1].ParamInfo=ParamInfo;
	m_Params[m_Params.GetCount()-1].ParamValue=Value;
	return true;
}

bool CDBParameterSet::AddParam(DB_PARAM_INFO& ParamInfo)
{
	m_Params.Resize(m_Params.GetCount()+1);
	m_Params[m_Params.GetCount()-1].ParamInfo=ParamInfo;
	m_Params[m_Params.GetCount()-1].ParamValue.SetNULLValue(ParamInfo.Type);
	return true;
}

bool CDBParameterSet::SetParam(int Index,DB_PARAM_INFO& ParamInfo,const CDBValue& Value)
{
	if((UINT)Index<m_Params.GetCount())
	{
		m_Params[Index].ParamInfo=ParamInfo;
		m_Params[Index].ParamValue=Value;
		return true;
	}
	return false;
}

bool CDBParameterSet::SetParam(int Index,DB_PARAM_INFO& ParamInfo)
{
	if((UINT)Index<m_Params.GetCount())
	{
		m_Params[Index].ParamInfo=ParamInfo;
		m_Params[Index].ParamValue.SetNULLValue(ParamInfo.Type);
		return true;
	}
	return false;
}


}