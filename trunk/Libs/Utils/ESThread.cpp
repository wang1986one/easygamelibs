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
	m_pFactionList=NULL;
	m_pScript=NULL;
	m_IsInInterrupt=false;
	m_InterruptPos=0;	
	m_InterruptRecentControlPos=0;
	m_ResultCode=0;

}

CESThread::~CESThread(void)
{
}
