/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DlgFXEditor.cpp                                          */
/*      ��������:  2011��10��07��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DlgFXEditor.cpp                                          */
/*      ��������:  2009��09��11��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
// DlgFXEditor.cpp : ʵ���ļ�
//

#include "stdafx.h"



// CDlgFXEditor �Ի���

IMPLEMENT_DYNAMIC(CDlgFXEditor, CPropertyConfigDialog)
CDlgFXEditor::CDlgFXEditor(CWnd* pParent /*=NULL*/)
	: CPropertyConfigDialog(CDlgFXEditor::IDD, pParent)
{
}

CDlgFXEditor::~CDlgFXEditor()
{
}

void CDlgFXEditor::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_FX, m_FxContent);
}


BEGIN_MESSAGE_MAP(CDlgFXEditor, CDialog)
END_MESSAGE_MAP()


// CDlgFXEditor ��Ϣ�������

BOOL CDlgFXEditor::OnInitDialog()
{
	CPropertyConfigDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	UpdateData(false);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CDlgFXEditor::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���
	UpdateData(true);
	CPropertyConfigDialog::OnOK();
}
