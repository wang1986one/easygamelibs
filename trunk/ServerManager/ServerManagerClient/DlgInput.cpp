// DlgInput.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgInput.h"




// CDlgInput 对话框

IMPLEMENT_DYNAMIC(CDlgInput, CDialog)

CDlgInput::CDlgInput(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgInput::IDD, pParent)
	, m_InputText(_T(""))
{

}

CDlgInput::~CDlgInput()
{
}

void CDlgInput::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_InputText);
}


BEGIN_MESSAGE_MAP(CDlgInput, CDialog)
END_MESSAGE_MAP()


// CDlgInput 消息处理程序

BOOL CDlgInput::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	SetWindowText(m_Title);	

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
