/****************************************************************************/
/*                                                                          */
/*      文件名:    ToolsAll.h                                               */
/*      创建日期:  2010年02月09日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once



inline size_t strnlen(const char * pStr,size_t BufferLen)
{
	size_t Len=0;
	while(*pStr&&BufferLen)
	{
		pStr++;
		Len++;
		BufferLen--;
	}
	return Len;
}

inline void Swap(FLOAT& v1,FLOAT& v2)
{
	FLOAT temp=v1;
	v1=v2;
	v2=temp;
}

inline BOOL PrintSystemLog(DWORD Color,LPCTSTR Format,...)
{
	va_list vl;
	va_start(vl,Format);
	BOOL ret=CLogManager::GetInstance()->PrintLogVL(LOG_SYSTEM_CHANNEL,ILogPrinter::LOG_LEVEL_NORMAL,Color,Format,vl);
	va_end(vl);
	return ret;
}

inline BOOL PrintImportantLog(DWORD Color,LPCTSTR Format,...)
{
	va_list vl;
	va_start(vl,Format);
	BOOL ret=CLogManager::GetInstance()->PrintLogVL(LOG_IMPORTANT_CHANNEL,ILogPrinter::LOG_LEVEL_NORMAL,Color,Format,vl);
	va_end(vl);
	return ret;
}


inline int GetRand(int Min,int Max)
{
	return rand()%(Max-Min+1)+Min;
}

inline float GetRandf(float Min,float Max)
{
	return (((float)rand())/RAND_MAX)*(Max-Min)+Min;
}


inline CEasyString FormatSplitNumber(ULONG64 Number)
{
	CEasyString Str;

	Str.Format("%I64u",Number);
	int i=(int)Str.GetLength()-3;
	while(i>0)
	{
		Str.Insert(i,',');
		i-=3;
	}
	return Str;
}

inline CEasyString FormatNumberWords(ULONG64 Number,bool IsTiny=false)
{
	CEasyString temp;
	

	if(Number==0) return "0";
	if(Number<=1024)
	{
		temp.Format("%llu",Number);
	}else if(Number<=1048576)
	{
		float size=Number/1024.0f;
		temp.Format("%.2fK",size);
	}else
	{
		float size=Number/1048576.0f;
		temp.Format("%.2fM",size);
	}
	
	if(IsTiny)
		return temp;
	else
		return temp+" Byte";
}

inline CEasyString FormatNumberWordsFloat(float Number,bool IsTiny=false)
{
	CEasyString temp;


	if(Number==0.0f)
	{
		temp="0";
	}
	else if(Number<=1024.0f)
	{
		temp.Format("%.2f",Number);
	}else if(Number<=1048576.0f)
	{
		float size=Number/1024.0f;
		temp.Format("%.2fK",size);
	}else
	{
		float size=Number/1048576.0f;
		temp.Format("%.2fM",size);
	}
	if(IsTiny)
		return temp;
	else
		return temp+" Byte";
}