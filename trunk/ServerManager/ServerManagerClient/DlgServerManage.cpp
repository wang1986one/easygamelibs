/****************************************************************************/
/*                                                                          */
/*      文件名:    DlgServerManage.cpp                                      */
/*      创建日期:  2010年02月09日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
// DlgServerManage.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgServerManage.h"




// CDlgServerManage 对话框

IMPLEMENT_DYNAMIC(CDlgServerManage, CDialog)

CDlgServerManage::CDlgServerManage(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgServerManage::IDD, pParent)
	, m_IP(_T(""))
	, m_Port(8300)
{

}

CDlgServerManage::~CDlgServerManage()
{
}

void CDlgServerManage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_lvServerList);
	DDX_Text(pDX, IDC_EDIT_IP, m_IP);
	DDX_Text(pDX, IDC_EDIT_PORT, m_Port);
}


BEGIN_MESSAGE_MAP(CDlgServerManage, CDialog)
	ON_BN_CLICKED(IDC_ADD_SERVER, &CDlgServerManage::OnBnClickedAddServer)
	ON_BN_CLICKED(IDC_DEL_SERVER, &CDlgServerManage::OnBnClickedDelServer)
	ON_BN_CLICKED(IDC_EDIT_SERVER, &CDlgServerManage::OnBnClickedEditServer)
	ON_BN_CLICKED(IDOK, &CDlgServerManage::OnBnClickedOk)
//	ON_NOTIFY(HDN_ITEMDBLCLICK, 0, &CDlgServerManage::OnHdnItemdblclickList1)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CDlgServerManage::OnNMDblclkList1)
END_MESSAGE_MAP()


// CDlgServerManage 消息处理程序

void CDlgServerManage::OnBnClickedAddServer()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	m_IP.Trim();
	if(!m_IP.IsEmpty())
	{
		SERVER_INFO ServerInfo;

		ServerInfo.ServerAddress=m_IP;
		ServerInfo.ServerPort=m_Port;
		ServerInfo.ServerStatus=SCS_NO_CONNECTION;
		

		m_ServerList.Add(ServerInfo);

		int Item=m_lvServerList.InsertItem(m_lvServerList.GetItemCount(),ServerInfo.ServerAddress);
		CString Temp;
		Temp.Format("%u",ServerInfo.ServerPort);
		m_lvServerList.SetItemText(Item,1,Temp);
		m_lvServerList.SetItemText(Item,2,"未连接");
		m_lvServerList.SetItemData(Item,m_ServerList.GetCount()-1);

	}
	else
	{
		AfxMessageBox("请输入服务器地址");
	}
}

void CDlgServerManage::OnBnClickedDelServer()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION Pos=m_lvServerList.GetFirstSelectedItemPosition();
	while(Pos)
	{
		int Item=m_lvServerList.GetNextSelectedItem(Pos);
		UINT Index=m_lvServerList.GetItemData(Item);
		if(Index<m_ServerList.GetCount())
		{
			m_ServerList.Delete(Index);
			m_lvServerList.DeleteItem(Item);
		}
	}
}

void CDlgServerManage::OnBnClickedEditServer()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	m_IP.Trim();
	if(!m_IP.IsEmpty())
	{
		POSITION Pos=m_lvServerList.GetFirstSelectedItemPosition();
		if(Pos)
		{
			int Item=m_lvServerList.GetNextSelectedItem(Pos);
			UINT Index=m_lvServerList.GetItemData(Item);
			if(Index<m_ServerList.GetCount())
			{
				m_ServerList[Index].ServerAddress=m_IP;
				m_ServerList[Index].ServerPort=m_Port;
				m_lvServerList.SetItemText(Item,0,m_IP);
				CString Temp;
				Temp.Format("%u",m_Port);
				m_lvServerList.SetItemText(Item,1,Temp);				
			}
		}
	}
	else
	{
		AfxMessageBox("请输入服务器地址");
	}
}

void CDlgServerManage::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}

BOOL CDlgServerManage::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_lvServerList.InsertColumn(0,"地址",LVCFMT_LEFT,150);
	m_lvServerList.InsertColumn(1,"Port",LVCFMT_LEFT,50);
	m_lvServerList.InsertColumn(2,"状态",LVCFMT_LEFT,50);

	m_lvServerList.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);

	// TODO:  在此添加额外的初始化
	for(UINT i=0;i<m_ServerList.GetCount();i++)
	{
		int Item=m_lvServerList.InsertItem(i,m_ServerList[i].ServerAddress);
		CString Temp;
		Temp.Format("%u",m_ServerList[i].ServerPort);
		m_lvServerList.SetItemText(Item,1,Temp);
		switch(m_ServerList[i].ServerStatus)
		{
		case SCS_NO_CONNECTION:
			m_lvServerList.SetItemText(Item,2,"未连接");
			break;
		case SCS_DISCONNECTED:
			m_lvServerList.SetItemText(Item,2,"连接断开");
			break;
		case SCS_CONNECTING:
			m_lvServerList.SetItemText(Item,2,"已连接");			
			break;
		}
		m_lvServerList.SetItemData(Item,i);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

//void CDlgServerManage::OnHdnItemdblclickList1(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
//	// TODO: 在此添加控件通知处理程序代码
//	*pResult = 0;
//
//
//	m_IP=m_lvServerList.GetItemText(phdr->iItem,0);
//	CString Temp=m_lvServerList.GetItemText(phdr->iItem,1);
//	m_Port=atoi(Temp);
//	UpdateData(false);
//}

void CDlgServerManage::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<NMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;


	POSITION Pos=m_lvServerList.GetFirstSelectedItemPosition();
	if(Pos)
	{
		int Item=m_lvServerList.GetNextSelectedItem(Pos);
		m_IP=m_lvServerList.GetItemText(Item,0);
		CString Temp=m_lvServerList.GetItemText(Item,1);
		m_Port=atoi(Temp);
		UpdateData(false);
	}
}
