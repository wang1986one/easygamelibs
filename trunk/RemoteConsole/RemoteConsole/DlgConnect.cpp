/****************************************************************************/
/*                                                                          */
/*      文件名:    DlgConnect.cpp                                           */
/*      创建日期:  2009年02月09日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
// DlgConnect.cpp : 实现文件
//

#include "stdafx.h"
#include "RemoteConsole.h"
#include "DlgConnect.h"


// CDlgConnect 对话框

IMPLEMENT_DYNAMIC(CDlgConnect, CDialog)
CDlgConnect::CDlgConnect(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgConnect::IDD, pParent)
	, m_Address(_T(""))
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
