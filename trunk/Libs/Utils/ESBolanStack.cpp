/****************************************************************************/
/*                                                                          */
/*      文件名:    ESBolanStack.cpp		                                    */
/*      创建日期:  2010年08月26日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#include "StdAfx.h"


CESBolanStack::CESBolanStack(int InitBuffSize,int GrowSize)
{	
	m_GrowSize=GrowSize;

	m_BolanBuff=new ES_BOLAN[InitBuffSize];
	m_BolanBuffSize=InitBuffSize;
	m_BolanCount=0;
}

CESBolanStack::~CESBolanStack()
{
	if(m_BolanBuff)
	{
		delete[] m_BolanBuff;
		m_BolanBuff=0;
		m_BolanBuffSize=0;
		m_BolanCount=0;
	}
}

void CESBolanStack::Grow()
{
	ES_BOLAN * NewBuff=new ES_BOLAN[m_BolanBuffSize+m_GrowSize];
	for(int i=0;i<m_BolanBuffSize;i++)
		NewBuff[i]=m_BolanBuff[i];
	//memcpy(NewBuff,m_BolanBuff,sizeof(CBolan)*m_BolanBuffSize);
	delete[] m_BolanBuff;
	m_BolanBuff=NewBuff;
	m_BolanBuffSize+=m_GrowSize;
}

bool CESBolanStack::Push(ES_BOLAN * Bolan)
{
	if(m_BolanCount>=m_BolanBuffSize)
		Grow();
	m_BolanBuff[m_BolanCount]=*Bolan;
	m_BolanCount++;
	return true;
}

ES_BOLAN *  CESBolanStack::Pop()
{
	if(m_BolanCount>0)
	{
		m_BolanCount--;
		return m_BolanBuff+(m_BolanCount);
	}
	else
		return NULL;
}

bool CESBolanStack::Pop(int Count)
{
	if(Count<=m_BolanCount)
	{
		m_BolanCount-=Count;
		return true;
	}
	return false;
}

ES_BOLAN * CESBolanStack::GetAt(int Index)
{
	if(Index<m_BolanCount)
	{
		return m_BolanBuff+Index;
	}
	else
		return NULL;
}

ES_BOLAN * CESBolanStack::GetTop()
{
	if(m_BolanCount>0)
	{
		return m_BolanBuff+(m_BolanCount-1);
	}
	else
		return NULL;
}

int  CESBolanStack::PushScript(const char * ExpStr,CESVariableList* pVariableList,CESFactionList * pFactionList,int& LastLine)
{
	char TempID[MAX_IDENTIFIER_LENGTH];

	CESBolanStack TempStack;
	ES_BOLAN Bolan;
	int i;
	bool IsInStr;
	LastLine=1;

	CEasyBuffer Buffer((int)strlen(ExpStr)+1);
	CEasyBuffer TempBuffer((int)strlen(ExpStr)+1);

	char * ExpBuff=(char *)Buffer.GetBuffer();
	char * TempBuff=(char *)TempBuffer.GetBuffer();


	i=0;
	IsInStr=false;
	while(*ExpStr)
	{
		if(*ExpStr=='"') IsInStr=!IsInStr;

		if(IsInStr)
			ExpBuff[i++]=(*ExpStr++);
		else
			ExpBuff[i++]=toupper(*ExpStr++);		

	}
	ExpBuff[i]=0;
	//CharUpper(ExpBuff);
	ExpStr=ExpBuff;
	while(*ExpStr)
	{
		Bolan.Clear();
		Bolan.Line=LastLine;
		if(*ExpStr==' '||*ExpStr=='\r'||*ExpStr=='\n'||*ExpStr=='	')
		{
			//空格、空行忽略
			if(*ExpStr=='\n')
				LastLine++;
			ExpStr++;
		}
		else if(*ExpStr=='/'&&*(ExpStr+1)=='/')
		{
			while((*ExpStr!='\r')&&(*ExpStr!='\n')&&(*ExpStr))
			{
				if(*ExpStr=='\n')
					LastLine++;
				ExpStr++;
			}
		}
		else if(*ExpStr=='"')//字符串
		{
			i=0;
			ExpStr++;
			while(*ExpStr!='"')
			{
				//if(i>MAX_STRING_LENGTH)
				//	return 1001;
				if(*ExpStr==0)
					return 1002;
				TempBuff[i++]=*ExpStr++;
			}
			TempBuff[i]=0;
			ExpStr++;

			Bolan.Type=BOLAN_TYPE_VALUE;
			Bolan.ValueType=VALUE_TYPE_STRING;
			Bolan.StrValue=TempBuff;
			Bolan.Level=0;
			Push(&Bolan);
		}
		else if((*ExpStr>='0'&&*ExpStr<='9')||*ExpStr=='.')	//数字
		{
			i=0;
			while((*ExpStr>='0'&&*ExpStr<='9')||*ExpStr=='.'||*ExpStr=='F'||*ExpStr=='D'||*ExpStr=='I'||*ExpStr=='L'||*ExpStr=='E')
			{
				TempBuff[i++]=*ExpStr++;
			}
			TempBuff[i]=0;
			Bolan.Type=BOLAN_TYPE_VALUE;			
			Bolan.Level=0;
			StrToNumber(TempBuff,Bolan);
			Push(&Bolan);
		}
		else if(*ExpStr==':')		//跳转标识
		{
			i=0;
			ExpStr++;
			while(CanMakeIdentifier(*ExpStr))
			{
				if(i>MAX_IDENTIFIER_LENGTH)
					return 1003;
				TempID[i++]=*ExpStr++;
			}
			TempID[i]=0;			
			Bolan.Type=BOLAN_TYPE_IDENTIFIER;
			Bolan.Level=0;				
			Bolan.StrValue=TempID;
			Push(&Bolan);			
		}
		else if(*ExpStr==';'||*ExpStr=='\r'||*ExpStr=='\n')//行结束符
		{
			//弹出所有操作符
			while(TempStack.GetTop()!=NULL)
			{				
				Push(TempStack.Pop());
			}
			Bolan.Type=BOLAN_TYPE_KEYWORD;
			Bolan.Level=0;
			Bolan.Index=KW_LINEEND;
			Push(&Bolan);
			//除去多余的行结束符
			while(*ExpStr==' '||*ExpStr==';'||*ExpStr=='\r'||*ExpStr=='\n')
			{
				if(*ExpStr=='\n')
					LastLine++;
				ExpStr++;
			}
		}
		else if(*ExpStr>='A'&&*ExpStr<='Z')//标识符
		{
			i=0;
			while(CanMakeIdentifier(*ExpStr))
			{
				if(i>MAX_IDENTIFIER_LENGTH)
					return 1003;
				TempBuff[i++]=*ExpStr++;
			}
			TempBuff[i]=0;	
			if(_stricmp(TempBuff,"AND")==0)
			{
				while(TempStack.GetTop()!=NULL&&TempStack.GetTop()->Level>15)
				{				
					Push(TempStack.Pop());
				}
				Bolan.Type=BOLAN_TYPE_OPERATOR;
				Bolan.Level=15;
				Bolan.Index=OPERATOR_AND;
				TempStack.Push(&Bolan);			
			}
			else if(_stricmp(TempBuff,"OR")==0)
			{
				while(TempStack.GetTop()!=NULL&&TempStack.GetTop()->Level>15)
				{				
					Push(TempStack.Pop());
				}
				Bolan.Type=BOLAN_TYPE_OPERATOR;
				Bolan.Level=15;
				Bolan.Index=OPERATOR_OR;
				TempStack.Push(&Bolan);			
			}
			else if(_stricmp(TempBuff,"NOT")==0)
			{
				while(TempStack.GetTop()!=NULL&&TempStack.GetTop()->Level>16)
				{				
					Push(TempStack.Pop());
				}
				Bolan.Type=BOLAN_TYPE_OPERATOR;
				Bolan.Level=16;
				Bolan.Index=OPERATOR_NOT;
				TempStack.Push(&Bolan);			
			}
			else
			{
				int KeyWord=FindKeyWord(TempBuff);
				if(KeyWord>=0)//关键字
				{
					//弹出所有操作符
					while(TempStack.GetTop()!=NULL)
					{				
						Push(TempStack.Pop());
					}

					Bolan.Type=BOLAN_TYPE_KEYWORD;
					Bolan.Level=0;
					Bolan.Index=KeyWord;
					Push(&Bolan);

				}
				else
				{
					//函数
					ES_FACTION * pFaction=pFactionList->FindFaction(TempBuff);
					if(pFaction)
					{		
						while(TempStack.GetTop()!=NULL&&TempStack.GetTop()->Level>50)
						{				
							Push(TempStack.Pop());
						}
						Bolan.Type=BOLAN_TYPE_FUNCTION;
						Bolan.Level=50;
						Bolan.Index=pFaction->ID;
						TempStack.Push(&Bolan);					
					}
					else
					{		
						//其他的标识符作为变量					
						Bolan.Type=BOLAN_TYPE_VARIABLE;
						Bolan.Level=0;							
						Bolan.StrValue=TempBuff;
						ES_VARIABLE * pVar=pVariableList->FindVariable(Bolan.StrValue);
						if(pVar)
							Bolan.Index=pVar->ID;
						else
							Bolan.Index=0;
						Push(&Bolan);					
					}
				}
			}
		}
		else if(*ExpStr=='(')
		{			
			Bolan.Type=BOLAN_TYPE_OPERATOR;
			Bolan.Level=0;
			Bolan.Index=OPERATOR_LP;
			TempStack.Push(&Bolan);
			ExpStr++;			
		}
		else if(*ExpStr==')')
		{
			while(TempStack.GetTop()!=NULL&&(TempStack.GetTop()->Type!=BOLAN_TYPE_OPERATOR||TempStack.GetTop()->Index!=OPERATOR_LP))
			{				
				Push(TempStack.Pop());
			}			
			TempStack.Pop();
			ExpStr++;
		}
		else if(*ExpStr=='=')
		{	
			if(*(ExpStr+1)=='=')
			{
				while(TempStack.GetTop()!=NULL&&TempStack.GetTop()->Level>20)
				{				
					Push(TempStack.Pop());
				}		
				Bolan.Type=BOLAN_TYPE_OPERATOR;
				Bolan.Level=20;
				Bolan.Index=OPERATOR_EQU;	
				TempStack.Push(&Bolan);
				ExpStr+=2;
			}
			else
			{
				while(TempStack.GetTop()!=NULL&&TempStack.GetTop()->Level>10)
				{				
					Push(TempStack.Pop());
				}		
				Bolan.Type=BOLAN_TYPE_OPERATOR;
				Bolan.Level=10;
				Bolan.Index=OPERATOR_EVA;	
				TempStack.Push(&Bolan);
				ExpStr++;
			}
		}
		else if(*ExpStr=='>')
		{			
			if(*(ExpStr+1)=='=')
			{
				while(TempStack.GetTop()!=NULL&&TempStack.GetTop()->Level>20)
				{				
					Push(TempStack.Pop());
				}	
				Bolan.Type=BOLAN_TYPE_OPERATOR;
				Bolan.Level=20;
				Bolan.Index=OPERATOR_MORE_EQU;	
				TempStack.Push(&Bolan);
				ExpStr+=2;	
			}
			else
			{
				while(TempStack.GetTop()!=NULL&&TempStack.GetTop()->Level>20)
				{				
					Push(TempStack.Pop());
				}	
				Bolan.Type=BOLAN_TYPE_OPERATOR;
				Bolan.Level=20;
				Bolan.Index=OPERATOR_MORE;	
				TempStack.Push(&Bolan);
				ExpStr++;				
			}
		}
		else if(*ExpStr=='<')
		{			
			if(*(ExpStr+1)=='=')
			{
				while(TempStack.GetTop()!=NULL&&TempStack.GetTop()->Level>20)
				{				
					Push(TempStack.Pop());
				}	
				Bolan.Type=BOLAN_TYPE_OPERATOR;
				Bolan.Level=20;
				Bolan.Index=OPERATOR_LESS_EQU;		
				TempStack.Push(&Bolan);
				ExpStr+=2;	
			}
			else if(*(ExpStr+1)=='>')
			{
				while(TempStack.GetTop()!=NULL&&TempStack.GetTop()->Level>20)
				{				
					Push(TempStack.Pop());
				}	
				Bolan.Type=BOLAN_TYPE_OPERATOR;
				Bolan.Level=20;
				Bolan.Index=OPERATOR_DIFF;		
				TempStack.Push(&Bolan);
				ExpStr+=2;	
			}
			else
			{
				while(TempStack.GetTop()!=NULL&&TempStack.GetTop()->Level>20)
				{				
					Push(TempStack.Pop());
				}	
				Bolan.Type=BOLAN_TYPE_OPERATOR;
				Bolan.Level=20;
				Bolan.Index=OPERATOR_LESS;			
				TempStack.Push(&Bolan);
				ExpStr++;				
			}			

		}
		else if(*ExpStr=='+'||*ExpStr=='-')
		{
			if(*ExpStr=='-'&&(ExpStr==ExpBuff||*(ExpStr-1)=='('))
			{
				Bolan.Type=BOLAN_TYPE_OPERATOR;
				Bolan.Index=OPERATOR_NEG;
				Bolan.Level=60;
				TempStack.Push(&Bolan);
				ExpStr++;
			}
			else
			{
				while(TempStack.GetTop()!=NULL&&TempStack.GetTop()->Level>30)
				{				
					Push(TempStack.Pop());
				}		
				Bolan.Type=BOLAN_TYPE_OPERATOR;
				Bolan.Level=30;
				if(*ExpStr=='+') Bolan.Index=OPERATOR_ADD;
				else Bolan.Index=OPERATOR_SUB;	
				TempStack.Push(&Bolan);
				ExpStr++;				
			}
		}
		else if(*ExpStr=='*'||*ExpStr=='/')
		{
			while(TempStack.GetTop()!=NULL&&TempStack.GetTop()->Level>40)
			{				
				Push(TempStack.Pop());
			}				
			Bolan.Type=BOLAN_TYPE_OPERATOR;
			Bolan.Level=40;
			if(*ExpStr=='*') Bolan.Index=OPERATOR_MUL;
			else Bolan.Index=OPERATOR_DIV;	
			TempStack.Push(&Bolan);
			ExpStr++;
		}
		else if(*ExpStr==',')
		{
			while(TempStack.GetTop()!=NULL&&TempStack.GetTop()->Level>0)
			{				
				Push(TempStack.Pop());
			}	
			ExpStr++;
		}		
		else
		{
			return 1005;				//非法字符
		}		
	}
	while(TempStack.GetTop()!=NULL)
	{				
		Push(TempStack.Pop());
	}
	return DealVariableDefine(pVariableList,LastLine);
}

int CESBolanStack::FindIdentifier(int Start,const char * Name)
{
	for(int i=Start;i<GetSize();i++)
	{
		if(GetAt(i)->Type==BOLAN_TYPE_IDENTIFIER)
		{
			if(i>0)
				if(GetAt(i-1)->Type==BOLAN_TYPE_KEYWORD&&GetAt(i-1)->Index==KW_GOTO)
					continue;
			if(GetAt(i)->StrValue.CompareNoCase(Name)==0)
				return i;
		}
	}
	return -1;
}

int CESBolanStack::DealVariableDefine(CESVariableList* pVariableList,int& LastLine)
{
	int StartPos=0;
	ES_VARIABLE * pVar;

	while(StartPos<GetSize())
	{
		LastLine=GetAt(StartPos)->Line;
		switch(GetAt(StartPos)->Type)
		{
		case BOLAN_TYPE_VARIABLE:
			pVar=pVariableList->FindVariable(GetAt(StartPos)->StrValue);
			if(pVar==NULL)
				return 4012;
			GetAt(StartPos)->Index=pVar->ID;
			StartPos++;
			break;
		case BOLAN_TYPE_KEYWORD:
			switch((int)(GetAt(StartPos)->Index))
			{
			case KW_INT:
				StartPos++;
				if(GetAt(StartPos)->Type!=BOLAN_TYPE_VARIABLE)
					return 4010;
				pVar=pVariableList->FindVariable(GetAt(StartPos)->StrValue);
				if(pVar==NULL)
					pVariableList->AddVariable(GetAt(StartPos)->StrValue,(int)0);
				StartPos++;
				break;
			case KW_INT64:
				StartPos++;
				if(GetAt(StartPos)->Type!=BOLAN_TYPE_VARIABLE)
					return 4010;
				pVar=pVariableList->FindVariable(GetAt(StartPos)->StrValue);
				if(pVar==NULL)
					pVariableList->AddVariable(GetAt(StartPos)->StrValue,(INT64)0);
				StartPos++;
				break;
			case KW_FLOAT:
				StartPos++;
				if(GetAt(StartPos)->Type!=BOLAN_TYPE_VARIABLE)
					return 4010;
				pVar=pVariableList->FindVariable(GetAt(StartPos)->StrValue);
				if(pVar==NULL)
					pVariableList->AddVariable(GetAt(StartPos)->StrValue,(float)0);
				StartPos++;
				break;
			case KW_DOUBLE:
				StartPos++;
				if(GetAt(StartPos)->Type!=BOLAN_TYPE_VARIABLE)
					return 4010;
				pVar=pVariableList->FindVariable(GetAt(StartPos)->StrValue);
				if(pVar==NULL)
					pVariableList->AddVariable(GetAt(StartPos)->StrValue,(double)0);
				StartPos++;
				break;
			case KW_STRING:			
				StartPos++;
				if(GetAt(StartPos)->Type!=BOLAN_TYPE_VARIABLE)
					return 4010;
				pVar=pVariableList->FindVariable(GetAt(StartPos)->StrValue);
				if(pVar==NULL)
					pVariableList->AddVariable(GetAt(StartPos)->StrValue,"");						
				StartPos++;
				break;			
			default:
				StartPos++;
			}
			break;
		default:
			StartPos++;
		}

	}
	return 0;
}

int CESBolanStack::FindKeyWord(const char * KeyWord)
{
	for(int i=0;i<KW_MAX;i++)
	{
		if(_stricmp(KeyWord,KEYWORD_STRINGS[i])==0)
			return i;
	}
	return -1;
}
