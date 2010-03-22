// DlgConnect.cpp : 实现文件
//

#include "stdafx.h"
#include "RemoteConsole.h"
#include "DlgConnect.h"


// CDlgConnect 对话框

IMPLEMENT_DYNAMIC(CDlgConnect, CDialog)
CDlgConnect::CDlgConnect(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgConnect::IDD, pParent)
	, m_Address(_T("192.168.29.146"))
	, m_Port(9200)
{
}

CDlgConnect::~CDlgConnect()
{
}

void CDlgConnect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ADDRESS, m_Address);
	DDX_Text(pDX, IDC_EDIT_PORT, m_Port);
}


BEGIN_MESSAGE_MAP(CDlgConnect, CDialog)
END_MESSAGE_MAP()


// CDlgConnect 消息处理程序
