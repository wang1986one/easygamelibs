/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DlgSimpleListTextureConfig.cpp                           */
/*      ��������:  2011��10��07��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
// DlgSimpleListTextureConfig.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "D3DUIEdit.h"
#include "DlgSimpleListTextureConfig.h"
#include "DlgScrollBarTextureConfig.h"
#include "DlgTextureConfig.h"

// CDlgSimpleListTextureConfig �Ի���

IMPLEMENT_DYNAMIC(CDlgSimpleListTextureConfig, CPropertyConfigDialog)
CDlgSimpleListTextureConfig::CDlgSimpleListTextureConfig(CWnd* pParent /*=NULL*/)
	: CPropertyConfigDialog(CDlgSimpleListTextureConfig::IDD, pParent)
{
}

CDlgSimpleListTextureConfig::~CDlgSimpleListTextureConfig()
{
}

void CDlgSimpleListTextureConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgSimpleListTextureConfig, CPropertyConfigDialog)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
END_MESSAGE_MAP()


// CDlgSimpleListTextureConfig ��Ϣ�������

void CDlgSimpleListTextureConfig::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(m_pGUIObject)
	{
		CDlgTextureConfig dlg;

		dlg.SetGUIObject(m_pGUIObject);		
		dlg.DoModal();
	}
}

void CDlgSimpleListTextureConfig::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(m_pGUIObject)
	{
		CDlgScrollBarTextureConfig dlg;

		dlg.SetGUIObject(m_pGUIObject->GetScrollBar());		
		dlg.DoModal();
	}
}
