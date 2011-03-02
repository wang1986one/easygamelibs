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
	struct DOS_OBJECT_INFO
	{
		OBJECT_ID			ObjectID;
		CDOSBaseObject *	pObject;
		int					Weight;
		UINT				Param;
	};
	CDOSObjectManager *								m_pManager;
	UINT											m_Index;
	volatile int									m_Weight;

	CThreadPerformanceCounter						m_ThreadPerformanceCounter;


	CThreadSafeIDStorage<DOS_OBJECT_REGISTER_INFO>	m_ObjectRegisterQueue;
	CThreadSafeIDStorage<OBJECT_ID>					m_ObjectUnregisterQueue;

	CIDStorage<DOS_OBJECT_INFO>						m_ObjectPool;

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