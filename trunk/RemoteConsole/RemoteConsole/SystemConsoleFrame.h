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


