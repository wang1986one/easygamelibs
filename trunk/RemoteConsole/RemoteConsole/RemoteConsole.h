/****************************************************************************/
/*                                                                          */
/*      �ļ���:    RemoteConsole.h                                          */
/*      ��������:  2009��02��09��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
// RemoteConsole.h : RemoteConsole Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h�� 
#endif

#include "resource.h"       // ������


// CRemoteConsoleApp:
// �йش����ʵ�֣������ RemoteConsole.cpp
//

class CRemoteConsoleApp : public CWinApp
{
protected:
	CNetServer			m_Server;
	CNetLinkManager		m_NetLinkManager;
	
public:
	CRemoteConsoleApp();


// ��д
public:
	virtual BOOL InitInstance();

	static CRemoteConsoleApp * GetInstance();

	CNetLinkManager * GetNetLinkManager()
	{
		return &m_NetLinkManager;
	}

	CDocTemplate* FindDocTemplate();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnIdle(LONG lCount);
	virtual int ExitInstance();

	virtual void OnFileNew();
	virtual void OnFileOpen();
};

extern CRemoteConsoleApp theApp;
