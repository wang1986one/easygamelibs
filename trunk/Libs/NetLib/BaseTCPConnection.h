/****************************************************************************/
/*                                                                          */
/*      �ļ���:    BaseTCPConnection.h                                      */
/*      ��������:  2010��02��09��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once

class CBaseTCPConnection :
	public CNameObject
{
protected:
	CNetSocket								m_Socket;

	DECLARE_CLASS_INFO_STATIC(CBaseTCPConnection);
public:
	CBaseTCPConnection(void);
	~CBaseTCPConnection(void);

	CNetSocket * GetSocket();

	void SetRemoteAddress(const CIPAddress& IPAddress);
	void SetLocalAddress(const CIPAddress& IPAddress);
	CIPAddress& GetRemoteAddress();
	CIPAddress& GetLocalAddress();
	void SetAddressPair(const CIPAddressPair& AddressPair);
	CIPAddressPair& GetAddressPair();

	BOOL IsConnected();
	BOOL IsDisconnected();

	virtual BOOL Create(UINT RecvQueueSize,UINT SendQueueSize)=0;
	virtual BOOL Create(SOCKET Socket,UINT RecvQueueSize,UINT SendQueueSize)=0;

	virtual BOOL StartWork()=0;

	virtual void OnConnection(BOOL IsSucceed)=0;
	virtual void OnDisconnection()=0;

	virtual int Update(int ProcessPacketLimit=DEFAULT_SERVER_PROCESS_PACKET_LIMIT)=0;

	virtual UINT GetCurSendQueryCount();
};

inline CNetSocket * CBaseTCPConnection::GetSocket()
{
	return &m_Socket;
}

inline void CBaseTCPConnection::SetRemoteAddress(const CIPAddress& IPAddress)
{
	m_Socket.SetRemoteAddress(IPAddress);
}
inline void CBaseTCPConnection::SetLocalAddress(const CIPAddress& IPAddress)
{
	m_Socket.SetLocalAddress(IPAddress);
}
inline CIPAddress& CBaseTCPConnection::GetRemoteAddress()
{
	return m_Socket.GetRemoteAddress();
}
inline CIPAddress& CBaseTCPConnection::GetLocalAddress()
{
	return m_Socket.GetLocalAddress();
}
inline void CBaseTCPConnection::SetAddressPair(const CIPAddressPair& AddressPair)
{
	m_Socket.SetAddressPair(AddressPair);
}
inline CIPAddressPair& CBaseTCPConnection::GetAddressPair()
{
	return m_Socket.GetAddressPair();
}

inline BOOL CBaseTCPConnection::IsConnected()
{
	return m_Socket.IsConnected();
}

inline BOOL CBaseTCPConnection::IsDisconnected()
{
	return m_Socket.IsDisconnected();
}