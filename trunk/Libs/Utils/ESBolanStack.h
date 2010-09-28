/****************************************************************************/
/*                                                                          */
/*      �ļ���:    ESBolanStack.h		                                    */
/*      ��������:  2010��08��26��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
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
