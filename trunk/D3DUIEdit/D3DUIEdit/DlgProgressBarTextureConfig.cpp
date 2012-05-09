/****************************************************************************/
/*                                                                          */
/*      文件名:    DlgProgressBarTextureConfig.cpp                          */
/*      创建日期:  2011年10月07日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
// DlgProgressBarTextureConfig.cpp : 实现文件
//

#include "stdafx.h"
#include "D3DUIEdit.h"
#include "DlgProgressBarTextureConfig.h"
#include ".\dlgprogressbartextureconfig.h"

#include "DlgTextureConfig.h"


// CDlgProgressBarTextureConfig 对话框

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


// CDlgProgressBarTextureConfig 消息处理程序

void CDlgProgressBarTextureConfig::OnBnClickedButtonTexture1()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_pGUIObject)
	{
		CDlgTextureConfig dlg;
		dlg.SetGUIObject(m_pGUIObject);
		dlg.DoModal();
	}
}

void CDlgProgressBarTextureConfig::OnBnClickedButtonTexture2()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_pGUIObject)
	{
		CDlgTextureConfig dlg;
		dlg.SetGUIObject(((CD3DProgressBar *)m_pGUIObject)->GetBarWnd());
		dlg.DoModal();
	}
}
