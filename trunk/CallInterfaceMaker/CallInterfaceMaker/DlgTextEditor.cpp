/****************************************************************************/
/*                                                                          */
/*      文件名:    DlgTextEditor.cpp                                        */
/*      创建日期:  2011年10月07日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
// DlgTextEditor.cpp : 实现文件
//

#include "stdafx.h"
#include "CallInterfaceMaker.h"
#include "DlgTextEditor.h"


// CDlgTextEditor 对话框

IMPLEMENT_DYNAMIC(CDlgTextEditor, CDialog)

CDlgTextEditor::CDlgTextEditor(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTextEditor::IDD, pParent)
	, m_Text(_T(""))
{

}

CDlgTextEditor::~CDlgTextEditor()
{
}

void CDlgTextEditor::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_Text);
}


BEGIN_MESSAGE_MAP(CDlgTextEditor, CDialog)
END_MESSAGE_MAP()


// CDlgTextEditor 消息处理程序

BOOL CDlgTextEditor::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	UpdateData(false);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgTextEditor::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	UpdateData(true);

	CDialog::OnOK();
}

void CDlgTextEditor::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类

	CDialog::OnCancel();
}
