// DlgStructEditor.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgStructList.h"




// CDlgStructList 对话框

IMPLEMENT_DYNAMIC(CDlgStructList, CDialog)

CDlgStructList::CDlgStructList(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgStructList::IDD, pParent)
	, m_ListName(_T(""))
{

}

CDlgStructList::~CDlgStructList()
{
}

void CDlgStructList::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_STRUCT, m_lvStruct);
	DDX_Text(pDX, IDC_EDIT_LIST_NAME, m_ListName);
}


BEGIN_MESSAGE_MAP(CDlgStructList, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_NEW, &CDlgStructList::OnBnClickedButtonNew)
	ON_BN_CLICKED(IDC_BUTTON_EDIT, &CDlgStructList::OnBnClickedButtonEdit)
	ON_BN_CLICKED(IDC_BUTTON_DEL, &CDlgStructList::OnBnClickedButtonDel)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_STRUCT, &CDlgStructList::OnNMDblclkListStruct)
	ON_BN_CLICKED(IDC_BUTTON_MOVE_UP, &CDlgStructList::OnBnClickedButtonMoveUp)
	ON_BN_CLICKED(IDC_BUTTON_OVE_DOWN, &CDlgStructList::OnBnClickedButtonOveDown)
	ON_BN_CLICKED(IDC_BUTTON_CLONE, &CDlgStructList::OnBnClickedButtonClone)
	ON_BN_CLICKED(IDC_BUTTON_MOVE, &CDlgStructList::OnBnClickedButtonMove)
END_MESSAGE_MAP()


// CDlgStructList 消息处理程序
void CDlgStructList::FillList()
{
	m_lvStruct.DeleteAllItems();
	for(size_t i=0;i<m_StructDefineList.size();i++)
	{
		int Item=m_lvStruct.InsertItem(i,"");
		FillItem(Item,m_StructDefineList[i],i);
	}
}
void CDlgStructList::FillItem(int Item,STRUCT_DEFINE_INFO& StructDefine,size_t Index)
{
	m_lvStruct.SetItemText(Item,0,StructDefine.Name);
	m_lvStruct.SetItemText(Item,1,StructDefine.Description);
	m_lvStruct.SetItemData(Item,Index);
}
void CDlgStructList::SelectItemByName(LPCTSTR szName)
{
	for(int i=0;i<m_lvStruct.GetItemCount();i++)
	{
		if(m_lvStruct.GetItemText(i,0)==szName)
		{
			m_lvStruct.SetItemState(i,LVIS_SELECTED,LVIS_SELECTED);
			m_lvStruct.EnsureVisible(i,false);
		}
		else
		{
			m_lvStruct.SetItemState(i,0,LVIS_SELECTED);
		}
	}
}
BOOL CDlgStructList::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	m_lvStruct.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_lvStruct.InsertColumn(0,_T("名称"),LVCFMT_LEFT,200);
	m_lvStruct.InsertColumn(1,_T("描述"),LVCFMT_LEFT,200);

	FillList();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgStructList::OnBnClickedButtonNew()
{
	// TODO: 在此添加控件通知处理程序代码
	CDlgStructEditor Dlg;

	if(Dlg.DoModal()==IDOK)
	{
		m_StructDefineList.push_back(Dlg.m_StructDefineInfo);
		FillList();
	}
}

void CDlgStructList::OnBnClickedButtonEdit()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION Pos=m_lvStruct.GetFirstSelectedItemPosition();
	if(Pos)
	{
		int Item=m_lvStruct.GetNextSelectedItem(Pos);
		size_t Index=m_lvStruct.GetItemData(Item);
		if(Index<m_StructDefineList.size())
		{
			CDlgStructEditor Dlg;

			Dlg.m_StructDefineInfo=m_StructDefineList[Index];
			if(Dlg.DoModal()==IDOK)
			{
				m_StructDefineList[Index]=Dlg.m_StructDefineInfo;
				FillList();
			}
		}
	}
}

void CDlgStructList::OnBnClickedButtonDel()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION Pos=m_lvStruct.GetFirstSelectedItemPosition();
	if(Pos)
	{
		int Item=m_lvStruct.GetNextSelectedItem(Pos);
		size_t Index=m_lvStruct.GetItemData(Item);
		CString Msg;
		Msg.Format("是否要删除结构[%s]",
			m_lvStruct.GetItemText(Item,0));
		if(AfxMessageBox(Msg,MB_YESNO)==IDYES)
		{
			m_StructDefineList.erase(m_StructDefineList.begin()+Index);
			FillList();
		}
	}
}

void CDlgStructList::OnNMDblclkListStruct(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	OnBnClickedButtonEdit();
}

void CDlgStructList::OnBnClickedButtonMoveUp()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION Pos=m_lvStruct.GetFirstSelectedItemPosition();
	if(Pos)
	{
		int Item=m_lvStruct.GetNextSelectedItem(Pos);
		UINT Index=(UINT)m_lvStruct.GetItemData(Item);
		if(Index>0&&Index<m_StructDefineList.size())
		{
			STRUCT_DEFINE_INFO Temp=m_StructDefineList[Index];
			m_StructDefineList.erase(m_StructDefineList.begin()+Index);
			m_StructDefineList.insert(m_StructDefineList.begin()+Index-1,Temp);
			FillList();
			SelectItemByName(Temp.Name);
		}
	}
}

void CDlgStructList::OnBnClickedButtonOveDown()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION Pos=m_lvStruct.GetFirstSelectedItemPosition();
	if(Pos)
	{
		int Item=m_lvStruct.GetNextSelectedItem(Pos);
		UINT Index=(UINT)m_lvStruct.GetItemData(Item);
		if(Index<m_StructDefineList.size()-1)
		{
			STRUCT_DEFINE_INFO Temp=m_StructDefineList[Index];
			m_StructDefineList.erase(m_StructDefineList.begin()+Index);
			m_StructDefineList.insert(m_StructDefineList.begin()+Index+1,Temp);
			FillList();
			SelectItemByName(Temp.Name);
		}
	}
}



void CDlgStructList::OnBnClickedButtonClone()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION Pos=m_lvStruct.GetFirstSelectedItemPosition();
	if(Pos)
	{
		int Item=m_lvStruct.GetNextSelectedItem(Pos);
		UINT Index=(UINT)m_lvStruct.GetItemData(Item);
		if(Index>0&&Index<m_StructDefineList.size())
		{
			m_StructDefineList.push_back(m_StructDefineList[Index]);
			FillList();
		}
	}
}

void CDlgStructList::OnBnClickedButtonMove()
{
	// TODO: 在此添加控件通知处理程序代码
	vector<UINT> MoveList;

	POSITION Pos=m_lvStruct.GetFirstSelectedItemPosition();
	while(Pos)
	{
		int Item=m_lvStruct.GetNextSelectedItem(Pos);
		UINT Index=(UINT)m_lvStruct.GetItemData(Item);
		if(Index<m_StructDefineList.size())
		{
			MoveList.push_back(Index);		
		}
	}

	if(MoveList.size())
	{
		vector<CString> StructPacketList;
		GetMainDlg()->GetDataStructPacketList(DATA_STRUCT_STRUCT,StructPacketList);
		CDlgListSelector Dlg;
		Dlg.Init("移动到...",StructPacketList);
		if(Dlg.DoModal()==IDOK)
		{
			for(size_t i=0;i<MoveList.size();i++)
			{
				UINT Index=MoveList[i];			

				if(!GetMainDlg()->MoveDataStruct(DATA_STRUCT_STRUCT,m_ListName,Dlg.m_SelectedItem,m_StructDefineList[Index].Name))				
				{
					AfxMessageBox("转移失败");
					return;
				}
			}

			for(int i=(int)MoveList.size()-1;i>=0;i--)
			{
				UINT Index=MoveList[i];			
				m_StructDefineList.erase(m_StructDefineList.begin()+Index);					
			}
		}
		FillList();

	}	
}
