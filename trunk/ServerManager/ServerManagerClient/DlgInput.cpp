/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DlgInput.cpp                                             */
/*      ��������:  2010��02��09��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
// DlgInput.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DlgInput.h"




// CDlgInput �Ի���

IMPLEMENT_DYNAMIC(CDlgInput, CDialog)

CDlgInput::CDlgInput(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgInput::IDD, pParent)
	, m_InputText(_T(""))
{

}

CDlgInput::~CDlgInput()
{
}

void CDlgInput::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_InputText);
}


BEGIN_MESSAGE_MAP(CDlgInput, CDialog)
END_MESSAGE_MAP()


// CDlgInput ��Ϣ�������

BOOL CDlgInput::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	SetWindowText(m_Title);	

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
