/****************************************************************************/
/*                                                                          */
/*      文件名:    DlgInterfaceEdtor.cpp                                    */
/*      创建日期:  2011年10月07日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
// DlgInterfaceEdtor.cpp : 实现文件
//

#include "stdafx.h"
#include "CallInterfaceMaker.h"
#include "DlgInterfaceEdtor.h"


// CDlgInterfaceEdtor 对话框

IMPLEMENT_DYNAMIC(CDlgInterfaceEdtor, CDialog)

CDlgInterfaceEdtor::CDlgInterfaceEdtor(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgInterfaceEdtor::IDD, pParent)
	, m_Name(_T(""))
	, m_Description(_T(""))
	, m_InterfaceType(0)
	, m_ID(_T(""))
{

}

CDlgInterfaceEdtor::~CDlgInterfaceEdtor()
{
}

void CDlgInterfaceEdtor::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_METHOD, m_lvMethodList);
	DDX_Text(pDX, IDC_EDIT_NAME, m_Name);
	DDX_Text(pDX, IDC_EDIT_DESCRIPTION, m_Description);	
	DDX_Control(pDX, IDC_RADIO_CALL, m_rdIsCall);
	DDX_Control(pDX, IDC_RADIO_CALL_BACK, m_rdIsCallBack);
	DDX_Text(pDX, IDC_EDIT_ID, m_ID);
}


BEGIN_MESSAGE_MAP(CDlgInterfaceEdtor, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_ADD_METHOD, &CDlgInterfaceEdtor::OnBnClickedButtonAddMethod)
	ON_BN_CLICKED(IDC_BUTTON_EDIT_METHOD, &CDlgInterfaceEdtor::OnBnClickedButtonEditMethod)
	ON_BN_CLICKED(IDC_BUTTON_DEL_METHOD, &CDlgInterfaceEdtor::OnBnClickedButtonDelMethod)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_METHOD, &CDlgInterfaceEdtor::OnNMDblclkListMethod)
END_MESSAGE_MAP()


// CDlgInterfaceEdtor 消息处理程序

BOOL CDlgInterfaceEdtor::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	m_lvMethodList.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);


	m_lvMethodList.InsertColumn(0,"名称",LVCFMT_LEFT,100);
	m_lvMethodList.InsertColumn(1,"参数数量",LVCFMT_LEFT,60);
	m_lvMethodList.InsertColumn(2,"ID",LVCFMT_LEFT,100);
	m_lvMethodList.InsertColumn(3,"描述",LVCFMT_LEFT,200);

	FillListItem();

	if(m_InterfaceType==INTERFACE_CALL)
	{
		m_rdIsCall.SetCheck(true);
		m_rdIsCallBack.SetCheck(false);
	}
	else
	{
		m_rdIsCall.SetCheck(false);
		m_rdIsCallBack.SetCheck(true);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgInterfaceEdtor::OnBnClickedButtonAddMethod()
{
	// TODO: 在此添加控件通知处理程序代码
	CDlgMethodEditor Dlg;

	if(Dlg.DoModal()==IDOK)
	{
		INTERFACE_METHOD MethodInfo;
		Dlg.GetData(MethodInfo);
		m_MethodList.push_back(MethodInfo);
		AddListItem(MethodInfo,m_MethodList.size()-1);
	}
}

void CDlgInterfaceEdtor::OnBnClickedButtonEditMethod()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION Pos=m_lvMethodList.GetFirstSelectedItemPosition();
	if(Pos)
	{
		int Item=m_lvMethodList.GetNextSelectedItem(Pos);
		UINT Index=(UINT)m_lvMethodList.GetItemData(Item);
		if(Index<m_MethodList.size())
		{
			CDlgMethodEditor Dlg;

			Dlg.SetData(m_MethodList[Index]);

			if(Dlg.DoModal()==IDOK)
			{
				Dlg.GetData(m_MethodList[Index]);
				EditListItem(Item,m_MethodList[Index]);
			}
		}
	}
	else
	{
		MessageBox("请选择一个方法");
	}
}

void CDlgInterfaceEdtor::OnBnClickedButtonDelMethod()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION Pos=m_lvMethodList.GetFirstSelectedItemPosition();
	if(Pos)
	{
		int Item=m_lvMethodList.GetNextSelectedItem(Pos);
		UINT Index=(UINT)m_lvMethodList.GetItemData(Item);
		if(Index<m_MethodList.size())
		{
			CString Msg;
			Msg.Format("是否要删除方法[%s]",
				m_MethodList[Index].Name);
			if(AfxMessageBox(Msg,MB_YESNO)==IDYES)
			{
				m_MethodList.erase(m_MethodList.begin()+Index);
				FillListItem();
			}
			
		}
	}
}

void CDlgInterfaceEdtor::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	UpdateData(true);
	if(m_rdIsCall.GetCheck()==BST_CHECKED)
	{
		m_InterfaceType=INTERFACE_CALL;
	}
	else
	{
		m_InterfaceType=INTERFACE_CALL_BACK;
	}
	CDialog::OnOK();
}

void CDlgInterfaceEdtor::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类

	CDialog::OnCancel();
}

void CDlgInterfaceEdtor::AddListItem(INTERFACE_METHOD& MethodInfo,UINT Index)
{
	CString Temp;
	int Item=m_lvMethodList.InsertItem(m_lvMethodList.GetItemCount(),MethodInfo.Name);
	m_lvMethodList.SetItemData(Item,(INT_PTR)Index);
	
	Temp.Format("%d",MethodInfo.ParamList.size());
	m_lvMethodList.SetItemText(Item,1,Temp);
	m_lvMethodList.SetItemText(Item,2,MethodInfo.ID);
	m_lvMethodList.SetItemText(Item,3,MethodInfo.Description);

}
void CDlgInterfaceEdtor::EditListItem(int Item,INTERFACE_METHOD& MethodInfo)
{
	CString Temp;
	m_lvMethodList.SetItemText(Item,0,MethodInfo.Name);
	Temp.Format("%d",MethodInfo.ParamList.size());
	m_lvMethodList.SetItemText(Item,1,Temp);
	m_lvMethodList.SetItemText(Item,2,MethodInfo.ID);
	m_lvMethodList.SetItemText(Item,3,MethodInfo.Description);
}
void CDlgInterfaceEdtor::FillListItem()
{
	m_lvMethodList.DeleteAllItems();
	for(UINT i=0;i<m_MethodList.size();i++)
	{
		AddListItem(m_MethodList[i],i);
	}
}

void CDlgInterfaceEdtor::OnNMDblclkListMethod(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<NMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	OnBnClickedButtonEditMethod();
}
