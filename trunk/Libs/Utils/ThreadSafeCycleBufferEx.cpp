/****************************************************************************/
/*                                                                          */
/*      �ļ���:    ThreadSafeCycleBufferEx.cpp                                        */
/*      ��������:  2009��09��11��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#include "StdAfx.h"


IMPLEMENT_CLASS_INFO_STATIC(CThreadSafeCycleBufferEx,CCycleBufferEx);

CThreadSafeCycleBufferEx::CThreadSafeCycleBufferEx(void):CCycleBufferEx()
{
	m_IsLockFront=true;
	m_IsLockBack=true;
}

CThreadSafeCycleBufferEx::CThreadSafeCycleBufferEx(UINT Size,UINT SmoothSize):CCycleBufferEx()
{
	Create(Size,SmoothSize);	
}

CThreadSafeCycleBufferEx::CThreadSafeCycleBufferEx(LPVOID pBuff,UINT Size,UINT SmoothSize):CCycleBufferEx()
{
	Create(pBuff,Size,SmoothSize);
}

CThreadSafeCycleBufferEx::~CThreadSafeCycleBufferEx(void)
{
	Destory();
}

BOOL CThreadSafeCycleBufferEx::Create(UINT Size,UINT SmoothSize)
{
	Destory();
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

	if(Size<=SmoothSize*2)
	{
		return FALSE;
	}
	m_pBuffer=new BYTE[Size];
	m_BufferSize=Size-SmoothSize;
	m_SmoothSize=SmoothSize;
	m_BufferHead=0;
	m_BufferTail=0;	
	m_IsSelfBuffer=true;
	return TRUE;
}

BOOL CThreadSafeCycleBufferEx::Create(LPVOID pBuff,UINT Size,UINT SmoothSize)
{
	Destory();
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
	
	if(Size<=SmoothSize*2)
	{
		return FALSE;
	}

	m_pBuffer=(BYTE *)pBuff;
	m_BufferSize=Size-SmoothSize;
	m_SmoothSize=SmoothSize;
	m_BufferHead=0;
	m_BufferTail=0;	
	m_IsSelfBuffer=false;
	return TRUE;
}

void CThreadSafeCycleBufferEx::Destory()
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
	m_SmoothSize=0;
	m_BufferHead=0;
	m_BufferTail=0;	
	m_IsSelfBuffer=true;
}
