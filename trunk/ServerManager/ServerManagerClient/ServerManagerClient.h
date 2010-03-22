// ServerManagerClient.h : ServerManagerClient Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif




// CServerManagerClientApp:
// �йش����ʵ�֣������ ServerManagerClient.cpp
//


class CServerManagerClientApp : public CWinApp
{
protected:
	CNetServer						m_Server;
	CEasyArray<CServerConnection *>	m_ConnectionList;
	CEasyArray<SERVER_INFO>			m_ServerList;
public:
	CServerManagerClientApp();

	static CServerManagerClientApp * GetInstance();
// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual BOOL OnIdle(LONG lCount);

	CServerConnection * GetServerConnection(LPCTSTR ServerAddress);

	void StartupService(LPCTSTR ServerAddress,UINT ServiceIndex);
	void ShutdownService(LPCTSTR ServerAddress,UINT ServiceIndex);
	void BrowseWorkDir(LPCTSTR ServerAddress,UINT ServiceIndex,LPCTSTR Dir);

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnAppAbout();
	afx_msg void OnServerManage();

	void CheckServerConnections();

	void LoadConfig();
	void SaveConfig();
	
};

extern CServerManagerClientApp theApp;