/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DlgWOWM2Import.cpp                                       */
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
/*      �ļ���:    DlgWOWM2Import.cpp                                       */
/*      ��������:  2010��02��09��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
// DlgWOWCharImport.cpp : ʵ���ļ�
//

#include "stdafx.h"



// CDlgWOWM2Import �Ի���

IMPLEMENT_DYNAMIC(CDlgWOWM2Import, CDialog)

CDlgWOWM2Import::CDlgWOWM2Import(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgWOWM2Import::IDD, pParent)
	, m_ModelFileName(_T(""))
	, m_SkinFileName(_T(""))		
{
}

CDlgWOWM2Import::~CDlgWOWM2Import()
{
}

void CDlgWOWM2Import::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_ModelFileName);
	DDX_Text(pDX, IDC_EDIT6, m_SkinFileName);
	
}


BEGIN_MESSAGE_MAP(CDlgWOWM2Import, CDialog)
	ON_BN_CLICKED(IDC_SELECT_MODEL_FILE, &CDlgWOWM2Import::OnBnClickedSelectModelFile)
	ON_BN_CLICKED(IDC_SELECT_SKIN_FILE, &CDlgWOWM2Import::OnBnClickedSelectSkinFile)
	ON_BN_CLICKED(IDOK, &CDlgWOWM2Import::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgWOWM2Import ��Ϣ�������

void CDlgWOWM2Import::OnBnClickedSelectModelFile()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog dlg(true,_T("*.m2"),_T("*.m2"),OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT|OFN_NOCHANGEDIR,
		_T("Blizzard M2 Model File(*.m2)|*.m2|All Files(*.*)|*.*||"));

	if(dlg.DoModal()==IDOK)
	{
		UpdateData(true);
		m_ModelFileName=dlg.GetPathName();
		int Pos=m_ModelFileName.ReverseFind('.');
		if(Pos>=0)
		{
			m_SkinFileName=m_ModelFileName.Left(Pos)+_T("00.skin");
		}
		UpdateData(false);
	}
}

void CDlgWOWM2Import::OnBnClickedSelectSkinFile()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog dlg(true,_T("*.skin"),_T("*.skin"),OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT|OFN_NOCHANGEDIR,
		_T("Blizzard M2 Model Skin File(*.skin)|*.skin|All Files(*.*)|*.*||"));

	if(dlg.DoModal()==IDOK)
	{
		UpdateData(true);
		m_SkinFileName=dlg.GetPathName();
		UpdateData(false);
	}
}

BOOL CDlgWOWM2Import::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��	

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CDlgWOWM2Import::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	OnOK();
}


