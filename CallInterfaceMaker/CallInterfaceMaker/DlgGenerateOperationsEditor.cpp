// DlgGenerateOperationsEditor.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgGenerateOperationsEditor.h"




// CDlgGenerateOperationsEditor 对话框

IMPLEMENT_DYNAMIC(CDlgGenerateOperationsEditor, CDialog)

CDlgGenerateOperationsEditor::CDlgGenerateOperationsEditor(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgGenerateOperationsEditor::IDD, pParent)
{
	m_IsModified=false;
	m_ShowIndexOperation=false;
}

CDlgGenerateOperationsEditor::~CDlgGenerateOperationsEditor()
{
}

void CDlgGenerateOperationsEditor::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_EDIT_OPERATION, m_edOperation);
	DDX_Control(pDX, IDC_TAB, m_tbOperations);
}


BEGIN_MESSAGE_MAP(CDlgGenerateOperationsEditor, CDialog)
	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB, &CDlgGenerateOperationsEditor::OnTcnSelchangingTab)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, &CDlgGenerateOperationsEditor::OnTcnSelchangeTab)	
	ON_EN_CHANGE(IDC_EDIT_OPERATION, &CDlgGenerateOperationsEditor::OnEnChangeEdit)
END_MESSAGE_MAP()


// CDlgGenerateOperationsEditor 消息处理程序

BOOL CDlgGenerateOperationsEditor::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	m_tbOperations.InsertItem(0,"长度计算操作");
	m_tbOperations.InsertItem(1,"打包操作");
	m_tbOperations.InsertItem(2,"解包操作");
	m_tbOperations.InsertItem(3,"引用定义方式");
	m_tbOperations.InsertItem(4,"引用使用方式");
	m_tbOperations.InsertItem(5,"变量定义");
	m_tbOperations.InsertItem(6,"初始化操作");
	m_tbOperations.InsertItem(7,"复制操作");
	m_tbOperations.InsertItem(8,"Get方法声明");
	m_tbOperations.InsertItem(9,"Get方法实现");
	m_tbOperations.InsertItem(10,"Set方法声明");
	m_tbOperations.InsertItem(11,"Set方法实现");
	if(m_ShowIndexOperation)
	{
		m_tbOperations.InsertItem(12,"索引操作");
		m_tbOperations.InsertItem(13,"只读索引操作");
	}

	m_tbOperations.SetCurSel(0);

	ShowItemData();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgGenerateOperationsEditor::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	if(m_IsModified)
	{
		if(AfxMessageBox("是否要放弃已作出的更改？",MB_YESNO)==IDYES)
		{
			CDialog::OnCancel();
		}
	}
	else
	{
		CDialog::OnCancel();
	}
}

void CDlgGenerateOperationsEditor::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	if(m_IsModified)
		FetchItemData();

	CDialog::OnOK();
}

void CDlgGenerateOperationsEditor::OnTcnSelchangingTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	FetchItemData();
}
void CDlgGenerateOperationsEditor::OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	ShowItemData();
}

void CDlgGenerateOperationsEditor::OnEnChangeEdit()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	m_IsModified=true;
}

void CDlgGenerateOperationsEditor::ShowItemData()
{
	switch(m_tbOperations.GetCurSel())
	{
	case 0:
		m_edOperation.SetWindowText(m_GenerateOperations.SizeCaculateOperation);
		break;
	case 1:
		m_edOperation.SetWindowText(m_GenerateOperations.PackOperation);
		break;
	case 2:
		m_edOperation.SetWindowText(m_GenerateOperations.UnpackOperation);
		break;	
	case 3:
		m_edOperation.SetWindowText(m_GenerateOperations.ReferenceDefine);
		break;
	case 4:
		m_edOperation.SetWindowText(m_GenerateOperations.ReferenceUse);
		break;
	case 5:
		m_edOperation.SetWindowText(m_GenerateOperations.VariableDefine);
		break;
	case 6:
		m_edOperation.SetWindowText(m_GenerateOperations.InitOperation);
		break;
	case 7:
		m_edOperation.SetWindowText(m_GenerateOperations.CloneOperation);
		break;
	case 8:
		m_edOperation.SetWindowText(m_GenerateOperations.GetMethodDeclare);
		break;
	case 9:
		m_edOperation.SetWindowText(m_GenerateOperations.GetMethodDefine);
		break;
	case 10:
		m_edOperation.SetWindowText(m_GenerateOperations.SetMethodDeclare);
		break;
	case 11:
		m_edOperation.SetWindowText(m_GenerateOperations.SetMethodDefine);
		break;
	case 12:
		m_edOperation.SetWindowText(m_GenerateOperations.IndexOperation);
		break;
	case 13:
		m_edOperation.SetWindowText(m_GenerateOperations.ConstIndexOperation);
		break;
	}
	
	UpdateData(false);
}
void CDlgGenerateOperationsEditor::FetchItemData()
{	
	UpdateData(true);
	
	switch(m_tbOperations.GetCurSel())
	{
	case 0:
		m_edOperation.GetWindowText(m_GenerateOperations.SizeCaculateOperation);
		break;
	case 1:
		m_edOperation.GetWindowText(m_GenerateOperations.PackOperation);
		break;
	case 2:
		m_edOperation.GetWindowText(m_GenerateOperations.UnpackOperation);
		break;	
	case 3:
		m_edOperation.GetWindowText(m_GenerateOperations.ReferenceDefine);
		break;
	case 4:
		m_edOperation.GetWindowText(m_GenerateOperations.ReferenceUse);
		break;
	case 5:
		m_edOperation.GetWindowText(m_GenerateOperations.VariableDefine);
		break;
	case 6:
		m_edOperation.GetWindowText(m_GenerateOperations.InitOperation);
		break;
	case 7:
		m_edOperation.GetWindowText(m_GenerateOperations.CloneOperation);
		break;
	case 8:
		m_edOperation.GetWindowText(m_GenerateOperations.GetMethodDeclare);
		break;
	case 9:
		m_edOperation.GetWindowText(m_GenerateOperations.GetMethodDefine);
		break;
	case 10:
		m_edOperation.GetWindowText(m_GenerateOperations.SetMethodDeclare);
		break;
	case 11:
		m_edOperation.GetWindowText(m_GenerateOperations.SetMethodDefine);
		break;
	case 12:
		m_edOperation.GetWindowText(m_GenerateOperations.IndexOperation);
		break;
	case 13:
		m_edOperation.GetWindowText(m_GenerateOperations.ConstIndexOperation);
		break;
	}
}

