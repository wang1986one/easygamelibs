/****************************************************************************/
/*                                                                          */
/*      �ļ���:    BaseServer.cpp                                           */
/*      ��������:  2009��07��06��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      ��������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ������������κ���ҵ�ͷ���ҵ������������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#include "StdAfx.h"

IMPLEMENT_CLASS_INFO_STATIC(CBaseServer,CEasyThread);

CBaseServer::CBaseServer(void)
{
	m_TCPRecvBytes=0;
	m_TCPSendBytes=0;
	m_UDPRecvBytes=0;
	m_UDPSendBytes=0;

	m_TCPRecvCount=0;
	m_TCPSendCount=0;
	m_UDPRecvCount=0;
	m_UDPSendCount=0;
}

CBaseServer::~CBaseServer(void)
{
}