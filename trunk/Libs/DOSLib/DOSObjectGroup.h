/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DOSObjectGroup.h                                         */
/*      ��������:  2009��10��23��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once



class CDOSObjectGroup :
	public CEasyThread
{
protected:
	struct DOS_OBJECT_INFO
	{
		OBJECT_ID			ObjectID;
		CDOSBaseObject *	pObject;
		int					Weight;
		UINT				Param;
	};
	struct OBJECT_STAT_INFO
	{
		int					Count;
		volatile UINT64		CPUCost;
	};
	CDOSObjectManager *								m_pManager;
	UINT											m_Index;
	volatile int									m_Weight;
	bool											m_StatObjectCPUCost;

	CThreadPerformanceCounter						m_ThreadPerformanceCounter;


	CThreadSafeIDStorage<DOS_OBJECT_REGISTER_INFO>	m_ObjectRegisterQueue;
	CThreadSafeIDStorage<OBJECT_ID>					m_ObjectUnregisterQueue;

	CIDStorage<DOS_OBJECT_INFO>						m_ObjectPool;

	CStaticMap<OBJECT_ID,OBJECT_STAT_INFO>			m_ObjectCountStatMap;

	CEasyCriticalSection							m_EasyCriticalSection;
	DECLARE_CLASS_INFO(CDOSObjectGroup);
public:
	CDOSObjectGroup(void);
	virtual ~CDOSObjectGroup(void);

	virtual bool Initialize(CDOSObjectManager * pManager,UINT Index);
	virtual void Destory();

	
	CDOSObjectManager * GetManager();

	BOOL RegisterObject(DOS_OBJECT_REGISTER_INFO& ObjectRegisterInfo);
	BOOL UnregisterObject(OBJECT_ID ObjectID);
	UINT GetObjectCount();

	int GetWeight();
	float GetCPUUsedRate();
	float GetCycleTime();
	UINT GetMaxObjectMsgQueueLen();
	UINT GetIndex();

	virtual BOOL OnStart();
	virtual BOOL OnRun();
	virtual void OnTerminate();

	BOOL PushMessage(OBJECT_ID ObjectID,CDOSMessagePacket * pPacket);

	void PrintObjectStat(UINT LogChannel);
protected:
	int ProcessObjectRegister(int ProcessLimit=DEFAULT_SERVER_PROCESS_PACKET_LIMIT);
	int ProcessObjectUnregister(int ProcessLimit=DEFAULT_SERVER_PROCESS_PACKET_LIMIT);

	void OnObjectRegister(OBJECT_ID ObjectID);
	void OnObjectUnregister(OBJECT_ID ObjectID);
	void AddObjectCPUCost(OBJECT_ID ObjectID,UINT64 CPUCost);
};

inline CDOSObjectManager * CDOSObjectGroup::GetManager()
{
	return m_pManager;
}

inline UINT CDOSObjectGroup::GetObjectCount()
{
	return m_ObjectPool.GetObjectCount();
}

inline int CDOSObjectGroup::GetWeight()
{
	return m_Weight;
}
inline float CDOSObjectGroup::GetCPUUsedRate()
{
	return m_ThreadPerformanceCounter.GetCPUUsedRate();
}

inline float CDOSObjectGroup::GetCycleTime()
{
	return m_ThreadPerformanceCounter.GetCycleTime();
}

inline UINT CDOSObjectGroup::GetIndex()
{
	return m_Index;
}