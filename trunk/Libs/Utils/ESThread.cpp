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
