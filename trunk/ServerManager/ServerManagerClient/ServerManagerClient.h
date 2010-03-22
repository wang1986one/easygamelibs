// ServerManagerClient.h : ServerManagerClient 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif




// CServerManagerClientApp:
// 有关此类的实现，请参阅 ServerManagerClient.cpp
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
// 重写
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