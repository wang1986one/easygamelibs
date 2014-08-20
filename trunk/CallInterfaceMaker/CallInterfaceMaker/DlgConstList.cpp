// DlgStructEditor.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgConstList.h"




// CDlgConstList 对话框

IMPLEMENT_DYNAMIC(CDlgConstList, CDialog)

CDlgConstList::CDlgConstList(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgConstList::IDD, pParent)
	, m_ListName(_T(""))
{

}

CDlgConstList::~CDlgConstList()
{
}

void CDlgConstList::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ENUM, m_lvList);

	DDX_Text(pDX, IDC_EDIT_NAME, m_ConstDefine.Name);
	DDX_Text(pDX, IDC_COMBO_TYPE, m_ConstDefine.Type);
	DDX_Text(pDX, IDC_EDIT_VALUE, m_ConstDefine.Value);
	DDX_Text(pDX, IDC_EDIT_DESCRIPTION, m_ConstDefine.Description);

	DDX_Control(pDX, IDC_COMBO_TYPE, m_cbType);
	DDX_Text(pDX, IDC_EDIT_LIST_NAME, m_ListName);
}


BEGIN_MESSAGE_MAP(CDlgConstList, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_NEW, &CDlgConstList::OnBnClickedButtonNew)
	ON_BN_CLICKED(IDC_BUTTON_EDIT, &CDlgConstList::OnBnClickedButtonEdit)
	ON_BN_CLICKED(IDC_BUTTON_DEL, &CDlgConstList::OnBnClickedButtonDel)
	ON_BN_CLICKED(IDC_BUTTON_MOVE_UP, &CDlgConstList::OnBnClickedButtonMoveUp)
	ON_BN_CLICKED(IDC_BUTTON_OVE_DOWN, &CDlgConstList::OnBnClickedButtonOveDown)
	ON_NOTIFY(NM_CLICK, IDC_LIST_ENUM, &CDlgConstList::OnNMClickListEnum)
	ON_BN_CLICKED(IDC_BUTTON_CLONE, &CDlgConstList::OnBnClickedButtonClone)
	ON_BN_CLICKED(IDC_BUTTON_MOVE, &CDlgConstList::OnBnClickedButtonMove)
END_MESSAGE_MAP()


// CDlgConstList 消息处理程序
void CDlgConstList::FillList()
{
	m_lvList.DeleteAllItems();
	for(size_t i=0;i<m_ConstDefineList.size();i++)
	{
		int Item=m_lvList.InsertItem(i,"");
		FillItem(Item,m_ConstDefineList[i],i);
	}
}
void CDlgConstList::FillItem(int Item,CONST_DEFINE_INFO& ConstDefine,size_t Index)
{
	m_lvList.SetItemText(Item,0,ConstDefine.Name);
	m_lvList.SetItemText(Item,1,ConstDefine.Type);
	m_lvList.SetItemText(Item,2,ConstDefine.Value);
	m_lvList.SetItemData(Item,Index);
}
void CDlgConstList::SelectItemByName(LPCTSTR szName)
{
	for(int i=0;i<m_lvList.GetItemCount();i++)
	{
		if(m_lvList.GetItemText(i,0)==szName)
		{
			m_lvList.SetItemState(i,LVIS_SELECTED,LVIS_SELECTED);
			m_lvList.EnsureVisible(i,false);
		}
		else
		{
			m_lvList.SetItemState(i,0,LVIS_SELECTED);
		}
	}
}
BOOL CDlgConstList::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	m_lvList.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_lvList.InsertColumn(0,_T("名称"),LVCFMT_LEFT,200);
	m_lvList.InsertColumn(1,_T("类型"),LVCFMT_LEFT,80);
	m_lvList.InsertColumn(2,_T("值"),LVCFMT_LEFT,200);


	m_cbType.AddString("(无)");
	for(UINT i=0;i<GetMainDlg()->GetVarTypeCount();i++)
	{
		m_cbType.AddString(GetMainDlg()->GetVarType(i)->Name);		
	}
	m_cbType.SetCurSel(0);

	FillList();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgConstList::OnBnClickedButtonNew()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	if(m_ConstDefine.Name.IsEmpty())
	{
		AfxMessageBox("请输入名称");
		return;
	}
	
	m_ConstDefineList.push_back(m_ConstDefine);
	FillList();
	
}

void CDlgConstList::OnBnClickedButtonEdit()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION Pos=m_lvList.GetFirstSelectedItemPosition();
	if(Pos)
	{
		int Item=m_lvList.GetNextSelectedItem(Pos);
		size_t Index=m_lvList.GetItemData(Item);
		if(Index<m_ConstDefineList.size())
		{
			UpdateData(true);
			m_ConstDefineList[Index]=m_ConstDefine;
				
			FillList();
		}
	}
}

void CDlgConstList::OnBnClickedButtonDel()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION Pos=m_lvList.GetFirstSelectedItemPosition();
	if(Pos)
	{
		int Item=m_lvList.GetNextSelectedItem(Pos);
		size_t Index=m_lvList.GetItemData(Item);
		CString Msg;
		Msg.Format("是否要删除常量[%s]",
			m_lvList.GetItemText(Item,0));
		if(AfxMessageBox(Msg,MB_YESNO)==IDYES)
		{
			m_ConstDefineList.erase(m_ConstDefineList.begin()+Index);
			FillList();
		}
	}
}



void CDlgConstList::OnBnClickedButtonMoveUp()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION Pos=m_lvList.GetFirstSelectedItemPosition();
	if(Pos)
	{
		int Item=m_lvList.GetNextSelectedItem(Pos);
		UINT Index=(UINT)m_lvList.GetItemData(Item);
		if(Index>0&&Index<m_ConstDefineList.size())
		{
			CONST_DEFINE_INFO Temp=m_ConstDefineList[Index];
			m_ConstDefineList.erase(m_ConstDefineList.begin()+Index);
			m_ConstDefineList.insert(m_ConstDefineList.begin()+Index-1,Temp);
			FillList();
			SelectItemByName(Temp.Name);
		}
	}
}

void CDlgConstList::OnBnClickedButtonOveDown()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION Pos=m_lvList.GetFirstSelectedItemPosition();
	if(Pos)
	{
		int Item=m_lvList.GetNextSelectedItem(Pos);
		UINT Index=(UINT)m_lvList.GetItemData(Item);
		if(Index<m_ConstDefineList.size()-1)
		{
			CONST_DEFINE_INFO Temp=m_ConstDefineList[Index];
			m_ConstDefineList.erase(m_ConstDefineList.begin()+Index);
			m_ConstDefineList.insert(m_ConstDefineList.begin()+Index+1,Temp);
			FillList();
			SelectItemByName(Temp.Name);
		}
	}
}


void CDlgConstList::OnNMClickListEnum(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	POSITION Pos=m_lvList.GetFirstSelectedItemPosition();
	if(Pos)
	{
		int Item=m_lvList.GetNextSelectedItem(Pos);
		size_t Index=m_lvList.GetItemData(Item);
		if(Index<m_ConstDefineList.size())
		{
			UpdateData(true);
			m_ConstDefine=m_ConstDefineList[Index];
			UpdateData(false);

			for(int i=0;i<m_cbType.GetCount();i++)
			{
				CString Text;
				m_cbType.GetLBText(i,Text);
				if(Text==m_ConstDefine.Type)
				{
					m_cbType.SetCurSel(i);
					break;
				}
			}
		}
	}
}

void CDlgConstList::OnBnClickedButtonClone()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION Pos=m_lvList.GetFirstSelectedItemPosition();
	if(Pos)
	{
		int Item=m_lvList.GetNextSelectedItem(Pos);
		UINT Index=(UINT)m_lvList.GetItemData(Item);
		if(Index>0&&Index<m_ConstDefineList.size())
		{
			m_ConstDefineList.push_back(m_ConstDefineList[Index]);
			FillList();
		}
	}
}

void CDlgConstList::OnBnClickedButtonMove()
{
	// TODO: 在此添加控件通知处理程序代码
	vector<UINT> MoveList;

	POSITION Pos=m_lvList.GetFirstSelectedItemPosition();
	while(Pos)
	{
		int Item=m_lvList.GetNextSelectedItem(Pos);
		UINT Index=(UINT)m_lvList.GetItemData(Item);
		if(Index<m_ConstDefineList.size())
		{
			MoveList.push_back(Index);		
		}
	}

	if(MoveList.size())
	{
		vector<CString> StructPacketList;
		GetMainDlg()->GetDataStructPacketList(DATA_STRUCT_CONST,StructPacketList);
		CDlgListSelector Dlg;
		Dlg.Init("移动到...",StructPacketList);
		if(Dlg.DoModal()==IDOK)
		{
			for(size_t i=0;i<MoveList.size();i++)
			{
				UINT Index=MoveList[i];			

				if(!GetMainDlg()->MoveDataStruct(DATA_STRUCT_CONST,m_ListName,Dlg.m_SelectedItem,m_ConstDefineList[Index].Name))				
				{
					AfxMessageBox("转移失败");
					return;
				}
			}
			for(int i=(int)MoveList.size()-1;i>=0;i--)
			{
				UINT Index=MoveList[i];			
				m_ConstDefineList.erase(m_ConstDefineList.begin()+Index);					
			}
		}
		FillList();
	}
}
