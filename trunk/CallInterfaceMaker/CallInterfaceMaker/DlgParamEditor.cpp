/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DlgParamEditor.cpp                                       */
/*      ��������:  2011��10��07��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
// DlgParamEditor.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CallInterfaceMaker.h"
#include "DlgParamEditor.h"


// CDlgParamEditor �Ի���

IMPLEMENT_DYNAMIC(CDlgParamEditor, CDialog)

CDlgParamEditor::CDlgParamEditor(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgParamEditor::IDD, pParent)
	, m_Name(_T(""))
	, m_Type(_T(""))
	, m_Length(0)
	, m_Description(_T(""))
	, m_DefaultValue(_T(""))
	, m_CanNull(FALSE)
	, m_IsReference(FALSE)
	, m_ID(0)
{

}

CDlgParamEditor::~CDlgParamEditor()
{
}

void CDlgParamEditor::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_NAME, m_Name);
	DDX_Control(pDX, IDC_COMBO_TYPE, m_cbType);
	DDX_Text(pDX, IDC_EDIT_LENGTH, m_Length);
	DDX_Text(pDX, IDC_EDIT_DESCRIPTION, m_Description);
	DDX_Text(pDX, IDC_EDIT_DEFAULT_VALUE, m_DefaultValue);
	DDX_Check(pDX, IDC_CHECK_CAN_NULL, m_CanNull);
	DDX_Check(pDX, IDC_CHECK_IS_REFERENCE, m_IsReference);
	DDX_Text(pDX, IDC_EDIT_PARAM_ID, m_ID);
}


BEGIN_MESSAGE_MAP(CDlgParamEditor, CDialog)
END_MESSAGE_MAP()


// CDlgParamEditor ��Ϣ�������

BOOL CDlgParamEditor::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	int Sel=-1;
	for(UINT i=0;i<GetMainDlg()->GetVarTypeCount();i++)
	{
		m_cbType.InsertString(i,GetMainDlg()->GetVarType(i)->Name);
		if(m_Type==GetMainDlg()->GetVarType(i)->Name)
			Sel=i;
	}
	m_cbType.SetCurSel(Sel);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CDlgParamEditor::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���
	UpdateData(true);
	m_cbType.GetWindowText(m_Type);
	m_Name.Trim();
	m_DefaultValue.Trim();
	m_Description.Trim();

	CDialog::OnOK();
}

void CDlgParamEditor::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���

	CDialog::OnCancel();
}
