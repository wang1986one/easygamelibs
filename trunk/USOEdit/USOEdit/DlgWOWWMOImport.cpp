/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DlgWOWWMOImport.cpp                                      */
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
/*      �ļ���:    DlgWOWWMOImport.cpp                                      */
/*      ��������:  2010��02��09��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
// DlgWOWWMOImport.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DlgWOWWMOImport.h"




// CDlgWOWWMOImport �Ի���

IMPLEMENT_DYNAMIC(CDlgWOWWMOImport, CDialog)

CDlgWOWWMOImport::CDlgWOWWMOImport(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgWOWWMOImport::IDD, pParent)
	, m_ModelFileName(_T(""))	
{
}

CDlgWOWWMOImport::~CDlgWOWWMOImport()
{
}

void CDlgWOWWMOImport::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_MODEL_FILE_NAME, m_ModelFileName);
}


BEGIN_MESSAGE_MAP(CDlgWOWWMOImport, CDialog)
	ON_BN_CLICKED(IDC_SELECT_MODEL_FILE, &CDlgWOWWMOImport::OnBnClickedSelectModelFile)
	ON_BN_CLICKED(IDOK, &CDlgWOWWMOImport::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgWOWWMOImport ��Ϣ�������

void CDlgWOWWMOImport::OnBnClickedSelectModelFile()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog dlg(true,_T("*.wmo"),_T("*.wmo"),OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT|OFN_NOCHANGEDIR,
		_T("Blizzard WMO File(*.wmo)|*.wmo|All Files(*.*)|*.*||"));

	if(dlg.DoModal()==IDOK)
	{
		UpdateData(true);
		m_ModelFileName=dlg.GetPathName();		
		UpdateData(false);
	}
}

void CDlgWOWWMOImport::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	OnOK();
}

BOOL CDlgWOWWMOImport::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
