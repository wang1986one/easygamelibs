#include "StdAfx.h"


IMPLEMENT_CLASS_INFO_STATIC(CCycleBufferEx,CNameObject);

CCycleBufferEx::CCycleBufferEx(void):CNameObject()
{
	m_pBuffer=NULL;
	m_BufferSize=0;	
	m_SmoothSize=0;
	m_BufferHead=0;
	m_BufferTail=0;	
	m_IsSelfBuffer=true;
}

CCycleBufferEx::CCycleBufferEx(UINT Size,UINT SmoothSize):CNameObject()
{
	Create(Size,SmoothSize);	
}

CCycleBufferEx::CCycleBufferEx(LPVOID pBuff,UINT Size,UINT SmoothSize):CNameObject()
{
	Create(pBuff,Size,SmoothSize);
}

CCycleBufferEx::~CCycleBufferEx(void)
{
	Destory();
}

BOOL CCycleBufferEx::Create(UINT Size,UINT SmoothSize)
{
	Destory();
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

BOOL CCycleBufferEx::Create(LPVOID pBuff,UINT Size,UINT SmoothSize)
{
	Destory();
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

void CCycleBufferEx::Destory()
{
	if(m_IsSelfBuffer)
		SAFE_DELETE_ARRAY(m_pBuffer);

	m_pBuffer=NULL;
	m_BufferSize=0;	
	m_SmoothSize=0;
	m_BufferHead=0;
	m_BufferTail=0;	
	m_IsSelfBuffer=true;
}
