// DlgDataObjectModifyFlagEditor.cpp : 实现文件
//

#include "stdafx.h"

#include "DlgDataObjectModifyFlagEditor.h"


// CDlgDataObjectModifyFlagEditor 对话框

IMPLEMENT_DYNAMIC(CDlgDataObjectModifyFlagEditor, CDialog)

CDlgDataObjectModifyFlagEditor::CDlgDataObjectModifyFlagEditor(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgDataObjectModifyFlagEditor::IDD, pParent)
{

}

CDlgDataObjectModifyFlagEditor::~CDlgDataObjectModifyFlagEditor()
{
}

void CDlgDataObjectModifyFlagEditor::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_DATA_OBJECTS, m_lvDataObjects);
	DDX_Control(pDX, IDC_LIST_DATA_OBJECT_MEMBERS, m_lvDataObjectMembers);
	DDX_Text(pDX, IDC_EDIT_NAME, m_DataObjectModifyFlag.Name);
}


BEGIN_MESSAGE_MAP(CDlgDataObjectModifyFlagEditor, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgDataObjectModifyFlagEditor::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgDataObjectModifyFlagEditor::OnBnClickedCancel)
	ON_NOTIFY(NM_CLICK, IDC_LIST_DATA_OBJECTS, &CDlgDataObjectModifyFlagEditor::OnNMClickListDataObjects)
END_MESSAGE_MAP()


// CDlgDataObjectModifyFlagEditor 消息处理程序

BOOL CDlgDataObjectModifyFlagEditor::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_lvDataObjects.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_lvDataObjects.InsertColumn(0,_T("数据对象名称"),LVCFMT_LEFT,300);

	m_lvDataObjectMembers.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_CHECKBOXES);
	m_lvDataObjectMembers.InsertColumn(0,_T("成员名称"),LVCFMT_LEFT,300);

	GetMainDlg()->GetDataObjectDefineList(m_DataObjectList);
	

	vector<MODIFY_FLAG_UNIT>	ModifyFlagUnits;
	for(size_t i=0;i<m_DataObjectList.size();i++)
	{
		MODIFY_FLAG_UNIT * pModifyFlagUnit=FindModifyFlagUnit(m_DataObjectModifyFlag,m_DataObjectList[i].Name);
		if(pModifyFlagUnit)
		{
			ModifyFlagUnits.push_back(*pModifyFlagUnit);
		}
		else
		{
			MODIFY_FLAG_UNIT Empty;
			Empty.ClassName=m_DataObjectList[i].Name;
			ModifyFlagUnits.push_back(Empty);
		}
	}
	m_DataObjectModifyFlag.ModifyFlag=ModifyFlagUnits;
	

	FillDataObjectList();
	m_CurSelectIndex=-1;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgDataObjectModifyFlagEditor::FillDataObjectList()
{
	m_lvDataObjects.DeleteAllItems();
	for(size_t i=0;i<m_DataObjectList.size();i++)
	{
		int Item=m_lvDataObjects.InsertItem(i,m_DataObjectList[i].Name);
		m_lvDataObjects.SetItemData(Item,i);
	}
}

void CDlgDataObjectModifyFlagEditor::FillDataObjectMemberList(size_t Index)
{
	m_lvDataObjectMembers.DeleteAllItems();
	if(Index<m_DataObjectList.size())
	{
		m_CurSelectIndex=Index;
		for(size_t i=0;i<m_DataObjectList[Index].MemberList.size();i++)
		{
			int Item=m_lvDataObjectMembers.InsertItem(i,m_DataObjectList[Index].MemberList[i].Name);
			m_lvDataObjectMembers.SetItemData(Item,i);
			MODIFY_FLAG_UNIT * pModifyFlagUnit=FindModifyFlagUnit(m_DataObjectModifyFlag,m_DataObjectList[Index].Name);
			if(pModifyFlagUnit)
			{
				for(size_t j=0;j<pModifyFlagUnit->Members.size();j++)
				{
					if(pModifyFlagUnit->Members[j]==m_DataObjectList[Index].MemberList[i].Name)
					{
						m_lvDataObjectMembers.SetCheck(i);
						break;
					}
				}
			}
		}
	}
}

void CDlgDataObjectModifyFlagEditor::FetchDataObjectModifyFlag(size_t Index)
{
	MODIFY_FLAG_UNIT * pModifyFlagUnit=FindModifyFlagUnit(m_DataObjectModifyFlag,m_DataObjectList[Index].Name);
	if(pModifyFlagUnit)
	{
		pModifyFlagUnit->Members.clear();
		for(int i=0;i<m_lvDataObjectMembers.GetItemCount();i++)
		{
			if(m_lvDataObjectMembers.GetCheck(i))
			{
				pModifyFlagUnit->Members.push_back(m_lvDataObjectMembers.GetItemText(i,0));
			}
		}
	}
}

MODIFY_FLAG_UNIT * CDlgDataObjectModifyFlagEditor::FindModifyFlagUnit(DATA_OBJECT_MODIFY_FLAG& DataObjectModifyFlag,LPCTSTR ClassName)
{
	for(size_t i=0;i<DataObjectModifyFlag.ModifyFlag.size();i++)
	{
		if(DataObjectModifyFlag.ModifyFlag[i].ClassName==ClassName)
		{
			return &DataObjectModifyFlag.ModifyFlag[i];
		}
	}
	return NULL;
}

void CDlgDataObjectModifyFlagEditor::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_CurSelectIndex>=0)
		FetchDataObjectModifyFlag(m_CurSelectIndex);
	OnOK();
}

void CDlgDataObjectModifyFlagEditor::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}



void CDlgDataObjectModifyFlagEditor::OnNMClickListDataObjects(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	POSITION Pos=m_lvDataObjects.GetFirstSelectedItemPosition();
	if(Pos)
	{
		int Item=m_lvDataObjects.GetNextSelectedItem(Pos);
		size_t Index=m_lvDataObjects.GetItemData(Item);
		if(m_CurSelectIndex>=0&&(int)Index!=m_CurSelectIndex)
		{
			FetchDataObjectModifyFlag(m_CurSelectIndex);
		}
		FillDataObjectMemberList(Index);
	}
}

