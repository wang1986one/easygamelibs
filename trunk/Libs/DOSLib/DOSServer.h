/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DOSServer.h                                              */
/*      ��������:  2009��10��23��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once



class CDOSServer :
	public CNetServer
{
protected:
	DOS_CONFIG					m_ServerConfig;
	CDOSObjectProxyService *	m_pDOSObjectProxyService;
	CDOSRouter*					m_pDOSRouterService;		
	CDOSObjectManager *			m_pObjectManager;

	CFastMemoryPool				m_MemoryPool;

	DECLARE_CLASS_INFO_STATIC(CDOSServer);
public:
	CDOSServer(void);
	virtual ~CDOSServer(void);		

	void SetConfig(DOS_CONFIG& Config);
	DOS_CONFIG& GetConfig();

	CDOSObjectProxyService * GetObjectProxy();
	CDOSRouter * GetRouter();
	CDOSObjectManager * GetObjectManager();
	CFastMemoryPool * GetMemoryPool();

	CDOSMessagePacket * NewMessagePacket(UINT Size);
	BOOL ReleaseMessagePacket(CDOSMessagePacket * pPacket);
	void AddRefMessagePacket(CDOSMessagePacket * pPacket);
protected:
	virtual BOOL OnStartUp();
	virtual void OnShutDown();
	
};

inline void CDOSServer::SetConfig(DOS_CONFIG& Config)
{
	m_ServerConfig=Config;
}
inline DOS_CONFIG& CDOSServer::GetConfig()
{
	return m_ServerConfig;
}

inline CDOSObjectProxyService * CDOSServer::GetObjectProxy()
{
	return m_pDOSObjectProxyService;
}
inline CDOSRouter * CDOSServer::GetRouter()
{
	return m_pDOSRouterService;
}
inline CDOSObjectManager * CDOSServer::GetObjectManager()
{
	return m_pObjectManager;
}
inline CFastMemoryPool * CDOSServer::GetMemoryPool()
{
	return &m_MemoryPool;
}
inline CDOSMessagePacket * CDOSServer::NewMessagePacket(UINT Size)
{	
	MSG_LEN_TYPE PacketLen=CDOSMessagePacket::CaculateRealPacketLength(Size);
	CDOSMessagePacket * pPacket=(CDOSMessagePacket *)m_MemoryPool.Alloc(PacketLen);
	if(pPacket)
	{
		pPacket->Init();	
		pPacket->SetAllocSize(Size);
		pPacket->SetPacketLength(Size);
		pPacket->SetAllocTime(0);
		UINT RefCount=pPacket->IncRefCount();
#ifdef LOG_MEM_CALL_STACK
		m_MemoryPool.LogMemUse(pPacket,RefCount);
#endif
	}
	else
	{
		PrintDOSLog(0,_T("����%u��С����Ϣ��ʧ��"),Size);
	}
	return pPacket;
}
inline BOOL CDOSServer::ReleaseMessagePacket(CDOSMessagePacket * pPacket)
{
	UINT RefCount=pPacket->DecRefCount();
#ifdef LOG_MEM_CALL_STACK
	m_MemoryPool.LogMemUse(pPacket,RefCount);
#endif
	if(RefCount<=0)
		return m_MemoryPool.Free(pPacket);
	return TRUE;
}
inline void CDOSServer::AddRefMessagePacket(CDOSMessagePacket * pPacket)
{
	UINT RefCount=pPacket->IncRefCount();
#ifdef LOG_MEM_CALL_STACK
	m_MemoryPool.LogMemUse(pPacket,RefCount);
#endif
}
