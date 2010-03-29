/****************************************************************************/
/*                                                                          */
/*      文件名:    RemoteConsole.h                                          */
/*      创建日期:  2009年02月09日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
// RemoteConsole.h : RemoteConsole 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error 在包含用于 PCH 的此文件之前包含“stdafx.h” 
#endif

#include "resource.h"       // 主符号


// CRemoteConsoleApp:
// 有关此类的实现，请参阅 RemoteConsole.cpp
//

class CRemoteConsoleApp : public CWinApp
{
protected:
	CNetServer			m_Server;
	CNetLinkManager		m_NetLinkManager;
	
public:
	CRemoteConsoleApp();


// 重写
public:
	virtual BOOL InitInstance();

	static CRemoteConsoleApp * GetInstance();

	CNetLinkManager * GetNetLinkManager()
	{
		return &m_NetLinkManager;
	}

	CDocTemplate* FindDocTemplate();

// 实现
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnIdle(LONG lCount);
	virtual int ExitInstance();

	virtual void OnFileNew();
	virtual void OnFileOpen();
};

extern CRemoteConsoleApp theApp;
