/****************************************************************************/
/*                                                                          */
/*      �ļ���:    EasyBuffer.cpp                                           */
/*      ��������:  2009��07��06��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#include "StdAfx.h"


IMPLEMENT_CLASS_INFO_STATIC(CEasyBuffer,CNameObject);

CEasyBuffer::CEasyBuffer(void)
{
	m_pBuffer=NULL;
	m_BufferSize=0;
	m_UsedSize=0;
	m_IsSelfBuffer=true;
}

CEasyBuffer::CEasyBuffer(UINT Size)
{
	m_pBuffer=NULL;
	m_BufferSize=0;
	m_UsedSize=0;
	m_IsSelfBuffer=true;
	Create(Size);
}

CEasyBuffer::CEasyBuffer(LPVOID pBuff,UINT Size)
{
	m_pBuffer=NULL;
	m_BufferSize=0;
	m_UsedSize=0;
	m_IsSelfBuffer=true;
	Create(pBuff,Size);
}

CEasyBuffer::CEasyBuffer(const CEasyBuffer& Buffer)
{
	m_pBuffer=NULL;
	m_BufferSize=0;
	m_UsedSize=0;
	m_IsSelfBuffer=true;
	Create(Buffer.GetBufferSize());
	PushBack(Buffer.GetBuffer(),Buffer.GetUsedSize());
}

CEasyBuffer::~CEasyBuffer(void)
{
	Destory();
}

BOOL CEasyBuffer::Create(UINT Size)
{
	Destory();
	if(Size)
	{
		m_pBuffer=new BYTE[Size];
		m_BufferSize=Size;
		m_UsedSize=0;
		m_IsSelfBuffer=true;
		return TRUE;
	}
	return FALSE;
}

BOOL CEasyBuffer::Create(LPVOID pBuff,UINT Size)
{
	Destory();
	if(pBuff&&Size)
	{
		m_pBuffer=(BYTE *)pBuff;
		m_BufferSize=Size;
		m_UsedSize=0;
		m_IsSelfBuffer=false;
		return TRUE;
	}
	return FALSE;
}

void CEasyBuffer::Destory()
{
	if(m_IsSelfBuffer)
		SAFE_DELETE_ARRAY(m_pBuffer);
}

