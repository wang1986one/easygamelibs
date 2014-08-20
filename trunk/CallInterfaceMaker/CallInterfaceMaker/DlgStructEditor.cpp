// DlgStructEditor.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DlgStructEditor.h"




// CDlgStructEditor �Ի���

IMPLEMENT_DYNAMIC(CDlgStructEditor, CDialog)

CDlgStructEditor::CDlgStructEditor(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgStructEditor::IDD, pParent)
{
	m_StructDefineInfo.IDSeed=1;

	//m_StructDefineInfo.PackOperation=GetMainDlg()->GetConfig().DefaultStructPackOperation;
	//m_StructDefineInfo.UnpackOperation=GetMainDlg()->GetConfig().DefaultStructUnpackOperation;
	//m_StructDefineInfo.SizeCaculateOperation=GetMainDlg()->GetConfig().DefaultStructSizeCaculateOperation;
	//m_StructDefineInfo.ReferenceDefine=GetMainDlg()->GetConfig().DefaultStructReferenceDefine;
}

CDlgStructEditor::~CDlgStructEditor()
{
}

void CDlgStructEditor::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_STRUCT_NAME, m_StructDefineInfo.Name);
	DDX_Text(pDX, IDC_EDIT_STRUCT_SHORT_NAME, m_StructDefineInfo.ShortName);
	DDX_Control(pDX, IDC_COMBO_BASE_STRUCT, m_cbBaseStruct);
	DDX_Control(pDX, IDC_LIST_MEMBER_LIST, m_lvMemberList);
	DDX_Text(pDX, IDC_EDIT_DESCRIPTION, m_StructDefineInfo.Description);
	DDX_Text(pDX, IDC_EDIT_OBJECT_ID, m_StructDefineInfo.ObjectID);
	DDX_Check(pDX, IDC_CHECK_IS_DATA_OBJECT, m_StructDefineInfo.IsDataObject);
	DDX_Check(pDX, IDC_CHECK_DECLARE_LATER, m_StructDefineInfo.DeclareLater);

	

	
}


BEGIN_MESSAGE_MAP(CDlgStructEditor, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_ADD_MEMBER, &CDlgStructEditor::OnBnClickedButtonAddMember)
	ON_BN_CLICKED(IDC_BUTTON_EDIT_MEMBER, &CDlgStructEditor::OnBnClickedButtonEditMember)
	ON_BN_CLICKED(IDC_BUTTON_DEL_MEMBER, &CDlgStructEditor::OnBnClickedButtonDelMember)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_MEMBER_LIST, &CDlgStructEditor::OnNMDblclkListMemberList)
	ON_BN_CLICKED(IDOK, &CDlgStructEditor::OnBnClickedOk)
	
	ON_BN_CLICKED(IDC_BUTTON_MEMBER_MOVE_UP, &CDlgStructEditor::OnBnClickedButtonMemberMoveUp)
	ON_BN_CLICKED(IDC_BUTTON_MEMBER_MOVE_DOWN, &CDlgStructEditor::OnBnClickedButtonMemberMoveDown)
	ON_BN_CLICKED(IDC_BUTTON_MEMBER_GENERATE_OPERATIONS, &CDlgStructEditor::OnBnClickedButtonMemberGenerateOperations)
END_MESSAGE_MAP()


// CDlgStructEditor ��Ϣ�������
void CDlgStructEditor::FillList()
{
	m_lvMemberList.DeleteAllItems();
	for(size_t i=0;i<m_StructDefineInfo.MemberList.size();i++)
	{
		int Item=m_lvMemberList.InsertItem(i,m_StructDefineInfo.MemberList[i].Name);
		m_lvMemberList.SetItemText(Item,1,m_StructDefineInfo.MemberList[i].Type);
		CString Temp;
		Temp.Format("%u",m_StructDefineInfo.MemberList[i].ID);
		m_lvMemberList.SetItemText(Item,2,Temp);
		m_lvMemberList.SetItemData(Item,i);
		
	}
}

void CDlgStructEditor::SelectItemByName(LPCTSTR szName)
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

BOOL CDlgStructEditor::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_lvMemberList.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_lvMemberList.InsertColumn(0,_T("����"),LVCFMT_LEFT,100);
	m_lvMemberList.InsertColumn(1,_T("����"),LVCFMT_LEFT,100);
	m_lvMemberList.InsertColumn(2,_T("ID"),LVCFMT_LEFT,60);

	

	m_cbBaseStruct.ResetContent();
	m_cbBaseStruct.AddString(_T("��"));
	for(UINT i=0;i<GetMainDlg()->GetVarTypeCount();i++)
	{
		if(GetMainDlg()->GetVarType(i)->Flag&TYPE_DEFINE_FLAG_STRUCT)
		{
			m_cbBaseStruct.AddString(GetMainDlg()->GetVarType(i)->Name);
		}
	}	

	if(!m_StructDefineInfo.BaseStruct.IsEmpty())
	{
		for(int i=0;i<m_cbBaseStruct.GetCount();i++)
		{
			CString Temp;
			m_cbBaseStruct.GetLBText(i,Temp);
			if(Temp==m_StructDefineInfo.BaseStruct)
			{
				m_cbBaseStruct.SetCurSel(i);
				break;
			}
		}
	}
	FillList();

	

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CDlgStructEditor::OnBnClickedButtonAddMember()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDlgStructMemberEditor Dlg;

	if(Dlg.DoModal()==IDOK)
	{
		Dlg.m_StructMemberInfo.ID=m_StructDefineInfo.IDSeed;
		m_StructDefineInfo.IDSeed++;
		m_StructDefineInfo.MemberList.push_back(Dlg.m_StructMemberInfo);
		FillList();
	}
}

void CDlgStructEditor::OnBnClickedButtonEditMember()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	POSITION Pos=m_lvMemberList.GetFirstSelectedItemPosition();
	if(Pos)
	{
		int Item=m_lvMemberList.GetNextSelectedItem(Pos);
		size_t Index=m_lvMemberList.GetItemData(Item);
		if(Index<m_StructDefineInfo.MemberList.size())
		{
			CDlgStructMemberEditor Dlg;

			Dlg.m_StructMemberInfo=m_StructDefineInfo.MemberList[Index];
			if(Dlg.DoModal()==IDOK)
			{
				m_StructDefineInfo.MemberList[Index]=Dlg.m_StructMemberInfo;
				FillList();
			}
		}
	}
}

void CDlgStructEditor::OnBnClickedButtonDelMember()
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
			m_StructDefineInfo.MemberList.erase(m_StructDefineInfo.MemberList.begin()+Index);
			FillList();
		}
	}
}

void CDlgStructEditor::OnNMDblclkListMemberList(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnBnClickedButtonEditMember();
	*pResult = 0;
}

void CDlgStructEditor::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	m_cbBaseStruct.GetWindowText(m_StructDefineInfo.BaseStruct);
	if(m_StructDefineInfo.BaseStruct=="��")
		m_StructDefineInfo.BaseStruct="";


	m_StructDefineInfo.Name.Trim();
	m_StructDefineInfo.BaseStruct.Trim();

	if(m_StructDefineInfo.ShortName.IsEmpty())
		m_StructDefineInfo.ShortName=m_StructDefineInfo.Name;
	UpdateData(false);
	OnOK();
}



void CDlgStructEditor::OnBnClickedButtonMemberMoveUp()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	POSITION Pos=m_lvMemberList.GetFirstSelectedItemPosition();
	if(Pos)
	{
		int Item=m_lvMemberList.GetNextSelectedItem(Pos);
		UINT Index=(UINT)m_lvMemberList.GetItemData(Item);
		if(Index>0&&Index<m_StructDefineInfo.MemberList.size())
		{
			STRUCT_MEMBER_INFO Temp=m_StructDefineInfo.MemberList[Index];
			m_StructDefineInfo.MemberList.erase(m_StructDefineInfo.MemberList.begin()+Index);
			m_StructDefineInfo.MemberList.insert(m_StructDefineInfo.MemberList.begin()+Index-1,Temp);
			FillList();
			SelectItemByName(Temp.Name);
		}
	}
}

void CDlgStructEditor::OnBnClickedButtonMemberMoveDown()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	POSITION Pos=m_lvMemberList.GetFirstSelectedItemPosition();
	if(Pos)
	{
		int Item=m_lvMemberList.GetNextSelectedItem(Pos);
		UINT Index=(UINT)m_lvMemberList.GetItemData(Item);
		if(Index<m_StructDefineInfo.MemberList.size()-1)
		{
			STRUCT_MEMBER_INFO Temp=m_StructDefineInfo.MemberList[Index];
			m_StructDefineInfo.MemberList.erase(m_StructDefineInfo.MemberList.begin()+Index);
			m_StructDefineInfo.MemberList.insert(m_StructDefineInfo.MemberList.begin()+Index+1,Temp);
			FillList();
			SelectItemByName(Temp.Name);
		}
	}
}

void CDlgStructEditor::OnBnClickedButtonMemberGenerateOperations()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDlgGenerateOperationsEditor Dlg;
	Dlg.m_GenerateOperations=m_StructDefineInfo.GenerateOperations;
	if(Dlg.DoModal()==IDOK)
	{
		m_StructDefineInfo.GenerateOperations=Dlg.m_GenerateOperations;
	}
}
