/****************************************************************************/
/*                                                                          */
/*      文件名:    D3DUIEdit.h                                              */
/*      创建日期:  2011年10月07日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
// D3DUIEdit.h : D3DUIEdit 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error 在包含用于 PCH 的此文件之前包含“stdafx.h” 
#endif

#include "resource.h"       // 主符号


// CD3DUIEditApp:
// 有关此类的实现，请参阅 D3DUIEdit.cpp
//
class CD3DViewPort;

class CD3DUIEditApp : public CWinApp
{
protected:
	CD3DViewPort * m_pD3DViewPort;
public:
	CD3DUIEditApp();


// 重写
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	void SetD3DViewPort(CD3DViewPort * pD3DViewPort)
	{
		m_pD3DViewPort=pD3DViewPort;
	}

// 实现
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnIdle(LONG lCount);
	afx_msg void OnFileOpen();
protected:
	void CheckSystemFile();
};

extern CD3DUIEditApp theApp;
