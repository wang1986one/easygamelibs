/****************************************************************************/
/*                                                                          */
/*      文件名:    DlgM2SkinView.cpp                                        */
/*      创建日期:  2011年10月07日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
// DlgM2SkinView.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgM2SkinView.h"




// CDlgM2SkinView 对话框

IMPLEMENT_DYNAMIC(CDlgM2SkinView, CPropertyConfigDialog)

CDlgM2SkinView::CDlgM2SkinView(CWnd* pParent /*=NULL*/)
	: CPropertyConfigDialog(CDlgM2SkinView::IDD, pParent)
{
	m_IsModual=false;
	m_pModel=NULL;
}

CDlgM2SkinView::~CDlgM2SkinView()
{
}

void CDlgM2SkinView::DoDataExchange(CDataExchange* pDX)
{
	CPropertyConfigDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_SKINS, m_lvSkins);
}


BEGIN_MESSAGE_MAP(CDlgM2SkinView, CPropertyConfigDialog)
	ON_BN_CLICKED(IDC_BUTTON_CHANGE_SKIN, &CDlgM2SkinView::OnBnClickedButtonChangeSkin)
END_MESSAGE_MAP()


// CDlgM2SkinView 消息处理程序

BOOL CDlgM2SkinView::OnInitDialog()
{
	CPropertyConfigDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_lvSkins.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);

	m_lvSkins.InsertColumn(0,_T("Index"),LVCFMT_LEFT,50);
	m_lvSkins.InsertColumn(1,_T("SkinFileName"),LVCFMT_LEFT,200);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgM2SkinView::OnBnClickedButtonChangeSkin()
{
	// TODO: 在此添加控件通知处理程序代码
}

bool CDlgM2SkinView::Init(CD3DWOWM2Model * pModel)
{
	m_pModel=pModel;
	m_lvSkins.DeleteAllItems();
	CEasyString Temp;
	for(UINT i=0;i<m_pModel->GetModelResource()->GetSkinCount();i++)
	{
		Temp.Format(_T("%d"),i);
		int Item=m_lvSkins.InsertItem(i,Temp);
		m_lvSkins.SetItemText(Item,1,m_pModel->GetModelResource()->GetSkinFileName(i));
		m_lvSkins.SetItemData(Item,i);
	}
	return true;
}
