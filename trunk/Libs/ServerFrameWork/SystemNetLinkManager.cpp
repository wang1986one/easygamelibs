/****************************************************************************/
/*                                                                          */
/*      �ļ���:    SystemNetLinkManager.cpp                                 */
/*      ��������:  2009��07��06��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
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

	if(m_ConnectionMap.GetObjectCount())
	{
		CSmartStruct LogStr(5000);

		LogStr.AddMember(SC_SST_SL_LOG_STR,LogMsg);


		CEasyBuffer	SendBuffer(s_SendBuffer,65536);
		SMSG_HEADER MsgHeader;

		MsgHeader.MsgID=SC_MSG_SERVER_LOG;
		MsgHeader.Size=(WORD)(sizeof(SMSG_HEADER)+LogStr.GetDataLen());
		SendBuffer.PushBack(&MsgHeader,sizeof(SMSG_HEADER));

		SendBuffer.PushBack(LogStr.GetData(),LogStr.GetDataLen());

		void * Pos=m_ConnectionMap.GetFirstObjectPos();
		while(Pos)
		{
			UINT Key;
			CEasyNetLinkConnection ** ppValue=m_ConnectionMap.GetNextObject(Pos,Key);
			if(ppValue)
			{
				CSystemNetLink * pLink=(CSystemNetLink *)(*ppValue);
				if(pLink->IsLinkLog())
				{
					pLink->SendData(SendBuffer.GetBuffer(),SendBuffer.GetUsedSize());
				}
			}
		}
	}
	FUNCTION_END;
}
