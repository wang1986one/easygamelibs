/****************************************************************************/
/*                                                                          */
/*      �ļ���:    ControlPanel.cpp                                         */
/*      ��������:  2009��07��06��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#include "StdAfx.h"

CControlPanel::CControlPanel(void)
{
	m_MsgPool.Create(1024);
	m_CommandPool.Create(1024);
	m_ServerStatus.Create(SERVER_STATUS_BLOCK_SIZE);
#ifdef WIN32
	m_hWnd=NULL;
#endif
}

CControlPanel::~CControlPanel(void)
{

}

BOOL CControlPanel::PushMsg(LPCTSTR szMsg)
{
	FUNCTION_BEGIN;
	if(g_ServiceWorkMode==SWM_APPLICATION)
	{
		return PushMsg(CON_MSG_LOG,szMsg,(int)strlen(szMsg)+1);
	}
	FUNCTION_END;
	return FALSE;
}

BOOL CControlPanel::PushMsg(UINT Type,LPCVOID pData,int Len)
{
	FUNCTION_BEGIN;
#ifdef WIN32
	if(m_hWnd)
	{
		PANEL_MSG * pMsg;
		UINT ID=m_MsgPool.NewObject(&pMsg);
		if(ID)
		{
			pMsg->ID=ID;
			pMsg->MsgType=Type;
			if(Len>MAX_CONSOLE_MSG_LEN)
				Len=MAX_CONSOLE_MSG_LEN;
			memcpy(pMsg->Msg,pData,Len);
			PostMessage(m_hWnd,WM_USER_LOG_MSG,0,(LPARAM)pMsg);
			return TRUE;
		}
	}
#else
	PANEL_MSG * pMsg;
	UINT ID=m_MsgPool.NewObject(&pMsg);
	if(ID)
	{
		pMsg->ID=ID;
		pMsg->MsgType=Type;
		if(Len>MAX_CONSOLE_MSG_LEN)
			Len=MAX_CONSOLE_MSG_LEN;
		memcpy(pMsg->Msg,pData,Len);
		return TRUE;
	}
#endif
	FUNCTION_END;
	return FALSE;
}

PANEL_MSG * CControlPanel::GetMsg()
{
	FUNCTION_BEGIN;
	LPVOID Pos=m_MsgPool.GetFirstObjectPos();

	if(Pos)
		return m_MsgPool.GetNext(Pos);
	else
		return NULL;
	FUNCTION_END;
	return NULL;
}

BOOL CControlPanel::ReleaseMsg(UINT ID)
{
	FUNCTION_BEGIN;

	return m_MsgPool.DeleteObject(ID);

	FUNCTION_END;
	return FALSE;
}


BOOL CControlPanel::PushCommand(LPCTSTR szCommand)
{
	FUNCTION_BEGIN;

	PANEL_MSG * pMsg;
	UINT ID=m_CommandPool.NewObject(&pMsg);
	if(ID)
	{
		pMsg->ID=ID;
		strncpy_0(pMsg->Msg,MAX_CONSOLE_MSG_LEN,szCommand,MAX_CONSOLE_MSG_LEN);
		return TRUE;
	}


	FUNCTION_END;
	return FALSE;
}

PANEL_MSG * CControlPanel::GetCommand()
{
	FUNCTION_BEGIN;
	LPVOID Pos=m_CommandPool.GetFirstObjectPos();

	if(Pos)
		return m_CommandPool.GetNext(Pos);
	else
		return NULL;
	FUNCTION_END;
	return NULL;
}

BOOL CControlPanel::ReleaseCommand(UINT ID)
{
	FUNCTION_BEGIN;

	return m_CommandPool.DeleteObject(ID);

	FUNCTION_END;
	return FALSE;
}

void CControlPanel::SetServerStatus(LPCVOID pData,UINT DataSize)
{
	CAutoLock AutoLock(m_CriticalSection);
	m_ServerStatus.SetUsedSize(0);
	m_ServerStatus.PushBack(pData,DataSize);
}
UINT CControlPanel::GetServerStatus(LPVOID pBuffer,UINT BufferSize)
{
	CAutoLock AutoLock(m_CriticalSection);
	if(BufferSize>=m_ServerStatus.GetUsedSize())
	{
		UINT Pos=0;
		m_ServerStatus.Peek(Pos,pBuffer,m_ServerStatus.GetUsedSize());
	}
	return m_ServerStatus.GetUsedSize();
}
void CControlPanel::SetServerStatusName(WORD StatusID,LPCTSTR szStatusName)
{
	CAutoLock AutoLock(m_CriticalSection);

	SERVER_STATUS_NAME Name;
	strncpy_0(Name.szName,MAX_SERVER_STATUS_NAME_LEN,szStatusName,MAX_SERVER_STATUS_NAME_LEN);
	m_ServerStatusName.Insert(StatusID,Name);
}
LPCTSTR CControlPanel::GetServerStatusName(WORD StatusID)
{
	SERVER_STATUS_NAME *pName=m_ServerStatusName.Find(StatusID);
	if(pName)
	{
		return pName->szName;
	}
	return NULL;
}