// DlgConnect.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RemoteConsole.h"
#include "DlgConnect.h"


// CDlgConnect �Ի���

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


// CDlgConnect ��Ϣ�������
