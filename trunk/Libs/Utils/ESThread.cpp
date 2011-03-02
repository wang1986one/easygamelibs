/****************************************************************************/
/*                                                                          */
/*      文件名:    ESThread.cpp			                                    */
/*      创建日期:  2010年08月26日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#include "StdAfx.h"


CESThread::CESThread(void)
{
	m_pVariableList=NULL;
	m_LocalVariableList.Create(DEFAULT_LOCAL_VARIABLE_COUNT);
	m_LocalVariableList.SetIDStart(LOCAL_VARIABLE_ID_START);
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
	m_LocalVariableList.Create(MaxLocalVariableCount);
	m_LocalVariableList.SetIDStart(LOCAL_VARIABLE_ID_START);
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
