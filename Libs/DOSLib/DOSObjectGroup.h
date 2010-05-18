/****************************************************************************/
/*                                                                          */
/*      文件名:    DOSObjectGroup.h                                         */
/*      创建日期:  2009年10月23日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once



class CDOSObjectGroup :
	public CEasyThread
{
protected:
	CDOSObjectManager *						m_pManager;
	UINT									m_Index;
	volatile int							m_Weight;
	CThreadSafeIDStorage<DOS_OBJECT_INFO>	m_ObjectRegisterQueue;
	CThreadSafeIDStorage<OBJECT_ID>			m_ObjectUnregisterQueue;

	CIDStorage<DOS_OBJECT_INFO>			m_ObjectPool;

	CEasyCriticalSection				m_EasyCriticalSection;
	DECLARE_CLASS_INFO(CDOSObjectGroup);
public:
	CDOSObjectGroup(void);
	virtual ~CDOSObjectGroup(void);

	virtual bool Initialize(CDOSObjectManager * pManager,UINT Index);
	virtual void Destory();

	
	CDOSObjectManager * GetManager();

	BOOL RegisterObject(DOS_OBJECT_INFO& ObjectInfo);
	BOOL UnregisterObject(OBJECT_ID ObjectID);

	int GetWeight();

	virtual BOOL OnStart();
	virtual BOOL OnRun();

	BOOL PushMessage(OBJECT_ID ObjectID,CDOSMessagePacket * pPacket);
protected:
	int ProcessObjectRegister(int ProcessLimit=DEFAULT_SERVER_PROCESS_PACKET_LIMIT);
	int ProcessObjectUnregister(int ProcessLimit=DEFAULT_SERVER_PROCESS_PACKET_LIMIT);
};

inline CDOSObjectManager * CDOSObjectGroup::GetManager()
{
	return m_pManager;
}

inline int CDOSObjectGroup::GetWeight()
{
	return m_Weight;
}