/****************************************************************************/
/*                                                                          */
/*      �ļ���:    EasyScriptExecutor.h	                                    */
/*      ��������:  2010��08��26��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once


class CEasyScriptExecutor
{
public:
	CEasyScriptExecutor(void);
	~CEasyScriptExecutor(void);	

	int ExecScript(CESThread& ESThread);
	int ContinueScript(CESThread& ESThread);
	int CallFunction(CESThread& ESThread,LPCTSTR szFunctionName);
	
		

protected:
	int ExecScript(CESThread& ESThread,int StartPos,int EndPos);
	//int CallFunction(CESThread& ESThread,ES_FUNCTION * pFunction);
	int DoEva(ES_BOLAN * pLeftValue,ES_BOLAN * pRightValue,CESThread& ESThread);
	int DoAdd(ES_BOLAN * pLeftValue,ES_BOLAN * pRightValue,ES_BOLAN * pResult);
	int DoSub(ES_BOLAN * pLeftValue,ES_BOLAN * pRightValue,ES_BOLAN * pResult);
	int DoMul(ES_BOLAN * pLeftValue,ES_BOLAN * pRightValue,ES_BOLAN * pResult);
	int DoDiv(ES_BOLAN * pLeftValue,ES_BOLAN * pRightValue,ES_BOLAN * pResult);
	int DoEqu(ES_BOLAN * pLeftValue,ES_BOLAN * pRightValue,ES_BOLAN * pResult);
	int DoDiff(ES_BOLAN * pLeftValue,ES_BOLAN * pRightValue,ES_BOLAN * pResult);
	int DoLess(ES_BOLAN * pLeftValue,ES_BOLAN * pRightValue,ES_BOLAN * pResult);
	int DoMore(ES_BOLAN * pLeftValue,ES_BOLAN * pRightValue,ES_BOLAN * pResult);
	int DoLessEqu(ES_BOLAN * pLeftValue,ES_BOLAN * pRightValue,ES_BOLAN * pResult);
	int DoMoreEqu(ES_BOLAN * pLeftValue,ES_BOLAN * pRightValue,ES_BOLAN * pResult);
	int DoNeg(ES_BOLAN * pValue,ES_BOLAN * pResult);
	int DoAnd(ES_BOLAN * pLeftValue,ES_BOLAN * pRightValue,ES_BOLAN * pResult);
	int DoOr(ES_BOLAN * pLeftValue,ES_BOLAN * pRightValue,ES_BOLAN * pResult);
	int DoNot(ES_BOLAN * pValue,ES_BOLAN * pResult);
	int IsZero(ES_BOLAN * pValue);
};
