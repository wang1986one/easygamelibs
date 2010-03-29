/****************************************************************************/
/*                                                                          */
/*      文件名:    SystemConsoleFrame.h                                     */
/*      创建日期:  2009年02月09日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once


// CSystemConsoleFrame 框架

class CSystemConsoleFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CSystemConsoleFrame)
protected:
	CSystemConsoleFrame();           // 动态创建所使用的受保护的构造函数
	virtual ~CSystemConsoleFrame();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
};


