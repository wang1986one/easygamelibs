// SystemConsoleFrame.cpp : ʵ���ļ�
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


// CSystemConsoleFrame ��Ϣ�������

void CSystemConsoleFrame::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	ShowWindow(SW_MINIMIZE);
	//CMDIChildWnd::OnClose();
}
