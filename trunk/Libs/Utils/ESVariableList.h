/****************************************************************************/
/*                                                                          */
/*      文件名:    ESVariableList.h		                                    */
/*      创建日期:  2010年08月26日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
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
	CStaticMap<CEasyString,ES_VARIABLE>	m_Map;
public:
	CESVariableList()
	{

	}
	~CESVariableList()
	{

	}
	BOOL Create(UINT Size)
	{
		return m_Map.Create(Size);
	}
	UINT AddVariable(LPCTSTR VarName,int Value)
	{
		CEasyString Key(VarName);
		Key.MakeUpper();
		ES_VARIABLE * pVar=NULL;
		UINT ID=m_Map.New(Key,&pVar);
		if(pVar)
		{
			pVar->ID=ID;
			pVar->Name=VarName;
			pVar->Type=VALUE_TYPE_INT;
			pVar->ValueInt=Value;
		}
		return 0;
	}
	UINT AddVariable(LPCTSTR VarName,INT64 Value)
	{
		CEasyString Key(VarName);
		Key.MakeUpper();
		ES_VARIABLE * pVar=NULL;
		UINT ID=m_Map.New(Key,&pVar);
		if(pVar)
		{
			pVar->ID=ID;
			pVar->Name=VarName;
			pVar->Type=VALUE_TYPE_INT64;
			pVar->ValueInt64=Value;
		}
		return 0;
	}
	UINT AddVariable(LPCTSTR VarName,float Value)
	{
		CEasyString Key(VarName);
		Key.MakeUpper();
		ES_VARIABLE * pVar=NULL;
		UINT ID=m_Map.New(Key,&pVar);
		if(pVar)
		{
			pVar->ID=ID;
			pVar->Name=VarName;
			pVar->Type=VALUE_TYPE_FLOAT;
			pVar->ValueFloat=Value;
		}
		return 0;
	}
	UINT AddVariable(LPCTSTR VarName,double Value)
	{
		CEasyString Key(VarName);
		Key.MakeUpper();
		ES_VARIABLE * pVar=NULL;
		UINT ID=m_Map.New(Key,&pVar);
		if(pVar)
		{
			pVar->ID=ID;
			pVar->Name=VarName;
			pVar->Type=VALUE_TYPE_DOUBLE;
			pVar->ValueDouble=Value;
		}
		return 0;
	}
	UINT AddVariable(LPCTSTR VarName,LPCTSTR Value)
	{
		CEasyString Key(VarName);
		Key.MakeUpper();
		ES_VARIABLE * pVar=NULL;
		UINT ID=m_Map.New(Key,&pVar);
		if(pVar)
		{
			pVar->ID=ID;
			pVar->Name=VarName;
			pVar->Type=VALUE_TYPE_STRING;
			pVar->StrValue=Value;
		}
		return 0;
	}
	bool EditVariable(LPCTSTR VarName, int Value)
	{
		CEasyString Key(VarName);
		Key.MakeUpper();
		ES_VARIABLE * pVar=m_Map.Find(Key);
		if(pVar)
		{
			pVar->ValueInt=Value;
		}
	}
	bool EditVariable(LPCTSTR VarName, INT64 Value)
	{
		CEasyString Key(VarName);
		Key.MakeUpper();
		ES_VARIABLE * pVar=m_Map.Find(Key);
		if(pVar)
		{
			pVar->ValueInt64=Value;
		}
	}
	bool EditVariable(LPCTSTR VarName, float Value)
	{
		CEasyString Key(VarName);
		Key.MakeUpper();
		ES_VARIABLE * pVar=m_Map.Find(Key);
		if(pVar)
		{
			pVar->ValueFloat=Value;
		}
	}
	bool EditVariable(LPCTSTR VarName, double Value)
	{
		CEasyString Key(VarName);
		Key.MakeUpper();
		ES_VARIABLE * pVar=m_Map.Find(Key);
		if(pVar)
		{
			pVar->ValueDouble=Value;
		}
	}
	bool EditVariable(LPCTSTR VarName, LPCTSTR Value)
	{
		CEasyString Key(VarName);
		Key.MakeUpper();
		ES_VARIABLE * pVar=m_Map.Find(Key);
		if(pVar)
		{
			pVar->StrValue=Value;
		}
	}
	ES_VARIABLE * FindVariable(UINT VarID)
	{
		return m_Map.GetObject(VarID);
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
};