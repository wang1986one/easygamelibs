/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DlgEditTextureConfig.cpp                                 */
/*      ��������:  2011��10��07��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
// DlgEditTextureConfig.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "D3DUIEdit.h"
#include "DlgEditTextureConfig.h"
#include "DlgScrollBarTextureConfig.h"
#include "DlgTextureConfig.h"

// CDlgEditTextureConfig �Ի���

IMPLEMENT_DYNAMIC(CDlgEditTextureConfig, CPropertyConfigDialog)
CDlgEditTextureConfig::CDlgEditTextureConfig(CWnd* pParent /*=NULL*/)
	: CPropertyConfigDialog(CDlgEditTextureConfig::IDD, pParent)
{
}

CDlgEditTextureConfig::~CDlgEditTextureConfig()
{
}

void CDlgEditTextureConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgEditTextureConfig, CPropertyConfigDialog)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
END_MESSAGE_MAP()


// CDlgEditTextureConfig ��Ϣ�������

void CDlgEditTextureConfig::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(m_pGUIObject)
	{
		CDlgTextureConfig dlg;

		dlg.SetGUIObject(m_pGUIObject);		
		dlg.DoModal();
	}
}

void CDlgEditTextureConfig::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(m_pGUIObject)
	{
		CDlgScrollBarTextureConfig dlg;

		dlg.SetGUIObject(m_pGUIObject->GetScrollBar());		
		dlg.DoModal();
	}
}
