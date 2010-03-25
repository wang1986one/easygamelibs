/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DOSClient.h                                              */
/*      ��������:  2009��10��15��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once


class CDOSClient :	
	public CNetConnection
{
protected:
	CEasyBuffer								m_AssembleBuffer;	

	DECLARE_CLASS_INFO(CDOSClient);
public:
	CDOSClient(void);
	virtual ~CDOSClient(void);	
	virtual void Destory();
	

	BOOL Start(UINT MaxMsgSize,const CIPAddress& Address,DWORD TimeOut=NO_CONNECTION_TIME_OUT);
	void Close();

	BOOL SendMessage(WORD CmdID,LPVOID pData=NULL,UINT DataSize=0);

protected:
	virtual void OnRecvData(const CEasyBuffer& DataBuffer);
	virtual void OnMsg(CDOSSimpleMessage * pMessage);

};
