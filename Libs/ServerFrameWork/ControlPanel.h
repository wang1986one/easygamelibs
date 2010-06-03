/****************************************************************************/
/*                                                                          */
/*      文件名:    ControlPanel.h                                           */
/*      创建日期:  2009年07月06日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once

#ifdef WIN32

#define WM_USER_LOG_MSG				WM_USER+1001
#define WM_USER_CLIENT_NUM			WM_USER+1002

#endif

#define MAX_CONSOLE_MSG_LEN			1024
#define MAX_SERVER_STATUS_NAME_LEN	128


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
	struct SERVER_STATUS_NAME
	{
		char szName[MAX_SERVER_STATUS_NAME_LEN];
	};

	CThreadSafeIDStorage<PANEL_MSG>		m_MsgPool;
	CThreadSafeIDStorage<PANEL_MSG>		m_CommandPool;
	CEasyBuffer							m_ServerStatus;
	CEasyCriticalSection				m_CriticalSection;
	CEasyMap<WORD,SERVER_STATUS_NAME>	m_ServerStatusName;
	
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

	void SetServerStatus(LPCVOID pData,UINT DataSize);
	UINT GetServerStatus(LPVOID pBuffer,UINT BufferSize);
	void SetServerStatusName(WORD StatusID,LPCTSTR szStatusName);
	LPCTSTR GetServerStatusName(WORD StatusID);
};
