#pragma once

class COverLappedObject;
class CIOCPEventRouter:
	public CNameObject
{
protected:
	IIOCPEventHandler *			m_pIOCPEventHandler;
	UINT						m_SessionID;
	CEasyReadWriteLock			m_EasyReadWriteLock;

	DECLARE_CLASS_INFO_STATIC(CIOCPEventRouter);
public:
	CIOCPEventRouter()
	{
		m_pIOCPEventHandler=NULL;
		m_SessionID=0;
	}
	~CIOCPEventRouter()
	{
		Destory();
	}
	virtual void Destory()
	{
		CAutoWriteLock Lock(m_EasyReadWriteLock);
		m_pIOCPEventHandler=NULL;
	}
	void Init(IIOCPEventHandler * pIOCPEventHander)
	{
		CAutoWriteLock Lock(m_EasyReadWriteLock);
		m_pIOCPEventHandler=pIOCPEventHander;
		m_SessionID++;
	}
	void SetEventHander(IIOCPEventHandler * pIOCPEventHander)
	{
		CAutoWriteLock Lock(m_EasyReadWriteLock);
		m_pIOCPEventHandler=pIOCPEventHander;		
	}
	UINT GetSessionID()
	{
		return m_SessionID;
	}
	BOOL OnIOCPEvent(int EventID,COverLappedObject * pOverLappedObject);
};