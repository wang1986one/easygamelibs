/****************************************************************************/
/*                                                                          */
/*      �ļ���:    ESVariableList.h		                                    */
/*      ��������:  2010��08��26��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once

struct ES_VARIABLE
{
public:
	UINT		ID;
	CEasyString	Name;
	VALUE_TYPE	Type;
	CEasyString	StrValue;	
	union
	{
		int			ValueInt;
		INT64		ValueInt64;
		float		ValueFloat;
		double		ValueDouble;
	};	
public:
	ES_VARIABLE()
	{
		ID=0;
		ValueInt64=0;
		StrValue.Clear();
		Type=VALUE_TYPE_INT;
	}
	ES_VARIABLE(ES_VARIABLE& Var)
	{
		ID=Var.ID;
		Name=Var.Name;
		ValueInt64=Var.ValueInt64;
		StrValue=Var.StrValue;
		Type=Var.Type;
	}	
	bool operator==(const ES_VARIABLE& Var) const
	{
		if(Name.CompareNoCase(Var.Name)) 
			return false; 
		else 
			return true;
	}
};

class CESVariableList
{
protected:
	UINT								m_IDStart;
	CStaticMap<CEasyString,ES_VARIABLE>	m_Map;
public:
	CESVariableList()
	{
		m_IDStart=0;
	}
	~CESVariableList()
	{

	}
	BOOL Create(UINT Size)
	{
		return m_Map.Create(Size);
	}
	void SetIDStart(UINT IDStart)
	{
		m_IDStart=IDStart;
	}
	void Clear()
	{
		m_Map.Clear();
	}	
	ES_VARIABLE * AddEmptyVariable(LPCTSTR VarName,VALUE_TYPE Type)
	{
		CEasyString Key(VarName);
		Key.MakeUpper();
		ES_VARIABLE * pVar=NULL;
		UINT ID=m_Map.New(Key,&pVar);
		if(pVar)
		{
			pVar->ID=ID+m_IDStart;
			pVar->Name=VarName;
			pVar->Type=Type;
			pVar->ValueInt64=0;
			pVar->StrValue.Clear();
		}
		return pVar;
	}
	ES_VARIABLE * AddVariable(LPCTSTR VarName,int Value)
	{
		CEasyString Key(VarName);
		Key.MakeUpper();
		ES_VARIABLE * pVar=NULL;
		UINT ID=m_Map.New(Key,&pVar);
		if(pVar)
		{
			pVar->ID=ID+m_IDStart;
			pVar->Name=VarName;
			pVar->Type=VALUE_TYPE_INT;
			pVar->ValueInt=Value;
		}
		return pVar;
	}
	ES_VARIABLE * AddVariable(LPCTSTR VarName,INT64 Value)
	{
		CEasyString Key(VarName);
		Key.MakeUpper();
		ES_VARIABLE * pVar=NULL;
		UINT ID=m_Map.New(Key,&pVar);
		if(pVar)
		{
			pVar->ID=ID+m_IDStart;
			pVar->Name=VarName;
			pVar->Type=VALUE_TYPE_INT64;
			pVar->ValueInt64=Value;
		}
		return pVar;
	}
	ES_VARIABLE * AddVariable(LPCTSTR VarName,float Value)
	{
		CEasyString Key(VarName);
		Key.MakeUpper();
		ES_VARIABLE * pVar=NULL;
		UINT ID=m_Map.New(Key,&pVar);
		if(pVar)
		{
			pVar->ID=ID+m_IDStart;
			pVar->Name=VarName;
			pVar->Type=VALUE_TYPE_FLOAT;
			pVar->ValueFloat=Value;
		}
		return pVar;
	}
	ES_VARIABLE * AddVariable(LPCTSTR VarName,double Value)
	{
		CEasyString Key(VarName);
		Key.MakeUpper();
		ES_VARIABLE * pVar=NULL;
		UINT ID=m_Map.New(Key,&pVar);
		if(pVar)
		{
			pVar->ID=ID+m_IDStart;
			pVar->Name=VarName;
			pVar->Type=VALUE_TYPE_DOUBLE;
			pVar->ValueDouble=Value;
		}
		return pVar;
	}
	ES_VARIABLE * AddVariable(LPCTSTR VarName,LPCTSTR Value)
	{
		CEasyString Key(VarName);
		Key.MakeUpper();
		ES_VARIABLE * pVar=NULL;
		UINT ID=m_Map.New(Key,&pVar);
		if(pVar)
		{
			pVar->ID=ID+m_IDStart;
			pVar->Name=VarName;
			pVar->Type=VALUE_TYPE_STRING;
			pVar->StrValue=Value;
		}
		return pVar;
	}
	bool EditVariable(LPCTSTR VarName, int Value)
	{
		CEasyString Key(VarName);
		Key.MakeUpper();
		ES_VARIABLE * pVar=m_Map.Find(Key);
		if(pVar)
		{
			pVar->ValueInt=Value;
			return true;
		}
		return false;
	}
	bool EditVariable(LPCTSTR VarName, INT64 Value)
	{
		CEasyString Key(VarName);
		Key.MakeUpper();
		ES_VARIABLE * pVar=m_Map.Find(Key);
		if(pVar)
		{
			pVar->ValueInt64=Value;
			return true;
		}
		return false;
	}
	bool EditVariable(LPCTSTR VarName, float Value)
	{
		CEasyString Key(VarName);
		Key.MakeUpper();
		ES_VARIABLE * pVar=m_Map.Find(Key);
		if(pVar)
		{
			pVar->ValueFloat=Value;
			return true;
		}
		return false;
	}
	bool EditVariable(LPCTSTR VarName, double Value)
	{
		CEasyString Key(VarName);
		Key.MakeUpper();
		ES_VARIABLE * pVar=m_Map.Find(Key);
		if(pVar)
		{
			pVar->ValueDouble=Value;
			return true;
		}
		return false;
	}
	bool EditVariable(LPCTSTR VarName, LPCTSTR Value)
	{
		CEasyString Key(VarName);
		Key.MakeUpper();
		ES_VARIABLE * pVar=m_Map.Find(Key);
		if(pVar)
		{
			pVar->StrValue=Value;
			return true;
		}
		return false;
	}
	ES_VARIABLE * FindVariable(UINT VarID)
	{
		return m_Map.GetObject(VarID-m_IDStart);
	}
	ES_VARIABLE * FindVariable(LPCTSTR VarName)
	{
		CEasyString Key(VarName);
		Key.MakeUpper();
		return m_Map.Find(Key);
	}
	BOOL DelVariable(LPCTSTR VarName)
	{
		CEasyString Key(VarName);
		Key.MakeUpper();
		return m_Map.Delete(Key);
	}
	BOOL DelVariable(UINT VarID)
	{
		return m_Map.DeleteByID(VarID-m_IDStart);
	}
};