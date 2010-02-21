#pragma once



class CDOSObjectGroup :
	public CEasyThread
{
protected:
	CDOSObjectManager *					m_pManager;
	UINT								m_Index;
	volatile int						m_Weight;
	CThreadSafeList<DOS_OBJECT_INFO>	m_ObjectRegisterQueue;
	CThreadSafeList<OBJECT_ID>			m_ObjectUnregisterQueue;

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