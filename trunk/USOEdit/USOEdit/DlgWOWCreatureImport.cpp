/****************************************************************************/
/*                                                                          */
/*      文件名:    DlgWOWCreatureImport.cpp                                 */
/*      创建日期:  2011年10月07日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
// DlgWOWCreatureImport.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgWOWCreatureImport.h"




// DlgWOWCreatureImport 对话框

IMPLEMENT_DYNAMIC(DlgWOWCreatureImport, CDialog)

DlgWOWCreatureImport::DlgWOWCreatureImport(CWnd* pParent /*=NULL*/)
	: CDialog(DlgWOWCreatureImport::IDD, pParent)
	, m_FilterText(_T(""))
{
	m_CreatureDisplayID=0;
}

DlgWOWCreatureImport::~DlgWOWCreatureImport()
{
}

void DlgWOWCreatureImport::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_lvCreatureList);
	DDX_Text(pDX, IDC_EDIT1, m_FilterText);
}


BEGIN_MESSAGE_MAP(DlgWOWCreatureImport, CDialog)
	ON_BN_CLICKED(IDOK, &DlgWOWCreatureImport::OnBnClickedOk)
	ON_BN_CLICKED(IDC_FILTER, &DlgWOWCreatureImport::OnBnClickedFilter)
END_MESSAGE_MAP()


// DlgWOWCreatureImport 消息处理程序

BOOL DlgWOWCreatureImport::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_lvCreatureList.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_lvCreatureList.InsertColumn(0,_T("ID"),LVCFMT_LEFT,50);
	m_lvCreatureList.InsertColumn(1,_T("Name"),LVCFMT_LEFT,200);
	
	FillList(m_FilterText);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}



void DlgWOWCreatureImport::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION Pos=m_lvCreatureList.GetFirstSelectedItemPosition();
	if(Pos)
	{
		int Item=m_lvCreatureList.GetNextSelectedItem(Pos);
		m_CreatureDisplayID=m_lvCreatureList.GetItemData(Item);
		OnOK();
	}
	else
	{
		AfxMessageBox(_T("请选择一个项"));
	}
	
}

void DlgWOWCreatureImport::OnBnClickedFilter()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	FillList(m_FilterText);
}

void DlgWOWCreatureImport::FillList(LPCTSTR szFilter)
{
	m_lvCreatureList.DeleteAllItems();
	m_lvCreatureList.SetRedraw(false);
	LPVOID Pos=CBLZWOWDatabase::GetInstance()->GetFirstCreatureDisplayInfoPos();
	while(Pos)
	{
		CBLZWOWDatabase::BLZ_DB_CREATURE_DISPLAY_INFO * pDisplayInfo=CBLZWOWDatabase::GetInstance()->GetNextCreatureDisplayInfo(Pos);
		if(pDisplayInfo)
		{
			if(szFilter[0])
			{
				if(pDisplayInfo->Name.Find(szFilter)<0)
					continue;
			}
			CEasyString Temp;
			Temp.Format(_T("%u"),pDisplayInfo->ID);
			int Item=m_lvCreatureList.InsertItem(m_lvCreatureList.GetItemCount(),Temp);
			m_lvCreatureList.SetItemText(Item,1,pDisplayInfo->Name);
			m_lvCreatureList.SetItemData(Item,(DWORD_PTR)pDisplayInfo->ID);
		}
	}
	m_lvCreatureList.SetRedraw(true);
	m_lvCreatureList.Invalidate();
}