/****************************************************************************/
/*                                                                          */
/*      文件名:    NetLink.cpp                                              */
/*      创建日期:  2009年02月09日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#include "StdAfx.h"


CNetLink::CNetLink(CServerThread * pServerThread)
{
	FUNCTION_BEGIN;
	m_pServerThread=pServerThread;
	m_pView=NULL;
	m_IsLinkLog=FALSE;
	FUNCTION_END;
}

CNetLink::~CNetLink(void)
{
}


void CNetLink::SendMsg(WORD Msg,LPCVOID pData,int DataLen)
{
	FUNCTION_BEGIN;
	static char s_SendBuffer[65536];



	CEasyBuffer	SendBuffer(s_SendBuffer,65536);
	SMSG_HEADER MsgHeader;

	MsgHeader.MsgID=Msg;
	MsgHeader.Size=(WORD)(sizeof(SMSG_HEADER)+DataLen);	
	SendBuffer.PushBack(&MsgHeader,sizeof(SMSG_HEADER));

	if(DataLen)
		SendBuffer.PushBack(pData,DataLen);
	SendData(SendBuffer.GetBuffer(),SendBuffer.GetUsedSize());


	FUNCTION_END;
}


void CNetLink::OnLinkStart()
{
	FUNCTION_BEGIN;	

	if(m_pView==NULL)
	{
		
		POSITION Pos=CRemoteConsoleApp::GetInstance()->GetFirstDocTemplatePosition();
		CDocTemplate* pTemplate=CRemoteConsoleApp::GetInstance()->GetNextDocTemplate(Pos);
		if(pTemplate==NULL)
		{
			ForceDisconnect();
			return;
		}
		CFrameWnd * pFrame=pTemplate->CreateNewFrame(NULL,NULL);
		CEasyString Title;
		Title.Format("%s:%u",GetRemoteAddress().GetIPString(),GetRemoteAddress().GetPort());
		pFrame->SetTitle(Title);
		pTemplate->InitialUpdateFrame(pFrame,NULL);
		
		m_pView=(CRemoteConsoleView *)pFrame->GetActiveView();
		m_pView->SetNetLink(this);
		
		
	}

	CClassifiedID ID=GetID();
	Log("建立与[%s][%s:%u]的连接",
		ID.ToStr(),
		GetRemoteAddress().GetIPString(),
		GetRemoteAddress().GetPort());

	SendMsg(SC_MSG_LINK_LOG);
	FUNCTION_END;
}

void CNetLink::OnLinkEnd()
{
	FUNCTION_BEGIN;
	CClassifiedID ID=GetID();
	Log("断开与[%s]的连接",ID.ToStr());

	//if(m_pView)
	//{
	//	m_pView->PostMessage(WM_QUIT);
	//	m_pView->SetNetLink(NULL);
	//	m_pView=NULL;
	//}
	FUNCTION_END;
}

//接受数据
void CNetLink::OnData(const CEasyBuffer& DataBuffer)
{
	FUNCTION_BEGIN;


	SMSG * pMsg=(SMSG *)DataBuffer.GetBuffer();
	UINT DataSize=pMsg->Header.Size-sizeof(SMSG_HEADER);

	switch(pMsg->Header.MsgID)
	{
	case SC_MSG_SERVER_LOG:
		{
			CSmartStruct LogInfo(pMsg->Data,DataSize,false);

			LPCTSTR szLogMsg=LogInfo.GetMember(SC_SST_SL_LOG_STR);
			if(szLogMsg&&m_pView)
			{
				m_pView->PrintLogMsg(szLogMsg);
			}

		}
		break;
	case SC_MSG_QUERY_SERVER_STATUS_RESULT:
		if(m_pView)
		{
			CSmartStruct ServerStatus(pMsg->Data,DataSize,false);
			m_pView->SetServerStatus(ServerStatus);
		}
		break;
	}

	FUNCTION_END;
}