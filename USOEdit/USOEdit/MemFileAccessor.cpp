/****************************************************************************/
/*                                                                          */
/*      文件名:    MemFileAccessor.cpp                                      */
/*      创建日期:  2011年10月07日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
/****************************************************************************/
/*                                                                          */
/*      文件名:    MemFileAccessor.cpp                                      */
/*      创建日期:  2010年02月09日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#include "StdAfx.h"


CMemFileAccessor::CMemFileAccessor(void)
{
	m_Buffer=NULL;
	m_BufferSize=0;
	m_DataPtr=0;
	m_WriteSize=0;
}

CMemFileAccessor::~CMemFileAccessor(void)
{
	SAFE_DELETE_ARRAY(m_Buffer);
}

BOOL CMemFileAccessor::Open(LPCTSTR FileName,int OpenMode)
{
	return TRUE;
}

void CMemFileAccessor::Close()
{
}

ULONG64 CMemFileAccessor::GetSize()
{
	return m_BufferSize;
}

ULONG64 CMemFileAccessor::Read(LPVOID pBuff,ULONG64 Size)
{
	if(Size>m_BufferSize-m_DataPtr)
		Size=m_BufferSize-m_DataPtr;	
	if(pBuff)
		memcpy(pBuff,m_Buffer+m_DataPtr,Size);
	m_DataPtr+=(UINT)Size;
	return Size;
}
ULONG64 CMemFileAccessor::Write(LPCVOID pBuff,ULONG64 Size)
{	
	if(m_Buffer)
	{
		if(Size>m_BufferSize-m_DataPtr)
			Size=m_BufferSize-m_DataPtr;
		
		if(pBuff)
			memcpy(m_Buffer+m_DataPtr,pBuff,Size);
	}

	m_DataPtr+=(UINT)Size;
	if(m_DataPtr>m_WriteSize)
		m_WriteSize=m_DataPtr;	
	
	return Size;
}


BOOL CMemFileAccessor::IsEOF()
{
	if(m_DataPtr>=m_BufferSize)
		return TRUE;
	return FALSE;
}

BOOL CMemFileAccessor::Seek(LONG64 Offset,int SeekMode)
{
	LONG64 Ptr=m_DataPtr;
	switch(SeekMode)
	{
	case seekBegin:
		Ptr=Offset;
		break;
	case seekCurrent:
		Ptr+=Offset;
		break;
	case seekEnd:
		Ptr=(LONG64)m_BufferSize-Offset;
		break;
	default:
		return false;
	}
	if(Ptr<0)
		Ptr=0;
	if(m_Buffer)
	{
		if(Ptr>(LONG64)m_BufferSize)
			Ptr=(LONG64)m_BufferSize;
	}	
	m_DataPtr=(UINT)Ptr;
	return true;	
}

ULONG64 CMemFileAccessor::GetCurPos()
{
	return m_DataPtr;
}

UINT CMemFileAccessor::GetWriteSize()
{
	return m_WriteSize;
}

bool CMemFileAccessor::AllocBuffer(UINT Size)
{
	m_BufferSize=Size;
	SAFE_DELETE_ARRAY(m_Buffer);
	m_Buffer=new char[m_BufferSize];
	m_DataPtr=0;
	m_WriteSize=0;
	return true;
}

bool CMemFileAccessor::CopyToClipBroad(UINT ClipFormat)
{
	if(m_Buffer==NULL||m_DataPtr==0)
		return false;
	HGLOBAL hClip = GlobalAlloc(GMEM_ZEROINIT|GMEM_MOVEABLE,m_BufferSize);
	if(!hClip)
		return false;
	char *pBuff = (char*)GlobalLock(hClip);
	if(!pBuff)
	{
		GlobalFree(hClip);
		return false;
	}
	memcpy(pBuff,m_Buffer,m_BufferSize);		

	GlobalUnlock(hClip);

	if(OpenClipboard(NULL))
	{
		EmptyClipboard();
		SetClipboardData( ClipFormat,hClip);
		CloseClipboard();
		return true;
	}
	else
	{
		GlobalFree(hClip);
		return false;
	}
}

bool CMemFileAccessor::CopyFromClipBroad(UINT ClipFormat)
{
	if(OpenClipboard(NULL))
	{
		HGLOBAL hClip=NULL;

		if( hClip=GetClipboardData(ClipFormat) )
		{
			
			char *pBuff = (char *)GlobalLock(hClip);				
			if(pBuff)
			{				
				SAFE_DELETE_ARRAY(m_Buffer);
				m_BufferSize=GlobalSize(hClip);
				m_DataPtr=0;
				m_WriteSize=0;
				m_Buffer=new char[m_BufferSize];
				memcpy(m_Buffer,pBuff,m_BufferSize);
				GlobalUnlock(hClip);
				CloseClipboard();
				return true;
			}							
		}
		CloseClipboard();		
	}
	return false;
}


BOOL CMemFileAccessor::SetCreateTime(const CEasyTime& Time)
{
	return FALSE;
}
BOOL CMemFileAccessor::GetCreateTime(CEasyTime& Time)
{
	return FALSE;
}

BOOL CMemFileAccessor::SetLastAccessTime(const CEasyTime& Time)
{
	return FALSE;
}
BOOL CMemFileAccessor::GetLastAccessTime(CEasyTime& Time)
{
	return FALSE;
}

BOOL CMemFileAccessor::SetLastWriteTime(const CEasyTime& Time)
{
	return FALSE;
}
BOOL CMemFileAccessor::GetLastWriteTime(CEasyTime& Time)
{
	return FALSE;
}