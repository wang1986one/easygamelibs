/****************************************************************************/
/*                                                                          */
/*      �ļ���:    NetSocket.cpp                                            */
/*      ��������:  2009��09��11��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#include "StdAfx.h"

IMPLEMENT_CLASS_INFO_STATIC(CNetSocket,CNameObject);

CNetSocket::CNetSocket(void):CNameObject()
{
	m_State=SS_UNINITED;
	m_IsBlocking=TRUE;
	m_Socket=INVALID_SOCKET;
}

CNetSocket::CNetSocket(SOCKET Socket):CNameObject()
{
	m_State=SS_UNINITED;
	m_IsBlocking=TRUE;
	m_Socket=INVALID_SOCKET;
	SetSocket(Socket);
}

CNetSocket::~CNetSocket(void)
{
	Close();
}

BOOL CNetSocket::MakeSocket( int af, int type, int protocol )
{
	if( m_Socket != INVALID_SOCKET )
	{
		Close();
	}	
	m_Socket = socket( af, type, protocol );
	if( m_Socket == INVALID_SOCKET )
	{
		int ErrorCode=GetLastError();
		PrintNetLog(0xffffffff,"(%d)Socket( %d, %d, %d )����ʧ�ܣ�",ErrorCode,af, type, protocol);
		SetState( SS_UNINITED );
		return FALSE;
	}	
	SetState( SS_UNUSED );
	return TRUE;
}

#ifdef WIN32
int	CNetSocket::GetProtocol()
{
	WSAPROTOCOL_INFO ProtocolInfo;
	int Len=sizeof(ProtocolInfo);


	if(getsockopt(m_Socket,SOL_SOCKET,SO_PROTOCOL_INFO,(char *)&ProtocolInfo,&Len)==SOCKET_ERROR)
	{
		return -1;
	}
	return ProtocolInfo.iProtocol;
}
#endif

BOOL CNetSocket::SetSocket( SOCKET Socket )
{
	if( m_Socket != INVALID_SOCKET )
	{
		Close();
	}
	m_Socket = Socket;
	if( Socket != INVALID_SOCKET )
		SetState( SS_UNUSED );
	else
	{
		m_IsBlocking=TRUE;
		SetState( SS_UNINITED );
	}
	return TRUE;
}

void CNetSocket::Close()
{
	closesocket(m_Socket);
	m_Socket = INVALID_SOCKET;
	SetState(SS_UNINITED);
}

int	CNetSocket::SetOption(int Level,int Option,char * pValue,socklen_t ValueLen)
{
	return setsockopt(m_Socket,Level,Option,pValue,ValueLen);
}

int	CNetSocket::GetOption(int Level,int Option,char * pValue,socklen_t& ValueLen)
{
	return getsockopt(m_Socket,Level,Option,pValue,&ValueLen);
}

bool CNetSocket::SetRecvBufferSize(int Size)
{
	int RetCode=SetOption(SOL_SOCKET,SO_RCVBUF,(char *)&Size,sizeof(int));
	if(RetCode==SOCKET_ERROR)
	{
		int ErrorCode=GetLastError();
		PrintNetLog(0xffffffff,"CNetSocket::SetRecvBufferSizeʧ��ErrorCode=%d��",ErrorCode);
		return false;
	}
	return true;
}

bool CNetSocket::SetSendBufferSize(int Size)
{
	int RetCode=SetOption(SOL_SOCKET,SO_SNDBUF,(char *)&Size,sizeof(int));
	if(RetCode==SOCKET_ERROR)
	{
		int ErrorCode=GetLastError();
		PrintNetLog(0xffffffff,"CNetSocket::SetSendBufferSizeʧ��ErrorCode=%d��",ErrorCode);
		return false;
	}
	return true;
}

int	CNetSocket::GetRecvBufferSize()
{
	int Size=0;
	socklen_t Len=sizeof(int);
	int RetCode=GetOption(SOL_SOCKET,SO_RCVBUF,(char *)&Size,Len);
	if(RetCode==SOCKET_ERROR)
	{
		int ErrorCode=GetLastError();
		PrintNetLog(0xffffffff,"CNetSocket::GetRecvBufferSizeʧ��ErrorCode=%d��",ErrorCode);
		return -1;
	}
	return Size;
}

int	CNetSocket::GetSendBufferSize()
{
	int Size=0;
	socklen_t Len=sizeof(int);
	int RetCode=GetOption(SOL_SOCKET,SO_SNDBUF,(char *)&Size,Len);
	if(RetCode==SOCKET_ERROR)
	{
		int ErrorCode=GetLastError();
		PrintNetLog(0xffffffff,"CNetSocket::GetSendBufferSizeʧ��ErrorCode=%d��",ErrorCode);
		return -1;
	}
	return Size;
}

BOOL CNetSocket::Connect(const CIPAddress& Address)
{
	if( GetState() != SS_UNUSED )
	{
		Close();
		MakeSocket();
	}

	if( !EnableBlocking(TRUE) )
	{
		return FALSE;
	}


	int erri = ::connect(m_Socket, (struct sockaddr *)&(Address.GetSockAddr()), sizeof(sockaddr_in));
	if( erri == SOCKET_ERROR )
	{
		erri = GetLastError();
		if( erri != EWOULDBLOCK )
		{
			PrintNetLog(0xffffffff,"(%d)Connect() �� connect() ʧ�ܣ�",erri);
			return FALSE;
		}
		PrintNetLog(0xffffffff,"(%d)Connect() ����������ɣ�",erri);

		SetState(SS_CONNECTING);
	}
	else
	{
		SetState(SS_CONNECTED);
	}
	SetRemoteAddress(Address);
	return	TRUE;
}

BOOL CNetSocket::ConnectNoBlocking(const CIPAddress& Address ,DWORD dwTimeOut)
{
	if( GetState() != SS_UNUSED )
	{
		Close();
		MakeSocket();
	}

	if( !EnableBlocking(FALSE) )
	{
		return FALSE;
	}

	int erri = connect(m_Socket, (struct sockaddr *)&(Address.GetSockAddr()), sizeof(sockaddr_in));
	if( erri == SOCKET_ERROR )
	{
		if( GetLastError() != EWOULDBLOCK )
		{
			PrintNetLog(0xffffffff,"(%d)ConnectNoBlocking() �� connect() ʧ�ܣ�",GetLastError());

			return FALSE;
		}
	}
	SetState(SS_CONNECTING);
	SetRemoteAddress(Address);
	m_ConnectTimer.SetTimeOut(dwTimeOut);
	return TRUE;
}

BOOL CNetSocket::Connected()
{
	BOOL bSuccess = FALSE;
	BOOL bFail = FALSE;
	if( GetState() != SS_CONNECTING )
		return FALSE;
	if( Select( NULL, &bSuccess, &bFail, 1 ) )
	{
		if( bSuccess )
		{
			SetState(SS_CONNECTED);
			CIPAddress Address;
			socklen_t AddressLen=sizeof(sockaddr);
			getsockname(m_Socket,(sockaddr*)&(Address.GetSockAddr()),&AddressLen);
			SetLocalAddress(Address);
			getpeername(m_Socket,(sockaddr*)&(Address.GetSockAddr()),&AddressLen);
			SetRemoteAddress(Address);
			return TRUE;
		}
		if( bFail )
		{
			SetState(SS_UNUSED);
			return FALSE;
		}
	}

	if( m_ConnectTimer.IsTimeOut() )
	{
		SetState(SS_UNUSED);
		return FALSE;
	}
	return FALSE;
}

BOOL CNetSocket::Bind(const CIPAddress& Address)
{
	if( ::bind(m_Socket, (struct sockaddr * )&(Address.GetSockAddr()), sizeof( struct sockaddr_in )) == SOCKET_ERROR )
	{
		int ErrorCode=GetLastError();
		PrintNetLog(0xffffffff,"(%d)Bind() �� bind() ʧ�ܣ�",ErrorCode);
		return FALSE;
	}
	CIPAddress LocalAddress;
	socklen_t AddressLen=sizeof(sockaddr);
	getsockname(m_Socket,(sockaddr*)&(LocalAddress.GetSockAddr()),&AddressLen);
	SetLocalAddress(LocalAddress);
	return TRUE;
}

BOOL CNetSocket::Listen(const CIPAddress& Address)
{
	if( GetState() != SS_UNUSED )
	{
		Close();
		MakeSocket();
	}

	if(!Bind(Address))
	{
		PrintNetLog(0xffffffff,"Listen() �� Bind() ʧ�ܣ�");
		return FALSE;
	}


	if( ::listen( m_Socket, MAX_LISTEN_BACKLOG ) == SOCKET_ERROR )
	{
		int ErrorCode=GetLastError();
		PrintNetLog(0xffffffff,"(%d)Listen() �� listen() ʧ�ܣ�",ErrorCode);
		return FALSE;
	}
	SetState(SS_LISTENING);


	return TRUE;
}

#ifdef WIN32
BOOL CNetSocket::EnableBlocking(BOOL Enable)
{
	u_long mode;
	if(Enable)
		mode=0;
	else
		mode=1;
	if( ioctlsocket( m_Socket, FIONBIO, &mode ) == SOCKET_ERROR )
	{
		int ErrorCode=GetLastError();
		PrintNetLog(0xffffffff,"(%d)EnableBlocking() �� ioctlsocket() ʧ�ܣ�",ErrorCode);
		return FALSE;
	}
	m_IsBlocking=Enable;
	return TRUE;
}
#else
BOOL CNetSocket::EnableBlocking(BOOL Enable)
{
	int Options;

	Options=fcntl(m_Socket,F_GETFL);

	if(Options<0)
	{
		return FALSE;
	}

	if(Enable)
	{
		Options = Options&(~O_NONBLOCK);
	}
	else
	{
		Options = Options|O_NONBLOCK;
	}


	if(fcntl(m_Socket,F_SETFL,Options)<0)
	{
		return FALSE;
	}
	return TRUE;
}
#endif


BOOL CNetSocket::Select( BOOL * pbRead, BOOL * pbWrite, BOOL * pbExcept, DWORD dwTimeOut )
{
	fd_set fdRead;
	fd_set fdWrite;
	fd_set fdExcept;

	FD_ZERO( &fdRead );
	FD_ZERO( &fdWrite );
	FD_ZERO( &fdExcept );

	fd_set * pfdRead = &fdRead, * pfdWrite = &fdWrite, * pfdExcept = &fdExcept;


	timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = dwTimeOut;


	if( pbRead == NULL )
		pfdRead = NULL;
	else
	{
		FD_SET( m_Socket, pfdRead );
		//*pbRead = TRUE;
	}

	if( pbWrite == NULL )
		pfdWrite = NULL;
	else
	{
		FD_SET( m_Socket, pfdWrite );
		//*pbWrite = TRUE;
	}

	if( pbExcept == NULL )
		pfdExcept = NULL;
	else
	{
		FD_SET( m_Socket, pfdExcept );
		//*pbExcept = TRUE;
	}

	int ierr = select(0,pfdRead, pfdWrite, pfdExcept, &tv );
	if( ierr == SOCKET_ERROR )return FALSE;

	if( pfdRead && FD_ISSET( m_Socket, pfdRead ) )
		*pbRead = TRUE;
	if( pfdWrite && FD_ISSET( m_Socket, pfdWrite ) )
		*pbWrite = TRUE;
	if( pfdExcept && FD_ISSET( m_Socket, pfdExcept ) )
		*pbExcept = TRUE;
	return TRUE;
}

#ifdef WIN32
BOOL CNetSocket::WMsgSelect(HWND hWnd,unsigned int wMsg,long lEvent)
{
	if(WSAAsyncSelect(m_Socket,hWnd,wMsg,lEvent)==SOCKET_ERROR)
	{
		int code = GetLastError();
		return FALSE;
	}
	return TRUE;
}

BOOL CNetSocket::EventSelect(WSAEVENT hEventObject,long lEvent)
{
	if(WSAEventSelect(m_Socket,hEventObject,lEvent)==SOCKET_ERROR)
	{
		int code = GetLastError();
		return FALSE;
	}
	return TRUE;
}
#endif

int	CNetSocket::Send( LPVOID lpBuffer, int iSize )
{
	return send( m_Socket, (char*)lpBuffer, iSize, 0 );
}

int	CNetSocket::Recv( LPVOID lpBuffer, int iSize )
{
	return recv( m_Socket, (char*)lpBuffer, iSize, 0 );
}

BOOL CNetSocket::Accept( CNetSocket & Socket )
{
	sockaddr_in	addr;
	socklen_t addrlen;
	addrlen = sizeof( addr );
	SOCKET sAccept = accept( m_Socket, (sockaddr*)&addr, &addrlen );
	if( sAccept == INVALID_SOCKET )return FALSE;
	Socket.Close();
	Socket.m_Socket = sAccept;
	Socket.SetState(SS_ACCEPTED);
	return TRUE;
}

int CNetSocket::SendTo(const CIPAddress& Address,LPVOID lpBuffer, int iSize)
{
	return sendto(m_Socket, (char*)lpBuffer, iSize, 0,(sockaddr *)(&Address.GetSockAddr()),sizeof(sockaddr_in));
}

int CNetSocket::RecvFrom(const CIPAddress& Address,LPVOID lpBuffer, int iSize)
{
	socklen_t Len=sizeof(sockaddr_in);

	return recvfrom(m_Socket, (char*)lpBuffer, iSize, 0,(sockaddr *)(&Address.GetSockAddr()),&Len);
}

#ifdef WIN32
BOOL CNetSocket::SendOverlapped( LPVOID lpData, int nDatasize, DWORD &dwBytesSent, DWORD dwFlag, LPOVERLAPPED lpOverlapped )
{
	FUNCTION_BEGIN;
	WSABUF	wsabuf;
	wsabuf.buf = (char*)lpData;
	wsabuf.len = nDatasize;

	if( WSASend( m_Socket, &wsabuf, 1, &dwBytesSent, dwFlag, lpOverlapped, NULL ) == SOCKET_ERROR )
	{
		int code = GetLastError();
		if( code != WSA_IO_PENDING )
		{
			PrintNetLog(0xffffffff,"(%d)WSASend() ʧ�ܣ�",code);
			return FALSE;
		}
	}
	return TRUE;
	FUNCTION_END;
	return FALSE;
}

BOOL CNetSocket::RecvOverlapped( LPVOID lpDataBuf, int nBufsize, DWORD &dwBytesReceived, DWORD &dwFlag, LPOVERLAPPED lpOverlapped )
{

	WSABUF	wsabuf;
	wsabuf.buf = (char*)lpDataBuf;
	wsabuf.len = nBufsize;

	if( WSARecv( m_Socket, &wsabuf, 1, &dwBytesReceived, &dwFlag, lpOverlapped, NULL ) == SOCKET_ERROR )
	{
		int code = GetLastError();
		if( code != WSA_IO_PENDING )
		{
			PrintNetLog(0xffffffff,"(%d)WSARecv() ʧ�ܣ�",code);
			return FALSE;
		}
	}
	return TRUE;
}

BOOL CNetSocket::AcceptOverlapped( SOCKET AcceptSocket, LPVOID lpDataBuf, DWORD dwRecvBufferLength, DWORD &dwBytesReceived, LPOVERLAPPED lpOverlapped )
{
	if(::AcceptEx( m_Socket, AcceptSocket, lpDataBuf, dwRecvBufferLength, sizeof( SOCKADDR_IN ) +16, sizeof( SOCKADDR_IN ) +16, &dwBytesReceived, lpOverlapped ) )
	{
		return TRUE;
	}

	int Code=GetLastError();
	if( Code == ERROR_IO_PENDING )
		return TRUE;

	PrintNetLog(0xffffffff,"(%d)AcceptEx() ʧ�ܣ�",Code);

	return FALSE;
}

BOOL CNetSocket::SendToOverlapped(const CIPAddress& Address,LPVOID lpData, int nDatasize, DWORD &dwBytesSent, DWORD dwFlag, LPOVERLAPPED lpOverlapped)
{
	WSABUF	wsabuf;
	wsabuf.buf = (char*)lpData;
	wsabuf.len = nDatasize;

	if( WSASendTo( m_Socket, &wsabuf, 1, &dwBytesSent, dwFlag, (sockaddr*) (&Address.GetSockAddr()),sizeof(sockaddr_in),lpOverlapped, NULL ) == SOCKET_ERROR )
	{
		int code = GetLastError();
		if( code != WSA_IO_PENDING )
		{
			PrintNetLog(0xffffffff,"(%d)WSASendTo() ʧ�ܣ�",GetLastError());
			return FALSE;
		}
	}
	return TRUE;
}

BOOL CNetSocket::RecvFromOverlapped(const CIPAddress& Address,int& AddresLen,LPVOID lpDataBuf, int nBufsize, DWORD &dwBytesReceived, DWORD &dwFlag, LPOVERLAPPED lpOverlapped )
{
	WSABUF	wsabuf;
	wsabuf.buf = (char*)lpDataBuf;
	wsabuf.len = nBufsize;

	if( WSARecvFrom( m_Socket, &wsabuf, 1, &dwBytesReceived, &dwFlag,(sockaddr*) (&Address.GetSockAddr()),&AddresLen, lpOverlapped, NULL ) == SOCKET_ERROR )
	{
		int code = GetLastError();
		if( code != WSA_IO_PENDING )
		{
			PrintNetLog(0xffffffff,"(%d)WSARecvFrom() ʧ�ܣ�",code);
			return FALSE;
		}
	}
	return TRUE;
}
#endif


#ifdef WIN32
void CNetSocket::NetStartup()
{
	WSADATA	wsa;
	WSAStartup( 0x202, &wsa );
}

void CNetSocket::NetCleanup()
{
	WSACleanup();
}
#endif

bool CNetSocket::StealFrom(CNameObject * pObject,UINT Param)
{
	if(pObject->IsKindOf(GET_CLASS_INFO(CNetSocket)))
	{
		if(!CNameObject::StealFrom(pObject,Param))
			return false;
		return StealSocket(*((CNetSocket *)pObject));
	}
	return false;
}
