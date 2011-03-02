/****************************************************************************/
/*                                                                          */
/*      �ļ���:    ESFunctionLib.cpp	                                    */
/*      ��������:  2010��08��27��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
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

void CESFunctionLib::AddFunction(CESFunctionList * pFunctionList)
{
	ADD_SCRIPT_CFUNCTION(CESFunctionLib,SinFN,"Sin",1,pFunctionList,this);
	ADD_SCRIPT_CFUNCTION(CESFunctionLib,CosFN,"Cos",1,pFunctionList,this);
	ADD_SCRIPT_CFUNCTION(CESFunctionLib,RoundFN,"Round",1,pFunctionList,this);
	ADD_SCRIPT_CFUNCTION(CESFunctionLib,StrCmpFN,"StrCmp",1,pFunctionList,this);
	ADD_SCRIPT_CFUNCTION(CESFunctionLib,NumToStrFN,"NumToStr",1,pFunctionList,this);
	ADD_SCRIPT_CFUNCTION(CESFunctionLib,RandomFN,"Random",1,pFunctionList,this);	
}

int CESFunctionLib::SinFN(CESThread * pESThread,ES_BOLAN* pResult,ES_BOLAN* pParams,int ParamCount)
{
	*pResult=sin((double)pParams[0]);
	return 0;	
}

int CESFunctionLib::CosFN(CESThread * pESThread,ES_BOLAN* pResult,ES_BOLAN* pParams,int ParamCount)
{
	*pResult=cos((double)pParams[0]);
	return 0;
}

int CESFunctionLib::RoundFN(CESThread * pESThread,ES_BOLAN* pResult,ES_BOLAN* pParams,int ParamCount)
{
	pResult->ValueType=VALUE_TYPE_DOUBLE;
	pResult->Level=0;
	if(pParams[0].ValueDouble>0)
		pResult->ValueDouble=floor(pParams[0].ValueDouble*pow(10.0f,pParams[1].ValueInt)+0.5)/pow(10.0f,pParams[1].ValueInt);
	else 
		pResult->ValueDouble=ceil(pParams[0].ValueDouble*pow(10.0f,pParams[1].ValueInt)-0.5)/pow(10.0f,pParams[1].ValueInt);
	return 0;
}

int CESFunctionLib::StrCmpFN(CESThread * pESThread,ES_BOLAN* pResult,ES_BOLAN* pParams,int ParamCount)
{
	*pResult=strcmp(pParams[0].StrValue,pParams[1].StrValue);
	return 0;
}

int CESFunctionLib::NumToStrFN(CESThread * pESThread,ES_BOLAN* pResult,ES_BOLAN* pParams,int ParamCount)
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

int CESFunctionLib::RandomFN(CESThread * pESThread,ES_BOLAN* pResult,ES_BOLAN* pParams,int ParamCount)
{
	*pResult=rand()%(int)pParams[0];
	return 0;
}