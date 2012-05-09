/****************************************************************************/
/*                                                                          */
/*      文件名:    DlgFXEditor.cpp                                          */
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
/*      文件名:    DlgFXEditor.cpp                                          */
/*      创建日期:  2009年09月11日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
// DlgFXEditor.cpp : 实现文件
//

#include "stdafx.h"



// CDlgFXEditor 对话框

IMPLEMENT_DYNAMIC(CDlgFXEditor, CPropertyConfigDialog)
CDlgFXEditor::CDlgFXEditor(CWnd* pParent /*=NULL*/)
	: CPropertyConfigDialog(CDlgFXEditor::IDD, pParent)
{
}

CDlgFXEditor::~CDlgFXEditor()
{
}

void CDlgFXEditor::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_FX, m_FxContent);
}


BEGIN_MESSAGE_MAP(CDlgFXEditor, CDialog)
END_MESSAGE_MAP()


// CDlgFXEditor 消息处理程序

BOOL CDlgFXEditor::OnInitDialog()
{
	CPropertyConfigDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	UpdateData(false);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgFXEditor::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	UpdateData(true);
	CPropertyConfigDialog::OnOK();
}
