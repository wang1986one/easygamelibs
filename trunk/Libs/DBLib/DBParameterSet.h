/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DBParameterSet.h                                         */
/*      ��������:  2009��07��06��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once

namespace DBLib
{


class CDBParameterSet :
	public IDBParameterSet
{
protected:	
	struct PARAM_DATA
	{
		DB_PARAM_INFO	ParamInfo;
		CDBValue		ParamValue;
	};

	vector<PARAM_DATA>			m_Params;
	CDBValue					m_EmptyValue;

	DECLARE_CLASS_INFO(CDBParameterSet)
public:
	CDBParameterSet(void);
	virtual ~CDBParameterSet(void);

	
	virtual void Destory();

	virtual int GetCount();
	
	virtual DB_PARAM_INFO * GetParamInfo(int Index);

	virtual CDBValue& GetParam(int Index);
	

	virtual bool AddParam(DB_PARAM_INFO& ParamInfo,CDBValue Value);
	virtual bool AddParam(DB_PARAM_INFO& ParamInfo);
	virtual bool SetParam(int Index,DB_PARAM_INFO& ParamInfo,CDBValue Value);
	virtual bool SetParam(int Index,DB_PARAM_INFO& ParamInfo);
	
};

}