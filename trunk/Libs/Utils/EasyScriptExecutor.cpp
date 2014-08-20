/****************************************************************************/
/*                                                                          */
/*      �ļ���:    EasyScriptExecutor.cpp                                   */
/*      ��������:  2010��08��26��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#include "StdAfx.h"

LPCTSTR OPERATOR_STRINGS[OPERATOR_MAX]={_T("��"),_T("��"),_T("��"),_T("��"),_T("������"),_T("������"),_T("ȡ��"),_T("����"),
			_T("С��"),_T("����"),_T("С�ڵ���"),_T("���ڵ���"),_T("����"),_T("������"),_T("��ֵ"),_T("��"),_T("��"),_T("��"),_T("��ת"),
			_T("������ת"),_T("��������"),_T("�������÷���"),_T("��ն�ջ"),_T("�����ʱ����"),_T("��ӵ��ò���"),_T("�ű��˳�"),_T("�ղ���")};

LPCTSTR KEYWORD_STRINGS[KW_MAX]=
{_T("EXIT"),_T("IF"),_T("THEN"),_T("ELSE"),_T("ELSEIF"),_T("ENDIF"),_T("WHILE"),_T("DO"),_T("ENDWHILE"),_T("BREAK"),_T("CONTINUE"),_T("GOTO"),_T(";"),
_T("INT"),_T("INT64"),_T("FLOAT"),_T("DOUBLE"),_T("STRING"),_T("FUNCTION"),_T("ENDFUN")};





LPCTSTR ESGetErrorMsg(int ErrCode)
{
	switch(ErrCode)
	{
	case -1:
		return _T("ִ���ж�");
		break;
	case 0:
		return _T("ִ�гɹ�");
		break;
	case 1001:
		return _T("�ַ�����������");
		break;
	case 1002:
		return _T("ȱ���ַ�����β��");
		break;
	case 1003:
		return _T("��ʶ������");
		break;
	case 1005:
		return _T("�Ƿ��ַ�");
		break;
	case 2001:
		return _T("��IF���δ��ȷ���");
		break;	
	case 2011:
		return _T("WHILE DO��ENDWHILEδ�����");
		break;
	case 2012:
		return _T("BREAK���������WHILEѭ����");
		break;
	case 2013:
		return _T("CONTINUE���������WHILEѭ����");
		break;
	case 2014:
		return _T("GOTO������ȱ�ٱ��");
		break;
	case 2015:
		return _T("GOTO������ı��δ����");
		break;
	case 2016:
		return _T("FUNCTION��ENDFUNδ���");
		break;
	case 2018:
		return _T("�������ں����ڶ��庯��");
		break;
	case 2019:
		return _T("���������ʽ����");
		break;	
	case 2020:
		return _T("IF,ELSEIF,WHILE����ı��ʽ�����з���ֵ");
		break;
	case 2021:
		return _T("����δ���");
		break;
	case 3001:
		return _T("��ֵĿ�겻�Ǳ���");
		break;	
	case 3002:
		return _T("�ַ������ܽ��м�������");
		break;
	case 3003:
		return _T("�ַ������ܽ��г˷�����");
		break;
	case 3004:
		return _T("������ǰ��ı����������Ͳ�����");
		break;
	case 3005:
		return _T("�ַ������ܽ��г�������");
		break;
	case 3006:
		return _T("�ַ������ܽ���ȡ��������");
		break;
	case 3007:
		return _T("������������");
		break;
	case 3008:
		return _T("�Ƿ��ĺ�������");
		break;
	case 3009:
		return _T("����ִ�д���");
		break;
	case 3010:
		return _T("AND���ܶ��ַ���ʹ��");
	case 3011:
		return _T("OR���ܶ��ַ���ʹ��");
	case 3012:
		return _T("NOT���ܶ��ַ���ʹ��");
	case 3013:
		return _T("��ջ���");
	case 3014:
		return _T("�ж����");
	case 3015:
		return _T("��ֵ��Դ���Ǳ�����ֵ");
	case 3016:
		return _T("��һ�����������Ǳ�����ֵ");
	case 3017:
		return _T("�ڶ������������Ǳ�����ֵ");
	case 3018:
		return _T("��������֧�ִ˲���������");
	case 3019:
		return _T("δ֪������");
	case 4010:	
		return _T("�����������");		
	case 4012:
		return _T("δ֪��ʶ��");	
	case 4013:
		return _T("��������������");
	case 4016:
		return _T("����δ����");
	case 5001:
		return _T("�жϲ�����");
	case 6001:
		return _T("δ���ñ����б�");
	case 6002:
		return _T("δ���ú����б�");
	case 6003:
		return _T("δ���ýű�����");
	case 6004:
		return _T("��ǰ�߳������ж��У�����ContinueScript");
	case 6005:
		return _T("��ǰ�߳�δ���ж��У�����ʹ��ContinueScript");
	case 6006:
		return _T("Ҫ���õĺ���δ�ҵ�");
	case 6007:
		return _T("Ҫ���õĺ������ǽű�����");
	case 6009:
		return _T("��Ӿֲ�����ʧ��");
	case 10000:
		return _T("��������Ĳ���ʽ");
	}
	return _T("δ֪����");
}

CEasyScriptExecutor::CEasyScriptExecutor(void)
{

}

CEasyScriptExecutor::~CEasyScriptExecutor(void)
{

}

int CEasyScriptExecutor::ExecScript(CESThread& ESThread)
{
	if(ESThread.IsInInterrupt())
	{
		ESThread.SetResultCode(6004);
		return 	6004;
	}

	ESThread.GetStack()->Clear();
	ESThread.ClearCallStack();

	int Ret=ExecScript(ESThread,0,-1);
	ESThread.SetResultCode(Ret);
	if(Ret==0)
	{
		ES_BOLAN * pResult=ESThread.GetStack()->Pop();
		if(pResult)
			ESThread.SetResult(*pResult);
	}
	return Ret;
}

int CEasyScriptExecutor::ContinueScript(CESThread& ESThread)
{
	if(!ESThread.IsInInterrupt())
	{
		ESThread.SetResultCode(6005);
		return 	6005;
	}	
	int Ret=ExecScript(ESThread,0,-1);
	ESThread.SetResultCode(Ret);
	ES_BOLAN * pResult=ESThread.GetStack()->Pop();
	if(pResult)
		ESThread.SetResult(*pResult);
	return Ret;
}
int CEasyScriptExecutor::CallFunction(CESThread& ESThread,LPCTSTR szFunctionName)
{
	if(ESThread.IsInInterrupt())
	{
		ESThread.SetResultCode(6004);
		return 	6004;
	}
	//ESThread.GetStack()->Clear();
	//ESThread.ClearLocalVariable();
	ESThread.ClearCallStack();

	ES_FUNCTION * pFunction=ESThread.GetFunctionList()->FindFunction(szFunctionName);
	if(pFunction==NULL)
	{
		ESThread.SetResultCode(6006);
		return 6006;
	}
	if(pFunction->Type!=FUNCTION_TYPE_SCRIPT)
	{
		ESThread.SetResultCode(6007);
		return 6007;
	}

	if(ESThread.GetStack()->GetSize()<pFunction->ParaCount)
		return 3007;

	ES_BOLAN * pParams=ESThread.GetStack()->GetTop()-(pFunction->ParaCount-1);

	for(int i=0;i<pFunction->ParaCount;i++)
	{
		if(pParams[i].Type!=BOLAN_TYPE_VALUE&&pParams[i].Type!=BOLAN_TYPE_VARIABLE)
			return 3008;
	}
	ESThread.NewLocalVariableList();
	int Ret=ExecScript(ESThread,pFunction->FunStartPos,pFunction->FunEndPos);

	ESThread.SetResultCode(Ret);
	ES_BOLAN * pResult=ESThread.GetStack()->Pop();
	if(pResult)
		ESThread.SetResult(*pResult);
	return Ret;
}

int CEasyScriptExecutor::ExecScript(CESThread& ESThread,int StartPos,int EndPos)
{
	ES_BOLAN *			pBolan;		
	ES_VARIABLE *		pVar;	
	ES_BOLAN *			t1;
	ES_BOLAN *			t2;	
	ES_BOLAN			FnResult;		
	int					ResultCode;
	CESBolanStack *		pScript=ESThread.GetScript();
	CESBolanStack * 	pStack=ESThread.GetStack();	
	CESFunctionList *	pFunctionList=ESThread.GetFunctionList();


	if(pStack==NULL)
		return 3013;

	int StackStartSize=pStack->GetSize();

	if(ESThread.IsInInterrupt())
	{
		pStack->PushValue(&ESThread.GetResult());
		StartPos=ESThread.GetInterruptPos();
		ESThread.ClearInterrupt();
	}

	

	while(StartPos<(int)pScript->GetSize())
	{
		//BLOCK_BEGIN("WHILE");
		//BLOCK_BEGIN("GET");
		pBolan=pScript->GetAt(StartPos);
		ESThread.SetLastLine(pBolan->Line);
		//BLOCK_END;
		
		switch(pBolan->Type)
		{
		case BOLAN_TYPE_VALUE:			
			//BLOCK_BEGIN("BOLAN_TYPE_VALUE");
			pStack->PushValue(pBolan);
			StartPos++;			
			//BLOCK_END;
			break;
		case BOLAN_TYPE_VARIABLE:	
			//BLOCK_BEGIN("BOLAN_TYPE_VARIABLE");
			pVar=ESThread.FindVariable(pBolan->Index);				
			if(pVar==NULL)
				return 4012;
			if(pBolan->ValueType==VALUE_TYPE_STRING)
				pBolan->StrValue=pVar->StrValue;
			else
				pBolan->ValueInt64=pVar->ValueInt64;
			pStack->PushValue(pBolan);			
			StartPos++;
			//BLOCK_END;
			break;		
		case BOLAN_TYPE_OPERATOR:
			switch((int)(pBolan->Index))
			{
			case OPERATOR_EVA:
				//BLOCK_BEGIN("OPERATOR_EVA");
				t2=pStack->Pop();
				t1=pStack->Pop();
				ResultCode=DoEva(t1,t2,ESThread);
				if(ResultCode)
					return ResultCode;
				StartPos++;
				//BLOCK_END;
				break;
			case OPERATOR_ADD:
				t2=pStack->Pop();
				t1=pStack->GetTop();
				ResultCode=DoAdd(t1,t2,t1);
				if(ResultCode)
					return ResultCode;
				StartPos++;
				break;
			case OPERATOR_SUB:
				//BLOCK_BEGIN("OPERATOR_SUB");
				t2=pStack->Pop();
				t1=pStack->GetTop();			
				ResultCode=DoSub(t1,t2,t1);
				if(ResultCode)
					return ResultCode;
				StartPos++;
				//BLOCK_END;
				break;
			case OPERATOR_MUL:
				t2=pStack->Pop();
				t1=pStack->GetTop();
				ResultCode=DoMul(t1,t2,t1);
				if(ResultCode)
					return ResultCode;
				StartPos++;
				break;
			case OPERATOR_DIV:
				t2=pStack->Pop();
				t1=pStack->GetTop();
				ResultCode=DoDiv(t1,t2,t1);
				if(ResultCode)
					return ResultCode;
				StartPos++;
				break;
			case OPERATOR_EQU:
				t2=pStack->Pop();
				t1=pStack->GetTop();				
				ResultCode=DoEqu(t1,t2,t1);
				if(ResultCode)
					return ResultCode;
				StartPos++;
				break;
			case OPERATOR_DIFF:
				t2=pStack->Pop();
				t1=pStack->GetTop();				
				ResultCode=DoDiff(t1,t2,t1);
				if(ResultCode)
					return ResultCode;
				StartPos++;
				break;
			case OPERATOR_LESS:
				t2=pStack->Pop();
				t1=pStack->GetTop();				
				ResultCode=DoLess(t1,t2,t1);
				if(ResultCode)
					return ResultCode;
				StartPos++;
				break;
			case OPERATOR_MORE:
				t2=pStack->Pop();
				t1=pStack->GetTop();				
				ResultCode=DoMore(t1,t2,t1);
				if(ResultCode)
					return ResultCode;
				StartPos++;
				break;
			case OPERATOR_LESS_EQU:
				t2=pStack->Pop();
				t1=pStack->GetTop();				
				ResultCode=DoLessEqu(t1,t2,t1);
				if(ResultCode)
					return ResultCode;
				StartPos++;
				break;
			case OPERATOR_MORE_EQU:
				t2=pStack->Pop();
				t1=pStack->GetTop();				
				ResultCode=DoMoreEqu(t1,t2,t1);
				if(ResultCode)
					return ResultCode;
				StartPos++;
				break;
			case OPERATOR_NEG:
				t1=pStack->GetTop();				
				ResultCode=DoNeg(t1,t1);
				if(ResultCode)
					return ResultCode;
				StartPos++;
				break;
			case OPERATOR_AND:
				t2=pStack->Pop();
				t1=pStack->GetTop();				
				ResultCode=DoAnd(t1,t2,t1);
				if(ResultCode)
					return ResultCode;
				StartPos++;
				break;
			case OPERATOR_OR:
				t2=pStack->Pop();
				t1=pStack->GetTop();				
				ResultCode=DoOr(t1,t2,t1);
				if(ResultCode)
					return ResultCode;
				StartPos++;
				break;
			case OPERATOR_NOT:				
				t1=pStack->GetTop();
				ResultCode=DoNot(t1,t1);
				if(ResultCode)
					return ResultCode;
				StartPos++;
				break;
			case OPERATOR_JMP:
			case OPERATOR_JMP_FUNC:
				//BLOCK_BEGIN("OPERATOR_JMP");
				StartPos=pBolan->Level;
				pStack->Clear();
				//BLOCK_END;
				break;
			case OPERATOR_JZ:
				//BLOCK_BEGIN("OPERATOR_JZ");
				t1=pStack->Pop();				
				ResultCode=IsZero(t1);
				if(ResultCode>0)
					return ResultCode;
				else if(ResultCode==0)
				{
					StartPos=pBolan->Level;
					pStack->Clear();
				}
				else
					StartPos++;
				//BLOCK_END;
				break;
			case OPERATOR_CALL:
				{
					ES_FUNCTION * pFunction=pFunctionList->FindFunction(pBolan->Level);

					if(pFunction->Type==FUNCTION_TYPE_C)
					{					
						if(pStack->GetSize()<pFunction->ParaCount)
							return 3007;

						t1=pStack->GetTop()-(pFunction->ParaCount-1);

						for(int i=0;i<pFunction->ParaCount;i++)
						{
							if(t1[i].Type!=BOLAN_TYPE_VALUE&&t1[i].Type!=BOLAN_TYPE_VARIABLE)
								return 3008;
						}

						FnResult=0;

						ResultCode=(pFunction->FnPtr)(pFunction->CallParam,&ESThread,&FnResult,t1,pFunction->ParaCount);

						pStack->Pop(pFunction->ParaCount);



						if(ResultCode<0) 
							return 3009;					//���ú�������				
						if(ResultCode>0)
						{							
							ESThread.SetInterruptCode(ResultCode);
							ESThread.SetInterruptPos(StartPos+1);					
							return -1;
						}
						else
						{
							pStack->PushValue(&FnResult);
						}		
						StartPos++;
					}
					else
					{

						if(ESThread.GetStack()->GetSize()<pFunction->ParaCount)
							return 3007;

						ES_BOLAN * pParams=ESThread.GetStack()->GetTop()-(pFunction->ParaCount-1);

						for(int i=0;i<pFunction->ParaCount;i++)
						{
							if(pParams[i].Type!=BOLAN_TYPE_VALUE&&pParams[i].Type!=BOLAN_TYPE_VARIABLE)
								return 3008;
						}

						ESThread.PushCallStack(StartPos+1);
						ESThread.NewLocalVariableList();
						StartPos=pFunction->FunStartPos;											
						
						
						//ResultCode=CallFunction(ESThread,pFunction);
						//if(ResultCode)
						//	return ResultCode;					
					}
				}				
				break;
			case OPERATOR_RET:
				ESThread.ReleaseLocalVariableList();
				StartPos=ESThread.PopCallStack();
				if(StartPos<0)
					return 0;
				break;
			case OPERATOR_CLEAR_STACK:
				pStack->Clear();
				StartPos++;
				break;
			case OPERATOR_ADD_VAR:				
				pVar=ESThread.AddEmptyLocalVariable(pBolan->StrValue,pBolan->ValueType);
				if(pVar==NULL)
				{
					return 6009;
				}
				StartPos++;
				break;
			case OPERATOR_ADD_CALL_PARAM:
				{
					pVar=ESThread.AddEmptyLocalVariable(pBolan->StrValue,pBolan->ValueType);
					if(pVar==NULL)
					{
						return 6009;
					}
					t1=pStack->Pop();					
					ES_BOLAN LeftValue;
					LeftValue.Type=BOLAN_TYPE_VARIABLE;
					LeftValue.Index=pVar->ID;
					LeftValue.Level=1;
					ResultCode=DoEva(&LeftValue,t1,ESThread);
					if(ResultCode)
						return ResultCode;
				}
				StartPos++;
				break;
			case OPERATOR_EXIT:
				return 0;
				break;
			case OPERATOR_NOP:
				StartPos++;
				break;
			default:
				return 3019;
				break;
			}			
			break;		
		default:
			return 10000;
			break;
		}		
		if(EndPos>=0)
		{
			if(StartPos>=EndPos)
				break;
		}
		//BLOCK_END;
	}
	return 0;
}


//int CEasyScriptExecutor::CallFunction(CESThread& ESThread,ES_FUNCTION * pFunction)
//{
//	int RetCode;
//
//
//	if(ESThread.GetStack()->GetSize()<pFunction->ParaCount)
//		return 3007;
//
//	ES_BOLAN * pParams=ESThread.GetStack()->GetTop()-(pFunction->ParaCount-1);
//
//	for(int i=0;i<pFunction->ParaCount;i++)
//	{
//		if(pParams[i].Type!=BOLAN_TYPE_VALUE&&pParams[i].Type!=BOLAN_TYPE_VARIABLE)
//			return 3008;
//	}
//	ESThread.NewLocalVariableList();
//	RetCode=ExecScript(ESThread,pFunction->FunStartPos,pFunction->FunEndPos);
//	if(RetCode)
//		return RetCode;	
//	return 0;
//}

int CEasyScriptExecutor::DoEva(ES_BOLAN * pLeftValue,ES_BOLAN * pRightValue,CESThread& ESThread)
{
	//FUNCTION_BEGIN;
	if(pLeftValue==NULL||pRightValue==NULL)
		return 4013;
	if(pLeftValue->Type!=BOLAN_TYPE_VARIABLE)
		return 3001;
	if(pRightValue->Type!=BOLAN_TYPE_VARIABLE&&pRightValue->Type!=BOLAN_TYPE_VALUE)
		return 3015;
	ES_VARIABLE * pVar=ESThread.FindVariable(pLeftValue->Index);
	if(pVar==NULL)
		return 4012;
	switch(pVar->Type)
	{
	case VALUE_TYPE_INT:
		{
			switch(pRightValue->ValueType)
			{
				case VALUE_TYPE_INT:
					pVar->ValueInt=pRightValue->ValueInt;					
					break;
				case VALUE_TYPE_INT64:
					pVar->ValueInt=(int)pRightValue->ValueInt64;
					break;
				case VALUE_TYPE_FLOAT:
					pVar->ValueInt=(int)pRightValue->ValueFloat;
					break;
				case VALUE_TYPE_DOUBLE:
					pVar->ValueInt=(int)pRightValue->ValueDouble;
					break;
				case VALUE_TYPE_STRING:
					return 3004;
			}
			//pLeftValue->ValueInt=pVar->ValueInt;
		}
		break;
	case VALUE_TYPE_INT64:
		{
			switch(pRightValue->ValueType)
			{
			case VALUE_TYPE_INT:
				pVar->ValueInt64=pRightValue->ValueInt;
				break;
			case VALUE_TYPE_INT64:
				pVar->ValueInt64=pRightValue->ValueInt64;
				break;
			case VALUE_TYPE_FLOAT:
				pVar->ValueInt64=(INT64)pRightValue->ValueFloat;
				break;
			case VALUE_TYPE_DOUBLE:
				pVar->ValueInt64=(INT64)pRightValue->ValueDouble;
				break;
			case VALUE_TYPE_STRING:
				return 3004;
			}
			//pLeftValue->ValueInt64=pVar->ValueInt64;
		}
		break;
	case VALUE_TYPE_FLOAT:
		{
			switch(pRightValue->ValueType)
			{
			case VALUE_TYPE_INT:
				pVar->ValueFloat=(float)pRightValue->ValueInt;
				break;
			case VALUE_TYPE_INT64:
				pVar->ValueFloat=(float)pRightValue->ValueInt64;
				break;
			case VALUE_TYPE_FLOAT:
				pVar->ValueFloat=pRightValue->ValueFloat;
				break;
			case VALUE_TYPE_DOUBLE:
				pVar->ValueFloat=(float)pRightValue->ValueDouble;
				break;
			case VALUE_TYPE_STRING:
				return 3004;
			}
			//pLeftValue->ValueFloat=pVar->ValueFloat;
		}
		break;
	case VALUE_TYPE_DOUBLE:
		{
			switch(pRightValue->ValueType)
			{
			case VALUE_TYPE_INT:
				pVar->ValueDouble=(double)pRightValue->ValueInt;
				break;
			case VALUE_TYPE_INT64:
				pVar->ValueDouble=(double)pRightValue->ValueInt64;
				break;
			case VALUE_TYPE_FLOAT:
				pVar->ValueDouble=(double)pRightValue->ValueFloat;
				break;
			case VALUE_TYPE_DOUBLE:
				pVar->ValueDouble=pRightValue->ValueDouble;
				break;
			case VALUE_TYPE_STRING:
				return 3004;
			}
			//pLeftValue->ValueDouble=pVar->ValueDouble;
		}
		break;
	case VALUE_TYPE_STRING:
		{
			switch(pRightValue->ValueType)
			{
			case VALUE_TYPE_INT:
			case VALUE_TYPE_INT64:
			case VALUE_TYPE_FLOAT:
			case VALUE_TYPE_DOUBLE:
				return 3004;
			case VALUE_TYPE_STRING:
				pVar->StrValue=pRightValue->StrValue;				
				break;
			}
			//pLeftValue->StrValue=pVar->StrValue;
		}
		break;
	}	
	return 0;	
	//FUNCTION_END;
	//return 0;
}

int CEasyScriptExecutor::DoAdd(ES_BOLAN * pLeftValue,ES_BOLAN * pRightValue,ES_BOLAN * pResult)
{
	//FUNCTION_BEGIN;
	if(pLeftValue==NULL||pRightValue==NULL||pResult==NULL)
		return 4013;
	if(pLeftValue->Type!=BOLAN_TYPE_VARIABLE&&pLeftValue->Type!=BOLAN_TYPE_VALUE)
		return 3016;
	if(pRightValue->Type!=BOLAN_TYPE_VARIABLE&&pRightValue->Type!=BOLAN_TYPE_VALUE)
		return 3017;

	
	
	switch(pLeftValue->ValueType)
	{
	case VALUE_TYPE_INT:
		{
			switch(pRightValue->ValueType)
			{
			case VALUE_TYPE_INT:
				pResult->ValueType=VALUE_TYPE_INT;
				pResult->ValueInt=pLeftValue->ValueInt+pRightValue->ValueInt;
				break;
			case VALUE_TYPE_INT64:
				pResult->ValueType=VALUE_TYPE_INT64;
				pResult->ValueInt64=(INT64)(pLeftValue->ValueInt+pRightValue->ValueInt64);
				break;
			case VALUE_TYPE_FLOAT:
				pResult->ValueType=VALUE_TYPE_FLOAT;
				pResult->ValueFloat=(float)(pLeftValue->ValueInt+pRightValue->ValueFloat);
				break;
			case VALUE_TYPE_DOUBLE:
				pResult->ValueType=VALUE_TYPE_FLOAT;
				pResult->ValueDouble=(double)(pLeftValue->ValueInt+pRightValue->ValueDouble);
				break;
			case VALUE_TYPE_STRING:
				return 3004;
			}
		}
		break;
	case VALUE_TYPE_INT64:
		{
			switch(pRightValue->ValueType)
			{
			case VALUE_TYPE_INT:
				pResult->ValueType=VALUE_TYPE_INT64;
				pResult->ValueInt64=(INT64)(pLeftValue->ValueInt64+pRightValue->ValueInt);
				break;
			case VALUE_TYPE_INT64:
				pResult->ValueType=VALUE_TYPE_INT64;
				pResult->ValueInt64=(INT64)(pLeftValue->ValueInt64+pRightValue->ValueInt64);
				break;
			case VALUE_TYPE_FLOAT:
				pResult->ValueType=VALUE_TYPE_DOUBLE;
				pResult->ValueDouble=(double)(pLeftValue->ValueInt64+pRightValue->ValueFloat);
				break;
			case VALUE_TYPE_DOUBLE:
				pResult->ValueType=VALUE_TYPE_DOUBLE;
				pResult->ValueDouble=(double)(pLeftValue->ValueInt64+pRightValue->ValueDouble);
				break;
			case VALUE_TYPE_STRING:
				return 3004;
			}
		}
		break;
	case VALUE_TYPE_FLOAT:
		{
			switch(pRightValue->ValueType)
			{
			case VALUE_TYPE_INT:
				pResult->ValueType=VALUE_TYPE_FLOAT;
				pResult->ValueFloat=(float)(pLeftValue->ValueFloat+pRightValue->ValueInt);
				break;
			case VALUE_TYPE_INT64:
				pResult->ValueType=VALUE_TYPE_DOUBLE;
				pResult->ValueDouble=(double)(pLeftValue->ValueFloat+pRightValue->ValueInt64);
				break;
			case VALUE_TYPE_FLOAT:
				pResult->ValueType=VALUE_TYPE_FLOAT;
				pResult->ValueFloat=(float)(pLeftValue->ValueFloat+pRightValue->ValueFloat);
				break;
			case VALUE_TYPE_DOUBLE:
				pResult->ValueType=VALUE_TYPE_DOUBLE;
				pResult->ValueDouble=(double)(pLeftValue->ValueFloat+pRightValue->ValueDouble);
				break;
			case VALUE_TYPE_STRING:
				return 3004;
			}
		}
		break;
	case VALUE_TYPE_DOUBLE:
		{
			switch(pRightValue->ValueType)
			{
			case VALUE_TYPE_INT:
				pResult->ValueType=VALUE_TYPE_DOUBLE;
				pResult->ValueDouble=(double)(pLeftValue->ValueDouble+pRightValue->ValueInt);
				break;
			case VALUE_TYPE_INT64:
				pResult->ValueType=VALUE_TYPE_DOUBLE;
				pResult->ValueDouble=(double)(pLeftValue->ValueDouble+pRightValue->ValueInt64);
				break;
			case VALUE_TYPE_FLOAT:
				pResult->ValueType=VALUE_TYPE_DOUBLE;
				pResult->ValueDouble=(double)(pLeftValue->ValueDouble+pRightValue->ValueFloat);
				break;
			case VALUE_TYPE_DOUBLE:
				pResult->ValueType=VALUE_TYPE_DOUBLE;
				pResult->ValueDouble=(double)(pLeftValue->ValueDouble+pRightValue->ValueDouble);
				break;
			case VALUE_TYPE_STRING:
				return 3004;
			}
		}
		break;
	case VALUE_TYPE_STRING:
		{
			switch(pRightValue->ValueType)
			{
			case VALUE_TYPE_INT:
			case VALUE_TYPE_INT64:
			case VALUE_TYPE_FLOAT:
			case VALUE_TYPE_DOUBLE:
				return 3004;
			case VALUE_TYPE_STRING:
				pResult->ValueType=VALUE_TYPE_STRING;
				pResult->StrValue=pLeftValue->StrValue+pRightValue->StrValue;
				break;
			}
		}
		break;
	}

	pResult->Type=BOLAN_TYPE_VALUE;
	return 0;
	//FUNCTION_END;
	//return 0;
}

int CEasyScriptExecutor::DoSub(ES_BOLAN * pLeftValue,ES_BOLAN * pRightValue,ES_BOLAN * pResult)
{
	//FUNCTION_BEGIN;
	if(pLeftValue==NULL||pRightValue==NULL||pResult==NULL)
		return 4013;
	if(pLeftValue->Type!=BOLAN_TYPE_VARIABLE&&pLeftValue->Type!=BOLAN_TYPE_VALUE)
		return 3016;
	if(pRightValue->Type!=BOLAN_TYPE_VARIABLE&&pRightValue->Type!=BOLAN_TYPE_VALUE)
		return 3017;



	switch(pLeftValue->ValueType)
	{
	case VALUE_TYPE_INT:
		{
			switch(pRightValue->ValueType)
			{
			case VALUE_TYPE_INT:
				pResult->ValueType=VALUE_TYPE_INT;
				pResult->ValueInt=pLeftValue->ValueInt-pRightValue->ValueInt;
				break;
			case VALUE_TYPE_INT64:
				pResult->ValueType=VALUE_TYPE_INT64;
				pResult->ValueInt64=(INT64)(pLeftValue->ValueInt-pRightValue->ValueInt64);
				break;
			case VALUE_TYPE_FLOAT:
				pResult->ValueType=VALUE_TYPE_FLOAT;
				pResult->ValueFloat=(float)(pLeftValue->ValueInt-pRightValue->ValueFloat);
				break;
			case VALUE_TYPE_DOUBLE:
				pResult->ValueType=VALUE_TYPE_FLOAT;
				pResult->ValueDouble=(double)(pLeftValue->ValueInt-pRightValue->ValueDouble);
				break;
			case VALUE_TYPE_STRING:
				return 3018;
			}
		}
		break;
	case VALUE_TYPE_INT64:
		{
			switch(pRightValue->ValueType)
			{
			case VALUE_TYPE_INT:
				pResult->ValueType=VALUE_TYPE_INT64;
				pResult->ValueInt64=(INT64)(pLeftValue->ValueInt64-pRightValue->ValueInt);
				break;
			case VALUE_TYPE_INT64:
				pResult->ValueType=VALUE_TYPE_INT64;
				pResult->ValueInt64=(INT64)(pLeftValue->ValueInt64-pRightValue->ValueInt64);
				break;
			case VALUE_TYPE_FLOAT:
				pResult->ValueType=VALUE_TYPE_DOUBLE;
				pResult->ValueDouble=(double)(pLeftValue->ValueInt64-pRightValue->ValueFloat);
				break;
			case VALUE_TYPE_DOUBLE:
				pResult->ValueType=VALUE_TYPE_DOUBLE;
				pResult->ValueDouble=(double)(pLeftValue->ValueInt64-pRightValue->ValueDouble);
				break;
			case VALUE_TYPE_STRING:
				return 3018;
			}
		}
		break;
	case VALUE_TYPE_FLOAT:
		{
			switch(pRightValue->ValueType)
			{
			case VALUE_TYPE_INT:
				pResult->ValueType=VALUE_TYPE_FLOAT;
				pResult->ValueFloat=(float)(pLeftValue->ValueFloat-pRightValue->ValueInt);
				break;
			case VALUE_TYPE_INT64:
				pResult->ValueType=VALUE_TYPE_DOUBLE;
				pResult->ValueDouble=(double)(pLeftValue->ValueFloat-pRightValue->ValueInt64);
				break;
			case VALUE_TYPE_FLOAT:
				pResult->ValueType=VALUE_TYPE_FLOAT;
				pResult->ValueFloat=(float)(pLeftValue->ValueFloat-pRightValue->ValueFloat);
				break;
			case VALUE_TYPE_DOUBLE:
				pResult->ValueType=VALUE_TYPE_DOUBLE;
				pResult->ValueDouble=(double)(pLeftValue->ValueFloat-pRightValue->ValueDouble);
				break;
			case VALUE_TYPE_STRING:
				return 3018;
			}
		}
		break;
	case VALUE_TYPE_DOUBLE:
		{
			switch(pRightValue->ValueType)
			{
			case VALUE_TYPE_INT:
				pResult->ValueType=VALUE_TYPE_DOUBLE;
				pResult->ValueDouble=(double)(pLeftValue->ValueDouble-pRightValue->ValueInt);
				break;
			case VALUE_TYPE_INT64:
				pResult->ValueType=VALUE_TYPE_DOUBLE;
				pResult->ValueDouble=(double)(pLeftValue->ValueDouble-pRightValue->ValueInt64);
				break;
			case VALUE_TYPE_FLOAT:
				pResult->ValueType=VALUE_TYPE_DOUBLE;
				pResult->ValueDouble=(double)(pLeftValue->ValueDouble-pRightValue->ValueFloat);
				break;
			case VALUE_TYPE_DOUBLE:
				pResult->ValueType=VALUE_TYPE_DOUBLE;
				pResult->ValueDouble=(double)(pLeftValue->ValueDouble-pRightValue->ValueDouble);
				break;
			case VALUE_TYPE_STRING:
				return 3018;
			}
		}
		break;
	case VALUE_TYPE_STRING:
		return 3018;			
	}

	pResult->Type=BOLAN_TYPE_VALUE;
	return 0;
	//FUNCTION_END;
	//return 0;
}

int CEasyScriptExecutor::DoMul(ES_BOLAN * pLeftValue,ES_BOLAN * pRightValue,ES_BOLAN * pResult)
{
	//FUNCTION_BEGIN;
	if(pLeftValue==NULL||pRightValue==NULL||pResult==NULL)
		return 4013;
	if(pLeftValue->Type!=BOLAN_TYPE_VARIABLE&&pLeftValue->Type!=BOLAN_TYPE_VALUE)
		return 3016;
	if(pRightValue->Type!=BOLAN_TYPE_VARIABLE&&pRightValue->Type!=BOLAN_TYPE_VALUE)
		return 3017;



	switch(pLeftValue->ValueType)
	{
	case VALUE_TYPE_INT:
		{
			switch(pRightValue->ValueType)
			{
			case VALUE_TYPE_INT:
				pResult->ValueType=VALUE_TYPE_INT;
				pResult->ValueInt=pLeftValue->ValueInt*pRightValue->ValueInt;
				break;
			case VALUE_TYPE_INT64:
				pResult->ValueType=VALUE_TYPE_INT64;
				pResult->ValueInt64=(INT64)(pLeftValue->ValueInt*pRightValue->ValueInt64);
				break;
			case VALUE_TYPE_FLOAT:
				pResult->ValueType=VALUE_TYPE_FLOAT;
				pResult->ValueFloat=(float)(pLeftValue->ValueInt*pRightValue->ValueFloat);
				break;
			case VALUE_TYPE_DOUBLE:
				pResult->ValueType=VALUE_TYPE_FLOAT;
				pResult->ValueDouble=(double)(pLeftValue->ValueInt*pRightValue->ValueDouble);
				break;
			case VALUE_TYPE_STRING:
				return 3018;
			}
		}
		break;
	case VALUE_TYPE_INT64:
		{
			switch(pRightValue->ValueType)
			{
			case VALUE_TYPE_INT:
				pResult->ValueType=VALUE_TYPE_INT64;
				pResult->ValueInt64=(INT64)(pLeftValue->ValueInt64*pRightValue->ValueInt);
				break;
			case VALUE_TYPE_INT64:
				pResult->ValueType=VALUE_TYPE_INT64;
				pResult->ValueInt64=(INT64)(pLeftValue->ValueInt64*pRightValue->ValueInt64);
				break;
			case VALUE_TYPE_FLOAT:
				pResult->ValueType=VALUE_TYPE_DOUBLE;
				pResult->ValueDouble=(double)(pLeftValue->ValueInt64*pRightValue->ValueFloat);
				break;
			case VALUE_TYPE_DOUBLE:
				pResult->ValueType=VALUE_TYPE_DOUBLE;
				pResult->ValueDouble=(double)(pLeftValue->ValueInt64*pRightValue->ValueDouble);
				break;
			case VALUE_TYPE_STRING:
				return 3018;
			}
		}
		break;
	case VALUE_TYPE_FLOAT:
		{
			switch(pRightValue->ValueType)
			{
			case VALUE_TYPE_INT:
				pResult->ValueType=VALUE_TYPE_FLOAT;
				pResult->ValueFloat=(float)(pLeftValue->ValueFloat*pRightValue->ValueInt);
				break;
			case VALUE_TYPE_INT64:
				pResult->ValueType=VALUE_TYPE_DOUBLE;
				pResult->ValueDouble=(double)(pLeftValue->ValueFloat*pRightValue->ValueInt64);
				break;
			case VALUE_TYPE_FLOAT:
				pResult->ValueType=VALUE_TYPE_FLOAT;
				pResult->ValueFloat=(float)(pLeftValue->ValueFloat*pRightValue->ValueFloat);
				break;
			case VALUE_TYPE_DOUBLE:
				pResult->ValueType=VALUE_TYPE_DOUBLE;
				pResult->ValueDouble=(double)(pLeftValue->ValueFloat*pRightValue->ValueDouble);
				break;
			case VALUE_TYPE_STRING:
				return 3018;
			}
		}
		break;
	case VALUE_TYPE_DOUBLE:
		{
			switch(pRightValue->ValueType)
			{
			case VALUE_TYPE_INT:
				pResult->ValueType=VALUE_TYPE_DOUBLE;
				pResult->ValueDouble=(double)(pLeftValue->ValueDouble*pRightValue->ValueInt);
				break;
			case VALUE_TYPE_INT64:
				pResult->ValueType=VALUE_TYPE_DOUBLE;
				pResult->ValueDouble=(double)(pLeftValue->ValueDouble*pRightValue->ValueInt64);
				break;
			case VALUE_TYPE_FLOAT:
				pResult->ValueType=VALUE_TYPE_DOUBLE;
				pResult->ValueDouble=(double)(pLeftValue->ValueDouble*pRightValue->ValueFloat);
				break;
			case VALUE_TYPE_DOUBLE:
				pResult->ValueType=VALUE_TYPE_DOUBLE;
				pResult->ValueDouble=(double)(pLeftValue->ValueDouble*pRightValue->ValueDouble);
				break;
			case VALUE_TYPE_STRING:
				return 3018;
			}
		}
		break;
	case VALUE_TYPE_STRING:
		return 3018;
		break;
	}

	pResult->Type=BOLAN_TYPE_VALUE;
	return 0;
	//FUNCTION_END;
	//return 0;
}

int CEasyScriptExecutor::DoDiv(ES_BOLAN * pLeftValue,ES_BOLAN * pRightValue,ES_BOLAN * pResult)
{
	//FUNCTION_BEGIN;
	if(pLeftValue==NULL||pRightValue==NULL||pResult==NULL)
		return 4013;
	if(pLeftValue->Type!=BOLAN_TYPE_VARIABLE&&pLeftValue->Type!=BOLAN_TYPE_VALUE)
		return 3016;
	if(pRightValue->Type!=BOLAN_TYPE_VARIABLE&&pRightValue->Type!=BOLAN_TYPE_VALUE)
		return 3017;



	switch(pLeftValue->ValueType)
	{
	case VALUE_TYPE_INT:
		{
			switch(pRightValue->ValueType)
			{
			case VALUE_TYPE_INT:
				pResult->ValueType=VALUE_TYPE_INT;
				pResult->ValueInt=pLeftValue->ValueInt/pRightValue->ValueInt;
				break;
			case VALUE_TYPE_INT64:
				pResult->ValueType=VALUE_TYPE_INT64;
				pResult->ValueInt64=(INT64)(pLeftValue->ValueInt/pRightValue->ValueInt64);
				break;
			case VALUE_TYPE_FLOAT:
				pResult->ValueType=VALUE_TYPE_FLOAT;
				pResult->ValueFloat=(float)(pLeftValue->ValueInt/pRightValue->ValueFloat);
				break;
			case VALUE_TYPE_DOUBLE:
				pResult->ValueType=VALUE_TYPE_DOUBLE;
				pResult->ValueDouble=(double)(pLeftValue->ValueInt/pRightValue->ValueDouble);
				break;
			case VALUE_TYPE_STRING:
				return 3018;
			}
		}
		break;
	case VALUE_TYPE_INT64:
		{
			switch(pRightValue->ValueType)
			{
			case VALUE_TYPE_INT:
				pResult->ValueType=VALUE_TYPE_INT64;
				pResult->ValueInt64=(INT64)(pLeftValue->ValueInt64/pRightValue->ValueInt);
				break;
			case VALUE_TYPE_INT64:
				pResult->ValueType=VALUE_TYPE_INT64;
				pResult->ValueInt64=(INT64)(pLeftValue->ValueInt64/pRightValue->ValueInt64);
				break;
			case VALUE_TYPE_FLOAT:
				pResult->ValueType=VALUE_TYPE_DOUBLE;
				pResult->ValueDouble=(double)(pLeftValue->ValueInt64/pRightValue->ValueFloat);
				break;
			case VALUE_TYPE_DOUBLE:
				pResult->ValueType=VALUE_TYPE_DOUBLE;
				pResult->ValueDouble=(double)(pLeftValue->ValueInt64/pRightValue->ValueDouble);
				break;
			case VALUE_TYPE_STRING:
				return 3018;
			}
		}
		break;
	case VALUE_TYPE_FLOAT:
		{
			switch(pRightValue->ValueType)
			{
			case VALUE_TYPE_INT:
				pResult->ValueType=VALUE_TYPE_FLOAT;
				pResult->ValueFloat=(float)(pLeftValue->ValueFloat/pRightValue->ValueInt);
				break;
			case VALUE_TYPE_INT64:
				pResult->ValueType=VALUE_TYPE_DOUBLE;
				pResult->ValueDouble=(double)(pLeftValue->ValueFloat/pRightValue->ValueInt64);
				break;
			case VALUE_TYPE_FLOAT:
				pResult->ValueType=VALUE_TYPE_FLOAT;
				pResult->ValueFloat=(float)(pLeftValue->ValueFloat/pRightValue->ValueFloat);
				break;
			case VALUE_TYPE_DOUBLE:
				pResult->ValueType=VALUE_TYPE_DOUBLE;
				pResult->ValueDouble=(double)(pLeftValue->ValueFloat/pRightValue->ValueDouble);
				break;
			case VALUE_TYPE_STRING:
				return 3018;
			}
		}
		break;
	case VALUE_TYPE_DOUBLE:
		{
			switch(pRightValue->ValueType)
			{
			case VALUE_TYPE_INT:
				pResult->ValueType=VALUE_TYPE_DOUBLE;
				pResult->ValueDouble=(double)(pLeftValue->ValueDouble/pRightValue->ValueInt);
				break;
			case VALUE_TYPE_INT64:
				pResult->ValueType=VALUE_TYPE_DOUBLE;
				pResult->ValueDouble=(double)(pLeftValue->ValueDouble/pRightValue->ValueInt64);
				break;
			case VALUE_TYPE_FLOAT:
				pResult->ValueType=VALUE_TYPE_DOUBLE;
				pResult->ValueDouble=(double)(pLeftValue->ValueDouble/pRightValue->ValueFloat);
				break;
			case VALUE_TYPE_DOUBLE:
				pResult->ValueType=VALUE_TYPE_DOUBLE;
				pResult->ValueDouble=(double)(pLeftValue->ValueDouble/pRightValue->ValueDouble);
				break;
			case VALUE_TYPE_STRING:
				return 3018;
			}
		}
		break;
	case VALUE_TYPE_STRING:
		return 3018;
		break;
	}

	pResult->Type=BOLAN_TYPE_VALUE;
	return 0;
	//FUNCTION_END;
	//return 0;
}


int CEasyScriptExecutor::DoEqu(ES_BOLAN * pLeftValue,ES_BOLAN * pRightValue,ES_BOLAN * pResult)
{
	//FUNCTION_BEGIN;
	if(pLeftValue==NULL||pRightValue==NULL||pResult==NULL)
		return 4013;
	if(pLeftValue->Type!=BOLAN_TYPE_VARIABLE&&pLeftValue->Type!=BOLAN_TYPE_VALUE)
		return 3016;
	if(pRightValue->Type!=BOLAN_TYPE_VARIABLE&&pRightValue->Type!=BOLAN_TYPE_VALUE)
		return 3017;

	

	switch(pLeftValue->ValueType)
	{
	case VALUE_TYPE_INT:
		{
			switch(pRightValue->ValueType)
			{
			case VALUE_TYPE_INT:				
				pResult->ValueInt=(pLeftValue->ValueInt==pRightValue->ValueInt);
				break;
			case VALUE_TYPE_INT64:
				pResult->ValueInt=(pLeftValue->ValueInt==pRightValue->ValueInt64);
				break;
			case VALUE_TYPE_FLOAT:
				pResult->ValueInt=(pLeftValue->ValueInt==pRightValue->ValueFloat);
				break;
			case VALUE_TYPE_DOUBLE:
				pResult->ValueInt=(pLeftValue->ValueInt==pRightValue->ValueDouble);
				break;
			case VALUE_TYPE_STRING:
				return 3004;
			}
		}
		break;
	case VALUE_TYPE_INT64:
		{
			switch(pRightValue->ValueType)
			{
			case VALUE_TYPE_INT:
				pResult->ValueInt=(pLeftValue->ValueInt64==pRightValue->ValueInt);
				break;
			case VALUE_TYPE_INT64:
				pResult->ValueInt=(pLeftValue->ValueInt64==pRightValue->ValueInt64);
				break;
			case VALUE_TYPE_FLOAT:
				pResult->ValueInt=(pLeftValue->ValueInt64==pRightValue->ValueFloat);
				break;
			case VALUE_TYPE_DOUBLE:
				pResult->ValueInt=(pLeftValue->ValueInt64==pRightValue->ValueDouble);
				break;
			case VALUE_TYPE_STRING:
				return 3004;
			}
		}
		break;
	case VALUE_TYPE_FLOAT:
		{
			switch(pRightValue->ValueType)
			{
			case VALUE_TYPE_INT:
				pResult->ValueInt=(pLeftValue->ValueFloat==pRightValue->ValueInt);
				break;
			case VALUE_TYPE_INT64:
				pResult->ValueInt=(pLeftValue->ValueFloat==pRightValue->ValueInt64);
				break;
			case VALUE_TYPE_FLOAT:
				pResult->ValueInt=(pLeftValue->ValueFloat==pRightValue->ValueFloat);
				break;
			case VALUE_TYPE_DOUBLE:
				pResult->ValueInt=(pLeftValue->ValueFloat==pRightValue->ValueDouble);
				break;
			case VALUE_TYPE_STRING:
				return 3004;
			}
		}
		break;
	case VALUE_TYPE_DOUBLE:
		{
			switch(pRightValue->ValueType)
			{
			case VALUE_TYPE_INT:
				pResult->ValueInt=(pLeftValue->ValueDouble==pRightValue->ValueInt);
				break;
			case VALUE_TYPE_INT64:
				pResult->ValueInt=(pLeftValue->ValueDouble==pRightValue->ValueInt64);
				break;
			case VALUE_TYPE_FLOAT:
				pResult->ValueInt=(pLeftValue->ValueDouble==pRightValue->ValueFloat);
				break;
			case VALUE_TYPE_DOUBLE:
				pResult->ValueInt=(pLeftValue->ValueDouble==pRightValue->ValueDouble);
				break;
			case VALUE_TYPE_STRING:
				return 3004;
			}
		}
		break;
	case VALUE_TYPE_STRING:
		{
			switch(pRightValue->ValueType)
			{
			case VALUE_TYPE_INT:
			case VALUE_TYPE_INT64:
			case VALUE_TYPE_FLOAT:
			case VALUE_TYPE_DOUBLE:
				return 3004;
			case VALUE_TYPE_STRING:
				pResult->ValueInt=(pLeftValue->StrValue==pRightValue->StrValue);
				break;
			}
		}
		break;
	}

	pResult->ValueType=VALUE_TYPE_INT;
	pResult->Type=BOLAN_TYPE_VALUE;
	return 0;
	//FUNCTION_END;
	//return 0;
}

int CEasyScriptExecutor::DoDiff(ES_BOLAN * pLeftValue,ES_BOLAN * pRightValue,ES_BOLAN * pResult)
{
	//FUNCTION_BEGIN;
	if(pLeftValue==NULL||pRightValue==NULL||pResult==NULL)
		return 4013;
	if(pLeftValue->Type!=BOLAN_TYPE_VARIABLE&&pLeftValue->Type!=BOLAN_TYPE_VALUE)
		return 3016;
	if(pRightValue->Type!=BOLAN_TYPE_VARIABLE&&pRightValue->Type!=BOLAN_TYPE_VALUE)
		return 3017;

	

	switch(pLeftValue->ValueType)
	{
	case VALUE_TYPE_INT:
		{
			switch(pRightValue->ValueType)
			{
			case VALUE_TYPE_INT:				
				pResult->ValueInt=(pLeftValue->ValueInt!=pRightValue->ValueInt);
				break;
			case VALUE_TYPE_INT64:
				pResult->ValueInt=(pLeftValue->ValueInt!=pRightValue->ValueInt64);
				break;
			case VALUE_TYPE_FLOAT:
				pResult->ValueInt=(pLeftValue->ValueInt!=pRightValue->ValueFloat);
				break;
			case VALUE_TYPE_DOUBLE:
				pResult->ValueInt=(pLeftValue->ValueInt!=pRightValue->ValueDouble);
				break;
			case VALUE_TYPE_STRING:
				return 3004;
			}
		}
		break;
	case VALUE_TYPE_INT64:
		{
			switch(pRightValue->ValueType)
			{
			case VALUE_TYPE_INT:
				pResult->ValueInt=(pLeftValue->ValueInt64!=pRightValue->ValueInt);
				break;
			case VALUE_TYPE_INT64:
				pResult->ValueInt=(pLeftValue->ValueInt64!=pRightValue->ValueInt64);
				break;
			case VALUE_TYPE_FLOAT:
				pResult->ValueInt=(pLeftValue->ValueInt64!=pRightValue->ValueFloat);
				break;
			case VALUE_TYPE_DOUBLE:
				pResult->ValueInt=(pLeftValue->ValueInt64!=pRightValue->ValueDouble);
				break;
			case VALUE_TYPE_STRING:
				return 3004;
			}
		}
		break;
	case VALUE_TYPE_FLOAT:
		{
			switch(pRightValue->ValueType)
			{
			case VALUE_TYPE_INT:
				pResult->ValueInt=(pLeftValue->ValueFloat!=pRightValue->ValueInt);
				break;
			case VALUE_TYPE_INT64:
				pResult->ValueInt=(pLeftValue->ValueFloat!=pRightValue->ValueInt64);
				break;
			case VALUE_TYPE_FLOAT:
				pResult->ValueInt=(pLeftValue->ValueFloat!=pRightValue->ValueFloat);
				break;
			case VALUE_TYPE_DOUBLE:
				pResult->ValueInt=(pLeftValue->ValueFloat!=pRightValue->ValueDouble);
				break;
			case VALUE_TYPE_STRING:
				return 3004;
			}
		}
		break;
	case VALUE_TYPE_DOUBLE:
		{
			switch(pRightValue->ValueType)
			{
			case VALUE_TYPE_INT:
				pResult->ValueInt=(pLeftValue->ValueDouble!=pRightValue->ValueInt);
				break;
			case VALUE_TYPE_INT64:
				pResult->ValueInt=(pLeftValue->ValueDouble!=pRightValue->ValueInt64);
				break;
			case VALUE_TYPE_FLOAT:
				pResult->ValueInt=(pLeftValue->ValueDouble!=pRightValue->ValueFloat);
				break;
			case VALUE_TYPE_DOUBLE:
				pResult->ValueInt=(pLeftValue->ValueDouble!=pRightValue->ValueDouble);
				break;
			case VALUE_TYPE_STRING:
				return 3004;
			}
		}
		break;
	case VALUE_TYPE_STRING:
		{
			switch(pRightValue->ValueType)
			{
			case VALUE_TYPE_INT:
			case VALUE_TYPE_INT64:
			case VALUE_TYPE_FLOAT:
			case VALUE_TYPE_DOUBLE:
				return 3004;
			case VALUE_TYPE_STRING:
				pResult->ValueInt=(pLeftValue->StrValue!=pRightValue->StrValue);
				break;
			}
		}
		break;
	}

	pResult->ValueType=VALUE_TYPE_INT;
	pResult->Type=BOLAN_TYPE_VALUE;
	return 0;
	//FUNCTION_END;
	//return 0;
}

int CEasyScriptExecutor::DoLess(ES_BOLAN * pLeftValue,ES_BOLAN * pRightValue,ES_BOLAN * pResult)
{
	//FUNCTION_BEGIN;
	if(pLeftValue==NULL||pRightValue==NULL||pResult==NULL)
		return 4013;
	if(pLeftValue->Type!=BOLAN_TYPE_VARIABLE&&pLeftValue->Type!=BOLAN_TYPE_VALUE)
		return 3016;
	if(pRightValue->Type!=BOLAN_TYPE_VARIABLE&&pRightValue->Type!=BOLAN_TYPE_VALUE)
		return 3017;

	

	switch(pLeftValue->ValueType)
	{
	case VALUE_TYPE_INT:
		{
			switch(pRightValue->ValueType)
			{
			case VALUE_TYPE_INT:				
				pResult->ValueInt=(pLeftValue->ValueInt<pRightValue->ValueInt);
				break;
			case VALUE_TYPE_INT64:
				pResult->ValueInt=(pLeftValue->ValueInt<pRightValue->ValueInt64);
				break;
			case VALUE_TYPE_FLOAT:
				pResult->ValueInt=(pLeftValue->ValueInt<pRightValue->ValueFloat);
				break;
			case VALUE_TYPE_DOUBLE:
				pResult->ValueInt=(pLeftValue->ValueInt<pRightValue->ValueDouble);
				break;
			case VALUE_TYPE_STRING:
				return 3004;
			}
		}
		break;
	case VALUE_TYPE_INT64:
		{
			switch(pRightValue->ValueType)
			{
			case VALUE_TYPE_INT:
				pResult->ValueInt=(pLeftValue->ValueInt64<pRightValue->ValueInt);
				break;
			case VALUE_TYPE_INT64:
				pResult->ValueInt=(pLeftValue->ValueInt64<pRightValue->ValueInt64);
				break;
			case VALUE_TYPE_FLOAT:
				pResult->ValueInt=(pLeftValue->ValueInt64<pRightValue->ValueFloat);
				break;
			case VALUE_TYPE_DOUBLE:
				pResult->ValueInt=(pLeftValue->ValueInt64<pRightValue->ValueDouble);
				break;
			case VALUE_TYPE_STRING:
				return 3004;
			}
		}
		break;
	case VALUE_TYPE_FLOAT:
		{
			switch(pRightValue->ValueType)
			{
			case VALUE_TYPE_INT:
				pResult->ValueInt=(pLeftValue->ValueFloat<pRightValue->ValueInt);
				break;
			case VALUE_TYPE_INT64:
				pResult->ValueInt=(pLeftValue->ValueFloat<pRightValue->ValueInt64);
				break;
			case VALUE_TYPE_FLOAT:
				pResult->ValueInt=(pLeftValue->ValueFloat<pRightValue->ValueFloat);
				break;
			case VALUE_TYPE_DOUBLE:
				pResult->ValueInt=(pLeftValue->ValueFloat<pRightValue->ValueDouble);
				break;
			case VALUE_TYPE_STRING:
				return 3004;
			}
		}
		break;
	case VALUE_TYPE_DOUBLE:
		{
			switch(pRightValue->ValueType)
			{
			case VALUE_TYPE_INT:
				pResult->ValueInt=(pLeftValue->ValueDouble<pRightValue->ValueInt);
				break;
			case VALUE_TYPE_INT64:
				pResult->ValueInt=(pLeftValue->ValueDouble<pRightValue->ValueInt64);
				break;
			case VALUE_TYPE_FLOAT:
				pResult->ValueInt=(pLeftValue->ValueDouble<pRightValue->ValueFloat);
				break;
			case VALUE_TYPE_DOUBLE:
				pResult->ValueInt=(pLeftValue->ValueDouble<pRightValue->ValueDouble);
				break;
			case VALUE_TYPE_STRING:
				return 3004;
			}
		}
		break;
	case VALUE_TYPE_STRING:
		{
			switch(pRightValue->ValueType)
			{
			case VALUE_TYPE_INT:
			case VALUE_TYPE_INT64:
			case VALUE_TYPE_FLOAT:
			case VALUE_TYPE_DOUBLE:
				return 3004;
			case VALUE_TYPE_STRING:
				pResult->ValueInt=(pLeftValue->StrValue<pRightValue->StrValue);
				break;
			}
		}
		break;
	}

	pResult->ValueType=VALUE_TYPE_INT;
	pResult->Type=BOLAN_TYPE_VALUE;
	return 0;
	//FUNCTION_END;
	//return 0;
}


int CEasyScriptExecutor::DoMore(ES_BOLAN * pLeftValue,ES_BOLAN * pRightValue,ES_BOLAN * pResult)
{
	//FUNCTION_BEGIN;
	if(pLeftValue==NULL||pRightValue==NULL||pResult==NULL)
		return 4013;
	if(pLeftValue->Type!=BOLAN_TYPE_VARIABLE&&pLeftValue->Type!=BOLAN_TYPE_VALUE)
		return 3016;
	if(pRightValue->Type!=BOLAN_TYPE_VARIABLE&&pRightValue->Type!=BOLAN_TYPE_VALUE)
		return 3017;

	

	switch(pLeftValue->ValueType)
	{
	case VALUE_TYPE_INT:
		{
			switch(pRightValue->ValueType)
			{
			case VALUE_TYPE_INT:				
				pResult->ValueInt=(pLeftValue->ValueInt>pRightValue->ValueInt);
				break;
			case VALUE_TYPE_INT64:
				pResult->ValueInt=(pLeftValue->ValueInt>pRightValue->ValueInt64);
				break;
			case VALUE_TYPE_FLOAT:
				pResult->ValueInt=(pLeftValue->ValueInt>pRightValue->ValueFloat);
				break;
			case VALUE_TYPE_DOUBLE:
				pResult->ValueInt=(pLeftValue->ValueInt>pRightValue->ValueDouble);
				break;
			case VALUE_TYPE_STRING:
				return 3004;
			}
		}
		break;
	case VALUE_TYPE_INT64:
		{
			switch(pRightValue->ValueType)
			{
			case VALUE_TYPE_INT:
				pResult->ValueInt=(pLeftValue->ValueInt64>pRightValue->ValueInt);
				break;
			case VALUE_TYPE_INT64:
				pResult->ValueInt=(pLeftValue->ValueInt64>pRightValue->ValueInt64);
				break;
			case VALUE_TYPE_FLOAT:
				pResult->ValueInt=(pLeftValue->ValueInt64>pRightValue->ValueFloat);
				break;
			case VALUE_TYPE_DOUBLE:
				pResult->ValueInt=(pLeftValue->ValueInt64>pRightValue->ValueDouble);
				break;
			case VALUE_TYPE_STRING:
				return 3004;
			}
		}
		break;
	case VALUE_TYPE_FLOAT:
		{
			switch(pRightValue->ValueType)
			{
			case VALUE_TYPE_INT:
				pResult->ValueInt=(pLeftValue->ValueFloat>pRightValue->ValueInt);
				break;
			case VALUE_TYPE_INT64:
				pResult->ValueInt=(pLeftValue->ValueFloat>pRightValue->ValueInt64);
				break;
			case VALUE_TYPE_FLOAT:
				pResult->ValueInt=(pLeftValue->ValueFloat>pRightValue->ValueFloat);
				break;
			case VALUE_TYPE_DOUBLE:
				pResult->ValueInt=(pLeftValue->ValueFloat>pRightValue->ValueDouble);
				break;
			case VALUE_TYPE_STRING:
				return 3004;
			}
		}
		break;
	case VALUE_TYPE_DOUBLE:
		{
			switch(pRightValue->ValueType)
			{
			case VALUE_TYPE_INT:
				pResult->ValueInt=(pLeftValue->ValueDouble>pRightValue->ValueInt);
				break;
			case VALUE_TYPE_INT64:
				pResult->ValueInt=(pLeftValue->ValueDouble>pRightValue->ValueInt64);
				break;
			case VALUE_TYPE_FLOAT:
				pResult->ValueInt=(pLeftValue->ValueDouble>pRightValue->ValueFloat);
				break;
			case VALUE_TYPE_DOUBLE:
				pResult->ValueInt=(pLeftValue->ValueDouble>pRightValue->ValueDouble);
				break;
			case VALUE_TYPE_STRING:
				return 3004;
			}
		}
		break;
	case VALUE_TYPE_STRING:
		{
			switch(pRightValue->ValueType)
			{
			case VALUE_TYPE_INT:
			case VALUE_TYPE_INT64:
			case VALUE_TYPE_FLOAT:
			case VALUE_TYPE_DOUBLE:
				return 3004;
			case VALUE_TYPE_STRING:
				pResult->ValueInt=(pLeftValue->StrValue>pRightValue->StrValue);
				break;
			}
		}
		break;
	}

	pResult->ValueType=VALUE_TYPE_INT;
	pResult->Type=BOLAN_TYPE_VALUE;
	return 0;
	//FUNCTION_END;
	//return 0;
}


int CEasyScriptExecutor::DoLessEqu(ES_BOLAN * pLeftValue,ES_BOLAN * pRightValue,ES_BOLAN * pResult)
{
	//FUNCTION_BEGIN;
	if(pLeftValue==NULL||pRightValue==NULL||pResult==NULL)
		return 4013;
	if(pLeftValue->Type!=BOLAN_TYPE_VARIABLE&&pLeftValue->Type!=BOLAN_TYPE_VALUE)
		return 3016;
	if(pRightValue->Type!=BOLAN_TYPE_VARIABLE&&pRightValue->Type!=BOLAN_TYPE_VALUE)
		return 3017;

	

	switch(pLeftValue->ValueType)
	{
	case VALUE_TYPE_INT:
		{
			switch(pRightValue->ValueType)
			{
			case VALUE_TYPE_INT:				
				pResult->ValueInt=(pLeftValue->ValueInt<=pRightValue->ValueInt);
				break;
			case VALUE_TYPE_INT64:
				pResult->ValueInt=(pLeftValue->ValueInt<=pRightValue->ValueInt64);
				break;
			case VALUE_TYPE_FLOAT:
				pResult->ValueInt=(pLeftValue->ValueInt<=pRightValue->ValueFloat);
				break;
			case VALUE_TYPE_DOUBLE:
				pResult->ValueInt=(pLeftValue->ValueInt<=pRightValue->ValueDouble);
				break;
			case VALUE_TYPE_STRING:
				return 3004;
			}
		}
		break;
	case VALUE_TYPE_INT64:
		{
			switch(pRightValue->ValueType)
			{
			case VALUE_TYPE_INT:
				pResult->ValueInt=(pLeftValue->ValueInt64<=pRightValue->ValueInt);
				break;
			case VALUE_TYPE_INT64:
				pResult->ValueInt=(pLeftValue->ValueInt64<=pRightValue->ValueInt64);
				break;
			case VALUE_TYPE_FLOAT:
				pResult->ValueInt=(pLeftValue->ValueInt64<=pRightValue->ValueFloat);
				break;
			case VALUE_TYPE_DOUBLE:
				pResult->ValueInt=(pLeftValue->ValueInt64<=pRightValue->ValueDouble);
				break;
			case VALUE_TYPE_STRING:
				return 3004;
			}
		}
		break;
	case VALUE_TYPE_FLOAT:
		{
			switch(pRightValue->ValueType)
			{
			case VALUE_TYPE_INT:
				pResult->ValueInt=(pLeftValue->ValueFloat<=pRightValue->ValueInt);
				break;
			case VALUE_TYPE_INT64:
				pResult->ValueInt=(pLeftValue->ValueFloat<=pRightValue->ValueInt64);
				break;
			case VALUE_TYPE_FLOAT:
				pResult->ValueInt=(pLeftValue->ValueFloat<=pRightValue->ValueFloat);
				break;
			case VALUE_TYPE_DOUBLE:
				pResult->ValueInt=(pLeftValue->ValueFloat<=pRightValue->ValueDouble);
				break;
			case VALUE_TYPE_STRING:
				return 3004;
			}
		}
		break;
	case VALUE_TYPE_DOUBLE:
		{
			switch(pRightValue->ValueType)
			{
			case VALUE_TYPE_INT:
				pResult->ValueInt=(pLeftValue->ValueDouble<=pRightValue->ValueInt);
				break;
			case VALUE_TYPE_INT64:
				pResult->ValueInt=(pLeftValue->ValueDouble<=pRightValue->ValueInt64);
				break;
			case VALUE_TYPE_FLOAT:
				pResult->ValueInt=(pLeftValue->ValueDouble<=pRightValue->ValueFloat);
				break;
			case VALUE_TYPE_DOUBLE:
				pResult->ValueInt=(pLeftValue->ValueDouble<=pRightValue->ValueDouble);
				break;
			case VALUE_TYPE_STRING:
				return 3004;
			}
		}
		break;
	case VALUE_TYPE_STRING:
		{
			switch(pRightValue->ValueType)
			{
			case VALUE_TYPE_INT:
			case VALUE_TYPE_INT64:
			case VALUE_TYPE_FLOAT:
			case VALUE_TYPE_DOUBLE:
				return 3004;
			case VALUE_TYPE_STRING:
				pResult->ValueInt=(pLeftValue->StrValue.Compare(pRightValue->StrValue)<=0);
				break;
			}
		}
		break;
	}

	pResult->ValueType=VALUE_TYPE_INT;
	pResult->Type=BOLAN_TYPE_VALUE;
	return 0;
	//FUNCTION_END;
	//return 0;
}


int CEasyScriptExecutor::DoMoreEqu(ES_BOLAN * pLeftValue,ES_BOLAN * pRightValue,ES_BOLAN * pResult)
{
	//FUNCTION_BEGIN;
	if(pLeftValue==NULL||pRightValue==NULL||pResult==NULL)
		return 4013;
	if(pLeftValue->Type!=BOLAN_TYPE_VARIABLE&&pLeftValue->Type!=BOLAN_TYPE_VALUE)
		return 3016;
	if(pRightValue->Type!=BOLAN_TYPE_VARIABLE&&pRightValue->Type!=BOLAN_TYPE_VALUE)
		return 3017;

	

	switch(pLeftValue->ValueType)
	{
	case VALUE_TYPE_INT:
		{
			switch(pRightValue->ValueType)
			{
			case VALUE_TYPE_INT:				
				pResult->ValueInt=(pLeftValue->ValueInt>=pRightValue->ValueInt);
				break;
			case VALUE_TYPE_INT64:
				pResult->ValueInt=(pLeftValue->ValueInt>=pRightValue->ValueInt64);
				break;
			case VALUE_TYPE_FLOAT:
				pResult->ValueInt=(pLeftValue->ValueInt>=pRightValue->ValueFloat);
				break;
			case VALUE_TYPE_DOUBLE:
				pResult->ValueInt=(pLeftValue->ValueInt>=pRightValue->ValueDouble);
				break;
			case VALUE_TYPE_STRING:
				return 3004;
			}
		}
		break;
	case VALUE_TYPE_INT64:
		{
			switch(pRightValue->ValueType)
			{
			case VALUE_TYPE_INT:
				pResult->ValueInt=(pLeftValue->ValueInt64>=pRightValue->ValueInt);
				break;
			case VALUE_TYPE_INT64:
				pResult->ValueInt=(pLeftValue->ValueInt64>=pRightValue->ValueInt64);
				break;
			case VALUE_TYPE_FLOAT:
				pResult->ValueInt=(pLeftValue->ValueInt64>=pRightValue->ValueFloat);
				break;
			case VALUE_TYPE_DOUBLE:
				pResult->ValueInt=(pLeftValue->ValueInt64>=pRightValue->ValueDouble);
				break;
			case VALUE_TYPE_STRING:
				return 3004;
			}
		}
		break;
	case VALUE_TYPE_FLOAT:
		{
			switch(pRightValue->ValueType)
			{
			case VALUE_TYPE_INT:
				pResult->ValueInt=(pLeftValue->ValueFloat>=pRightValue->ValueInt);
				break;
			case VALUE_TYPE_INT64:
				pResult->ValueInt=(pLeftValue->ValueFloat>=pRightValue->ValueInt64);
				break;
			case VALUE_TYPE_FLOAT:
				pResult->ValueInt=(pLeftValue->ValueFloat>=pRightValue->ValueFloat);
				break;
			case VALUE_TYPE_DOUBLE:
				pResult->ValueInt=(pLeftValue->ValueFloat>=pRightValue->ValueDouble);
				break;
			case VALUE_TYPE_STRING:
				return 3004;
			}
		}
		break;
	case VALUE_TYPE_DOUBLE:
		{
			switch(pRightValue->ValueType)
			{
			case VALUE_TYPE_INT:
				pResult->ValueInt=(pLeftValue->ValueDouble>=pRightValue->ValueInt);
				break;
			case VALUE_TYPE_INT64:
				pResult->ValueInt=(pLeftValue->ValueDouble>=pRightValue->ValueInt64);
				break;
			case VALUE_TYPE_FLOAT:
				pResult->ValueInt=(pLeftValue->ValueDouble>=pRightValue->ValueFloat);
				break;
			case VALUE_TYPE_DOUBLE:
				pResult->ValueInt=(pLeftValue->ValueDouble>=pRightValue->ValueDouble);
				break;
			case VALUE_TYPE_STRING:
				return 3004;
			}
		}
		break;
	case VALUE_TYPE_STRING:
		{
			switch(pRightValue->ValueType)
			{
			case VALUE_TYPE_INT:
			case VALUE_TYPE_INT64:
			case VALUE_TYPE_FLOAT:
			case VALUE_TYPE_DOUBLE:
				return 3004;
			case VALUE_TYPE_STRING:
				pResult->ValueInt=(pLeftValue->StrValue.Compare(pRightValue->StrValue)>=0);
				break;
			}
		}
		break;
	}

	pResult->ValueType=VALUE_TYPE_INT;
	pResult->Type=BOLAN_TYPE_VALUE;
	return 0;
	//FUNCTION_END;
	//return 0;
}

int CEasyScriptExecutor::DoNeg(ES_BOLAN * pValue,ES_BOLAN * pResult)
{
	//FUNCTION_BEGIN;
	if(pValue==NULL||pResult==NULL)
		return 4013;
	if(pValue->Type!=BOLAN_TYPE_VARIABLE&&pValue->Type!=BOLAN_TYPE_VALUE)
		return 3016;
	if(pValue->Type!=BOLAN_TYPE_VARIABLE&&pValue->Type!=BOLAN_TYPE_VALUE)
		return 3017;

	
	switch(pValue->ValueType)
	{
	case VALUE_TYPE_INT:
		pResult->ValueInt=-pValue->ValueInt;
		break;
	case VALUE_TYPE_INT64:		
		pResult->ValueInt64=-pValue->ValueInt64;
		break;
	case VALUE_TYPE_FLOAT:		
		pResult->ValueFloat=-pValue->ValueFloat;
		break;
	case VALUE_TYPE_DOUBLE:		
		pResult->ValueDouble=-pValue->ValueDouble;
		break;
	case VALUE_TYPE_STRING:
		return 3018;
	}

	pResult->Type=BOLAN_TYPE_VALUE;
	pResult->ValueType=pValue->ValueType;

	return 0;
	//FUNCTION_END;
	//return 0;

}

int CEasyScriptExecutor::DoAnd(ES_BOLAN * pLeftValue,ES_BOLAN * pRightValue,ES_BOLAN * pResult)
{
	//FUNCTION_BEGIN;
	if(pLeftValue==NULL||pRightValue==NULL||pResult==NULL)
		return 4013;
	if(pLeftValue->Type!=BOLAN_TYPE_VARIABLE&&pLeftValue->Type!=BOLAN_TYPE_VALUE)
		return 3016;
	if(pRightValue->Type!=BOLAN_TYPE_VARIABLE&&pRightValue->Type!=BOLAN_TYPE_VALUE)
		return 3017;

	

	switch(pLeftValue->ValueType)
	{
	case VALUE_TYPE_INT:
		{
			switch(pRightValue->ValueType)
			{
			case VALUE_TYPE_INT:				
				pResult->ValueInt=(pLeftValue->ValueInt&&pRightValue->ValueInt);
				break;
			case VALUE_TYPE_INT64:
				pResult->ValueInt=(pLeftValue->ValueInt&&pRightValue->ValueInt64);
				break;
			case VALUE_TYPE_FLOAT:
				pResult->ValueInt=(pLeftValue->ValueInt&&pRightValue->ValueFloat);
				break;
			case VALUE_TYPE_DOUBLE:
				pResult->ValueInt=(pLeftValue->ValueInt&&pRightValue->ValueDouble);
				break;
			case VALUE_TYPE_STRING:
				return 3018;
			}
		}
		break;
	case VALUE_TYPE_INT64:
		{
			switch(pRightValue->ValueType)
			{
			case VALUE_TYPE_INT:
				pResult->ValueInt=(pLeftValue->ValueInt64&&pRightValue->ValueInt);
				break;
			case VALUE_TYPE_INT64:
				pResult->ValueInt=(pLeftValue->ValueInt64&&pRightValue->ValueInt64);
				break;
			case VALUE_TYPE_FLOAT:
				pResult->ValueInt=(pLeftValue->ValueInt64&&pRightValue->ValueFloat);
				break;
			case VALUE_TYPE_DOUBLE:
				pResult->ValueInt=(pLeftValue->ValueInt64&&pRightValue->ValueDouble);
				break;
			case VALUE_TYPE_STRING:
				return 3018;
			}
		}
		break;
	case VALUE_TYPE_FLOAT:
		{
			switch(pRightValue->ValueType)
			{
			case VALUE_TYPE_INT:
				pResult->ValueInt=(pLeftValue->ValueFloat&&pRightValue->ValueInt);
				break;
			case VALUE_TYPE_INT64:
				pResult->ValueInt=(pLeftValue->ValueFloat&&pRightValue->ValueInt64);
				break;
			case VALUE_TYPE_FLOAT:
				pResult->ValueInt=(pLeftValue->ValueFloat&&pRightValue->ValueFloat);
				break;
			case VALUE_TYPE_DOUBLE:
				pResult->ValueInt=(pLeftValue->ValueFloat&&pRightValue->ValueDouble);
				break;
			case VALUE_TYPE_STRING:
				return 3018;
			}
		}
		break;
	case VALUE_TYPE_DOUBLE:
		{
			switch(pRightValue->ValueType)
			{
			case VALUE_TYPE_INT:
				pResult->ValueInt=(pLeftValue->ValueDouble&&pRightValue->ValueInt);
				break;
			case VALUE_TYPE_INT64:
				pResult->ValueInt=(pLeftValue->ValueDouble&&pRightValue->ValueInt64);
				break;
			case VALUE_TYPE_FLOAT:
				pResult->ValueInt=(pLeftValue->ValueDouble&&pRightValue->ValueFloat);
				break;
			case VALUE_TYPE_DOUBLE:
				pResult->ValueInt=(pLeftValue->ValueDouble&&pRightValue->ValueDouble);
				break;
			case VALUE_TYPE_STRING:
				return 3018;
			}
		}
		break;
	case VALUE_TYPE_STRING:
		return 3018;
	}

	pResult->ValueType=VALUE_TYPE_INT;
	pResult->Type=BOLAN_TYPE_VALUE;
	return 0;
	//FUNCTION_END;
	//return 0;
}

int CEasyScriptExecutor::DoOr(ES_BOLAN * pLeftValue,ES_BOLAN * pRightValue,ES_BOLAN * pResult)
{
	//FUNCTION_BEGIN;
	if(pLeftValue==NULL||pRightValue==NULL||pResult==NULL)
		return 4013;
	if(pLeftValue->Type!=BOLAN_TYPE_VARIABLE&&pLeftValue->Type!=BOLAN_TYPE_VALUE)
		return 3016;
	if(pRightValue->Type!=BOLAN_TYPE_VARIABLE&&pRightValue->Type!=BOLAN_TYPE_VALUE)
		return 3017;

	

	switch(pLeftValue->ValueType)
	{
	case VALUE_TYPE_INT:
		{
			switch(pRightValue->ValueType)
			{
			case VALUE_TYPE_INT:				
				pResult->ValueInt=(pLeftValue->ValueInt||pRightValue->ValueInt);
				break;
			case VALUE_TYPE_INT64:
				pResult->ValueInt=(pLeftValue->ValueInt||pRightValue->ValueInt64);
				break;
			case VALUE_TYPE_FLOAT:
				pResult->ValueInt=(pLeftValue->ValueInt||pRightValue->ValueFloat);
				break;
			case VALUE_TYPE_DOUBLE:
				pResult->ValueInt=(pLeftValue->ValueInt||pRightValue->ValueDouble);
				break;
			case VALUE_TYPE_STRING:
				return 3018;
			}
		}
		break;
	case VALUE_TYPE_INT64:
		{
			switch(pRightValue->ValueType)
			{
			case VALUE_TYPE_INT:
				pResult->ValueInt=(pLeftValue->ValueInt64||pRightValue->ValueInt);
				break;
			case VALUE_TYPE_INT64:
				pResult->ValueInt=(pLeftValue->ValueInt64||pRightValue->ValueInt64);
				break;
			case VALUE_TYPE_FLOAT:
				pResult->ValueInt=(pLeftValue->ValueInt64||pRightValue->ValueFloat);
				break;
			case VALUE_TYPE_DOUBLE:
				pResult->ValueInt=(pLeftValue->ValueInt64||pRightValue->ValueDouble);
				break;
			case VALUE_TYPE_STRING:
				return 3018;
			}
		}
		break;
	case VALUE_TYPE_FLOAT:
		{
			switch(pRightValue->ValueType)
			{
			case VALUE_TYPE_INT:
				pResult->ValueInt=(pLeftValue->ValueFloat||pRightValue->ValueInt);
				break;
			case VALUE_TYPE_INT64:
				pResult->ValueInt=(pLeftValue->ValueFloat||pRightValue->ValueInt64);
				break;
			case VALUE_TYPE_FLOAT:
				pResult->ValueInt=(pLeftValue->ValueFloat||pRightValue->ValueFloat);
				break;
			case VALUE_TYPE_DOUBLE:
				pResult->ValueInt=(pLeftValue->ValueFloat||pRightValue->ValueDouble);
				break;
			case VALUE_TYPE_STRING:
				return 3018;
			}
		}
		break;
	case VALUE_TYPE_DOUBLE:
		{
			switch(pRightValue->ValueType)
			{
			case VALUE_TYPE_INT:
				pResult->ValueInt=(pLeftValue->ValueDouble||pRightValue->ValueInt);
				break;
			case VALUE_TYPE_INT64:
				pResult->ValueInt=(pLeftValue->ValueDouble||pRightValue->ValueInt64);
				break;
			case VALUE_TYPE_FLOAT:
				pResult->ValueInt=(pLeftValue->ValueDouble||pRightValue->ValueFloat);
				break;
			case VALUE_TYPE_DOUBLE:
				pResult->ValueInt=(pLeftValue->ValueDouble||pRightValue->ValueDouble);
				break;
			case VALUE_TYPE_STRING:
				return 3018;
			}
		}
		break;
	case VALUE_TYPE_STRING:
		return 3018;
	}

	pResult->Type=BOLAN_TYPE_VALUE;
	pResult->ValueType=VALUE_TYPE_INT;
	return 0;
	//FUNCTION_END;
	//return 0;
}


int CEasyScriptExecutor::DoNot(ES_BOLAN * pValue,ES_BOLAN * pResult)
{
	//FUNCTION_BEGIN;
	if(pValue==NULL||pResult==NULL)
		return 4013;
	if(pValue->Type!=BOLAN_TYPE_VARIABLE&&pValue->Type!=BOLAN_TYPE_VALUE)
		return 3016;
	if(pValue->Type!=BOLAN_TYPE_VARIABLE&&pValue->Type!=BOLAN_TYPE_VALUE)
		return 3017;

	
	switch(pValue->ValueType)
	{
	case VALUE_TYPE_INT:
		pResult->ValueInt=!pValue->ValueInt;
		break;
	case VALUE_TYPE_INT64:		
		pResult->ValueInt=!pValue->ValueInt64;
		break;
	case VALUE_TYPE_FLOAT:		
		pResult->ValueInt=!pValue->ValueFloat;
		break;
	case VALUE_TYPE_DOUBLE:		
		pResult->ValueInt=!pValue->ValueDouble;
		break;
	case VALUE_TYPE_STRING:
		return 3018;
	}

	pResult->Type=BOLAN_TYPE_VALUE;
	pResult->ValueType=VALUE_TYPE_INT;
	return 0;
	//FUNCTION_END;
	//return 0;

}

int CEasyScriptExecutor::IsZero(ES_BOLAN * pValue)
{
	//FUNCTION_BEGIN;
	if(pValue==NULL)
		return 4013;
	if(pValue->Type!=BOLAN_TYPE_VARIABLE&&pValue->Type!=BOLAN_TYPE_VALUE)
		return 3016;
	switch(pValue->ValueType)
	{
	case VALUE_TYPE_INT:
		if(pValue->ValueInt)
			return -1;
		else
			return 0;
		break;
	case VALUE_TYPE_INT64:
		if(pValue->ValueInt64)
			return -1;
		else
			return 0;
		break;
	case VALUE_TYPE_FLOAT:
		if(pValue->ValueFloat)
			return -1;
		else
			return 0;
		break;
	case VALUE_TYPE_DOUBLE:
		if(pValue->ValueDouble)
			return -1;
		else
			return 0;
		break;
	default:
		return 3018;
	}
	//FUNCTION_END;
	//return 0;
}
