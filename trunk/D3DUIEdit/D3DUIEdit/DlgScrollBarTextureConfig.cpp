/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DlgScrollBarTextureConfig.cpp                            */
/*      ��������:  2011��10��07��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
// DlgScrollBarTextureConfig.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "D3DUIEdit.h"
#include "DlgScrollBarTextureConfig.h"
#include "DlgTextureConfig.h"
#include "DlgButtonTextureConfig.h"


// CDlgScrollBarTextureConfig �Ի���

IMPLEMENT_DYNAMIC(CDlgScrollBarTextureConfig, CPropertyConfigDialog)
CDlgScrollBarTextureConfig::CDlgScrollBarTextureConfig(CWnd* pParent /*=NULL*/)
	: CPropertyConfigDialog(CDlgScrollBarTextureConfig::IDD, pParent)
	, m_TextureFileName(_T(""))
{
}

CDlgScrollBarTextureConfig::~CDlgScrollBarTextureConfig()
{
}

void CDlgScrollBarTextureConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_TextureFileName);
}


BEGIN_MESSAGE_MAP(CDlgScrollBarTextureConfig, CPropertyConfigDialog)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_TEXTURE, OnBnClickedButtonSelectTexture)
	ON_BN_CLICKED(IDC_BUTTON_DEC, OnBnClickedButtonDec)
	ON_BN_CLICKED(IDC_BUTTON_INC, OnBnClickedButtonInc)
	ON_BN_CLICKED(IDC_BUTTON_TRACK, OnBnClickedButtonTrack)
	ON_BN_CLICKED(IDAPPLY, OnBnClickedApply)
END_MESSAGE_MAP()


// CDlgScrollBarTextureConfig ��Ϣ�������

void CDlgScrollBarTextureConfig::OnBnClickedButtonSelectTexture()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDlgTextureConfig dlg;

	dlg.m_TextureFileName=m_TextureFileName;
	dlg.m_TextureRect=m_TextureRect;
	memcpy(dlg.m_WndBorders,m_WndBorders,sizeof(WIN_BORDERS));
	if(dlg.DoModal()==IDOK)
	{
		m_TextureFileName=dlg.m_TextureFileName;
		m_TextureRect=dlg.m_TextureRect;
		memcpy(m_WndBorders,dlg.m_WndBorders,sizeof(WIN_BORDERS));
	}
}

void CDlgScrollBarTextureConfig::OnBnClickedButtonDec()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(m_pGUIObject)
	{
		CDlgButtonTextureConfig dlg;
		dlg.SetGUIObject(((CD3DScrollBar *)m_pGUIObject)->GetDecButton());
		dlg.DoModal();
	}
}

void CDlgScrollBarTextureConfig::OnBnClickedButtonInc()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(m_pGUIObject)
	{
		CDlgButtonTextureConfig dlg;
		dlg.SetGUIObject(((CD3DScrollBar *)m_pGUIObject)->GetIncButton());
		dlg.DoModal();
	}
}

void CDlgScrollBarTextureConfig::OnBnClickedButtonTrack()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(m_pGUIObject)
	{
		CDlgButtonTextureConfig dlg;
		dlg.SetGUIObject(((CD3DScrollBar *)m_pGUIObject)->GetTrackButton());
		dlg.DoModal();
	}
}

BOOL CDlgScrollBarTextureConfig::OnInitDialog()
{
	CPropertyConfigDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	
	if(m_pGUIObject)
	{
		CEasyString Temp;
		m_pGUIObject->GetTexture(Temp,m_TextureRect);	
		m_TextureFileName=Temp;
		m_pGUIObject->GetBorders(m_WndBorders);
	}
	UpdateData(false);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CDlgScrollBarTextureConfig::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���
	OnBnClickedApply();
	CPropertyConfigDialog::OnOK();
}

void CDlgScrollBarTextureConfig::OnBnClickedApply()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	if(m_pGUIObject)
	{
		m_pGUIObject->SetTexture(m_TextureFileName,m_TextureRect);	
		m_pGUIObject->SetBorders(m_WndBorders);
	}
}
