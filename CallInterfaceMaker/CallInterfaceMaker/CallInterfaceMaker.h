/****************************************************************************/
/*                                                                          */
/*      文件名:    CallInterfaceMaker.h                                     */
/*      创建日期:  2011年10月07日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
// CallInterfaceMaker.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号


// CCallInterfaceMakerApp:
// 有关此类的实现，请参阅 CallInterfaceMaker.cpp
//

class CCallInterfaceMakerApp : public CWinApp
{
protected:
	CCallInterfaceMakerDlg	*m_pMainDlg;	
public:
	CCallInterfaceMakerApp();

	
	CCallInterfaceMakerDlg * GetMainDlg()
	{
		return m_pMainDlg;
	}

// 重写
	public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
};

extern CCallInterfaceMakerApp theApp;

inline CCallInterfaceMakerDlg * GetMainDlg()
{
	return theApp.GetMainDlg();
}