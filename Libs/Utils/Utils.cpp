#include "stdafx.h"


void Test()
{
	CSmartValue Value1;
	CSmartValue Value2;

	Value1=(char)0;
	Value1=(unsigned char)0;
	Value1=(short)0;
	Value1=(unsigned short)0;
	Value1=(int)0;
	Value1=(unsigned int)0;
	Value1=(__int64)0;
	Value1=(unsigned __int64)0;

	Value2=Value1;

}




