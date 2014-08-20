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
	DDX_Control(pDX, IDC_EDIT_OPERATION, m_edOperation);	
	DDX_Control(pDX, IDC_LIST_TYPES, m_lvTypeList);
	DDX_Text(pDX, IDC_EDIT_NAME, m_TypeName);
	DDX_Text(pDX, IDC_EDIT_CTYPE, m_CType);
}


BEGIN_MESSAGE_MAP(CDlgTypeEditor, CDialog)
	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB1, &CDlgTypeEditor::OnTcnSelchangingTab1)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CDlgTypeEditor::OnTcnSelchangeTab1)
	ON_EN_CHANGE(IDC_EDIT_NAME, &CDlgTypeEditor::OnEnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT_CTYPE, &CDlgTypeEditor::OnEnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT_OPERATION, &CDlgTypeEditor::OnEnChangeEdit)
	ON_NOTIFY(NM_CLICK, IDC_LIST_TYPES, &CDlgTypeEditor::OnNMClickListTypes)
	ON_BN_CLICKED(ID_NEW, &CDlgTypeEditor::OnBnClickedNew)
	ON_BN_CLICKED(ID_DEL, &CDlgTypeEditor::OnBnClickedDel)
	
END_MESSAGE_MAP()


// CDlgTypeEditor ��Ϣ�������

BOOL CDlgTypeEditor::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_tabMain.InsertItem(0,"���ȼ������");
	m_tabMain.InsertItem(1,"�������");
	m_tabMain.InsertItem(2,"�������");
	m_tabMain.InsertItem(3,"���ö��巽ʽ");
	m_tabMain.InsertItem(4,"����ʹ�÷�ʽ");
	m_tabMain.InsertItem(5,"��������");
	m_tabMain.InsertItem(6,"��ʼ������");
	m_tabMain.InsertItem(7,"���Ʋ���");
	m_tabMain.InsertItem(8,"Get��������");
	m_tabMain.InsertItem(9,"Get����ʵ��");
	m_tabMain.InsertItem(10,"Set��������");
	m_tabMain.InsertItem(11,"Set����ʵ��");


	m_tabMain.SetCurSel(0);
	m_lvTypeList.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);

	m_lvTypeList.InsertColumn(0,"����",LVCFMT_LEFT,80);
	m_lvTypeList.InsertColumn(1,"C����",LVCFMT_LEFT,80);

	FillList();


	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CDlgTypeEditor::OnTcnSelchangingTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	POSITION Pos=m_lvTypeList.GetFirstSelectedItemPosition();
	if(Pos)
	{
		int Item=m_lvTypeList.GetNextSelectedItem(Pos);
		FetchItemData(Item);
	}
}

void CDlgTypeEditor::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	POSITION Pos=m_lvTypeList.GetFirstSelectedItemPosition();
	if(Pos)
	{
		int Item=m_lvTypeList.GetNextSelectedItem(Pos);
		ShowItemData(Item);
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
		if(m_CurSelectItem>=0)
			FetchItemData(m_CurSelectItem);
		m_CurSelectItem=NewSelect;			
		ShowItemData(m_CurSelectItem);
		
	}
	
	*pResult = 0;
}

void CDlgTypeEditor::ShowItemData(int Item)
{
	if(Item<0)
	{
		m_TypeName.Empty();
		m_CType.Empty();
		m_edOperation.SetWindowText("");		
		UpdateData(false);
		return;
	}
	UINT Index=m_lvTypeList.GetItemData(Item);
	if(Index<m_TypeList.size())
	{
		m_TypeName=m_TypeList[Index].Name;
		m_CType=m_TypeList[Index].CType;

		switch(m_tabMain.GetCurSel())
		{
		case 0:
			m_edOperation.SetWindowText(m_TypeList[Index].GenerateOperations.SizeCaculateOperation);
			break;
		case 1:
			m_edOperation.SetWindowText(m_TypeList[Index].GenerateOperations.PackOperation);
			break;
		case 2:
			m_edOperation.SetWindowText(m_TypeList[Index].GenerateOperations.UnpackOperation);
			break;	
		case 3:
			m_edOperation.SetWindowText(m_TypeList[Index].GenerateOperations.ReferenceDefine);
			break;
		case 4:
			m_edOperation.SetWindowText(m_TypeList[Index].GenerateOperations.ReferenceUse);
			break;
		case 5:
			m_edOperation.SetWindowText(m_TypeList[Index].GenerateOperations.VariableDefine);
			break;
		case 6:
			m_edOperation.SetWindowText(m_TypeList[Index].GenerateOperations.InitOperation);
			break;
		case 7:
			m_edOperation.SetWindowText(m_TypeList[Index].GenerateOperations.CloneOperation);
			break;
		case 8:
			m_edOperation.SetWindowText(m_TypeList[Index].GenerateOperations.GetMethodDeclare);
			break;
		case 9:
			m_edOperation.SetWindowText(m_TypeList[Index].GenerateOperations.GetMethodDefine);
			break;
		case 10:
			m_edOperation.SetWindowText(m_TypeList[Index].GenerateOperations.SetMethodDeclare);
			break;
		case 11:
			m_edOperation.SetWindowText(m_TypeList[Index].GenerateOperations.SetMethodDefine);
			break;
		}
		
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
		switch(m_tabMain.GetCurSel())
		{
		case 0:
			m_edOperation.GetWindowText(m_TypeList[Index].GenerateOperations.SizeCaculateOperation);
			break;
		case 1:
			m_edOperation.GetWindowText(m_TypeList[Index].GenerateOperations.PackOperation);
			break;
		case 2:
			m_edOperation.GetWindowText(m_TypeList[Index].GenerateOperations.UnpackOperation);
			break;	
		case 3:
			m_edOperation.GetWindowText(m_TypeList[Index].GenerateOperations.ReferenceDefine);
			break;
		case 4:
			m_edOperation.GetWindowText(m_TypeList[Index].GenerateOperations.ReferenceUse);
			break;
		case 5:
			m_edOperation.GetWindowText(m_TypeList[Index].GenerateOperations.VariableDefine);
			break;
		case 6:
			m_edOperation.GetWindowText(m_TypeList[Index].GenerateOperations.InitOperation);
			break;
		case 7:
			m_edOperation.GetWindowText(m_TypeList[Index].GenerateOperations.CloneOperation);
			break;
		case 8:
			m_edOperation.GetWindowText(m_TypeList[Index].GenerateOperations.GetMethodDeclare);
			break;
		case 9:
			m_edOperation.GetWindowText(m_TypeList[Index].GenerateOperations.GetMethodDefine);
			break;
		case 10:
			m_edOperation.GetWindowText(m_TypeList[Index].GenerateOperations.SetMethodDeclare);
			break;
		case 11:
			m_edOperation.GetWindowText(m_TypeList[Index].GenerateOperations.SetMethodDefine);
			break;
		}
		m_lvTypeList.SetItemText(Item,0,m_TypeList[Index].Name);
		m_lvTypeList.SetItemText(Item,1,m_TypeList[Index].CType);
	}
	
}
void CDlgTypeEditor::OnBnClickedNew()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	TYPE_DEFINE NewType;
	NewType.Name="������";
	NewType.CType="char";
	m_TypeList.push_back(NewType);
	UINT Index=m_TypeList.size()-1;
	m_IsModified=true;
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
			m_IsModified=true;
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
void CDlgTypeEditor::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���
	if(m_IsModified)
	{
		if(AfxMessageBox("�Ƿ�Ҫ�����������ĸ��ģ�",MB_YESNO)==IDYES)
		{
			CDialog::OnCancel();
		}
	}
	else
	{
		CDialog::OnCancel();
	}
}

void CDlgTypeEditor::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���
	if(m_IsModified)
		FetchItemData(m_CurSelectItem);
	CDialog::OnOK();
}


