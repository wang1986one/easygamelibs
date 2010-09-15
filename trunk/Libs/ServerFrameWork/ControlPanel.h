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

enum SERVER_STATUS_FORMAT_TYPE
{
	SSFT_DEFAULT,
	SSFT_FLOW,
};
struct SERVER_STATUS_FORMAT_INFO
{
	char	szName[MAX_SERVER_STATUS_NAME_LEN];
	int		FormatType;
};

class CControlPanel :
	public CStaticObject<CControlPanel>
{
public:
	
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
	

	CThreadSafeIDStorage<PANEL_MSG>				m_MsgPool;
	CThreadSafeIDStorage<PANEL_MSG>				m_CommandPool;
	CEasyBuffer									m_ServerStatus;
	CEasyCriticalSection						m_CriticalSection;
	CEasyMap<WORD,SERVER_STATUS_FORMAT_INFO>	m_ServerStatusFormats;
	
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
	void SetServerStatusFormat(WORD StatusID,LPCTSTR szStatusName,int FormatType=SSFT_DEFAULT);
	SERVER_STATUS_FORMAT_INFO * GetServerStatusFormat(WORD StatusID);
	CEasyMap<WORD,SERVER_STATUS_FORMAT_INFO>& GetAllServerStatusFormat();
};

inline CEasyMap<WORD,SERVER_STATUS_FORMAT_INFO>& CControlPanel::GetAllServerStatusFormat()
{
	return m_ServerStatusFormats;
}