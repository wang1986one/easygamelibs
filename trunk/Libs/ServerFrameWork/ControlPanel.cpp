/****************************************************************************/
/*                                                                          */
/*      文件名:    ControlPanel.cpp                                         */
/*      创建日期:  2009年07月06日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#include "StdAfx.h"

CControlPanel::CControlPanel(void)
{
	m_MsgPool.Create(1024);
	m_CommandPool.Create(1024);
	ZeroMemory(&m_ServerInfo,sizeof(m_ServerInfo));
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

void CControlPanel::SetServerInfo(SERVER_INFO& ServerInfo)
{
	CAutoLock AutoLock(m_CriticalSection);
	m_ServerInfo=ServerInfo;
}
void CControlPanel::GetServerInfo(SERVER_INFO& ServerInfo)
{
	CAutoLock AutoLock(m_CriticalSection);
	ServerInfo=m_ServerInfo;
}
