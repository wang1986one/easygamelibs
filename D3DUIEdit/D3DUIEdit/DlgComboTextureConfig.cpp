/****************************************************************************/
/*                                                                          */
/*      文件名:    DlgComboTextureConfig.cpp                                */
/*      创建日期:  2011年10月07日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
// DlgComboTextureConfig.cpp : 实现文件
//

#include "stdafx.h"
#include "D3DUIEdit.h"
#include "DlgComboTextureConfig.h"
#include "DlgTextureConfig.h"
#include "DlgSimpleListTextureConfig.h"
#include "DlgButtonTextureConfig.h"
// CDlgComboTextureConfig 对话框

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


// CDlgComboTextureConfig 消息处理程序

void CDlgComboTextureConfig::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_pGUIObject)
	{
		CDlgTextureConfig dlg;

		dlg.SetGUIObject(m_pGUIObject);		
		dlg.DoModal();
	}
}

void CDlgComboTextureConfig::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_pGUIObject)
	{
		CDlgButtonTextureConfig dlg;

		dlg.SetGUIObject(m_pGUIObject->GetExpandButton());		
		dlg.DoModal();
	}
}

void CDlgComboTextureConfig::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_pGUIObject)
	{
		CDlgSimpleListTextureConfig dlg;

		dlg.SetGUIObject(m_pGUIObject->GetComboList());		
		dlg.DoModal();
	}
}
