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

	void AddFunction(CESFunctionList * pFunctionList);

	int SinFN(CESThread * pESThread,ES_BOLAN* pResult,ES_BOLAN* pParams,int ParamCount);
	int CosFN(CESThread * pESThread,ES_BOLAN* pResult,ES_BOLAN* pParams,int ParamCount);
	int RoundFN(CESThread * pESThread,ES_BOLAN* pResult,ES_BOLAN* pParams,int ParamCount);
	int StrCmpFN(CESThread * pESThread,ES_BOLAN* pResult,ES_BOLAN* pParams,int ParamCount);
	int NumToStrFN(CESThread * pESThread,ES_BOLAN* pResult,ES_BOLAN* pParams,int ParamCount);
	int RandomFN(CESThread * pESThread,ES_BOLAN* pResult,ES_BOLAN* pParams,int ParamCount);
};
