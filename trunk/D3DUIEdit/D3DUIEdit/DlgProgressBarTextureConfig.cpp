/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DlgProgressBarTextureConfig.cpp                          */
/*      ��������:  2011��10��07��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
// DlgProgressBarTextureConfig.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "D3DUIEdit.h"
#include "DlgProgressBarTextureConfig.h"
#include ".\dlgprogressbartextureconfig.h"

#include "DlgTextureConfig.h"


// CDlgProgressBarTextureConfig �Ի���

IMPLEMENT_DYNAMIC(CDlgProgressBarTextureConfig, CPropertyConfigDialog)
CDlgProgressBarTextureConfig::CDlgProgressBarTextureConfig(CWnd* pParent /*=NULL*/)
	: CPropertyConfigDialog(CDlgProgressBarTextureConfig::IDD, pParent)
{
}

CDlgProgressBarTextureConfig::~CDlgProgressBarTextureConfig()
{
}

void CDlgProgressBarTextureConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgProgressBarTextureConfig, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_TEXTURE1, OnBnClickedButtonTexture1)
	ON_BN_CLICKED(IDC_BUTTON_TEXTURE2, OnBnClickedButtonTexture2)
END_MESSAGE_MAP()


// CDlgProgressBarTextureConfig ��Ϣ�������

void CDlgProgressBarTextureConfig::OnBnClickedButtonTexture1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(m_pGUIObject)
	{
		CDlgTextureConfig dlg;
		dlg.SetGUIObject(m_pGUIObject);
		dlg.DoModal();
	}
}

void CDlgProgressBarTextureConfig::OnBnClickedButtonTexture2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(m_pGUIObject)
	{
		CDlgTextureConfig dlg;
		dlg.SetGUIObject(((CD3DProgressBar *)m_pGUIObject)->GetBarWnd());
		dlg.DoModal();
	}
}
