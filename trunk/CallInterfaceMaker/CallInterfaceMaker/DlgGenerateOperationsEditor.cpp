// DlgGenerateOperationsEditor.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DlgGenerateOperationsEditor.h"




// CDlgGenerateOperationsEditor �Ի���

IMPLEMENT_DYNAMIC(CDlgGenerateOperationsEditor, CDialog)

CDlgGenerateOperationsEditor::CDlgGenerateOperationsEditor(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgGenerateOperationsEditor::IDD, pParent)
{
	m_IsModified=false;
	m_ShowIndexOperation=false;
}

CDlgGenerateOperationsEditor::~CDlgGenerateOperationsEditor()
{
}

void CDlgGenerateOperationsEditor::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_EDIT_OPERATION, m_edOperation);
	DDX_Control(pDX, IDC_TAB, m_tbOperations);
}


BEGIN_MESSAGE_MAP(CDlgGenerateOperationsEditor, CDialog)
	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB, &CDlgGenerateOperationsEditor::OnTcnSelchangingTab)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, &CDlgGenerateOperationsEditor::OnTcnSelchangeTab)	
	ON_EN_CHANGE(IDC_EDIT_OPERATION, &CDlgGenerateOperationsEditor::OnEnChangeEdit)
END_MESSAGE_MAP()


// CDlgGenerateOperationsEditor ��Ϣ�������

BOOL CDlgGenerateOperationsEditor::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	m_tbOperations.InsertItem(0,"���ȼ������");
	m_tbOperations.InsertItem(1,"�������");
	m_tbOperations.InsertItem(2,"�������");
	m_tbOperations.InsertItem(3,"���ö��巽ʽ");
	m_tbOperations.InsertItem(4,"����ʹ�÷�ʽ");
	m_tbOperations.InsertItem(5,"��������");
	m_tbOperations.InsertItem(6,"��ʼ������");
	m_tbOperations.InsertItem(7,"���Ʋ���");
	m_tbOperations.InsertItem(8,"Get��������");
	m_tbOperations.InsertItem(9,"Get����ʵ��");
	m_tbOperations.InsertItem(10,"Set��������");
	m_tbOperations.InsertItem(11,"Set����ʵ��");
	if(m_ShowIndexOperation)
	{
		m_tbOperations.InsertItem(12,"��������");
		m_tbOperations.InsertItem(13,"ֻ����������");
	}

	m_tbOperations.SetCurSel(0);

	ShowItemData();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CDlgGenerateOperationsEditor::OnCancel()
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

void CDlgGenerateOperationsEditor::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���

	if(m_IsModified)
		FetchItemData();

	CDialog::OnOK();
}

void CDlgGenerateOperationsEditor::OnTcnSelchangingTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	FetchItemData();
}
void CDlgGenerateOperationsEditor::OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	ShowItemData();
}

void CDlgGenerateOperationsEditor::OnEnChangeEdit()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	m_IsModified=true;
}

void CDlgGenerateOperationsEditor::ShowItemData()
{
	switch(m_tbOperations.GetCurSel())
	{
	case 0:
		m_edOperation.SetWindowText(m_GenerateOperations.SizeCaculateOperation);
		break;
	case 1:
		m_edOperation.SetWindowText(m_GenerateOperations.PackOperation);
		break;
	case 2:
		m_edOperation.SetWindowText(m_GenerateOperations.UnpackOperation);
		break;	
	case 3:
		m_edOperation.SetWindowText(m_GenerateOperations.ReferenceDefine);
		break;
	case 4:
		m_edOperation.SetWindowText(m_GenerateOperations.ReferenceUse);
		break;
	case 5:
		m_edOperation.SetWindowText(m_GenerateOperations.VariableDefine);
		break;
	case 6:
		m_edOperation.SetWindowText(m_GenerateOperations.InitOperation);
		break;
	case 7:
		m_edOperation.SetWindowText(m_GenerateOperations.CloneOperation);
		break;
	case 8:
		m_edOperation.SetWindowText(m_GenerateOperations.GetMethodDeclare);
		break;
	case 9:
		m_edOperation.SetWindowText(m_GenerateOperations.GetMethodDefine);
		break;
	case 10:
		m_edOperation.SetWindowText(m_GenerateOperations.SetMethodDeclare);
		break;
	case 11:
		m_edOperation.SetWindowText(m_GenerateOperations.SetMethodDefine);
		break;
	case 12:
		m_edOperation.SetWindowText(m_GenerateOperations.IndexOperation);
		break;
	case 13:
		m_edOperation.SetWindowText(m_GenerateOperations.ConstIndexOperation);
		break;
	}
	
	UpdateData(false);
}
void CDlgGenerateOperationsEditor::FetchItemData()
{	
	UpdateData(true);
	
	switch(m_tbOperations.GetCurSel())
	{
	case 0:
		m_edOperation.GetWindowText(m_GenerateOperations.SizeCaculateOperation);
		break;
	case 1:
		m_edOperation.GetWindowText(m_GenerateOperations.PackOperation);
		break;
	case 2:
		m_edOperation.GetWindowText(m_GenerateOperations.UnpackOperation);
		break;	
	case 3:
		m_edOperation.GetWindowText(m_GenerateOperations.ReferenceDefine);
		break;
	case 4:
		m_edOperation.GetWindowText(m_GenerateOperations.ReferenceUse);
		break;
	case 5:
		m_edOperation.GetWindowText(m_GenerateOperations.VariableDefine);
		break;
	case 6:
		m_edOperation.GetWindowText(m_GenerateOperations.InitOperation);
		break;
	case 7:
		m_edOperation.GetWindowText(m_GenerateOperations.CloneOperation);
		break;
	case 8:
		m_edOperation.GetWindowText(m_GenerateOperations.GetMethodDeclare);
		break;
	case 9:
		m_edOperation.GetWindowText(m_GenerateOperations.GetMethodDefine);
		break;
	case 10:
		m_edOperation.GetWindowText(m_GenerateOperations.SetMethodDeclare);
		break;
	case 11:
		m_edOperation.GetWindowText(m_GenerateOperations.SetMethodDefine);
		break;
	case 12:
		m_edOperation.GetWindowText(m_GenerateOperations.IndexOperation);
		break;
	case 13:
		m_edOperation.GetWindowText(m_GenerateOperations.ConstIndexOperation);
		break;
	}
}

