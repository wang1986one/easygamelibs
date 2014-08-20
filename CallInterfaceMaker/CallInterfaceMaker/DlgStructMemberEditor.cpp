// DlgStructMemberEditor.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgStructMemberEditor.h"




// CDlgStructMemberEditor 对话框

IMPLEMENT_DYNAMIC(CDlgStructMemberEditor, CDialog)

CDlgStructMemberEditor::CDlgStructMemberEditor(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgStructMemberEditor::IDD, pParent)
	, m_bMergeToParent(FALSE)
	, m_beMustPack(FALSE)
{
	
}

CDlgStructMemberEditor::~CDlgStructMemberEditor()
{
}

void CDlgStructMemberEditor::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_NAME, m_StructMemberInfo.Name);
	DDX_Control(pDX, IDC_COMBO_TYPE, m_cbType);
	DDX_Check(pDX, IDC_CHECK_IS_ARRAY, m_StructMemberInfo.IsArray);
	DDX_Text(pDX, IDC_EDIT_ARRAY_START_LENGTH, m_StructMemberInfo.ArrayStartLength);
	DDX_Text(pDX, IDC_EDIT_ARRAY_GROW_LENGTH, m_StructMemberInfo.ArrayGrowLength);
	DDX_Text(pDX, IDC_EDIT_DESCRIPTION, m_StructMemberInfo.Description);
	DDX_Check(pDX, IDC_CHECK_NOT_MONITOR_UPDATE, m_bNotMonitorUpdate);
	DDX_Check(pDX, IDC_CHECK_EXCLUDE_IN_PACKET, m_bExcludeInPacket);
	DDX_Check(pDX, IDC_CHECK_MERGE_TO_PARENT, m_bMergeToParent);
	DDX_Check(pDX, IDC_CHECK_MUST_PACK, m_beMustPack);
}


BEGIN_MESSAGE_MAP(CDlgStructMemberEditor, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgStructMemberEditor::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgStructMemberEditor 消息处理程序

BOOL CDlgStructMemberEditor::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	int Sel=-1;
	for(UINT i=0;i<GetMainDlg()->GetVarTypeCount();i++)
	{
		m_cbType.InsertString(i,GetMainDlg()->GetVarType(i)->Name);
		if(m_StructMemberInfo.Type==GetMainDlg()->GetVarType(i)->Name)
			Sel=i;
	}
	m_cbType.SetCurSel(Sel);

	m_bNotMonitorUpdate=(m_StructMemberInfo.Flag&STRUCT_MEMBER_FLAG_NOT_MONITOR_UPDATE)?TRUE:FALSE;
	m_bExcludeInPacket=(m_StructMemberInfo.Flag&STRUCT_MEMBER_FLAG_EXCLUDE_IN_PACKET)?TRUE:FALSE;
	m_bMergeToParent=(m_StructMemberInfo.Flag&STRUCT_MEMBER_FLAG_MERGE_TO_PARENT)?TRUE:FALSE;
	m_beMustPack=(m_StructMemberInfo.Flag&STRUCT_MEMBER_FLAG_MUST_PACK)?TRUE:FALSE;

	UpdateData(false);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgStructMemberEditor::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	m_cbType.GetWindowText(m_StructMemberInfo.Type);
	m_StructMemberInfo.Name.Trim();
	m_StructMemberInfo.Description.Trim();

	m_StructMemberInfo.Flag=(m_bNotMonitorUpdate?STRUCT_MEMBER_FLAG_NOT_MONITOR_UPDATE:0)|
		(m_bExcludeInPacket?STRUCT_MEMBER_FLAG_EXCLUDE_IN_PACKET:0)|
		(m_bMergeToParent?STRUCT_MEMBER_FLAG_MERGE_TO_PARENT:0)|
		(m_beMustPack?STRUCT_MEMBER_FLAG_MUST_PACK:0);
	UpdateData(false);
	OnOK();
}
