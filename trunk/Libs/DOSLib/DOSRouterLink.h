/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DOSRouterLink.h                                          */
/*      ��������:  2009��07��06��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once


class CDOSServer;
class CDOSRouter;

class CDOSRouterLink :
	public CEasyNetLinkConnection
{
protected:
	

	DECLARE_CLASS_INFO(CDOSRouterLink);

	
public:
	CDOSRouterLink();
	virtual ~CDOSRouterLink(void);	
	
	CDOSRouter * GetRouter();

	virtual void OnLinkStart();
	virtual void OnLinkEnd();
	virtual void OnData(const CEasyBuffer& DataBuffer);

	
	void OnMsgPacket(CDOSMessagePacket * pPacket);

};



inline CDOSRouter * CDOSRouterLink::GetRouter()
{
	return (CDOSRouter *)GetManager();
}


