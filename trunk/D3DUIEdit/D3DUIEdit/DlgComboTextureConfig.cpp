/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DlgComboTextureConfig.cpp                                */
/*      ��������:  2011��10��07��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
// DlgComboTextureConfig.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "D3DUIEdit.h"
#include "DlgComboTextureConfig.h"
#include "DlgTextureConfig.h"
#include "DlgSimpleListTextureConfig.h"
#include "DlgButtonTextureConfig.h"
// CDlgComboTextureConfig �Ի���

IMPLEMENT_DYNAMIC(CDlgComboTextureConfig, CPropertyConfigDialog)
CDlgComboTextureConfig::CDlgComboTextureConfig(CWnd* pParent /*=NULL*/)
	: CPropertyConfigDialog(CDlgComboTextureConfig::IDD, pParent)
{
}

CDlgComboTextureConfig::~CDlgComboTextureConfig()
{
}

void CDlgComboTextureConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgComboTextureConfig, CPropertyConfigDialog)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnBnClickedButton3)
END_MESSAGE_MAP()


// CDlgComboTextureConfig ��Ϣ�������

void CDlgComboTextureConfig::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(m_pGUIObject)
	{
		CDlgTextureConfig dlg;

		dlg.SetGUIObject(m_pGUIObject);		
		dlg.DoModal();
	}
}

void CDlgComboTextureConfig::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(m_pGUIObject)
	{
		CDlgButtonTextureConfig dlg;

		dlg.SetGUIObject(m_pGUIObject->GetExpandButton());		
		dlg.DoModal();
	}
}

void CDlgComboTextureConfig::OnBnClickedButton3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(m_pGUIObject)
	{
		CDlgSimpleListTextureConfig dlg;

		dlg.SetGUIObject(m_pGUIObject->GetComboList());		
		dlg.DoModal();
	}
}
