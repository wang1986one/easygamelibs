/****************************************************************************/
/*                                                                          */
/*      文件名:    ESFunctionLib.h		                                    */
/*      创建日期:  2010年08月27日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once


class CESFunctionLib :
	public CNameObject,
	public CStaticObject<CESFunctionLib>
{
public:
	CESFunctionLib(void);
	~CESFunctionLib(void);

	void AddFunction(CESFactionList * pFunctionList);

	int SinFN(CESVariableList* pVarList,ES_BOLAN* pResult,ES_BOLAN* pParams,int ParamCount);
	int CosFN(CESVariableList* pVarList,ES_BOLAN* pResult,ES_BOLAN* pParams,int ParamCount);
	int RoundFN(CESVariableList* pVarList,ES_BOLAN* pResult,ES_BOLAN* pParams,int ParamCount);
	int StrCmpFN(CESVariableList* pVarList,ES_BOLAN* pResult,ES_BOLAN* pParams,int ParamCount);
	int NumToStrFN(CESVariableList* pVarList,ES_BOLAN* pResult,ES_BOLAN* pParams,int ParamCount);
	int RandomFN(CESVariableList* pVarList,ES_BOLAN* pResult,ES_BOLAN* pParams,int ParamCount);
};
