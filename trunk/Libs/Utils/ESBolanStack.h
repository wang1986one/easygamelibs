/****************************************************************************/
/*                                                                          */
/*      文件名:    ESBolanStack.h		                                    */
/*      创建日期:  2010年08月26日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once

class CESBolanStack
{
protected:
	ES_BOLAN *	m_BolanBuff;
	int			m_BolanBuffSize;
	int			m_BolanCount;
	int			m_GrowSize;	
public:
	CESBolanStack(int InitBuffSize=64,int GrowSize=16);
	~CESBolanStack();
	void Grow();
	int GetSize()
	{
		return m_BolanCount;
	}
	bool Push(ES_BOLAN * Bolan);
	ES_BOLAN * Pop();	
	bool Pop(int Count);
	ES_BOLAN * GetTop();
	ES_BOLAN * GetAt(int Index);
	void Clear()
	{
		m_BolanCount=0;
	}


	int PushScript(const char * ExpStr,CESVariableList* pVariableList,CESFactionList * pFactionList,int& LastLine);
	
	
	int FindIdentifier(int Start,const char * Name);
protected:
	int DealVariableDefine(CESVariableList* pVariableList,int& LastLine);
	int FindKeyWord(const char * KeyWord);
};
