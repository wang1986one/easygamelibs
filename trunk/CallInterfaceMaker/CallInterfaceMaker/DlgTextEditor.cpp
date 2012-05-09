/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DlgTextEditor.cpp                                        */
/*      ��������:  2011��10��07��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
// DlgTextEditor.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CallInterfaceMaker.h"
#include "DlgTextEditor.h"


// CDlgTextEditor �Ի���

IMPLEMENT_DYNAMIC(CDlgTextEditor, CDialog)

CDlgTextEditor::CDlgTextEditor(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTextEditor::IDD, pParent)
	, m_Text(_T(""))
{

}

CDlgTextEditor::~CDlgTextEditor()
{
}

void CDlgTextEditor::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_Text);
}


BEGIN_MESSAGE_MAP(CDlgTextEditor, CDialog)
END_MESSAGE_MAP()


// CDlgTextEditor ��Ϣ�������

BOOL CDlgTextEditor::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	UpdateData(false);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CDlgTextEditor::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���
	UpdateData(true);

	CDialog::OnOK();
}

void CDlgTextEditor::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���

	CDialog::OnCancel();
}
