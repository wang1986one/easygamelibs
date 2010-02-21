#pragma once


class CDOSServer;



class CDOSObjectManager :
	public CNameObject
{
protected:
	CDOSServer *						m_pServer;	
	CEasyArray<CDOSObjectGroup *>		m_ObjectGroups;	

	DECLARE_CLASS_INFO(CDOSObjectManager);
public:
	CDOSObjectManager(void);
	virtual ~CDOSObjectManager(void);

	virtual bool Initialize();
	virtual void Destory();

	BOOL RegisterObject(CDOSBaseObject * pObject,OBJECT_ID ObjectID,int Weight=1);
	BOOL UnregisterObject(CDOSBaseObject * pObject);

	void SetServer(CDOSServer * pServer);
	CDOSServer * GetServer();
	
	BOOL PushMessage(OBJECT_ID ObjectID,CDOSMessagePacket * pPacket);

protected:

	CDOSObjectGroup * SelectGroup();

};

inline void CDOSObjectManager::SetServer(CDOSServer * pServer)
{
	m_pServer=pServer;
}
inline CDOSServer * CDOSObjectManager::GetServer()
{
	return m_pServer;
}

