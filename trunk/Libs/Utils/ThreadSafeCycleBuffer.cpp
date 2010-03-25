/****************************************************************************/
/*                                                                          */
/*      �ļ���:    ThreadSafeCycleBuffer.cpp                                */
/*      ��������:  2009��07��06��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#include "StdAfx.h"


IMPLEMENT_CLASS_INFO_STATIC(CThreadSafeCycleBuffer,CNameObject);

CThreadSafeCycleBuffer::CThreadSafeCycleBuffer(void):CNameObject()
{
	m_pBuffer=NULL;
	m_BufferSize=0;	
	m_BufferHead=0;
	m_BufferTail=0;	
	m_IsSelfBuffer=true;
	m_IsLockFront=true;
	m_IsLockBack=true;
}

CThreadSafeCycleBuffer::CThreadSafeCycleBuffer(UINT Size):CNameObject()
{
	Create(Size);	
}

CThreadSafeCycleBuffer::CThreadSafeCycleBuffer(LPVOID pBuff,UINT Size):CNameObject()
{
	Create(pBuff,Size);
}

CThreadSafeCycleBuffer::~CThreadSafeCycleBuffer(void)
{
	Destory();
}

BOOL CThreadSafeCycleBuffer::Create(UINT Size)
{
	CAutoLockEx FrontLock;
	if(m_IsLockFront)
	{
		FrontLock.Lock(m_FrontLock);
	}
	CAutoLockEx BackLock;
	if(m_IsLockBack)
	{
		BackLock.Lock(m_BackLock);
	}
	m_pBuffer=new BYTE[Size];
	m_BufferSize=Size;
	m_BufferHead=0;
	m_BufferTail=0;	
	m_IsSelfBuffer=true;
	m_IsLockFront=true;
	m_IsLockBack=true;
	return TRUE;
}

BOOL CThreadSafeCycleBuffer::Create(LPVOID pBuff,UINT Size)
{
	CAutoLockEx FrontLock;
	if(m_IsLockFront)
	{
		FrontLock.Lock(m_FrontLock);
	}
	CAutoLockEx BackLock;
	if(m_IsLockBack)
	{
		BackLock.Lock(m_BackLock);
	}
	m_pBuffer=(BYTE *)pBuff;
	m_BufferSize=Size;
	m_BufferHead=0;
	m_BufferTail=0;	
	m_IsSelfBuffer=false;
	m_IsLockFront=true;
	m_IsLockBack=true;
	return TRUE;
}

void CThreadSafeCycleBuffer::Destory()
{
	CAutoLockEx FrontLock;
	if(m_IsLockFront)
	{
		FrontLock.Lock(m_FrontLock);
	}
	CAutoLockEx BackLock;
	if(m_IsLockBack)
	{
		BackLock.Lock(m_BackLock);
	}
	if(m_IsSelfBuffer)
		SAFE_DELETE_ARRAY(m_pBuffer);
	m_pBuffer=NULL;
	m_BufferSize=0;	
	m_BufferHead=0;
	m_BufferTail=0;	
	m_IsSelfBuffer=true;
	m_IsLockFront=true;
	m_IsLockBack=true;
}