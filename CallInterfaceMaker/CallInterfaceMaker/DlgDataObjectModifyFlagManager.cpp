// DlgDataObjectModifyFlagManager.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgDataObjectModifyFlagManager.h"




// CDlgDataObjectModifyFlagManager 对话框

IMPLEMENT_DYNAMIC(CDlgDataObjectModifyFlagManager, CDialog)

CDlgDataObjectModifyFlagManager::CDlgDataObjectModifyFlagManager(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgDataObjectModifyFlagManager::IDD, pParent)
{

}

CDlgDataObjectModifyFlagManager::~CDlgDataObjectModifyFlagManager()
{
}

void CDlgDataObjectModifyFlagManager::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_lvList);
}


BEGIN_MESSAGE_MAP(CDlgDataObjectModifyFlagManager, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgDataObjectModifyFlagManager::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgDataObjectModifyFlagManager::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_NEW, &CDlgDataObjectModifyFlagManager::OnBnClickedButtonNew)
	ON_BN_CLICKED(IDC_BUTTON_EDIT, &CDlgDataObjectModifyFlagManager::OnBnClickedButtonEdit)
	ON_BN_CLICKED(IDC_BUTTON_DEL, &CDlgDataObjectModifyFlagManager::OnBnClickedButtonDel)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, &CDlgDataObjectModifyFlagManager::OnNMDblclkList)
	ON_BN_CLICKED(IDC_BUTTON_MOVE_UP, &CDlgDataObjectModifyFlagManager::OnBnClickedButtonMoveUp)
	ON_BN_CLICKED(IDC_BUTTON_MOVE_DOWN, &CDlgDataObjectModifyFlagManager::OnBnClickedButtonMoveDown)
	ON_BN_CLICKED(IDC_BUTTON_MOVE_SORT, &CDlgDataObjectModifyFlagManager::OnBnClickedButtonMoveSort)
END_MESSAGE_MAP()


// CDlgDataObjectModifyFlagManager 消息处理程序

void CDlgDataObjectModifyFlagManager::FillList()
{
	m_lvList.DeleteAllItems();
	for(size_t i=0;i<m_DataObjectModifyFlags.size();i++)
	{
		int Item=m_lvList.InsertItem(i,m_DataObjectModifyFlags[i].Name);
		m_lvList.SetItemData(Item,i);
	}
}
void CDlgDataObjectModifyFlagManager::SelectItemByName(LPCTSTR szName)
{
	for(int i=0;i<m_lvList.GetItemCount();i++)
	{
		if(m_lvList.GetItemText(i,0)==szName)
		{
			m_lvList.SetItemState(i,LVIS_SELECTED,LVIS_SELECTED);
		}
		else
		{
			m_lvList.SetItemState(i,0,LVIS_SELECTED);
		}
	}
}
BOOL CDlgDataObjectModifyFlagManager::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_lvList.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_lvList.InsertColumn(0,_T("名称"),LVCFMT_LEFT,300);

	FillList();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgDataObjectModifyFlagManager::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}

void CDlgDataObjectModifyFlagManager::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}

void CDlgDataObjectModifyFlagManager::OnBnClickedButtonNew()
{
	// TODO: 在此添加控件通知处理程序代码
	CDlgDataObjectModifyFlagEditor Dlg;
	if(Dlg.DoModal()==IDOK)
	{
		m_DataObjectModifyFlags.push_back(Dlg.m_DataObjectModifyFlag);
		FillList();
	}
}

void CDlgDataObjectModifyFlagManager::OnBnClickedButtonEdit()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION Pos=m_lvList.GetFirstSelectedItemPosition();
	if(Pos)
	{
		int Item=m_lvList.GetNextSelectedItem(Pos);
		size_t Index=m_lvList.GetItemData(Item);
		if(Index<m_DataObjectModifyFlags.size())
		{
			CDlgDataObjectModifyFlagEditor Dlg;

			Dlg.m_DataObjectModifyFlag=m_DataObjectModifyFlags[Index];
			if(Dlg.DoModal()==IDOK)
			{
				m_DataObjectModifyFlags[Index]=Dlg.m_DataObjectModifyFlag;
				FillList();
			}
		}
	}
}

void CDlgDataObjectModifyFlagManager::OnBnClickedButtonDel()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION Pos=m_lvList.GetFirstSelectedItemPosition();
	if(Pos)
	{
		int Item=m_lvList.GetNextSelectedItem(Pos);
		size_t Index=m_lvList.GetItemData(Item);
		CString Msg;
		Msg.Format("是否要删除更新集[%s]",
			m_lvList.GetItemText(Item,0));
		if(AfxMessageBox(Msg,MB_YESNO)==IDYES)
		{
			m_DataObjectModifyFlags.erase(m_DataObjectModifyFlags.begin()+Index);
			FillList();
		}
	}
}

void CDlgDataObjectModifyFlagManager::OnNMDblclkList(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	OnBnClickedButtonEdit();
}

void CDlgDataObjectModifyFlagManager::OnBnClickedButtonMoveUp()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION Pos=m_lvList.GetFirstSelectedItemPosition();
	if(Pos)
	{
		int Item=m_lvList.GetNextSelectedItem(Pos);
		UINT Index=(UINT)m_lvList.GetItemData(Item);
		if(Index>0&&Index<m_DataObjectModifyFlags.size())
		{
			DATA_OBJECT_MODIFY_FLAG Temp=m_DataObjectModifyFlags[Index];
			m_DataObjectModifyFlags.erase(m_DataObjectModifyFlags.begin()+Index);
			m_DataObjectModifyFlags.insert(m_DataObjectModifyFlags.begin()+Index-1,Temp);
			FillList();
			SelectItemByName(Temp.Name);
		}
	}
}

void CDlgDataObjectModifyFlagManager::OnBnClickedButtonMoveDown()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION Pos=m_lvList.GetFirstSelectedItemPosition();
	if(Pos)
	{
		int Item=m_lvList.GetNextSelectedItem(Pos);
		UINT Index=(UINT)m_lvList.GetItemData(Item);
		if(Index<m_DataObjectModifyFlags.size()-1)
		{
			DATA_OBJECT_MODIFY_FLAG Temp=m_DataObjectModifyFlags[Index];
			m_DataObjectModifyFlags.erase(m_DataObjectModifyFlags.begin()+Index);
			m_DataObjectModifyFlags.insert(m_DataObjectModifyFlags.begin()+Index+1,Temp);
			FillList();
			SelectItemByName(Temp.Name);
		}
	}
}

void CDlgDataObjectModifyFlagManager::OnBnClickedButtonMoveSort()
{
	// TODO: 在此添加控件通知处理程序代码
	vector<STRUCT_DEFINE_INFO>	DataObjectList;
	GetMainDlg()->GetDataObjectDefineList(DataObjectList);

	for(size_t j=0;j<m_DataObjectModifyFlags.size();j++)
	{
		vector<MODIFY_FLAG_UNIT>	ModifyFlagUnits;
		for(size_t i=0;i<DataObjectList.size();i++)
		{
			MODIFY_FLAG_UNIT * pModifyFlagUnit=FindModifyFlagUnit(m_DataObjectModifyFlags[j],DataObjectList[i].Name);
			if(pModifyFlagUnit)
			{
				ModifyFlagUnits.push_back(*pModifyFlagUnit);
			}
			else
			{
				MODIFY_FLAG_UNIT Empty;
				Empty.ClassName=DataObjectList[i].Name;
				ModifyFlagUnits.push_back(Empty);
			}
		}
		m_DataObjectModifyFlags[j].ModifyFlag=ModifyFlagUnits;
	}

	AfxMessageBox("整理完毕");
}


MODIFY_FLAG_UNIT * CDlgDataObjectModifyFlagManager::FindModifyFlagUnit(DATA_OBJECT_MODIFY_FLAG& DataObjectModifyFlag,LPCTSTR ClassName)
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