/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DOSObjectManager.h                                       */
/*      ��������:  2009��10��23��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      ��������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ������������κ���ҵ�ͷ���ҵ������������                      */
/*      ���뱣���˰�Ȩ����                                                  */
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

	UINT GetObejctCount();
	
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

inline UINT CDOSObjectManager::GetObejctCount()
{
	UINT Count=0;
	for(UINT i=0;i<m_ObjectGroups.GetCount();i++)
	{
		Count+=m_ObjectGroups[i]->GetObjectCount();
	}
	return Count;
}