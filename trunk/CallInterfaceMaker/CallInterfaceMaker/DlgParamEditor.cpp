/****************************************************************************/
/*                                                                          */
/*      文件名:    DlgParamEditor.cpp                                       */
/*      创建日期:  2011年10月07日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
// DlgParamEditor.cpp : 实现文件
//

#include "stdafx.h"
#include "CallInterfaceMaker.h"
#include "DlgParamEditor.h"


// CDlgParamEditor 对话框

IMPLEMENT_DYNAMIC(CDlgParamEditor, CDialog)

CDlgParamEditor::CDlgParamEditor(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgParamEditor::IDD, pParent)
	, m_Name(_T(""))
	, m_Type(_T(""))
	, m_Length(0)
	, m_Description(_T(""))
	, m_DefaultValue(_T(""))
	, m_CanNull(FALSE)
	, m_IsReference(FALSE)
	, m_ID(0)
{

}

CDlgParamEditor::~CDlgParamEditor()
{
}

void CDlgParamEditor::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_NAME, m_Name);
	DDX_Control(pDX, IDC_COMBO_TYPE, m_cbType);
	DDX_Text(pDX, IDC_EDIT_LENGTH, m_Length);
	DDX_Text(pDX, IDC_EDIT_DESCRIPTION, m_Description);
	DDX_Text(pDX, IDC_EDIT_DEFAULT_VALUE, m_DefaultValue);
	DDX_Check(pDX, IDC_CHECK_CAN_NULL, m_CanNull);
	DDX_Check(pDX, IDC_CHECK_IS_REFERENCE, m_IsReference);
	DDX_Text(pDX, IDC_EDIT_PARAM_ID, m_ID);
}


BEGIN_MESSAGE_MAP(CDlgParamEditor, CDialog)
END_MESSAGE_MAP()


// CDlgParamEditor 消息处理程序

BOOL CDlgParamEditor::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	int Sel=-1;
	for(UINT i=0;i<GetMainDlg()->GetVarTypeCount();i++)
	{
		m_cbType.InsertString(i,GetMainDlg()->GetVarType(i)->Name);
		if(m_Type==GetMainDlg()->GetVarType(i)->Name)
			Sel=i;
	}
	m_cbType.SetCurSel(Sel);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgParamEditor::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	UpdateData(true);
	m_cbType.GetWindowText(m_Type);
	m_Name.Trim();
	m_DefaultValue.Trim();
	m_Description.Trim();

	CDialog::OnOK();
}

void CDlgParamEditor::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类

	CDialog::OnCancel();
}
