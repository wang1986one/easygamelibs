/****************************************************************************/
/*                                                                          */
/*      文件名:    ESFunctionLib.cpp	                                    */
/*      创建日期:  2010年08月27日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#include "StdAfx.h"
#include <math.h>

CESFunctionLib::CESFunctionLib(void)
{
}

CESFunctionLib::~CESFunctionLib(void)
{
}

void CESFunctionLib::AddFunction(CESFactionList * pFunctionList)
{
	pFunctionList->AddFaction("Sin",1,this,(LPSCRIPT_FACTION)&CESFunctionLib::SinFN);
	pFunctionList->AddFaction("Cos",1,this,(LPSCRIPT_FACTION)&CESFunctionLib::CosFN);
	pFunctionList->AddFaction("Round",2,this,(LPSCRIPT_FACTION)&CESFunctionLib::RoundFN);
	pFunctionList->AddFaction("StrCmp",2,this,(LPSCRIPT_FACTION)&CESFunctionLib::StrCmpFN);
	pFunctionList->AddFaction("NumToStr",1,this,(LPSCRIPT_FACTION)&CESFunctionLib::NumToStrFN);
	pFunctionList->AddFaction("Random",1,this,(LPSCRIPT_FACTION)&CESFunctionLib::RandomFN);
}

int CESFunctionLib::SinFN(CESVariableList* pVarList,ES_BOLAN* pResult,ES_BOLAN* pParams,int ParamCount)
{
	*pResult=sin((double)pParams[0]);
	return 0;	
}

int CESFunctionLib::CosFN(CESVariableList* pVarList,ES_BOLAN* pResult,ES_BOLAN* pParams,int ParamCount)
{
	*pResult=cos((double)pParams[0]);
	return 0;
}

int CESFunctionLib::RoundFN(CESVariableList* pVarList,ES_BOLAN* pResult,ES_BOLAN* pParams,int ParamCount)
{
	pResult->ValueType=VALUE_TYPE_DOUBLE;
	pResult->Level=0;
	if(pParams[0].ValueDouble>0)
		pResult->ValueDouble=floor(pParams[0].ValueDouble*pow(10.0f,pParams[1].ValueInt)+0.5)/pow(10.0f,pParams[1].ValueInt);
	else 
		pResult->ValueDouble=ceil(pParams[0].ValueDouble*pow(10.0f,pParams[1].ValueInt)-0.5)/pow(10.0f,pParams[1].ValueInt);
	return 0;
}

int CESFunctionLib::StrCmpFN(CESVariableList* pVarList,ES_BOLAN* pResult,ES_BOLAN* pParams,int ParamCount)
{
	*pResult=strcmp(pParams[0].StrValue,pParams[1].StrValue);
	return 0;
}

int CESFunctionLib::NumToStrFN(CESVariableList* pVarList,ES_BOLAN* pResult,ES_BOLAN* pParams,int ParamCount)
{
	pResult->ValueType=VALUE_TYPE_STRING;
	switch(pParams[0].ValueType)
	{
	case VALUE_TYPE_INT:
		pResult->StrValue.Format("%d",pParams[0].ValueInt);
		break;
	case VALUE_TYPE_INT64:
		pResult->StrValue.Format("%lld",pParams[0].ValueInt64);
		break;
	case VALUE_TYPE_FLOAT:
		pResult->StrValue.Format("%g",pParams[0].ValueFloat);
		break;
	case VALUE_TYPE_DOUBLE:
		pResult->StrValue.Format("%g",pParams[0].ValueDouble);
		break;
	}
	
	return 0;
}

int CESFunctionLib::RandomFN(CESVariableList* pVarList,ES_BOLAN* pResult,ES_BOLAN* pParams,int ParamCount)
{
	*pResult=rand()%(int)pParams[0];
	return 0;
}