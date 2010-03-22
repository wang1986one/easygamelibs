// SystemConsoleFrame.cpp : 实现文件
//

#include "stdafx.h"
#include "RemoteConsole.h"
#include "SystemConsoleFrame.h"


// CSystemConsoleFrame

IMPLEMENT_DYNCREATE(CSystemConsoleFrame, CMDIChildWnd)

CSystemConsoleFrame::CSystemConsoleFrame()
{

}

CSystemConsoleFrame::~CSystemConsoleFrame()
{
}


BEGIN_MESSAGE_MAP(CSystemConsoleFrame, CMDIChildWnd)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CSystemConsoleFrame 消息处理程序

void CSystemConsoleFrame::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	ShowWindow(SW_MINIMIZE);
	//CMDIChildWnd::OnClose();
}
