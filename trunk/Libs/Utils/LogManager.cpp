/****************************************************************************/
/*                                                                          */
/*      �ļ���:    LogManager.cpp                                           */
/*      ��������:  2010��02��09��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#include "StdAfx.h"


IMPLEMENT_CLASS_INFO_STATIC(CLogManager,CNameObject);

TCHAR LOG_MANAGER_INSTANCE[]="LogManagerInstance";

CLogManager::CLogManager(void):CNameObject()
{
	m_LogChannels.Insert(LOG_SYSTEM_CHANNEL,new CVSOutputLogPrinter());

	CEasyString ImportLogFileName=MakeModuleFullPath(NULL,IMPORTANT_LOG_FILE_NAME);	

	m_LogChannels.Insert(LOG_IMPORTANT_CHANNEL,new CFileLogPrinter(ImportLogFileName,FILE_LOG_APPEND|FILE_LOG_SAFE_WRITE));
}

CLogManager::~CLogManager(void)
{
	void * Pos=m_LogChannels.GetFirstObjectPos();
	while(Pos)
	{
		UINT Key;
		ILogPrinter ** ppLogPrinter=m_LogChannels.GetNextObject(Pos,Key);
		if(ppLogPrinter)
		{
			SAFE_RELEASE(*ppLogPrinter)
		}
	}
	m_LogChannels.Clear();
}

UINT CLogManager::GetChannelCount()
{
	return m_LogChannels.GetObjectCount();
}

void CLogManager::AddChannel(UINT ChannelID,ILogPrinter * pLogPrinter)
{
	if(pLogPrinter==NULL)
		return;
	if(m_LogChannels.Find(ChannelID)!=NULL)
	{
		DelChannel(ChannelID);
	}
	m_LogChannels.Insert(ChannelID,pLogPrinter);
	pLogPrinter->AddUseRef();
}

ILogPrinter * CLogManager::GetChannel(UINT ChannelID)
{
	ILogPrinter ** ppLogPrinter=m_LogChannels.Find(ChannelID);
	if(ppLogPrinter)
	{
		return *ppLogPrinter;
	}
	return NULL;
}

BOOL CLogManager::DelChannel(UINT ChannelID)
{
	ILogPrinter ** ppLogPrinter=m_LogChannels.Find(ChannelID);
	if(ppLogPrinter)
	{
		SAFE_RELEASE(*ppLogPrinter);
		m_LogChannels.Delete(ChannelID);
		return TRUE;
	}
	return FALSE;
}

BOOL CLogManager::PrintLog(UINT ChannelID,int Level,DWORD Color,LPCTSTR Format,...)
{
	ILogPrinter * pLogPrinter=GetChannel(ChannelID);
	if(pLogPrinter)
	{
		va_list	vl;
		va_start(vl,Format);
		pLogPrinter->PrintLog(Level,Color,Format,vl);
		va_end( vl);
		return TRUE;
	}
	return FALSE;
}
BOOL CLogManager::PrintLogVL(UINT ChannelID,int Level,DWORD Color,LPCTSTR Format,va_list vl)
{
	ILogPrinter * pLogPrinter=GetChannel(ChannelID);
	if(pLogPrinter)
	{
		pLogPrinter->PrintLog(Level,Color,Format,vl);
		return TRUE;
	}
	return FALSE;
}


