/****************************************************************************/
/*                                                                          */
/*      文件名:    DOSObjectManager.h                                       */
/*      创建日期:  2009年10月23日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
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

