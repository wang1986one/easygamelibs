/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DlgEditBox.cpp                                           */
/*      ��������:  2010��02��09��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
// DlgEditBox.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DlgEditBox.h"




// CDlgEditBox �Ի���

IMPLEMENT_DYNAMIC(CDlgEditBox, CDialog)

CDlgEditBox::CDlgEditBox(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgEditBox::IDD, pParent)
	, m_Text(_T(""))
{

}

CDlgEditBox::~CDlgEditBox()
{
}

void CDlgEditBox::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_TEXT, m_Text);
}


BEGIN_MESSAGE_MAP(CDlgEditBox, CDialog)
END_MESSAGE_MAP()


// CDlgEditBox ��Ϣ�������

BOOL CDlgEditBox::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
