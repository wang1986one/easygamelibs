/****************************************************************************/
/*                                                                          */
/*      文件名:    USOEdit.h                                                */
/*      创建日期:  2011年10月07日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
/****************************************************************************/
/*                                                                          */
/*      文件名:    USOEdit.h                                                */
/*      创建日期:  2010年02月09日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
// USOEdit.h : USOEdit 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error 在包含用于 PCH 的此文件之前包含“stdafx.h” 
#endif

#include "resource.h"       // 主符号
//#include "D3DViewPort.h"

// CUSOEditApp:
// 有关此类的实现，请参阅 USOEdit.cpp
//

class CD3DViewPort;

class CUSOEditApp : public CWinApp
{
public:
	CUSOEditApp();

protected:	
	CArray<CD3DViewPort *,CD3DViewPort *>		m_D3DViewPortList;
	CEasyArray<CEasyString>						m_TexturePathList;
	CEasyArray<CEasyString>						m_FXPathList;
	CEasyArray<CEasyString>						m_MPQFileList;
	CEasyString									m_BLZDBCPath;
	CAMERA_SETTING								m_CameraSetting;
	VIEWPORT_SETTING							m_ViewportSetting;
	CMPQFileSystem								m_MPQFileSystem;
	CDlgFileViewer								m_DlgFileViewer;
	
	
// 重写
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
// 实现
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnIdle(LONG lCount);
	virtual BOOL OnDDECommand(LPTSTR lpszCommand);
public:
	static CUSOEditApp * GetInstance();
	void AddD3DViewPort(CD3DViewPort * pD3DViewPort)
	{
		DeleteD3DViewPort(pD3DViewPort);
		m_D3DViewPortList.Add(pD3DViewPort);
	}
	void DeleteD3DViewPort(CD3DViewPort * pD3DViewPort)
	{
		for(int i=0;i<m_D3DViewPortList.GetCount();i++)
		{
			if(m_D3DViewPortList[i]==pD3DViewPort)
			{
				m_D3DViewPortList.RemoveAt(i);
				return;
			}
		}
	}
	CEasyArray<CEasyString>& GetTexturePathList()
	{
		return m_TexturePathList;
	}
	CEasyArray<CEasyString>& GetFXPathList()
	{
		return m_FXPathList;
	}

	CAMERA_SETTING& GetCameraSetting()
	{
		return m_CameraSetting;
	}
	VIEWPORT_SETTING& GetViewportSetting()
	{
		return m_ViewportSetting;
	}

	CMPQFileSystem& GetMPQFileSystem()
	{
		return m_MPQFileSystem;
	}
	CDlgFileViewer& GetFileViewer()
	{
		return m_DlgFileViewer;
	}
	
	
	afx_msg void OnSetTexturePath();
	afx_msg void OnSetFxPath();
	afx_msg void OnLoadBlzDbc();


	void LoadSetting();
	void SaveSetting();
public:
	
	
	afx_msg void OnSetMpqFile();
};




