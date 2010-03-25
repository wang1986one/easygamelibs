/****************************************************************************/
/*                                                                          */
/*      �ļ���:    OverLappedObject.cpp                                     */
/*      ��������:  2009��07��06��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#include "StdAfx.h"


COverLappedObject::COverLappedObject(void)		
{
	m_ID=0;	
	m_DataBuff.Create(MAX_DATA_PACKET_SIZE);
	Create(NULL);
}

COverLappedObject::~COverLappedObject(void)
{	
	Destory();
}

BOOL COverLappedObject::Create(CNetServer * pCreator)
{
	m_pCreator=pCreator;	
	ZeroMemory(&m_OverLapped,sizeof(OVERLAPPED_EX));
	m_OverLapped.pOverLappedObject=this;
	m_DataBuff.SetUsedSize(0);
	m_TypeID=IO_NOTSET;
	m_AcceptSocket=INVALID_SOCKET;
	m_pIOCPEventRouter=NULL;
	m_RouterSession=0;
	m_Address.SetIP((ULONG)0);
	m_Address.SetPort(0);	
	m_AddressLen=sizeof(sockaddr_in);
	m_ParentID=0;	
	m_ErrorCode=0;
	m_EventID=0;
	return TRUE;
}
void COverLappedObject::Destory()
{		
	m_DataBuff.SetUsedSize(0);
	m_TypeID=IO_NOTSET;
	m_AcceptSocket=INVALID_SOCKET;
	m_pIOCPEventRouter=NULL;
	m_Address.SetIP((ULONG)0);
	m_Address.SetPort(0);	
	m_AddressLen=sizeof(sockaddr_in);
	m_RouterSession=0;
	m_ParentID=0;	
	m_ErrorCode=0;
	m_EventID=0;
}

void COverLappedObject::Release()
{
	m_pCreator->DeleteOverLappedObject(this);
}
