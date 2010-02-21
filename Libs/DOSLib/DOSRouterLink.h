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


