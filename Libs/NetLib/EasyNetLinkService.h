#pragma once

class CEasyNetLinkManager;

class CEasyNetLinkService :
	public CNetService
{
protected:
	CEasyNetLinkManager*		m_pManager;
	UINT						m_ReportID;
	BOOL						m_NeedReallocConnectionID;
	UINT						m_MaxPacketSize;	
	std::vector<CIPPattern>		m_IPList;

	DECLARE_CLASS_INFO_STATIC(CEasyNetLinkService);
public:
	CEasyNetLinkService(void);
	virtual ~CEasyNetLinkService(void);		

	void SetMaxPacketSize(UINT MaxPacketSize);

	virtual CBaseTCPConnection * CreateConnection(CIPAddress& RemoteAddress);
	virtual BOOL DeleteConnection(CBaseTCPConnection * pConnection);

	void SetManager(CEasyNetLinkManager* pManager);
	CEasyNetLinkManager* GetManager();

	void SetReportID(UINT ID);
	UINT GetReportID();

	void EnableReallocConnectionID(BOOL Enable);
	BOOL NeedReallocConnectionID();

	void SetIPList(std::vector<CIPPattern>& IPList);

	void PrintInfo(UINT LogChannel);

	
};

inline void CEasyNetLinkService::SetMaxPacketSize(UINT MaxPacketSize)
{
	m_MaxPacketSize=MaxPacketSize;
}

inline void CEasyNetLinkService::SetManager(CEasyNetLinkManager* pManager)
{
	m_pManager=pManager;
}

inline CEasyNetLinkManager* CEasyNetLinkService::GetManager()
{
	return m_pManager;
}

inline void CEasyNetLinkService::SetReportID(UINT ID)
{
	m_ReportID=ID;
}

inline UINT CEasyNetLinkService::GetReportID()
{
	return m_ReportID;
}

inline void CEasyNetLinkService::EnableReallocConnectionID(BOOL Enable)
{
	m_NeedReallocConnectionID=Enable;
}
inline BOOL CEasyNetLinkService::NeedReallocConnectionID()
{
	return m_NeedReallocConnectionID;
}

inline void CEasyNetLinkService::SetIPList(std::vector<CIPPattern>& IPList)
{
	m_IPList=IPList;
}