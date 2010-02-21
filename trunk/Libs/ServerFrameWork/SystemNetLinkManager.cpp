#include "StdAfx.h"

CSystemNetLinkManager::CSystemNetLinkManager(void)
{
	FUNCTION_BEGIN;
	m_pServer=NULL;
	FUNCTION_END;
}

CSystemNetLinkManager::~CSystemNetLinkManager(void)
{
}


CEasyNetLinkConnection * CSystemNetLinkManager::OnCreateConnection(UINT ReportID)
{
	FUNCTION_BEGIN;
	return new CSystemNetLink(m_pServer);
	FUNCTION_END;
	return NULL;
}


void CSystemNetLinkManager::SendLogMsg(LPCTSTR LogMsg)
{
	FUNCTION_BEGIN;

	static char s_SendBuffer[65536];

	if(m_ConnectionMap.size())
	{
		CSmartStruct LogStr(5000);

		LogStr.AddMember(SC_SST_SL_LOG_STR,LogMsg);


		CEasyBuffer	SendBuffer(s_SendBuffer,65536);
		SMSG_HEADER MsgHeader;

		MsgHeader.MsgID=SC_MSG_SERVER_LOG;
		MsgHeader.Size=(WORD)(sizeof(SMSG_HEADER)+LogStr.GetDataLen());
		SendBuffer.PushBack(&MsgHeader,sizeof(SMSG_HEADER));

		SendBuffer.PushBack(LogStr.GetData(),LogStr.GetDataLen());


		std::map<UINT,CEasyNetLinkConnection*>::iterator itr;
		for(itr=m_ConnectionMap.begin();itr!=m_ConnectionMap.end();itr++)
		{
			CSystemNetLink * pLink=(CSystemNetLink *)(itr->second);
			if(pLink->IsLinkLog())
			{
				pLink->SendData(SendBuffer.GetBuffer(),SendBuffer.GetUsedSize());
			}
		}
	}
	FUNCTION_END;
}
