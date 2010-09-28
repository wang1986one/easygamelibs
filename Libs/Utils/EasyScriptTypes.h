/****************************************************************************/
/*                                                                          */
/*      文件名:    EasyScriptTypes.h	                                    */
/*      创建日期:  2010年08月26日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once

#define MAX_EXP_LENGTH			40960
#define MAX_IDENTIFIER_LENGTH	128
//#define MAX_STRING_LENGTH		4096
//#define MAX_STACK				16

enum BOLAN_TYPE:WORD
{
	BOLAN_TYPE_VALUE,
	BOLAN_TYPE_OPERATOR,
	BOLAN_TYPE_FUNCTION,
	BOLAN_TYPE_VARIABLE,
	BOLAN_TYPE_KEYWORD,
	BOLAN_TYPE_IDENTIFIER,
};

enum OPERATOR_TYPE:WORD
{
	OPERATOR_ADD,
	OPERATOR_SUB,
	OPERATOR_MUL,
	OPERATOR_DIV,
	OPERATOR_RP,
	OPERATOR_LP,
	OPERATOR_NEG,
	OPERATOR_COMMA,
	OPERATOR_LESS,
	OPERATOR_MORE,
	OPERATOR_LESS_EQU,
	OPERATOR_MORE_EQU,
	OPERATOR_EQU,
	OPERATOR_DIFF,
	OPERATOR_EVA,
	OPERATOR_AND,
	OPERATOR_OR,
	OPERATOR_NOT,
};

enum VALUE_TYPE:WORD
{
	VALUE_TYPE_INT,
	VALUE_TYPE_INT64,
	VALUE_TYPE_FLOAT,
	VALUE_TYPE_DOUBLE,
	VALUE_TYPE_STRING,
};

enum SCRIPT_KEYWORDS:WORD
{		
	KW_END=0,
	KW_IF,
	KW_THEN,
	KW_ELSE,
	KW_ELSEIF,
	KW_ENDIF,
	KW_WHILE,
	KW_DO,
	KW_ENDWHILE,
	KW_BREAK,
	KW_CONTINUE,
	KW_GOTO,
	KW_IDHEADER,
	KW_LINEEND,
	KW_DECLARE,
	KW_INT,
	KW_INT64,
	KW_FLOAT,
	KW_DOUBLE,
	KW_STRING,
	KW_MAX,
};

extern LPCTSTR KEYWORD_STRINGS[KW_MAX];



struct ES_BOLAN
{
	BOLAN_TYPE		Type;
	VALUE_TYPE		ValueType;
	WORD			Index;
	WORD			Level;
	CEasyString		StrValue;
	union
	{
		int			ValueInt;
		INT64		ValueInt64;
		float		ValueFloat;
		double		ValueDouble;
	};
	WORD			Line;

	ES_BOLAN()
	{
		Type=BOLAN_TYPE_VALUE;
		ValueType=VALUE_TYPE_INT;
		ValueInt64=0;
		Index=0;
		Level=0;
		Line=0;
	}
	ES_BOLAN(const ES_BOLAN& Bolan)
	{
		Type=Bolan.Type;
		ValueType=Bolan.ValueType;
		ValueInt64=Bolan.ValueInt64;
		StrValue=Bolan.StrValue;
		Index=Bolan.Index;
		Level=Bolan.Level;
		Line=Bolan.Line;
	}
	ES_BOLAN& operator=(const ES_BOLAN& Bolan)
	{
		Type=Bolan.Type;
		ValueType=Bolan.ValueType;
		ValueInt64=Bolan.ValueInt64;
		StrValue=Bolan.StrValue;
		Index=Bolan.Index;
		Level=Bolan.Level;
		Line=Bolan.Line;
		return *this;
	}
	void Clear()
	{
		Type=BOLAN_TYPE_VALUE;
		ValueType=VALUE_TYPE_INT;
		ValueInt64=0;
		StrValue.Clear();
		Index=0;
		Level=0;
		Line=0;
	}
	operator char()
	{
		switch(ValueType)
		{
		case VALUE_TYPE_INT:
			return (char)ValueInt;
		case VALUE_TYPE_INT64:
			return (char)ValueInt64;
		case VALUE_TYPE_FLOAT:
			return (char)ValueFloat;
		case VALUE_TYPE_DOUBLE:
			return (char)ValueDouble;
		}		
		return 0;
	}
	operator unsigned char()
	{
		switch(ValueType)
		{
		case VALUE_TYPE_INT:
			return (unsigned char)ValueInt;
		case VALUE_TYPE_INT64:
			return (unsigned char)ValueInt64;
		case VALUE_TYPE_FLOAT:
			return (unsigned char)ValueFloat;
		case VALUE_TYPE_DOUBLE:
			return (unsigned char)ValueDouble;
		}		
		return 0;
	}
	operator short()
	{
		switch(ValueType)
		{
		case VALUE_TYPE_INT:
			return (short)ValueInt;
		case VALUE_TYPE_INT64:
			return (short)ValueInt64;
		case VALUE_TYPE_FLOAT:
			return (short)ValueFloat;
		case VALUE_TYPE_DOUBLE:
			return (short)ValueDouble;
		}		
		return 0;
	}
	operator unsigned short()
	{
		switch(ValueType)
		{
		case VALUE_TYPE_INT:
			return (unsigned short)ValueInt;
		case VALUE_TYPE_INT64:
			return (unsigned short)ValueInt64;
		case VALUE_TYPE_FLOAT:
			return (unsigned short)ValueFloat;
		case VALUE_TYPE_DOUBLE:
			return (unsigned short)ValueDouble;
		}		
		return 0;
	}
	operator int()
	{
		switch(ValueType)
		{
		case VALUE_TYPE_INT:
			return (int)ValueInt;
		case VALUE_TYPE_INT64:
			return (int)ValueInt64;
		case VALUE_TYPE_FLOAT:
			return (int)ValueFloat;
		case VALUE_TYPE_DOUBLE:
			return (int)ValueDouble;
		}		
		return 0;
	}
	operator unsigned int()
	{
		switch(ValueType)
		{
		case VALUE_TYPE_INT:
			return (unsigned int)ValueInt;
		case VALUE_TYPE_INT64:
			return (unsigned int)ValueInt64;
		case VALUE_TYPE_FLOAT:
			return (unsigned int)ValueFloat;
		case VALUE_TYPE_DOUBLE:
			return (unsigned int)ValueDouble;
		}		
		return 0;
	}
	operator long()
	{
		switch(ValueType)
		{
		case VALUE_TYPE_INT:
			return (long)ValueInt;
		case VALUE_TYPE_INT64:
			return (long)ValueInt64;
		case VALUE_TYPE_FLOAT:
			return (long)ValueFloat;
		case VALUE_TYPE_DOUBLE:
			return (long)ValueDouble;
		}		
		return 0;
	}
	operator unsigned long()
	{
		switch(ValueType)
		{
		case VALUE_TYPE_INT:
			return (unsigned long)ValueInt;
		case VALUE_TYPE_INT64:
			return (unsigned long)ValueInt64;
		case VALUE_TYPE_FLOAT:
			return (unsigned long)ValueFloat;
		case VALUE_TYPE_DOUBLE:
			return (unsigned long)ValueDouble;
		}		
		return 0;
	}
	operator __int64()
	{
		switch(ValueType)
		{
		case VALUE_TYPE_INT:
			return (__int64)ValueInt;
		case VALUE_TYPE_INT64:
			return (__int64)ValueInt64;
		case VALUE_TYPE_FLOAT:
			return (__int64)ValueFloat;
		case VALUE_TYPE_DOUBLE:
			return (__int64)ValueDouble;
		}		
		return 0;
	}
	operator unsigned __int64()
	{
		switch(ValueType)
		{
		case VALUE_TYPE_INT:
			return (unsigned __int64)ValueInt;
		case VALUE_TYPE_INT64:
			return (unsigned __int64)ValueInt64;
		case VALUE_TYPE_FLOAT:
			return (unsigned __int64)ValueFloat;
		case VALUE_TYPE_DOUBLE:
			return (unsigned __int64)ValueDouble;
		}		
		return 0;
	}
	operator float()
	{
		switch(ValueType)
		{
		case VALUE_TYPE_INT:
			return (float)ValueInt;
		case VALUE_TYPE_INT64:
			return (float)ValueInt64;
		case VALUE_TYPE_FLOAT:
			return (float)ValueFloat;
		case VALUE_TYPE_DOUBLE:
			return (float)ValueDouble;
		}		
		return 0;
	}
	operator double()
	{
		switch(ValueType)
		{
		case VALUE_TYPE_INT:
			return (double)ValueInt;
		case VALUE_TYPE_INT64:
			return (double)ValueInt64;
		case VALUE_TYPE_FLOAT:
			return (double)ValueFloat;
		case VALUE_TYPE_DOUBLE:
			return (double)ValueDouble;
		}		
		return 0;
	}
	operator LPCTSTR()
	{
		return StrValue;
	}
	char operator=(char Value)
	{
		Type=BOLAN_TYPE_VALUE;
		ValueType=VALUE_TYPE_INT;
		ValueInt=Value;
		return ValueInt;
	}
	unsigned char operator=(unsigned char Value)
	{
		Type=BOLAN_TYPE_VALUE;
		ValueType=VALUE_TYPE_INT;
		ValueInt=Value;
		return ValueInt;
	}
	short operator=(short Value)
	{
		Type=BOLAN_TYPE_VALUE;
		ValueType=VALUE_TYPE_INT;
		ValueInt=Value;
		return ValueInt;
	}
	unsigned short operator=(unsigned short Value)
	{
		Type=BOLAN_TYPE_VALUE;
		ValueType=VALUE_TYPE_INT;
		ValueInt=Value;
		return ValueInt;
	}
	int operator=(int Value)
	{
		Type=BOLAN_TYPE_VALUE;
		ValueType=VALUE_TYPE_INT;
		ValueInt=Value;
		return ValueInt;
	}
	unsigned int operator=(unsigned int Value)
	{
		Type=BOLAN_TYPE_VALUE;
		ValueType=VALUE_TYPE_INT;
		ValueInt=Value;
		return ValueInt;
	}
	__int64 operator=(__int64 Value)
	{
		Type=BOLAN_TYPE_VALUE;
		ValueType=VALUE_TYPE_INT64;
		ValueInt64=Value;
		return ValueInt64;
	}
	unsigned __int64 operator=(unsigned __int64 Value)
	{
		Type=BOLAN_TYPE_VALUE;
		ValueType=VALUE_TYPE_INT64;
		ValueInt64=Value;
		return ValueInt64;
	}
	float operator=(float Value)
	{
		Type=BOLAN_TYPE_VALUE;
		ValueType=VALUE_TYPE_FLOAT;
		ValueFloat=Value;
		return ValueFloat;
	}
	double operator=(double Value)
	{
		Type=BOLAN_TYPE_VALUE;
		ValueType=VALUE_TYPE_DOUBLE;
		ValueDouble=Value;
		return ValueDouble;
	}
	LPCTSTR operator=(LPCTSTR Value)
	{
		Type=BOLAN_TYPE_VALUE;
		ValueType=VALUE_TYPE_STRING;
		StrValue=Value;
		return StrValue;
	}
};


inline BOOL CanMakeIdentifier(char c)
{
	if(c!='+'&&c!='-'&&c!='*'&&c!='/'&&
		c!=','&&c!='('&&c!=')'&&c!=' '&&
		c!='='&&c!='>'&&c!='<'&&c!='~'&&
		c!=0&&c!=':'&&c!=';'&&c>'\r'&&c!='\n')
		return true;
	else 
		return false;
}

inline void StrToNumber(LPCTSTR szNumberStr,ES_BOLAN& Value)
{
	bool IsReal=strchr(szNumberStr,'.')!=NULL||strchr(szNumberStr,'E')!=NULL;
	bool IsFloat=strchr(szNumberStr,'F')!=NULL;
	bool IsDouble=strchr(szNumberStr,'D')!=NULL;
	bool IsInt=strchr(szNumberStr,'I')!=NULL;
	bool IsInt64=strchr(szNumberStr,'L')!=NULL;	
	if(IsReal||IsFloat||IsDouble)
	{
		double ValueMax=atof(szNumberStr);
		if(IsDouble)
		{
			Value.ValueType=VALUE_TYPE_DOUBLE;
			Value.ValueDouble=ValueMax;
		}
		else if(IsFloat)
		{
			Value.ValueType=VALUE_TYPE_FLOAT;
			Value.ValueFloat=(float)ValueMax;
		}
		else
		{
			if(ValueMax<(1.175494351E-38)||ValueMax>(3.402823466E+38))
			{
				Value.ValueType=VALUE_TYPE_DOUBLE;
				Value.ValueDouble=ValueMax;
			}
			else
			{
				Value.ValueType=VALUE_TYPE_FLOAT;
				Value.ValueFloat=(float)ValueMax;
			}
		}
	}
	else
	{		
		INT64 ValueMax=_atoi64(szNumberStr);
		if(IsInt)
		{
			Value.ValueType=VALUE_TYPE_INT;
			Value.ValueInt=(int)ValueMax;
		}
		else if(IsInt64)
		{
			Value.ValueType=VALUE_TYPE_INT64;
			Value.ValueInt64=ValueMax;
		}
		else
		{
			if(ValueMax>=(-(int)2147483648)&&ValueMax<=(2147483647))
			{
				Value.ValueType=VALUE_TYPE_INT;
				Value.ValueInt=(int)ValueMax;
			}			
			else
			{
				Value.ValueType=VALUE_TYPE_INT64;
				Value.ValueInt64=ValueMax;
			}
		}

	}
}

inline CEasyString BolanToString(const ES_BOLAN& bolan)
{
	CEasyString temp;
	switch(bolan.ValueType)
	{
	case VALUE_TYPE_INT:
		temp.Format("Int=%d",bolan.ValueInt);
		break;
	case VALUE_TYPE_INT64:
		temp.Format("Int64=%lld",bolan.ValueInt64);
		break;
	case VALUE_TYPE_FLOAT:
		temp.Format("Float=%g",bolan.ValueFloat);
		break;
	case VALUE_TYPE_DOUBLE:
		temp.Format("Double=%g",bolan.ValueDouble);
		break;
	case VALUE_TYPE_STRING:
		temp.Format("Str=%s",(LPCTSTR)bolan.StrValue);
		break;
	}
	switch(bolan.Type)
	{
	case BOLAN_TYPE_VALUE:				
		break;	
	case BOLAN_TYPE_VARIABLE:	
		temp="Var"+temp;
		break;
	default:
		temp="不可解释的符号";
	}
	return temp;
}

extern char * ESGetErrorMsg(int ErrCode);