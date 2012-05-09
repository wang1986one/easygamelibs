/****************************************************************************/
/*                                                                          */
/*      文件名:    DlgEditTextureConfig.cpp                                 */
/*      创建日期:  2011年10月07日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
// DlgEditTextureConfig.cpp : 实现文件
//

#include "stdafx.h"
#include "D3DUIEdit.h"
#include "DlgEditTextureConfig.h"
#include "DlgScrollBarTextureConfig.h"
#include "DlgTextureConfig.h"

// CDlgEditTextureConfig 对话框

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


// CDlgEditTextureConfig 消息处理程序

void CDlgEditTextureConfig::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_pGUIObject)
	{
		CDlgTextureConfig dlg;

		dlg.SetGUIObject(m_pGUIObject);		
		dlg.DoModal();
	}
}

void CDlgEditTextureConfig::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_pGUIObject)
	{
		CDlgScrollBarTextureConfig dlg;

		dlg.SetGUIObject(m_pGUIObject->GetScrollBar());		
		dlg.DoModal();
	}
}
