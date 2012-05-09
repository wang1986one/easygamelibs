/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DlgColorConfig.cpp                                       */
/*      ��������:  2011��10��07��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
// DlgColorConfig.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "D3DUIEdit.h"
#include "DlgColorConfig.h"
#include ".\dlgcolorconfig.h"


// CDlgColorConfig �Ի���

IMPLEMENT_DYNAMIC(CDlgColorConfig, CPropertyConfigDialog)
CDlgColorConfig::CDlgColorConfig(CWnd* pParent /*=NULL*/)
	: CPropertyConfigDialog(CDlgColorConfig::IDD, pParent)
	, m_ColorRed(0)
	, m_ColorGreen(0)
	, m_ColorBlue(0)
	, m_ColorAlpha(0)
{
}

CDlgColorConfig::~CDlgColorConfig()
{
}

void CDlgColorConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_ColorRed);
	DDV_MinMaxInt(pDX, m_ColorRed, 0, 255);
	DDX_Text(pDX, IDC_EDIT15, m_ColorGreen);
	DDV_MinMaxInt(pDX, m_ColorGreen, 0, 255);
	DDX_Text(pDX, IDC_EDIT2, m_ColorBlue);
	DDV_MinMaxInt(pDX, m_ColorBlue, 0, 255);
	DDX_Text(pDX, IDC_EDIT17, m_ColorAlpha);
	DDV_MinMaxInt(pDX, m_ColorAlpha, 0, 255);
	
}


BEGIN_MESSAGE_MAP(CDlgColorConfig, CPropertyConfigDialog)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_COLOR, OnBnClickedButtonSelectColor)
END_MESSAGE_MAP()


// CDlgColorConfig ��Ϣ�������

void CDlgColorConfig::OnBnClickedButtonSelectColor()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CColorDialog dlg(RGB(m_ColorRed,m_ColorGreen,m_ColorBlue));

	if(dlg.DoModal()==IDOK)
	{
		UpdateData(true);
		m_ColorRed=GetRValue(dlg.GetColor());
		m_ColorGreen=GetGValue(dlg.GetColor());
		m_ColorBlue=GetBValue(dlg.GetColor());
		UpdateData(false);
	}
}

BOOL CDlgColorConfig::OnInitDialog()
{
	CPropertyConfigDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_ColorAlpha=(m_D3DColor>>24)&0xff;
	m_ColorRed=(m_D3DColor>>16)&0xff;
	m_ColorGreen=(m_D3DColor>>8)&0xff;
	m_ColorBlue=(m_D3DColor)&0xff;

	UpdateData(false);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CDlgColorConfig::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���
	UpdateData(true);
	m_D3DColor=D3DCOLOR_RGBA(m_ColorRed,m_ColorGreen,m_ColorBlue,m_ColorAlpha);
	CPropertyConfigDialog::OnOK();
}
