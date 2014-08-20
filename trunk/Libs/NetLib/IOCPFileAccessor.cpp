/****************************************************************************/
/*                                                                          */
/*      �ļ���:    IOCPFileAccessor.cpp                                     */
/*      ��������:  2010��02��09��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#include "StdAfx.h"


IMPLEMENT_CLASS_INFO_STATIC(CIOCPFileAccessor,CNameObject);

CIOCPFileAccessor::CIOCPFileAccessor(void)
{
	m_FileHandle=INVALID_HANDLE_VALUE ;
	m_pServer=NULL;
	m_WantClose=FALSE;	
	m_Session=0;
	m_pIOCPEventRouter=NULL;	
	m_ReadPosition.QuadPart=0;
	m_WritePosition.QuadPart=0;
	m_UseAsyncQuery=false;
}

CIOCPFileAccessor::~CIOCPFileAccessor(void)
{
}


BOOL CIOCPFileAccessor::OnIOCPEvent(int EventID,COverLappedObject * pOverLappedObject)
{	
	if(m_FileHandle!=INVALID_HANDLE_VALUE)
	{
		
			
		if(EventID==IOE_PACKET)
		{
			if(pOverLappedObject->GetType()==IO_FILE_WRITE)
			{
				if(m_DataQueue.PushBack(pOverLappedObject))
				{
					if(!m_UseAsyncQuery)
					{
						if(!DoNextOverlappedOperation())
						{
						}
					}					
					return TRUE;
				}
				else
				{
					PrintNetLog(0xffffffff,_T("CIOCPFileAccessor::���ݶ���������"));										
					PushError(FET_QUEUE_ERROR,FQEC_QUEUE_FULL,pOverLappedObject->GetType(),
						pOverLappedObject->GetOverlapped()->OffsetHigh,
						pOverLappedObject->GetOverlapped()->Offset,
						pOverLappedObject->GetDataBuff()->GetUsedSize());
				}
			}
			else if(pOverLappedObject->GetType()==IO_FILE_READ)
			{												
				if(m_DataQueue.PushBack(pOverLappedObject))
				{
					if(!m_UseAsyncQuery)
					{
						if(!DoNextOverlappedOperation())
						{
						}
					}	
					return TRUE;
				}
				else
				{
					PrintNetLog(0xffffffff,_T("CIOCPFileAccessor::���ݶ���������"));										
					PushError(FET_QUEUE_ERROR,FQEC_QUEUE_FULL,pOverLappedObject->GetType(),
						pOverLappedObject->GetOverlapped()->OffsetHigh,
						pOverLappedObject->GetOverlapped()->Offset,
						pOverLappedObject->GetDataBuff()->GetUsedSize());
				}
			}
			else
			{
				PrintNetLog(0xffffffff,_T("CIOCPFileAccessor::�յ��Ƿ�IOCP����"));
				PushError(FET_UNKNOW,0,
					pOverLappedObject->GetOverlapped()->OffsetHigh,
					pOverLappedObject->GetOverlapped()->Offset,
					pOverLappedObject->GetDataBuff()->GetUsedSize());
			}

		}
		else
		{
			PrintNetLog(0xffffffff,_T("CIOCPFileAccessor::�յ�IOCP����"));	
			PushError(FET_SYSTEM_ERROR,pOverLappedObject->GetErrorCode(),
				pOverLappedObject->GetOverlapped()->OffsetHigh,
				pOverLappedObject->GetOverlapped()->Offset,
				pOverLappedObject->GetDataBuff()->GetUsedSize());
			
		}			
	}
	else
	{
		PrintNetLog(0xffffffff,_T("CIOCPFileAccessor::�ļ�δ��IOCP�������ԣ�"));
	}
	GetServer()->DeleteOverLappedObject(pOverLappedObject);


	return TRUE;
}




void CIOCPFileAccessor::Destory()
{
	Close();

	if(m_pIOCPEventRouter)
	{
		m_pIOCPEventRouter->SetEventHander(NULL);
		GetServer()->DeleteEventRouter(m_pIOCPEventRouter);
		m_pIOCPEventRouter=NULL;
	}

	InterlockedExchange((LPLONG)&(m_WantClose),FALSE);
}

BOOL CIOCPFileAccessor::Open(LPCTSTR lpFileName,
		  DWORD dwDesiredAccess, 
		  DWORD dwShareMode, 
		  LPSECURITY_ATTRIBUTES lpSecurityAttributes, 
		  DWORD dwCreationDisposition, 
		  DWORD dwFlagsAndAttributes,
		  int DataQueueSize,
		  int RequestQueuSize,
		  int ErrorQueuSize)
{

	if(m_FileHandle!=INVALID_HANDLE_VALUE)
	{
		Close();
	}

	if(GetServer()==NULL)
		return FALSE;

	if(m_pIOCPEventRouter==NULL)
	{
		m_pIOCPEventRouter=GetServer()->CreateEventRouter();	
		m_pIOCPEventRouter->Init(this);
	}

	if(!m_DataQueue.Create(DataQueueSize))
	{
		PrintNetLog(0,_T("CIOCPFileAccessor::�������ݻ���ʧ��"));
		return FALSE;
	}
	if(RequestQueuSize==0)
	{		
		m_UseAsyncQuery=true;
		PrintNetLog(0,_T("CIOCPFileAccessor::�����첽����ģʽ"));
	}
	else
	{
		m_UseAsyncQuery=false;
		if(!m_RequestQueue.Create(RequestQueuSize))
		{
			PrintNetLog(0,_T("CIOCPFileAccessor::�������󻺳�ʧ��"));
			return FALSE;
		}
	}
	if(!m_ErrorQueue.Create(ErrorQueuSize))
	{
		PrintNetLog(0,_T("CIOCPFileAccessor::�������󻺳�ʧ��"));
		return FALSE;
	}


	

	dwFlagsAndAttributes|=FILE_FLAG_OVERLAPPED;

	m_FileHandle=::CreateFile(lpFileName,dwDesiredAccess,dwShareMode,lpSecurityAttributes,
		dwCreationDisposition,dwFlagsAndAttributes,NULL);

	if(m_FileHandle==INVALID_HANDLE_VALUE)
	{
		UINT ErrorCode=GetLastError();
		PrintNetLog(0,_T("CIOCPFileAccessor::���ļ�ʧ��%u"),ErrorCode);
		PushError(FET_SYSTEM_ERROR,ErrorCode);
		return FALSE;
	}

	if(!GetServer()->BindFile(m_FileHandle))
	{
		Close();
		return FALSE;		
	}
	m_ReadPosition.QuadPart=0;
	m_WritePosition.QuadPart=0;
	OnOpen();
	return TRUE;
}

BOOL CIOCPFileAccessor::OpenByHandle(HANDLE FileHandle,int DataQueueSize,int RequestQueuSize,int ErrorQueuSize)
{

	if(FileHandle==INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	if(m_FileHandle!=INVALID_HANDLE_VALUE)
	{
		Close();
	}

	if(GetServer()==NULL)
		return FALSE;

	if(m_pIOCPEventRouter==NULL)
	{
		m_pIOCPEventRouter=GetServer()->CreateEventRouter();	
		m_pIOCPEventRouter->Init(this);
	}

	if(!m_DataQueue.Create(DataQueueSize))
	{
		PrintNetLog(0,_T("CIOCPFileAccessor::�������ݻ���ʧ��"));
		return FALSE;
	}

	if(RequestQueuSize==0)
	{		
		m_UseAsyncQuery=true;
		PrintNetLog(0,_T("CIOCPFileAccessor::�����첽����ģʽ"));
	}
	else
	{
		m_UseAsyncQuery=false;
		if(!m_RequestQueue.Create(RequestQueuSize))
		{
			PrintNetLog(0,_T("CIOCPFileAccessor::�������󻺳�ʧ��"));
			return FALSE;
		}
	}
	if(!m_ErrorQueue.Create(ErrorQueuSize))
	{
		PrintNetLog(0,_T("CIOCPFileAccessor::�������󻺳�ʧ��"));
		return FALSE;
	}

	if(!GetServer()->BindFile(FileHandle))
	{
		return FALSE;		
	}
	m_FileHandle=FileHandle;
	m_ReadPosition.QuadPart=0;
	m_WritePosition.QuadPart=0;
	OnOpen();
	return TRUE;
}


void CIOCPFileAccessor::Close()
{
	if(m_FileHandle!=INVALID_HANDLE_VALUE)
	{	
		CloseHandle(m_FileHandle);
		m_FileHandle=INVALID_HANDLE_VALUE;
	}

	InterlockedExchange((LPLONG)&(m_WantClose),FALSE);

	COverLappedObject * pOverLappedObject;
	while(m_DataQueue.PopFront(pOverLappedObject))
	{	
		GetServer()->DeleteOverLappedObject(pOverLappedObject);		
	}
	while(m_RequestQueue.PopFront(pOverLappedObject))
	{	
		GetServer()->DeleteOverLappedObject(pOverLappedObject);		
	}	


	OnClose();	
}

void CIOCPFileAccessor::QueryClose()
{
	InterlockedExchange((LPLONG)&(m_WantClose),TRUE);
}

void CIOCPFileAccessor::OnOpen()
{

}
void CIOCPFileAccessor::OnClose()
{

}

int CIOCPFileAccessor::Update(int ProcessPacketLimit)
{
	int PacketCount1=0;
	int PacketCount2=0;
	
	COverLappedObject * pOverLappedObject;
	while(m_DataQueue.PopFront(pOverLappedObject))
	{			
		if(pOverLappedObject->GetType()==IO_FILE_READ)
		{
			ULONG64_CONVERTER ReadOffset;
			ReadOffset.HighPart=pOverLappedObject->GetOverlapped()->OffsetHigh;
			ReadOffset.LowPart=pOverLappedObject->GetOverlapped()->Offset;
			OnReadData(ReadOffset.QuadPart,*(pOverLappedObject->GetDataBuff()));
		}
		else
		{
			ULONG64_CONVERTER WriteOffset;
			WriteOffset.HighPart=pOverLappedObject->GetOverlapped()->OffsetHigh;
			WriteOffset.LowPart=pOverLappedObject->GetOverlapped()->Offset;
			OnWriteData(WriteOffset.QuadPart,*(pOverLappedObject->GetDataBuff()));
		}
		
			
		GetServer()->DeleteOverLappedObject(pOverLappedObject);
		PacketCount1++;
		if(PacketCount1>=ProcessPacketLimit)
			break;
	}

	FIOCP_ERROR_INFO ErrorInfo;
	while(m_ErrorQueue.PopFront(ErrorInfo))
	{			
		OnError(ErrorInfo.ErrorType,ErrorInfo.ErrorCode,ErrorInfo.QueryType,ErrorInfo.QueryPos,ErrorInfo.QuerySize);
		PacketCount2++;
		if(PacketCount2>=ProcessPacketLimit)
			break;
	}
	
	if(m_WantClose)
	{		
		Close();		
	}
	return PacketCount1+PacketCount2;
}

void CIOCPFileAccessor::OnReadData(ULONG64 ReadPosition,const CEasyBuffer& DataBuffer)
{

}

void CIOCPFileAccessor::OnWriteData(ULONG64 WritePosition,const CEasyBuffer& DataBuffer)
{

}

void CIOCPFileAccessor::OnError(UINT ErrorType,UINT ErrorCode,int QueryType,ULONG64 QueryPos,ULONG64 QuerySize)
{

}

BOOL CIOCPFileAccessor::QueryRead(ULONG64 ReadSize)
{
	if(QueryRead(m_ReadPosition.QuadPart,ReadSize))
	{
		m_ReadPosition.QuadPart+=ReadSize;
		return TRUE;
	}
	return FALSE;
}

BOOL CIOCPFileAccessor::QueryRead(ULONG64 StartPos,ULONG64 ReadSize)
{
	if(m_FileHandle!=INVALID_HANDLE_VALUE)
	{
		while(ReadSize)
		{
			ULONG64_CONVERTER Pos;
			Pos.QuadPart=StartPos;

			ULONG64 PacketSize=ReadSize;
			if(PacketSize>MAX_DATA_PACKET_SIZE)
				PacketSize=MAX_DATA_PACKET_SIZE;
			ReadSize-=PacketSize;

			COverLappedObject * pOverLappedObject=GetServer()->CreateOverLappedObject();
			if(pOverLappedObject==NULL)
			{
				PrintNetLog(0xffffffff,_T("CIOCPFileAccessor::����Write��OverLappedObjectʧ�ܣ�"));
				PushError(FET_QUEUE_ERROR,FQEC_QUEUE_FULL);
				return FALSE;
			}

			pOverLappedObject->SetType(IO_FILE_READ);	
			pOverLappedObject->SetIOCPEventRouter(m_pIOCPEventRouter);
			pOverLappedObject->GetDataBuff()->SetUsedSize((int)PacketSize);
			pOverLappedObject->SetParentID(GetID());
			pOverLappedObject->GetOverlapped()->Offset=Pos.LowPart;
			pOverLappedObject->GetOverlapped()->OffsetHigh=Pos.HighPart;
					
			if(m_UseAsyncQuery)
			{		
				if(!DoOverlappedOperation(pOverLappedObject))
				{					
					GetServer()->DeleteOverLappedObject(pOverLappedObject);
					return FALSE;					
				}
			}
			else
			{
				if(!m_RequestQueue.PushBack(pOverLappedObject))
				{
					PrintNetLog(0xffffffff,_T("CIOCPFileAccessor::���󻺳����������"));	
					GetServer()->DeleteOverLappedObject(pOverLappedObject);
					PushError(FET_QUEUE_ERROR,FQEC_QUEUE_FULL);
					return FALSE;
				}				
			}

			Pos.QuadPart+=PacketSize;
		}
		DoNextOverlappedOperation();
		return TRUE;
	}	
	return FALSE;	
}


BOOL CIOCPFileAccessor::QueryWrite(LPVOID pData,ULONG64 WriteSize)
{
	if(QueryWrite(m_WritePosition.QuadPart,pData,WriteSize))
	{
		m_WritePosition.QuadPart+=WriteSize;
		return TRUE;
	}
	return FALSE;
}

BOOL CIOCPFileAccessor::QueryWrite(ULONG64 StartPos,LPVOID pData,ULONG64 WriteSize)
{
	if(m_FileHandle!=INVALID_HANDLE_VALUE)
	{
		while(WriteSize)
		{
			ULONG64_CONVERTER Pos;
			Pos.QuadPart=StartPos;

			ULONG64 PacketSize=WriteSize;
			if(PacketSize>MAX_DATA_PACKET_SIZE)
				PacketSize=MAX_DATA_PACKET_SIZE;
			WriteSize-=PacketSize;

			COverLappedObject * pOverLappedObject=GetServer()->CreateOverLappedObject();
			if(pOverLappedObject==NULL)
			{
				PrintNetLog(0xffffffff,_T("CIOCPFileAccessor::����Write��OverLappedObjectʧ�ܣ�"));
				PushError(FET_QUEUE_ERROR,FQEC_QUEUE_FULL);
				return FALSE;
			}

			pOverLappedObject->SetType(IO_FILE_WRITE);	
			pOverLappedObject->SetIOCPEventRouter(m_pIOCPEventRouter);
			pOverLappedObject->GetDataBuff()->SetUsedSize(0);
			pOverLappedObject->SetParentID(GetID());
			pOverLappedObject->GetOverlapped()->Offset=Pos.LowPart;
			pOverLappedObject->GetOverlapped()->OffsetHigh=Pos.HighPart;
			

			if(!pOverLappedObject->GetDataBuff()->PushBack(pData,(int)PacketSize))
			{
				GetServer()->DeleteOverLappedObject(pOverLappedObject);
				PrintNetLog(0xffffffff,_T("CIOCPFileAccessor::Ҫд������ݰ�����"));
				PushError(FET_UNKNOW,0);
				return FALSE;
			}			
			if(m_UseAsyncQuery)
			{		
				if(!DoOverlappedOperation(pOverLappedObject))
				{					
					GetServer()->DeleteOverLappedObject(pOverLappedObject);
					return FALSE;					
				}
			}
			else
			{
				if(!m_RequestQueue.PushBack(pOverLappedObject))
				{
					PrintNetLog(0xffffffff,_T("CIOCPFileAccessor::���󻺳����������"));	
					GetServer()->DeleteOverLappedObject(pOverLappedObject);
					PushError(FET_QUEUE_ERROR,FQEC_QUEUE_FULL);
					return FALSE;
				}
				
			}
			
			pData=(char *)pData+PacketSize;
			Pos.QuadPart+=PacketSize;
		}
		DoNextOverlappedOperation();
		return TRUE;
	}	
	return FALSE;	
}

ULONG64 CIOCPFileAccessor::GetFileSize()
{
	if(m_FileHandle!=INVALID_HANDLE_VALUE)
	{
		ULONG64_CONVERTER Size;

		Size.QuadPart=0;

		Size.LowPart=::GetFileSize(m_FileHandle,&(Size.HighPart));
		return Size.QuadPart;
	}
	else
		return 0;
}

BOOL CIOCPFileAccessor::SeekRead(LONG64 Offset,int SeekMode)
{
	if(m_FileHandle!=INVALID_HANDLE_VALUE)
	{
		switch(SeekMode)
		{
		case FILE_BEGIN:
			m_ReadPosition.QuadPart=Offset;
		case FILE_CURRENT:
			m_ReadPosition.QuadPart=(LONG64)m_ReadPosition.QuadPart+Offset;
		case FILE_END:
			return FALSE;
		}		
		return TRUE;
	}
	else
		return FALSE;
}

BOOL CIOCPFileAccessor::SeekWrite(LONG64 Offset,int SeekMode)
{
	if(m_FileHandle!=INVALID_HANDLE_VALUE)
	{
		switch(SeekMode)
		{
		case FILE_BEGIN:
			m_WritePosition.QuadPart=Offset;
		case FILE_CURRENT:
			m_WritePosition.QuadPart=(LONG64)m_ReadPosition.QuadPart+Offset;
		case FILE_END:
			return FALSE;
		}		
		return TRUE;
	}
	else
		return FALSE;
}

ULONG64 CIOCPFileAccessor::GetCurReadPos()
{
	return m_ReadPosition.QuadPart;
}

ULONG64 CIOCPFileAccessor::GetCurWritePos()
{
	return m_WritePosition.QuadPart;
}


BOOL CIOCPFileAccessor::DoOverlappedOperation(COverLappedObject * pOverLappedObject)
{
	if(m_FileHandle!=INVALID_HANDLE_VALUE)
	{
		if(pOverLappedObject->GetType()==IO_FILE_READ)
		{
			if(!ReadFile(m_FileHandle,
				pOverLappedObject->GetDataBuff()->GetBuffer(),
				pOverLappedObject->GetDataBuff()->GetUsedSize(),
				NULL,pOverLappedObject->GetOverlapped()))
			{
				int ErrorCode=GetLastError();
				if(ErrorCode!=ERROR_IO_PENDING)
				{
					PrintNetLog(0xffffffff,_T("CIOCPFileAccessor::����Read����ʧ��%d��"),ErrorCode);
					PushError(FET_SYSTEM_ERROR,ErrorCode,
						pOverLappedObject->GetOverlapped()->OffsetHigh,
						pOverLappedObject->GetOverlapped()->Offset,
						pOverLappedObject->GetDataBuff()->GetUsedSize());
					return FALSE;
				}
			}
			return TRUE;
		}
		else if(pOverLappedObject->GetType()==IO_FILE_WRITE)
		{
			if(!WriteFile(m_FileHandle,
				pOverLappedObject->GetDataBuff()->GetBuffer(),
				pOverLappedObject->GetDataBuff()->GetUsedSize(),
				NULL,pOverLappedObject->GetOverlapped()))
			{
				int ErrorCode=GetLastError();
				if(ErrorCode!=ERROR_IO_PENDING)
				{
					PrintNetLog(0xffffffff,_T("CIOCPFileAccessor::����Write����ʧ��%d��"),ErrorCode);
					PushError(FET_SYSTEM_ERROR,ErrorCode,
						pOverLappedObject->GetOverlapped()->OffsetHigh,
						pOverLappedObject->GetOverlapped()->Offset,
						pOverLappedObject->GetDataBuff()->GetUsedSize());
					return FALSE;
				}
			}
		}
		else
		{
			PrintNetLog(0xffffffff,_T("CIOCPFileAccessor::δ֪��Overlapped����%d��"),pOverLappedObject->GetType());
			PushError(FET_UNKNOW,0);
			return FALSE;
		}
	}
	return FALSE;
}

BOOL CIOCPFileAccessor::DoNextOverlappedOperation()
{
	if(m_FileHandle!=INVALID_HANDLE_VALUE)
	{
		COverLappedObject * pOverLappedObject;
		if(m_RequestQueue.PopFront(pOverLappedObject))
		{
			if(DoOverlappedOperation(pOverLappedObject))
			{
				return TRUE;
			}
			GetServer()->DeleteOverLappedObject(pOverLappedObject);
			return FALSE;
		}
		else
		{
			return TRUE;
		}
	}
	return FALSE;
}

void CIOCPFileAccessor::PushError(UINT ErrorType,UINT ErrorCode,int QueryType,UINT QueryPosHigh,UINT QueryPosLow,UINT QuerySize)
{
	FIOCP_ERROR_INFO ErrorInfo;
	ULONG64_CONVERTER QueryPos;
	QueryPos.HighPart=QueryPosHigh;
	QueryPos.LowPart=QueryPosLow;
	ErrorInfo.ErrorType=ErrorType;
	ErrorInfo.ErrorCode=ErrorCode;
	ErrorInfo.QueryType=QueryType;
	ErrorInfo.QueryPos=QueryPos.QuadPart;
	ErrorInfo.QuerySize=QuerySize;
	m_ErrorQueue.PushBack(ErrorInfo);
}