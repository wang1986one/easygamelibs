/****************************************************************************/
/*                                                                          */
/*      文件名:    DOSClient.h                                              */
/*      创建日期:  2009年10月15日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
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
