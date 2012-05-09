/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DlgTypeEditor.cpp                                        */
/*      ��������:  2012��04��22��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
// DlgTypeEditor.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DlgTypeEditor.h"




// CDlgTypeEditor �Ի���

IMPLEMENT_DYNAMIC(CDlgTypeEditor, CDialog)

CDlgTypeEditor::CDlgTypeEditor(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTypeEditor::IDD, pParent)
	, m_TypeName(_T(""))
	, m_CType(_T(""))
	, m_IsFixLength(FALSE)
	, m_NeedNullCheck(FALSE)
{
	m_IsModified=false;
	m_CurSelectItem=-1;
}

CDlgTypeEditor::~CDlgTypeEditor()
{
}

void CDlgTypeEditor::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_tabMain);
	DDX_Control(pDX, IDC_EDIT_CONTENT1, m_edConstructOperation);
	DDX_Control(pDX, IDC_EDIT_CONTENT2, m_edEvaluateOperation);
	DDX_Control(pDX, IDC_EDIT_CONTENT3, m_edSizeCaculateOperation);
	DDX_Control(pDX, IDC_EDIT_CONTENT4, m_edPackOperation);
	DDX_Control(pDX, IDC_EDIT_CONTENT5, m_edUnpackOperation);
	DDX_Control(pDX, IDC_EDIT_CONTENT6, m_edNULLCheckOperation);
	DDX_Control(pDX, IDC_EDIT_CONTENT7, m_edReferenceDefine);
	DDX_Control(pDX, IDC_LIST_TYPES, m_lvTypeList);
	DDX_Text(pDX, IDC_EDIT_NAME, m_TypeName);
	DDX_Text(pDX, IDC_EDIT_CTYPE, m_CType);
	DDX_Check(pDX, IDC_CHECK_IS_FIX_LENGTH, m_IsFixLength);
	DDX_Check(pDX, IDC_CHECK_NEED_NULL_CHECK, m_NeedNullCheck);
}


BEGIN_MESSAGE_MAP(CDlgTypeEditor, CDialog)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CDlgTypeEditor::OnTcnSelchangeTab1)
	ON_EN_CHANGE(IDC_EDIT_NAME, &CDlgTypeEditor::OnEnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT_CTYPE, &CDlgTypeEditor::OnEnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT_CONTENT1, &CDlgTypeEditor::OnEnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT_CONTENT2, &CDlgTypeEditor::OnEnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT_CONTENT3, &CDlgTypeEditor::OnEnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT_CONTENT4, &CDlgTypeEditor::OnEnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT_CONTENT5, &CDlgTypeEditor::OnEnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT_CONTENT6, &CDlgTypeEditor::OnEnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT_CONTENT7, &CDlgTypeEditor::OnEnChangeEdit)
	ON_BN_CLICKED(IDC_CHECK_IS_FIX_LENGTH, &CDlgTypeEditor::OnBnClickedCheckIsFixLength)
	ON_BN_CLICKED(IDC_CHECK_NEED_NULL_CHECK, &CDlgTypeEditor::OnBnClickedCheckNeedNullCheck)
	ON_NOTIFY(NM_CLICK, IDC_LIST_TYPES, &CDlgTypeEditor::OnNMClickListTypes)
	ON_BN_CLICKED(ID_NEW, &CDlgTypeEditor::OnBnClickedNew)
	ON_BN_CLICKED(ID_DEL, &CDlgTypeEditor::OnBnClickedDel)
END_MESSAGE_MAP()


// CDlgTypeEditor ��Ϣ�������

BOOL CDlgTypeEditor::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_tabMain.InsertItem(0,"�������");
	m_tabMain.InsertItem(1,"��ֵ����");
	m_tabMain.InsertItem(2,"���ȼ������");
	m_tabMain.InsertItem(3,"�������");
	m_tabMain.InsertItem(4,"�������");
	m_tabMain.InsertItem(5,"��ֵ������");
	m_tabMain.InsertItem(6,"���ö��巽ʽ");

	m_edConstructOperation.ShowWindow(SW_HIDE);
	m_edEvaluateOperation.ShowWindow(SW_HIDE);
	m_edSizeCaculateOperation.ShowWindow(SW_HIDE);
	m_edPackOperation.ShowWindow(SW_HIDE);
	m_edUnpackOperation.ShowWindow(SW_HIDE);
	m_edNULLCheckOperation.ShowWindow(SW_HIDE);
	m_edReferenceDefine.ShowWindow(SW_HIDE);

	m_tabMain.SetCurSel(0);
	m_edConstructOperation.ShowWindow(SW_SHOW);

	m_lvTypeList.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);

	m_lvTypeList.InsertColumn(0,"����",LVCFMT_LEFT,80);
	m_lvTypeList.InsertColumn(1,"C����",LVCFMT_LEFT,80);

	FillList();


	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CDlgTypeEditor::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	m_edConstructOperation.ShowWindow(SW_HIDE);
	m_edEvaluateOperation.ShowWindow(SW_HIDE);
	m_edSizeCaculateOperation.ShowWindow(SW_HIDE);
	m_edPackOperation.ShowWindow(SW_HIDE);
	m_edUnpackOperation.ShowWindow(SW_HIDE);
	m_edNULLCheckOperation.ShowWindow(SW_HIDE);
	m_edReferenceDefine.ShowWindow(SW_HIDE);
	switch(m_tabMain.GetCurSel())
	{
	case 0:
		m_edConstructOperation.ShowWindow(SW_SHOW);
		break;
	case 1:
		m_edEvaluateOperation.ShowWindow(SW_SHOW);
		break;
	case 2:
		m_edSizeCaculateOperation.ShowWindow(SW_SHOW);
		break;
	case 3:
		m_edPackOperation.ShowWindow(SW_SHOW);
		break;
	case 4:
		m_edUnpackOperation.ShowWindow(SW_SHOW);
		break;	
	case 5:
		m_edNULLCheckOperation.ShowWindow(SW_SHOW);
		break;
	case 6:
		m_edReferenceDefine.ShowWindow(SW_SHOW);
		break;
	}
}


void CDlgTypeEditor::OnEnChangeEdit()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	m_IsModified=true;
}


void CDlgTypeEditor::OnBnClickedCheckIsFixLength()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_IsModified=true;
}

void CDlgTypeEditor::OnBnClickedCheckNeedNullCheck()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_IsModified=true;
}

void CDlgTypeEditor::OnNMClickListTypes(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<NMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
	int NewSelect=-1;
	POSITION Pos=m_lvTypeList.GetFirstSelectedItemPosition();
	if(Pos)
	{
		NewSelect=m_lvTypeList.GetNextSelectedItem(Pos);
	}
	
	if(NewSelect!=m_CurSelectItem)
	{
		if(m_IsModified&&m_CurSelectItem>=0)
		{
			int Ret=AfxMessageBox("�Ƿ�Ҫ�������?",MB_YESNOCANCEL);
			switch(Ret)
			{
			case IDYES:
				FetchItemData(m_CurSelectItem);
				m_CurSelectItem=NewSelect;
				m_IsModified=false;
				ShowItemData(m_CurSelectItem);
				break;
			case IDNO:
				m_CurSelectItem=NewSelect;
				m_IsModified=false;
				ShowItemData(m_CurSelectItem);
				break;
			case IDCANCEL:
				m_lvTypeList.SetItemState(m_CurSelectItem,LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED);
				m_lvTypeList.SetItemState(NewSelect,0,LVIS_SELECTED|LVIS_FOCUSED);
				break;;
			}
		}
		else
		{
			m_CurSelectItem=NewSelect;
			ShowItemData(m_CurSelectItem);
		}
	}
	
	*pResult = 0;
}

void CDlgTypeEditor::ShowItemData(int Item)
{
	if(Item<0)
	{
		m_TypeName.Empty();
		m_CType.Empty();
		m_IsFixLength=FALSE;
		m_NeedNullCheck=FALSE;
		m_edConstructOperation.SetWindowText("");
		m_edEvaluateOperation.SetWindowText("");
		m_edSizeCaculateOperation.SetWindowText("");
		m_edPackOperation.SetWindowText("");
		m_edUnpackOperation.SetWindowText("");
		m_edNULLCheckOperation.SetWindowText("");
		m_edReferenceDefine.SetWindowText("");
		UpdateData(false);
		return;
	}
	UINT Index=m_lvTypeList.GetItemData(Item);
	if(Index<m_TypeList.size())
	{
		m_TypeName=m_TypeList[Index].Name;
		m_CType=m_TypeList[Index].CType;
		m_IsFixLength=m_TypeList[Index].IsFixedLength;
		m_NeedNullCheck=m_TypeList[Index].NeedNULLCheck;
		m_edConstructOperation.SetWindowText(m_TypeList[Index].ConstructOperation);
		m_edEvaluateOperation.SetWindowText(m_TypeList[Index].EvaluateOperation);
		m_edSizeCaculateOperation.SetWindowText(m_TypeList[Index].SizeCaculateOperation);
		m_edPackOperation.SetWindowText(m_TypeList[Index].PackOperation);
		m_edUnpackOperation.SetWindowText(m_TypeList[Index].UnpackOperation);
		m_edNULLCheckOperation.SetWindowText(m_TypeList[Index].NULLCheckOperation);
		m_edReferenceDefine.SetWindowText(m_TypeList[Index].ReferenceDefine);
		UpdateData(false);
	}
}
void CDlgTypeEditor::FetchItemData(int Item)
{	
	UINT Index=m_lvTypeList.GetItemData(Item);
	if(Index<m_TypeList.size())
	{
		UpdateData(true);
		m_TypeList[Index].Name=m_TypeName;
		m_TypeList[Index].CType=m_CType;
		m_TypeList[Index].IsFixedLength=m_IsFixLength;
		m_TypeList[Index].NeedNULLCheck=m_NeedNullCheck;
		m_edConstructOperation.GetWindowText(m_TypeList[Index].ConstructOperation);
		m_edEvaluateOperation.GetWindowText(m_TypeList[Index].EvaluateOperation);
		m_edSizeCaculateOperation.GetWindowText(m_TypeList[Index].SizeCaculateOperation);
		m_edPackOperation.GetWindowText(m_TypeList[Index].PackOperation);
		m_edUnpackOperation.GetWindowText(m_TypeList[Index].UnpackOperation);
		m_edNULLCheckOperation.GetWindowText(m_TypeList[Index].NULLCheckOperation);
		m_edReferenceDefine.GetWindowText(m_TypeList[Index].ReferenceDefine);
		m_lvTypeList.SetItemText(Item,0,m_TypeList[Index].Name);
		m_lvTypeList.SetItemText(Item,1,m_TypeList[Index].CType);
	}
	
}
void CDlgTypeEditor::OnBnClickedNew()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	PARAM_TYPE NewType;
	NewType.Name="������";
	NewType.CType="char";
	NewType.IsFixedLength=true;
	NewType.NeedNULLCheck=false;
	m_TypeList.push_back(NewType);
	UINT Index=m_TypeList.size()-1;

	int Item=m_lvTypeList.InsertItem(m_lvTypeList.GetItemCount(),m_TypeList[Index].Name);
	m_lvTypeList.SetItemText(Item,1,m_TypeList[Index].CType);
	m_lvTypeList.SetItemData(Item,Index);
}

void CDlgTypeEditor::OnBnClickedDel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	POSITION Pos=m_lvTypeList.GetFirstSelectedItemPosition();
	if(Pos)
	{
		int Item=m_lvTypeList.GetNextSelectedItem(Pos);
		if(AfxMessageBox("�Ƿ�Ҫɾ�������Ͷ���?",MB_YESNO)==IDYES)
		{
			UINT Index=m_lvTypeList.GetItemData(Item);
			m_TypeList.erase(m_TypeList.begin()+Index);
			FillList();
		}
	}
}


void CDlgTypeEditor::FillList()
{
	m_lvTypeList.DeleteAllItems();
	for(size_t i=0;i<m_TypeList.size();i++)
	{
		int Item=m_lvTypeList.InsertItem(i,m_TypeList[i].Name);
		m_lvTypeList.SetItemText(Item,1,m_TypeList[i].CType);
		m_lvTypeList.SetItemData(Item,i);
	}
}