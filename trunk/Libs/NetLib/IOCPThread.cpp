#include "StdAfx.h"



IMPLEMENT_CLASS_INFO_STATIC(CIOCPThread,CEasyThread);

CIOCPThread::CIOCPThread(void):CEasyThread()
{
	m_hIOCP=NULL;
}

CIOCPThread::~CIOCPThread(void)
{
}

BOOL CIOCPThread::OnStart()
{
	PrintNetLog(0xff,"IOCP工作线程启动");	
	return TRUE;
}

BOOL CIOCPThread::OnRun()
{		

	DWORD	NumberOfBytes = 0;
	ULONG_PTR	CompletionKey = 0;
	OVERLAPPED_EX	* pOverlapped=NULL;
	
	

	if( GetQueuedCompletionStatus( m_hIOCP,&NumberOfBytes,&CompletionKey,(LPOVERLAPPED*)(&pOverlapped),100 ))
	{		
		pOverlapped->pOverLappedObject->GetDataBuff()->SetUsedSize(NumberOfBytes);
		pOverlapped->pOverLappedObject->SetEventID(IOE_PACKET);
		pOverlapped->pOverLappedObject->Process(IOE_PACKET);
		
	}
	else
	{		
		if(pOverlapped)
		{			
			UINT ErrorCode=GetLastError();
			if(ErrorCode!=64&&ErrorCode!=1236)
				PrintNetLog(0xff,"IOCP返回错误%d(%d)",
				ErrorCode,
				pOverlapped->pOverLappedObject->GetType());
			pOverlapped->pOverLappedObject->SetErrorCode(ErrorCode);
			pOverlapped->pOverLappedObject->SetEventID(IOE_ERROR);
			pOverlapped->pOverLappedObject->Process(IOE_ERROR);				
		}

	}	

	
	return TRUE;
}

void CIOCPThread::OnTerminate()
{
	PrintNetLog(0xff,"IOCP工作线程退出");
}