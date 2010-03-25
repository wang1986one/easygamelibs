/****************************************************************************/
/*                                                                          */
/*      �ļ���:    SystemControlPort.h                                      */
/*      ��������:  2009��07��06��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once

class CSystemControlPort :
	public CNetService
{
protected:
	IBaseServer * m_pServer;
public:
	CSystemControlPort(void);
	~CSystemControlPort(void);

	bool Init(IBaseServer * pServer);

	virtual void OnStartUp();
	virtual void OnClose();

	virtual void OnRecvData(const CIPAddress& IPAddress,const CEasyBuffer& DataBuffer);

	void SendMsg(const CIPAddress& Address,WORD Msg,LPCVOID pData=NULL,int DataLen=0);
};
