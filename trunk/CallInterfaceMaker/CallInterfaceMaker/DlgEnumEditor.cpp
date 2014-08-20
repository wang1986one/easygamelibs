// DlgStructEditor.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DlgEnumEditor.h"




// CDlgEnumEditor �Ի���

IMPLEMENT_DYNAMIC(CDlgEnumEditor, CDialog)

CDlgEnumEditor::CDlgEnumEditor(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgEnumEditor::IDD, pParent)	
{
}

CDlgEnumEditor::~CDlgEnumEditor()
{
}

void CDlgEnumEditor::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_NAME, m_EnumDefineInfo.Name);
	DDX_Text(pDX, IDC_EDIT_SHORT_NAME, m_EnumDefineInfo.ShortName);	
	DDX_Control(pDX, IDC_LIST_MEMBER_LIST, m_lvMemberList);
	DDX_Text(pDX, IDC_EDIT_DESCRIPTION, m_EnumDefineInfo.Description);

	BOOL Is64Bit=(m_EnumDefineInfo.Flag&ENUM_DEFINE_FLAG_IS_64BIT)?TRUE:FALSE;
	BOOL ExportStrValue=(m_EnumDefineInfo.Flag&ENUM_DEFINE_FLAG_EXPORT_STR_VALUE)?TRUE:FALSE;

	DDX_Check(pDX, IDC_CHECK_IS_64BIT,Is64Bit);
	DDX_Check(pDX, IDC_CHECK_EXPORT_STR_VALUE,ExportStrValue);

	m_EnumDefineInfo.Flag=(Is64Bit?ENUM_DEFINE_FLAG_IS_64BIT:0)|
		(ExportStrValue?ENUM_DEFINE_FLAG_EXPORT_STR_VALUE:0);



	DDX_Text(pDX, IDC_EDIT_MEMBER_NAME, m_EnumMemberInfo.Name);
	DDX_Text(pDX, IDC_EDIT_MEMBER_VALUE, m_EnumMemberInfo.Value);
	DDX_Text(pDX, IDC_EDIT_MEMBER_STR_VALUE, m_EnumMemberInfo.StrValue);
	DDX_Text(pDX, IDC_EDIT_MEMBER_DESCRIPTION, m_EnumMemberInfo.Description);

	
}


BEGIN_MESSAGE_MAP(CDlgEnumEditor, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_ADD_MEMBER, &CDlgEnumEditor::OnBnClickedButtonAddMember)
	ON_BN_CLICKED(IDC_BUTTON_EDIT_MEMBER, &CDlgEnumEditor::OnBnClickedButtonEditMember)
	ON_BN_CLICKED(IDC_BUTTON_DEL_MEMBER, &CDlgEnumEditor::OnBnClickedButtonDelMember)
	ON_BN_CLICKED(IDOK, &CDlgEnumEditor::OnBnClickedOk)
	
	ON_BN_CLICKED(IDC_BUTTON_MEMBER_MOVE_UP, &CDlgEnumEditor::OnBnClickedButtonMemberMoveUp)
	ON_BN_CLICKED(IDC_BUTTON_MEMBER_MOVE_DOWN, &CDlgEnumEditor::OnBnClickedButtonMemberMoveDown)
	ON_NOTIFY(NM_CLICK, IDC_LIST_MEMBER_LIST, &CDlgEnumEditor::OnNMClickListMemberList)
END_MESSAGE_MAP()


// CDlgEnumEditor ��Ϣ�������
void CDlgEnumEditor::FillList()
{
	m_lvMemberList.DeleteAllItems();
	for(size_t i=0;i<m_EnumDefineInfo.MemberList.size();i++)
	{
		int Item=m_lvMemberList.InsertItem(i,m_EnumDefineInfo.MemberList[i].Name);
		m_lvMemberList.SetItemText(Item,1,m_EnumDefineInfo.MemberList[i].Value);
		m_lvMemberList.SetItemText(Item,2,m_EnumDefineInfo.MemberList[i].StrValue);
		m_lvMemberList.SetItemData(Item,i);
		
	}
}

void CDlgEnumEditor::SelectItemByName(LPCTSTR szName)
{
	for(int i=0;i<m_lvMemberList.GetItemCount();i++)
	{
		if(m_lvMemberList.GetItemText(i,0)==szName)
		{
			m_lvMemberList.SetItemState(i,LVIS_SELECTED,LVIS_SELECTED);
			m_lvMemberList.EnsureVisible(i,false);
		}
		else
		{
			m_lvMemberList.SetItemState(i,0,LVIS_SELECTED);
		}
	}
}

BOOL CDlgEnumEditor::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_lvMemberList.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_lvMemberList.InsertColumn(0,_T("����"),LVCFMT_LEFT,300);
	m_lvMemberList.InsertColumn(1,_T("ֵ"),LVCFMT_LEFT,60);	
	m_lvMemberList.InsertColumn(2,_T("����"),LVCFMT_LEFT,100);


	
	FillList();

	

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CDlgEnumEditor::OnBnClickedButtonAddMember()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);

	if(m_EnumMemberInfo.Name.IsEmpty())
	{
		AfxMessageBox("�������Ա����");
		return;
	}
	
	m_EnumDefineInfo.MemberList.push_back(m_EnumMemberInfo);
	FillList();
	m_lvMemberList.EnsureVisible(m_lvMemberList.GetItemCount()-1,false);
}

void CDlgEnumEditor::OnBnClickedButtonEditMember()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	POSITION Pos=m_lvMemberList.GetFirstSelectedItemPosition();
	if(Pos)
	{
		int Item=m_lvMemberList.GetNextSelectedItem(Pos);
		size_t Index=m_lvMemberList.GetItemData(Item);
		if(Index<m_EnumDefineInfo.MemberList.size())
		{
			UpdateData(true);
			m_EnumDefineInfo.MemberList[Index]=m_EnumMemberInfo;
			FillList();
		}
	}
}

void CDlgEnumEditor::OnBnClickedButtonDelMember()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	POSITION Pos=m_lvMemberList.GetFirstSelectedItemPosition();
	if(Pos)
	{
		int Item=m_lvMemberList.GetNextSelectedItem(Pos);
		size_t Index=m_lvMemberList.GetItemData(Item);
		CString Msg;
		Msg.Format("�Ƿ�Ҫɾ����Ա[%s]",
			m_lvMemberList.GetItemText(Item,0));
		if(AfxMessageBox(Msg,MB_YESNO)==IDYES)
		{
			m_EnumDefineInfo.MemberList.erase(m_EnumDefineInfo.MemberList.begin()+Index);
			FillList();
		}
	}
}



void CDlgEnumEditor::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);	

	m_EnumDefineInfo.Name.Trim();

	if(m_EnumDefineInfo.ShortName.IsEmpty())
		m_EnumDefineInfo.ShortName=m_EnumDefineInfo.Name;
	UpdateData(false);
	OnOK();
}



void CDlgEnumEditor::OnBnClickedButtonMemberMoveUp()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	POSITION Pos=m_lvMemberList.GetFirstSelectedItemPosition();
	if(Pos)
	{
		int Item=m_lvMemberList.GetNextSelectedItem(Pos);
		UINT Index=(UINT)m_lvMemberList.GetItemData(Item);
		if(Index>0&&Index<m_EnumDefineInfo.MemberList.size())
		{
			ENUM_MEMBER_INFO Temp=m_EnumDefineInfo.MemberList[Index];
			m_EnumDefineInfo.MemberList.erase(m_EnumDefineInfo.MemberList.begin()+Index);
			m_EnumDefineInfo.MemberList.insert(m_EnumDefineInfo.MemberList.begin()+Index-1,Temp);
			FillList();
			SelectItemByName(Temp.Name);
		}
	}
}

void CDlgEnumEditor::OnBnClickedButtonMemberMoveDown()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	POSITION Pos=m_lvMemberList.GetFirstSelectedItemPosition();
	if(Pos)
	{
		int Item=m_lvMemberList.GetNextSelectedItem(Pos);
		UINT Index=(UINT)m_lvMemberList.GetItemData(Item);
		if(Index<m_EnumDefineInfo.MemberList.size()-1)
		{
			ENUM_MEMBER_INFO Temp=m_EnumDefineInfo.MemberList[Index];
			m_EnumDefineInfo.MemberList.erase(m_EnumDefineInfo.MemberList.begin()+Index);
			m_EnumDefineInfo.MemberList.insert(m_EnumDefineInfo.MemberList.begin()+Index+1,Temp);
			FillList();
			SelectItemByName(Temp.Name);
		}
	}
}


void CDlgEnumEditor::OnNMClickListMemberList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;

	POSITION Pos=m_lvMemberList.GetFirstSelectedItemPosition();
	if(Pos)
	{
		int Item=m_lvMemberList.GetNextSelectedItem(Pos);
		size_t Index=m_lvMemberList.GetItemData(Item);
		if(Index<m_EnumDefineInfo.MemberList.size())
		{
			UpdateData(true);
			m_EnumMemberInfo=m_EnumDefineInfo.MemberList[Index];
			UpdateData(false);
		}
	}
}
