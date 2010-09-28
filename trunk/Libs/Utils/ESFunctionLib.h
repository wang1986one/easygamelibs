/****************************************************************************/
/*                                                                          */
/*      �ļ���:    ESFunctionLib.h		                                    */
/*      ��������:  2010��08��27��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
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
