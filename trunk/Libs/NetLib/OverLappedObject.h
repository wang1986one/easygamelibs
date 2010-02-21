#pragma once

class COverLappedObject;

struct OVERLAPPED_EX:public OVERLAPPED
{	
	COverLappedObject *	pOverLappedObject;
};

enum OVERLAPPEDOBJECT_TYPE
{
	IO_NOTSET,
	IO_RECV,
	IO_SEND,
	IO_ACCEPT,
	IO_ACCEPT2,
	IO_FILE_READ,
	IO_FILE_WRITE,
};

class CNetServer;

class COverLappedObject 
{
protected:
	OVERLAPPED_EX 				m_OverLapped;
	UINT						m_ID;	
	SOCKET						m_AcceptSocket;	
	CIPAddress					m_Address;
	socklen_t					m_AddressLen;
	CEasyBuffer					m_DataBuff;	
	int							m_TypeID;	
	UINT						m_RouterSession;
	UINT						m_ParentID;	
	UINT						m_ErrorCode;
	UINT						m_EventID;
	CNetServer *				m_pCreator;	
	CIOCPEventRouter *			m_pIOCPEventRouter;
	


	
public:
	COverLappedObject(void);
	~COverLappedObject(void);

	void SetID(UINT ID);
	UINT GetID();

	BOOL Create(CNetServer * pCreator);
	void Destory();

	OVERLAPPED_EX * GetOverlapped();	
	CEasyBuffer * GetDataBuff();	

	void SetIOCPEventRouter(CIOCPEventRouter * pIOCPEventRouter);
	void Process(int EventID);

	void SetType(int Type);
	int GetType();

	void SetAcceptSocket(SOCKET Socket);
	SOCKET GetAcceptSocket();

	void SetAddress(const CIPAddress& Address);
	CIPAddress& GetAddress();
	socklen_t& GetAddressLen();

	void SetRouterSession(UINT Session);
	UINT GetRouterSession();

	void SetParentID(UINT ID);
	UINT GetParentID();	

	void SetErrorCode(UINT ErrorCode);
	UINT GetErrorCode();

	void SetEventID(UINT EventID);
	UINT GetEventID();

	void Release();
};

inline void COverLappedObject::SetID(UINT ID)
{
	m_ID=ID;
}
inline UINT COverLappedObject::GetID()
{
	return m_ID;
}

inline OVERLAPPED_EX * COverLappedObject::GetOverlapped()
{
	return &m_OverLapped;
}

inline CEasyBuffer * COverLappedObject::GetDataBuff()
{
	return &m_DataBuff;
}

inline void COverLappedObject::SetIOCPEventRouter(CIOCPEventRouter * pIOCPEventRouter)
{
	m_pIOCPEventRouter=pIOCPEventRouter;
	if(m_pIOCPEventRouter)
		m_RouterSession=m_pIOCPEventRouter->GetSessionID();
	else
		m_RouterSession=0;
}

inline void COverLappedObject::Process(int EventID)
{
	if(m_pIOCPEventRouter)
	{			
		m_pIOCPEventRouter->OnIOCPEvent(EventID,this);			
		
	}
	else
	{
		PrintNetLog(0xffffffff,"IOCP��(%u)û��EventRouter��������",GetParentID());
		Release();
	}
}

inline void COverLappedObject::SetType(int Type)
{
	m_TypeID=Type;
}
inline int COverLappedObject::GetType()
{
	return m_TypeID;
}
inline void COverLappedObject::SetAcceptSocket(SOCKET Socket)
{
	m_AcceptSocket=Socket;
}
inline SOCKET COverLappedObject::GetAcceptSocket()
{
	return m_AcceptSocket;
}
inline void COverLappedObject::SetAddress(const CIPAddress& Address)
{
	m_Address=Address;
}
inline CIPAddress& COverLappedObject::GetAddress()
{
	return m_Address;
}

inline socklen_t& COverLappedObject::GetAddressLen()
{
	return m_AddressLen;
}



inline void COverLappedObject::SetRouterSession(UINT Session)
{
	m_RouterSession=Session;
}
inline UINT COverLappedObject::GetRouterSession()
{
	return m_RouterSession;
}

inline void COverLappedObject::SetParentID(UINT ID)
{
	m_ParentID=ID;
}
inline UINT COverLappedObject::GetParentID()
{
	return m_ParentID;
}

inline void COverLappedObject::SetErrorCode(UINT ErrorCode)
{
	m_ErrorCode=ErrorCode;
}

inline UINT COverLappedObject::GetErrorCode()
{
	return m_ErrorCode;
}

inline void COverLappedObject::SetEventID(UINT EventID)
{
	m_EventID=EventID;
}
inline UINT COverLappedObject::GetEventID()
{
	return m_EventID;
}