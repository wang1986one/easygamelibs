/****************************************************************************/
/*                                                                          */
/*      �ļ���:    ESThread.cpp			                                    */
/*      ��������:  2010��08��26��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#include "StdAfx.h"


CESThread::CESThread(void)
{
	m_pVariableList=NULL;	
	m_MaxLocalVariableCount=DEFAULT_LOCAL_VARIABLE_COUNT;
	m_pCurLocalVariableList=NULL;
	m_pFunctionList=NULL;
	m_pScript=NULL;
	m_IsInInterrupt=false;
	m_InterruptPos=-1;	
	m_ResultCode=0;
	m_CallStack.Create(DEFAULT_CALL_STACK_SIZE,DEFAULT_CALL_STACK_SIZE);

}

CESThread::CESThread(UINT MaxLocalVariableCount)
{
	m_pVariableList=NULL;
	m_MaxLocalVariableCount=MaxLocalVariableCount;
	m_pCurLocalVariableList=NULL;
	m_pFunctionList=NULL;
	m_pScript=NULL;
	m_IsInInterrupt=false;
	m_InterruptPos=-1;	
	m_ResultCode=0;
	m_CallStack.Create(DEFAULT_CALL_STACK_SIZE,DEFAULT_CALL_STACK_SIZE);
}

CESThread::~CESThread(void)
{
}
