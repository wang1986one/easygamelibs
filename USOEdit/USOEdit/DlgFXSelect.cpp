/****************************************************************************/
/*                                                                          */
/*      文件名:    DlgFXSelect.cpp                                          */
/*      创建日期:  2011年10月07日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
/****************************************************************************/
/*                                                                          */
/*      文件名:    DlgFXSelect.cpp                                          */
/*      创建日期:  2009年09月11日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
// DlgFXSelect.cpp : 实现文件
//

#include "stdafx.h"



// CDlgFXSelect 对话框

IMPLEMENT_DYNAMIC(CDlgFXSelect, CPropertyConfigDialog)
CDlgFXSelect::CDlgFXSelect(CWnd* pParent /*=NULL*/)
	: CPropertyConfigDialog(CDlgFXSelect::IDD, pParent)
{
	m_SelectedFX=NULL;
}

CDlgFXSelect::~CDlgFXSelect()
{
}

void CDlgFXSelect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_lvFXList);
}


BEGIN_MESSAGE_MAP(CDlgFXSelect, CPropertyConfigDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgFXSelect 消息处理程序

void CDlgFXSelect::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION Pos=m_lvFXList.GetFirstSelectedItemPosition();
	if(Pos)
	{
		int item=m_lvFXList.GetNextSelectedItem(Pos);
		m_SelectedFX=m_FXPtrArray[item];
	}
	OnOK();
}

BOOL CDlgFXSelect::OnInitDialog()
{
	CPropertyConfigDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	int item=0;
	for(int i=0;i<(int)m_FXPtrArray.size();i++)
	{
		item=m_lvFXList.InsertItem(item+1,m_FXPtrArray[i]->GetName());
		if(m_FXPtrArray[i]==m_SelectedFX)
			m_lvFXList.SetItemState(item,LVIS_SELECTED,LVIS_SELECTED);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
