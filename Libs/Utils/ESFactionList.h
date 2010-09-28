/****************************************************************************/
/*                                                                          */
/*      文件名:    ESFactionList.h		                                    */
/*      创建日期:  2010年08月26日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once

typedef int (__thiscall CNameObject::*LPSCRIPT_FACTION)(CESVariableList* pVarList,ES_BOLAN* pResult,ES_BOLAN* pParams,int ParamCount);


struct ES_FACTION
{

	UINT				ID;
	CEasyString			Name;
	int					ParaCount;
	CNameObject *		pObject;
	LPSCRIPT_FACTION 	FnPtr;

	ES_FACTION()
	{
		ID=0;
		ParaCount=0;
		pObject=NULL;
		FnPtr=NULL;
	}
	ES_FACTION(ES_FACTION& FN)
	{
		ID=FN.ID;
		Name=FN.Name;
		ParaCount=FN.ParaCount;
		pObject=FN.pObject;
		FnPtr=FN.FnPtr;
	}

	bool operator==(const ES_FACTION& FN) const
	{
		if(Name.CompareNoCase(FN.Name)) 
			return false; 
		else 
			return true;
	}
};


class CESFactionList
{
protected:
	CStaticMap<CEasyString,ES_FACTION>	m_Map;
public:
	CESFactionList()
	{

	}
	~CESFactionList()
	{

	}
	BOOL Create(UINT Size)
	{
		return m_Map.Create(Size);
	}
	UINT AddFaction(LPCTSTR FactionName, int ParaCount,CNameObject * pObject,LPSCRIPT_FACTION FnPtr)
	{
		CEasyString Key(FactionName);
		Key.MakeUpper();
		ES_FACTION * pFn=NULL;
		UINT ID=m_Map.New(Key,&pFn);
		if(pFn)
		{
			pFn->ID=ID;
			pFn->Name=FactionName;
			pFn->ParaCount=ParaCount;
			pFn->pObject=pObject;
			pFn->FnPtr=FnPtr;
		}
		return 0;
	}
	ES_FACTION * FindFaction(UINT ID)
	{
		return m_Map.GetObject(ID);
	}
	ES_FACTION * FindFaction(LPCTSTR FactionName)
	{
		CEasyString Key(FactionName);
		Key.MakeUpper();
		return m_Map.Find(Key);
	}
};