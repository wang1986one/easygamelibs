/****************************************************************************/
/*                                                                          */
/*      文件名:    DlgMethodEditor.cpp                                      */
/*      创建日期:  2012年04月22日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
// DlgMethodEditor.cpp : 实现文件
//

#include "stdafx.h"
#include "CallInterfaceMaker.h"
#include "DlgMethodEditor.h"


// CDlgMethodEditor 对话框

IMPLEMENT_DYNAMIC(CDlgMethodEditor, CDialog)

CDlgMethodEditor::CDlgMethodEditor(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMethodEditor::IDD, pParent)
	, m_Name(_T(""))
	, m_Description(_T(""))
	, m_ID(_T(""))
	,m_ParamIDSeed(0)
{

}

CDlgMethodEditor::~CDlgMethodEditor()
{
}

void CDlgMethodEditor::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PARAM, m_lvParamList);
	DDX_Text(pDX, IDC_EDIT_NAME, m_Name);
	DDX_Text(pDX, IDC_EDIT_DESCRIPTION, m_Description);
	DDX_Text(pDX, IDC_EDIT_METHOD_ID, m_ID);
}


BEGIN_MESSAGE_MAP(CDlgMethodEditor, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_ADD_PARAM, &CDlgMethodEditor::OnBnClickedButtonAddParam)
	ON_BN_CLICKED(IDC_BUTTON_EDIT_PARAM, &CDlgMethodEditor::OnBnClickedButtonEditParam)
	ON_BN_CLICKED(IDC_BUTTON_DEL_PARAM, &CDlgMethodEditor::OnBnClickedButtonDelParam)
	ON_BN_CLICKED(IDC_BUTTON_PARAM_UP, &CDlgMethodEditor::OnBnClickedButtonParamUp)
	ON_BN_CLICKED(IDC_BUTTON_PARAM_DOWN, &CDlgMethodEditor::OnBnClickedButtonParamDown)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_PARAM, &CDlgMethodEditor::OnNMDblclkListParam)
END_MESSAGE_MAP()


// CDlgMethodEditor 消息处理程序

BOOL CDlgMethodEditor::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_lvParamList.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);


	m_lvParamList.InsertColumn(0,"名称",LVCFMT_LEFT,100);
	m_lvParamList.InsertColumn(1,"ID",LVCFMT_LEFT,50);
	m_lvParamList.InsertColumn(2,"类型",LVCFMT_LEFT,60);	
	m_lvParamList.InsertColumn(3,"长度",LVCFMT_LEFT,50);
	m_lvParamList.InsertColumn(4,"缺省值",LVCFMT_LEFT,50);
	m_lvParamList.InsertColumn(5,"可为空",LVCFMT_LEFT,60);
	m_lvParamList.InsertColumn(6,"描述",LVCFMT_LEFT,200);


	FillListItem();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgMethodEditor::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	UpdateData(true);
	CDialog::OnOK();
}

void CDlgMethodEditor::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类

	CDialog::OnCancel();
}

void CDlgMethodEditor::OnBnClickedButtonAddParam()
{
	// TODO: 在此添加控件通知处理程序代码
	CDlgParamEditor Dlg;

	

	m_ParamIDSeed++;

	
	Dlg.SetID(m_ParamIDSeed);
	
	if(Dlg.DoModal()==IDOK)
	{
		
		METHOD_PARAM ParamInfo;

		Dlg.GetData(ParamInfo);
		m_ParamList.push_back(ParamInfo);
		AddListItem(ParamInfo,m_ParamList.size()-1);
	}
}

void CDlgMethodEditor::OnBnClickedButtonEditParam()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION Pos=m_lvParamList.GetFirstSelectedItemPosition();
	if(Pos)
	{
		int Item=m_lvParamList.GetNextSelectedItem(Pos);
		UINT Index=(UINT)m_lvParamList.GetItemData(Item);
		if(Index<m_ParamList.size())
		{
			CDlgParamEditor Dlg;

			Dlg.SetData(m_ParamList[Index]);

			if(Dlg.DoModal()==IDOK)
			{
				Dlg.GetData(m_ParamList[Index]);
				EditListItem(Item,m_ParamList[Index]);
			}
		}
	}
	else
	{
		MessageBox("请选择一个参数");
	}
}

void CDlgMethodEditor::OnBnClickedButtonDelParam()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION Pos=m_lvParamList.GetFirstSelectedItemPosition();
	if(Pos)
	{
		int Item=m_lvParamList.GetNextSelectedItem(Pos);
		UINT Index=(UINT)m_lvParamList.GetItemData(Item);
		if(Index<m_ParamList.size())
		{
			CString Msg;
			Msg.Format("是否要删除参数[%s]",
				m_ParamList[Index].Name);
			if(AfxMessageBox(Msg,MB_YESNO)==IDYES)
			{
				m_ParamList.erase(m_ParamList.begin()+Index);
				FillListItem();
			}			
		}
	}
}
void CDlgMethodEditor::OnBnClickedButtonParamUp()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION Pos=m_lvParamList.GetFirstSelectedItemPosition();
	if(Pos)
	{
		int Item=m_lvParamList.GetNextSelectedItem(Pos);
		UINT Index=(UINT)m_lvParamList.GetItemData(Item);
		if(Index>0&&Index<m_ParamList.size())
		{
			METHOD_PARAM Temp=m_ParamList[Index];
			m_ParamList.erase(m_ParamList.begin()+Index);
			m_ParamList.insert(m_ParamList.begin()+Index-1,Temp);
			FillListItem();
		}
	}
}

void CDlgMethodEditor::OnBnClickedButtonParamDown()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION Pos=m_lvParamList.GetFirstSelectedItemPosition();
	if(Pos)
	{
		int Item=m_lvParamList.GetNextSelectedItem(Pos);
		UINT Index=(UINT)m_lvParamList.GetItemData(Item);
		if(Index<m_ParamList.size()-1)
		{
			METHOD_PARAM Temp=m_ParamList[Index];
			m_ParamList.erase(m_ParamList.begin()+Index);
			m_ParamList.insert(m_ParamList.begin()+Index+1,Temp);
			FillListItem();
		}
	}
}

void CDlgMethodEditor::AddListItem(METHOD_PARAM& ParamInfo,UINT Index)
{
	CString Temp;
	int Item=m_lvParamList.InsertItem(m_lvParamList.GetItemCount(),ParamInfo.Name);
	m_lvParamList.SetItemData(Item,(INT_PTR)Index);

	Temp.Format("%u",ParamInfo.ID);
	m_lvParamList.SetItemText(Item,1,Temp);

	PARAM_TYPE * pTypeInfo=GetMainDlg()->GetParamType(ParamInfo.Type);
	if(pTypeInfo)
	{
		if(ParamInfo.IsReference)			
			m_lvParamList.SetItemText(Item,2,pTypeInfo->ReferenceDefine);
		else
			m_lvParamList.SetItemText(Item,2,pTypeInfo->Name);
	}
	else
	{
		m_lvParamList.SetItemText(Item,2,"未知");
	}
	Temp.Format("%d",ParamInfo.Length);
	m_lvParamList.SetItemText(Item,3,Temp);
	m_lvParamList.SetItemText(Item,4,ParamInfo.DefaultValue);
	if(ParamInfo.CanNull)
	{
		m_lvParamList.SetItemText(Item,5,"可为空");
	}
	else
	{
		m_lvParamList.SetItemText(Item,5,"不可为空");
	}
	m_lvParamList.SetItemText(Item,6,ParamInfo.Description);
}

void CDlgMethodEditor::EditListItem(int Item,METHOD_PARAM& ParamInfo)
{
	CString Temp;
	m_lvParamList.SetItemText(Item,0,ParamInfo.Name);
	Temp.Format("%u",ParamInfo.ID);
	m_lvParamList.SetItemText(Item,1,Temp);

	PARAM_TYPE * pTypeInfo=GetMainDlg()->GetParamType(ParamInfo.Type);
	if(pTypeInfo)
	{
		if(ParamInfo.IsReference)			
			m_lvParamList.SetItemText(Item,2,pTypeInfo->ReferenceDefine);
		else
			m_lvParamList.SetItemText(Item,2,pTypeInfo->Name);
	}
	else
	{
		m_lvParamList.SetItemText(Item,2,"未知");
	}
	Temp.Format("%d",ParamInfo.Length);
	m_lvParamList.SetItemText(Item,3,Temp);
	m_lvParamList.SetItemText(Item,4,ParamInfo.DefaultValue);
	if(ParamInfo.CanNull)
	{
		m_lvParamList.SetItemText(Item,5,"可为空");
	}
	else
	{
		m_lvParamList.SetItemText(Item,5,"不可为空");
	}
	m_lvParamList.SetItemText(Item,6,ParamInfo.Description);
}

void CDlgMethodEditor::FillListItem()
{
	m_lvParamList.DeleteAllItems();
	for(UINT i=0;i<m_ParamList.size();i++)
	{
		AddListItem(m_ParamList[i],i);
	}
}


void CDlgMethodEditor::OnNMDblclkListParam(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<NMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	OnBnClickedButtonEditParam();
}
