/****************************************************************************/
/*                                                                          */
/*      �ļ���:    ControlPanel.h                                           */
/*      ��������:  2009��07��06��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once

#ifdef WIN32

#define WM_USER_LOG_MSG				WM_USER+1001
#define WM_USER_CLIENT_NUM			WM_USER+1002

#endif

#define MAX_CONSOLE_MSG_LEN			1024

enum CONSOLE_MSG_TYPE
{
	CON_MSG_LOG,
};

struct PANEL_MSG
{
	UINT	ID;
	UINT	MsgType;
	char	Msg[MAX_CONSOLE_MSG_LEN];
};

struct SERVER_INFO
{
	UINT	ClientCount;
	double	CycleTime;
	double	TCPRecvFlow;
	double	TCPSendFlow;
	double	UDPRecvFlow;
	double	UDPSendFlow;
	double	TCPRecvCount;
	double	TCPSendCount;
	double	UDPRecvCount;
	double	UDPSendCount;
};

class CControlPanel :
	public CStaticObject<CControlPanel>
{
#ifdef WIN32
protected:
	HWND								m_hWnd;
public:
	void SetHwnd(HWND hWnd)
	{
		m_hWnd=hWnd;
	}
#endif

protected:
	CThreadSafeIDStorage<PANEL_MSG>		m_MsgPool;
	CThreadSafeIDStorage<PANEL_MSG>		m_CommandPool;
	SERVER_INFO							m_ServerInfo;
	CEasyCriticalSection				m_CriticalSection;
	
public:
	CControlPanel(void);
	~CControlPanel(void);

	

	BOOL PushMsg(LPCTSTR szMsg);
	BOOL PushMsg(UINT Type,LPCVOID pData,int Len);
	PANEL_MSG * GetMsg();
	BOOL ReleaseMsg(UINT ID);	
	

	BOOL PushCommand(LPCTSTR szCommand);
	PANEL_MSG * GetCommand();
	BOOL ReleaseCommand(UINT ID);

	void SetServerInfo(SERVER_INFO& ServerInfo);
	void GetServerInfo(SERVER_INFO& ServerInfo);
};
