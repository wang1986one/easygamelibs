/****************************************************************************/
/*                                                                          */
/*      文件名:    EasyScriptExecutor.cpp                                   */
/*      创建日期:  2010年08月26日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#include "StdAfx.h"



LPCTSTR KEYWORD_STRINGS[KW_MAX]=
{"END","IF","THEN","ELSE","ELSEIF","ENDIF","WHILE","DO","ENDWHILE","BREAK","CONTINUE","GOTO",":",";",
"DECLARE","INT","INT64","FLOAT","DOUBLE","STRING"};





char * ESGetErrorMsg(int ErrCode)
{
	switch(ErrCode)
	{
	case -1:
		return "执行中断";
		break;
	case 0:
		return "执行成功";
		break;
	case 1001:
		return "字符串常量过长";
		break;
	case 1002:
		return "缺少字符串结尾符";
		break;
	case 1003:
		return "标识符过长";
		break;
	case 1005:
		return "非法字符";
		break;
	case 2001:
		return "有IF而缺少ENDIF";
		break;
	case 2002:
		return "有ELSE而缺少ENDIF";
		break;
	case 2003:
		return "有IF或ELSEIF而缺少THEN";
		break;
	case 2004:
		return "有ELSEIF而缺少ENDIF";
		break;
	case 2010:
	case 2011:
		return "有ENDWHILE而缺少WHILE";
		break;
	case 2012:
		return "BREAK必须出现在WHILE循环中";
		break;
	case 2013:
		return "CONTINUE必须出现在WHILE循环中";
		break;
	case 2014:
		return "GOTO语句后面缺少标记";
		break;
	case 2015:
		return "GOTO语句后面的标记未定义";
		break;
	case 3001:
		return "赋值目标不是变量";
		break;	
	case 3002:
		return "字符串不能进行减法操作";
		break;
	case 3003:
		return "字符串不能进行乘法操作";
		break;
	case 3004:
		return "操作符前后的变量或常量类型不兼容";
		break;
	case 3005:
		return "字符串不能进行除法操作";
		break;
	case 3006:
		return "字符串不能进行取负数操作";
		break;
	case 3007:
		return "函数参数不足";
		break;
	case 3008:
		return "非法的函数参数";
		break;
	case 3009:
		return "函数执行错误";
		break;
	case 3010:
		return "AND不能对字符串使用";
	case 3011:
		return "OR不能对字符串使用";
	case 3012:
		return "NOT不能对字符串使用";
	case 3013:
		return "堆栈溢出";
	case 3014:
		return "中断溢出";
	case 3015:
		return "赋值的源不是变量或值";
	case 3016:
		return "第一操作数必须是变量或值";
	case 3017:
		return "第二操作数必须是变量或值";
	case 3018:
		return "操作符不支持此操作数类型";
	case 4010:	
		return "定义变量错误";		
	case 4012:
		return "变量未定义";	
	case 4013:
		return "操作符参数不足";
	case 5001:
		return "中断不存在";
	case 6001:
		return "未设置变量列表";
	case 6002:
		return "未设置函数列表";
	case 6003:
		return "未设置脚本代码";
	}
	return "未知错误";
}

CEasyScriptExecutor::CEasyScriptExecutor(void)
{

}

CEasyScriptExecutor::~CEasyScriptExecutor(void)
{

}

int CEasyScriptExecutor::ExecScript(CESThread& ESThread,int StartPos,int EndPos)
{

	int ReturnCode;	
	bool IsAfterInterrupt=false;
	int OldPos;
	int SavedPos;
	CESBolanStack *		pScript=ESThread.GetScript();

	
	//处理中断恢复
	if(ESThread.IsInInterrupt())
	{	
		ReturnCode=CaculateBolanExpression(ESThread,StartPos);
		if(ReturnCode)
		{
			ESThread.SetResultCode(ReturnCode);
			return ReturnCode;
		}
		if(ESThread.GetInterruptRecentControlPos()>=0)
		{
			IsAfterInterrupt=true;
			SavedPos=StartPos;		
			StartPos=ESThread.GetInterruptRecentControlPos();
		}
	}

	while(StartPos<pScript->GetSize())
	{
		ESThread.SetLastLine(pScript->GetAt(StartPos)->Line);
		OldPos=StartPos;
		switch(pScript->GetAt(StartPos)->Type)
		{
		case BOLAN_TYPE_VALUE:
		case BOLAN_TYPE_OPERATOR:
		case BOLAN_TYPE_FUNCTION:		
		case BOLAN_TYPE_VARIABLE:
			ESThread.SetInterruptRecentControlPos(-1);
			ReturnCode=CaculateBolanExpression(ESThread,StartPos);
			if(ReturnCode)
			{
				ESThread.SetResultCode(ReturnCode);
				return ReturnCode;
			}
			break;
		case BOLAN_TYPE_KEYWORD:
			switch((int)(pScript->GetAt(StartPos)->Index))
			{			
			case KW_END:
				StartPos++;
				ESThread.SetResultCode(0);
				return 0;
			case KW_IF:			
			case KW_ELSEIF:
				{
					StartPos++;
					if(IsAfterInterrupt)
					{
						StartPos=SavedPos;
						IsAfterInterrupt=false;
					}
					else
					{
						ESThread.SetInterruptRecentControlPos(OldPos);
						ReturnCode=CaculateBolanExpression(ESThread,StartPos);
						if(ReturnCode)
						{
							ESThread.SetResultCode(ReturnCode);
							return ReturnCode;
						}
					}	
					//校验Then关键字
					int ThenPos=FindCoupleIF(pScript,KW_THEN,KW_IF,KW_ENDIF,StartPos,KW_ENDIF);
					if(ThenPos<0)
					{
						ESThread.SetResultCode(2003);
						return 2003;
					}
					//查找配对的ElseIF、Else、EndIF关键词
					int ElseIfPos=FindCoupleIF(pScript,KW_ELSEIF,KW_IF,KW_ENDIF,StartPos,KW_ENDIF);
					int ElsePos=FindCoupleIF(pScript,KW_ELSE,KW_IF,KW_ENDIF,StartPos,KW_ENDIF);
					int EndIfPos=FindCoupleIF(pScript,KW_ENDIF,KW_IF,KW_ENDIF,StartPos,-1);				
					if(ESThread.GetResult().ValueInt)
					{
						//执行代码段
						StartPos++;		
						int EndPos=0;
						if(ElseIfPos>=0)
							EndPos=ElseIfPos;
						else if(ElsePos>=0)
							EndPos=ElsePos+1;
						else if(EndIfPos>=0)
							EndPos=EndIfPos+1;
						else
						{
							ESThread.SetResultCode(2001);
							return 2001;
						}						
						ReturnCode=ExecScript(ESThread,StartPos,EndPos);
						if(ReturnCode)
						{
							ESThread.SetResultCode(ReturnCode);
							return ReturnCode;
						}
						StartPos=EndIfPos+1;
					}
					else
					{				
						//跳过代码段
						if(ElseIfPos>=0)
							StartPos=ElseIfPos;
						else if(ElsePos>=0)
							StartPos=ElsePos+1;
						else if(EndIfPos>=0)
							StartPos=EndIfPos+1;
						else
						{
							ESThread.SetResultCode(2001);
							return 2001;
						}
					}
				}
				break;
			case KW_THEN:
				StartPos++;
				break;
			case KW_ELSE:
				StartPos++;					
				break;				
			case KW_ENDIF:
				StartPos++;
				break;
			case KW_WHILE:				
				StartPos++;			
				if(IsAfterInterrupt)
				{
					StartPos=SavedPos;
					IsAfterInterrupt=false;
				}
				else
				{
					ESThread.SetInterruptRecentControlPos(OldPos);
					ReturnCode=CaculateBolanExpression(ESThread,StartPos);
					if(ReturnCode)
					{
						ESThread.SetResultCode(ReturnCode);
						return ReturnCode;
					}
				}
				if(ESThread.GetResult().ValueInt==0)
				{
					int EndWhilePos=FindCoupleKeyWord(pScript,KW_ENDWHILE,KW_WHILE,StartPos);
					if(EndWhilePos<0)
					{
						ESThread.SetResultCode(2010);
						return 2010;
					}
					StartPos=EndWhilePos+1;					
				}				
				break;
			case KW_DO:
				StartPos++;
				break;
			case KW_ENDWHILE:
				{
					if(StartPos==0)
					{
						ESThread.SetResultCode(2014);
						return 2014;
					}
					int WhilePos=FindCoupleKeyWordReverse(pScript,KW_WHILE,KW_ENDWHILE,StartPos-1);
					if(WhilePos<0)
					{
						ESThread.SetResultCode(2011);
						return 2011;
					}
					StartPos=WhilePos;
				}
				break;
			case KW_BREAK:
				{
					int EndWhilePos=FindKeyWord(pScript,KW_ENDWHILE,StartPos,-1);
					if(EndWhilePos<0)
					{
						ESThread.SetResultCode(2012);
						return 2012;
					}
					StartPos=EndWhilePos+1;		
				}		
				break;
			case KW_CONTINUE:
				{
					int WhilePos=FindKeyWordReverse(pScript,KW_WHILE,StartPos,-1);
					if(WhilePos<0)
					{
						ESThread.SetResultCode(2013);
						return 2013;
					}
					StartPos=WhilePos;
				}
				break;
			case KW_GOTO:
				{
					if(pScript->GetAt(StartPos+1)==NULL)
					{
						ESThread.SetResultCode(2014);
						return 2014;
					}
					if(pScript->GetAt(StartPos+1)->Type!=BOLAN_TYPE_IDENTIFIER)
					{
						ESThread.SetResultCode(2014);
						return 2014;
					}
					int IdentifierPos=pScript->FindIdentifier(0,pScript->GetAt(StartPos+1)->StrValue);
					if(IdentifierPos<0)
					{
						ESThread.SetResultCode(2015);
						return 2015;
					}
					StartPos=IdentifierPos+1;
				}
				break;
			case KW_IDHEADER:
				StartPos++;
				break;
			case KW_LINEEND:
				StartPos++;				
				break;
			case KW_DECLARE:
				StartPos++;				
				break;
			case KW_INT:
			case KW_INT64:
			case KW_FLOAT:
			case KW_DOUBLE:
			case KW_STRING:			
				StartPos+=2;
				break;
			default:
				StartPos++;
			}
			break;
		case BOLAN_TYPE_IDENTIFIER:
			StartPos++;			
			break;
		default:
			StartPos++;
		}
		if(EndPos>=0)
		{
			if(StartPos>=EndPos)
				break;
		}
	}
	return 0;
}


int CEasyScriptExecutor::CaculateBolanExpression(CESThread& ESThread,int& StartPos)
{
	ES_BOLAN *			pBolan;		
	ES_VARIABLE *		pVar;	
	ES_BOLAN *			t1;
	ES_BOLAN *			t2;	
	ES_BOLAN			FnResult,Temp;		
	int					ResultCode;
	CESBolanStack *		pScript=ESThread.GetScript();
	CESBolanStack * 	pStack=ESThread.GetStack();
	CESVariableList *	pVariableList=ESThread.GetVariableList();
	CESFactionList *	pFactionList=ESThread.GetFactionList();
	

	if(ESThread.IsInInterrupt())
	{
		pStack->Push(&ESThread.GetResult());
		StartPos=ESThread.GetInterruptPos();		
	}
	else
	{
		pStack->Clear();
	}
	if(pStack==NULL)
		return 3013;

	pBolan=pScript->GetAt(StartPos);
	while(pBolan&&
		(pBolan->Type==BOLAN_TYPE_VALUE||pBolan->Type==BOLAN_TYPE_OPERATOR||
		pBolan->Type==BOLAN_TYPE_FUNCTION||pBolan->Type==BOLAN_TYPE_VARIABLE))
	{		
		ESThread.SetLastLine(pBolan->Line);
		switch(pBolan->Type)
		{
		case BOLAN_TYPE_VALUE:
			pStack->Push(pBolan);
			break;
		case BOLAN_TYPE_VARIABLE:						
			pVar=pVariableList->FindVariable(pBolan->Index);
			if(pVar==NULL)
				return 4012;
			Temp=*pBolan;
			Temp.ValueType=pVar->Type;
			if(Temp.ValueType==VALUE_TYPE_STRING)
				Temp.StrValue=pVar->StrValue;
			else
				Temp.ValueInt64=pVar->ValueInt64;
			pStack->Push(&Temp);
			break;		
		case BOLAN_TYPE_OPERATOR:
			switch((int)(pBolan->Index))
			{
			case OPERATOR_EVA:
				t2=pStack->Pop();
				t1=pStack->GetTop();
				ResultCode=DoEva(t1,t2,pVariableList);
				if(ResultCode)
					return ResultCode;
				break;
			case OPERATOR_ADD:
				t2=pStack->Pop();
				t1=pStack->GetTop();
				ResultCode=DoAdd(t1,t2,t1);
				if(ResultCode)
					return ResultCode;
				break;
			case OPERATOR_SUB:
				t2=pStack->Pop();
				t1=pStack->GetTop();			
				ResultCode=DoSub(t1,t2,t1);
				if(ResultCode)
					return ResultCode;
				break;
			case OPERATOR_MUL:
				t2=pStack->Pop();
				t1=pStack->GetTop();
				ResultCode=DoMul(t1,t2,t1);
				if(ResultCode)
					return ResultCode;
				break;
			case OPERATOR_DIV:
				t2=pStack->Pop();
				t1=pStack->GetTop();
				ResultCode=DoDiv(t1,t2,t1);
				if(ResultCode)
					return ResultCode;
				break;
			case OPERATOR_EQU:
				t2=pStack->Pop();
				t1=pStack->GetTop();				
				ResultCode=DoEqu(t1,t2,t1);
				if(ResultCode)
					return ResultCode;
				break;
			case OPERATOR_DIFF:
				t2=pStack->Pop();
				t1=pStack->GetTop();				
				ResultCode=DoDiff(t1,t2,t1);
				if(ResultCode)
					return ResultCode;
				break;
			case OPERATOR_LESS:
				t2=pStack->Pop();
				t1=pStack->GetTop();				
				ResultCode=DoLess(t1,t2,t1);
				if(ResultCode)
					return ResultCode;
				break;
			case OPERATOR_MORE:
				t2=pStack->Pop();
				t1=pStack->GetTop();				
				ResultCode=DoMore(t1,t2,t1);
				if(ResultCode)
					return ResultCode;
				break;
			case OPERATOR_LESS_EQU:
				t2=pStack->Pop();
				t1=pStack->GetTop();				
				ResultCode=DoLessEqu(t1,t2,t1);
				if(ResultCode)
					return ResultCode;
				break;
			case OPERATOR_MORE_EQU:
				t2=pStack->Pop();
				t1=pStack->GetTop();				
				ResultCode=DoMoreEqu(t1,t2,t1);
				if(ResultCode)
					return ResultCode;
				break;
			case OPERATOR_NEG:
				t1=pStack->GetTop();				
				ResultCode=DoNeg(t1,t1);
				if(ResultCode)
					return ResultCode;
				break;
			case OPERATOR_AND:
				t2=pStack->Pop();
				t1=pStack->GetTop();				
				ResultCode=DoAnd(t1,t2,t1);
				if(ResultCode)
					return ResultCode;
				break;
			case OPERATOR_OR:
				t2=pStack->Pop();
				t1=pStack->GetTop();				
				ResultCode=DoOr(t1,t2,t1);
				if(ResultCode)
					return ResultCode;
				break;
			case OPERATOR_NOT:				
				t1=pStack->GetTop();								
				ResultCode=DoNot(t1,t1);
				if(ResultCode)
					return ResultCode;
				break;
			}
			break;
		case BOLAN_TYPE_FUNCTION:
			{
				ES_FACTION * pFaction=pFactionList->FindFaction(pBolan->Index);

				if(pStack->GetSize()<pFaction->ParaCount)
					return 3007;

				t1=pStack->GetTop()-(pFaction->ParaCount-1);
				pStack->Pop(pFaction->ParaCount);
				for(int i=0;i<pFaction->ParaCount;i++)
				{
					if(t1[i].Type!=BOLAN_TYPE_VALUE&&t1[i].Type!=BOLAN_TYPE_VARIABLE)
						return 3008;
				}

				ResultCode=((pFaction->pObject)->*(pFaction->FnPtr))(pVariableList,&FnResult,t1,pFaction->ParaCount);	

				FnResult.Type=BOLAN_TYPE_VALUE;

				if(ResultCode<0) 
					return 3009;					//调用函数出错				
				if(ResultCode>0)
				{
					ESThread.GetResult()=ResultCode;
					ESThread.SetInterruptPos(StartPos+1);					
					return -1;
				}
				pStack->Push(&FnResult);
			}
			break;		
		}
		StartPos++;
		pBolan=pScript->GetAt(StartPos);
	}
	pBolan=pStack->Pop();
	if(pBolan)
	{
		ESThread.SetResult(*pBolan);
	}
	if(pStack->GetSize()!=0)
		PrintSystemLog(0,"表达式计算对堆栈未能清空！");	
	return 0;
}

int CEasyScriptExecutor::DoEva(ES_BOLAN * pLeftValue,ES_BOLAN * pRightValue,CESVariableList * pVariableList)
{
	if(pLeftValue==NULL||pRightValue==NULL)
		return 4013;
	if(pLeftValue->Type!=BOLAN_TYPE_VARIABLE)
		return 3001;
	if(pRightValue->Type!=BOLAN_TYPE_VARIABLE&&pRightValue->Type!=BOLAN_TYPE_VALUE)
		return 3015;
	ES_VARIABLE * pVar=pVariableList->FindVariable(pLeftValue->Index);
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
		}
		break;
	}	
	pLeftValue->ValueInt64=pVar->ValueInt64;
	pLeftValue->StrValue=pVar->StrValue;
	return 0;				
}

int CEasyScriptExecutor::DoAdd(ES_BOLAN * pLeftValue,ES_BOLAN * pRightValue,ES_BOLAN * pResult)
{
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
}

int CEasyScriptExecutor::DoSub(ES_BOLAN * pLeftValue,ES_BOLAN * pRightValue,ES_BOLAN * pResult)
{
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
}

int CEasyScriptExecutor::DoMul(ES_BOLAN * pLeftValue,ES_BOLAN * pRightValue,ES_BOLAN * pResult)
{
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
}

int CEasyScriptExecutor::DoDiv(ES_BOLAN * pLeftValue,ES_BOLAN * pRightValue,ES_BOLAN * pResult)
{
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
}


int CEasyScriptExecutor::DoEqu(ES_BOLAN * pLeftValue,ES_BOLAN * pRightValue,ES_BOLAN * pResult)
{
	if(pLeftValue==NULL||pRightValue==NULL||pResult==NULL)
		return 4013;
	if(pLeftValue->Type!=BOLAN_TYPE_VARIABLE&&pLeftValue->Type!=BOLAN_TYPE_VALUE)
		return 3016;
	if(pRightValue->Type!=BOLAN_TYPE_VARIABLE&&pRightValue->Type!=BOLAN_TYPE_VALUE)
		return 3017;

	pResult->ValueType=VALUE_TYPE_INT;

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

	pResult->Type=BOLAN_TYPE_VALUE;
	return 0;
}

int CEasyScriptExecutor::DoDiff(ES_BOLAN * pLeftValue,ES_BOLAN * pRightValue,ES_BOLAN * pResult)
{
	if(pLeftValue==NULL||pRightValue==NULL||pResult==NULL)
		return 4013;
	if(pLeftValue->Type!=BOLAN_TYPE_VARIABLE&&pLeftValue->Type!=BOLAN_TYPE_VALUE)
		return 3016;
	if(pRightValue->Type!=BOLAN_TYPE_VARIABLE&&pRightValue->Type!=BOLAN_TYPE_VALUE)
		return 3017;

	pResult->ValueType=VALUE_TYPE_INT;

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

	pResult->Type=BOLAN_TYPE_VALUE;
	return 0;
}

int CEasyScriptExecutor::DoLess(ES_BOLAN * pLeftValue,ES_BOLAN * pRightValue,ES_BOLAN * pResult)
{
	if(pLeftValue==NULL||pRightValue==NULL||pResult==NULL)
		return 4013;
	if(pLeftValue->Type!=BOLAN_TYPE_VARIABLE&&pLeftValue->Type!=BOLAN_TYPE_VALUE)
		return 3016;
	if(pRightValue->Type!=BOLAN_TYPE_VARIABLE&&pRightValue->Type!=BOLAN_TYPE_VALUE)
		return 3017;

	pResult->ValueType=VALUE_TYPE_INT;

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

	pResult->Type=BOLAN_TYPE_VALUE;
	return 0;
}


int CEasyScriptExecutor::DoMore(ES_BOLAN * pLeftValue,ES_BOLAN * pRightValue,ES_BOLAN * pResult)
{
	if(pLeftValue==NULL||pRightValue==NULL||pResult==NULL)
		return 4013;
	if(pLeftValue->Type!=BOLAN_TYPE_VARIABLE&&pLeftValue->Type!=BOLAN_TYPE_VALUE)
		return 3016;
	if(pRightValue->Type!=BOLAN_TYPE_VARIABLE&&pRightValue->Type!=BOLAN_TYPE_VALUE)
		return 3017;

	pResult->ValueType=VALUE_TYPE_INT;

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

	pResult->Type=BOLAN_TYPE_VALUE;
	return 0;
}


int CEasyScriptExecutor::DoLessEqu(ES_BOLAN * pLeftValue,ES_BOLAN * pRightValue,ES_BOLAN * pResult)
{
	if(pLeftValue==NULL||pRightValue==NULL||pResult==NULL)
		return 4013;
	if(pLeftValue->Type!=BOLAN_TYPE_VARIABLE&&pLeftValue->Type!=BOLAN_TYPE_VALUE)
		return 3016;
	if(pRightValue->Type!=BOLAN_TYPE_VARIABLE&&pRightValue->Type!=BOLAN_TYPE_VALUE)
		return 3017;

	pResult->ValueType=VALUE_TYPE_INT;

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

	pResult->Type=BOLAN_TYPE_VALUE;
	return 0;
}


int CEasyScriptExecutor::DoMoreEqu(ES_BOLAN * pLeftValue,ES_BOLAN * pRightValue,ES_BOLAN * pResult)
{
	if(pLeftValue==NULL||pRightValue==NULL||pResult==NULL)
		return 4013;
	if(pLeftValue->Type!=BOLAN_TYPE_VARIABLE&&pLeftValue->Type!=BOLAN_TYPE_VALUE)
		return 3016;
	if(pRightValue->Type!=BOLAN_TYPE_VARIABLE&&pRightValue->Type!=BOLAN_TYPE_VALUE)
		return 3017;

	pResult->ValueType=VALUE_TYPE_INT;

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

	pResult->Type=BOLAN_TYPE_VALUE;
	return 0;
}

int CEasyScriptExecutor::DoNeg(ES_BOLAN * pValue,ES_BOLAN * pResult)
{
	if(pValue==NULL||pResult==NULL)
		return 4013;
	if(pValue->Type!=BOLAN_TYPE_VARIABLE&&pValue->Type!=BOLAN_TYPE_VALUE)
		return 3016;
	if(pValue->Type!=BOLAN_TYPE_VARIABLE&&pValue->Type!=BOLAN_TYPE_VALUE)
		return 3017;

	pResult->ValueType=pValue->ValueType;
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
	return 0;

}

int CEasyScriptExecutor::DoAnd(ES_BOLAN * pLeftValue,ES_BOLAN * pRightValue,ES_BOLAN * pResult)
{
	if(pLeftValue==NULL||pRightValue==NULL||pResult==NULL)
		return 4013;
	if(pLeftValue->Type!=BOLAN_TYPE_VARIABLE&&pLeftValue->Type!=BOLAN_TYPE_VALUE)
		return 3016;
	if(pRightValue->Type!=BOLAN_TYPE_VARIABLE&&pRightValue->Type!=BOLAN_TYPE_VALUE)
		return 3017;

	pResult->ValueType=VALUE_TYPE_INT;

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

	pResult->Type=BOLAN_TYPE_VALUE;
	return 0;
}

int CEasyScriptExecutor::DoOr(ES_BOLAN * pLeftValue,ES_BOLAN * pRightValue,ES_BOLAN * pResult)
{
	if(pLeftValue==NULL||pRightValue==NULL||pResult==NULL)
		return 4013;
	if(pLeftValue->Type!=BOLAN_TYPE_VARIABLE&&pLeftValue->Type!=BOLAN_TYPE_VALUE)
		return 3016;
	if(pRightValue->Type!=BOLAN_TYPE_VARIABLE&&pRightValue->Type!=BOLAN_TYPE_VALUE)
		return 3017;

	pResult->ValueType=VALUE_TYPE_INT;

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
	return 0;
}


int CEasyScriptExecutor::DoNot(ES_BOLAN * pValue,ES_BOLAN * pResult)
{
	if(pValue==NULL||pResult==NULL)
		return 4013;
	if(pValue->Type!=BOLAN_TYPE_VARIABLE&&pValue->Type!=BOLAN_TYPE_VALUE)
		return 3016;
	if(pValue->Type!=BOLAN_TYPE_VARIABLE&&pValue->Type!=BOLAN_TYPE_VALUE)
		return 3017;

	pResult->ValueType=VALUE_TYPE_INT;
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
	return 0;

}

int CEasyScriptExecutor::FindKeyWord(CESBolanStack * pScriptList,int KeyWord,int StartPos,int StopKeyWord)
{
	for(int i=StartPos;i<pScriptList->GetSize();i++)
	{
		if(StopKeyWord>0&&pScriptList->GetAt(i)->Type==BOLAN_TYPE_KEYWORD&&pScriptList->GetAt(i)->Index==StopKeyWord)
			return -1;
		if(pScriptList->GetAt(i)->Type==BOLAN_TYPE_KEYWORD&&pScriptList->GetAt(i)->Index==KeyWord)
			return i;
	}
	return -1;
}

int CEasyScriptExecutor::FindKeyWordReverse(CESBolanStack * pScriptList,int KeyWord,int StartPos,int StopKeyWord)
{
	if(StartPos>=pScriptList->GetSize())
		return -1;
	for(;StartPos>=0;StartPos--)
	{
		if(pScriptList->GetAt(StartPos)->Type==BOLAN_TYPE_KEYWORD)
		{
			if(StopKeyWord>0&&pScriptList->GetAt(StartPos)->Index==StopKeyWord)
				return -1;
			if(pScriptList->GetAt(StartPos)->Index==KeyWord)
				return StartPos;
		}
	}
	return -1;
}

int CEasyScriptExecutor::FindCoupleKeyWord(CESBolanStack * pScriptList,int KeyWord,int CoupleKeyWord,int StartPos)
{
	int CoupleCount=0;
	for(int i=StartPos;i<pScriptList->GetSize();i++)
	{
		if(pScriptList->GetAt(i)->Type==BOLAN_TYPE_KEYWORD)
		{
			if(pScriptList->GetAt(i)->Index==CoupleKeyWord)
				CoupleCount++;
			if(pScriptList->GetAt(i)->Index==KeyWord)
			{
				if(CoupleCount)
					CoupleCount--;
				else
					return i;
			}
		}
	}
	return -1;
}

int CEasyScriptExecutor::FindCoupleKeyWordReverse(CESBolanStack * pScriptList,int KeyWord,int CoupleKeyWord,int StartPos)
{
	int CoupleCount=0;
	if(StartPos>=pScriptList->GetSize())
		return -1;
	for(;StartPos>=0;StartPos--)
	{
		if(pScriptList->GetAt(StartPos)->Type==BOLAN_TYPE_KEYWORD)
		{
			if(pScriptList->GetAt(StartPos)->Index==CoupleKeyWord)
				CoupleCount++;
			if(pScriptList->GetAt(StartPos)->Index==KeyWord)
			{
				if(CoupleCount)
					CoupleCount--;
				else
					return StartPos;
			}
		}
	}
	return -1;
}

int CEasyScriptExecutor::FindCoupleIF(CESBolanStack * pScriptList,int KeyWord,int CoupleKeyWord1,int CoupleKeyWord2,int StartPos,int StopKeyWord)
{
	int CoupleCount=0;
	for(int i=StartPos;i<pScriptList->GetSize();i++)
	{
		if(pScriptList->GetAt(i)->Type==BOLAN_TYPE_KEYWORD)
		{	
			if(CoupleCount==0)
			{
				if(pScriptList->GetAt(i)->Index==KeyWord)
					return i;
				if(StopKeyWord>0&&pScriptList->GetAt(i)->Index==StopKeyWord)
					return -1;
			}
			if(pScriptList->GetAt(i)->Index==CoupleKeyWord1)
				CoupleCount++;
			if(pScriptList->GetAt(i)->Index==CoupleKeyWord2)
				CoupleCount--;
		}
	}
	return -1;
}

