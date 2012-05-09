/****************************************************************************/
/*                                                                          */
/*      文件名:    DlgScrollBarTextureConfig.cpp                            */
/*      创建日期:  2011年10月07日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
// DlgScrollBarTextureConfig.cpp : 实现文件
//

#include "stdafx.h"
#include "D3DUIEdit.h"
#include "DlgScrollBarTextureConfig.h"
#include "DlgTextureConfig.h"
#include "DlgButtonTextureConfig.h"


// CDlgScrollBarTextureConfig 对话框

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


// CDlgScrollBarTextureConfig 消息处理程序

void CDlgScrollBarTextureConfig::OnBnClickedButtonSelectTexture()
{
	// TODO: 在此添加控件通知处理程序代码
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
	// TODO: 在此添加控件通知处理程序代码
	if(m_pGUIObject)
	{
		CDlgButtonTextureConfig dlg;
		dlg.SetGUIObject(((CD3DScrollBar *)m_pGUIObject)->GetDecButton());
		dlg.DoModal();
	}
}

void CDlgScrollBarTextureConfig::OnBnClickedButtonInc()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_pGUIObject)
	{
		CDlgButtonTextureConfig dlg;
		dlg.SetGUIObject(((CD3DScrollBar *)m_pGUIObject)->GetIncButton());
		dlg.DoModal();
	}
}

void CDlgScrollBarTextureConfig::OnBnClickedButtonTrack()
{
	// TODO: 在此添加控件通知处理程序代码
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

	// TODO:  在此添加额外的初始化
	
	if(m_pGUIObject)
	{
		CEasyString Temp;
		m_pGUIObject->GetTexture(Temp,m_TextureRect);	
		m_TextureFileName=Temp;
		m_pGUIObject->GetBorders(m_WndBorders);
	}
	UpdateData(false);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgScrollBarTextureConfig::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	OnBnClickedApply();
	CPropertyConfigDialog::OnOK();
}

void CDlgScrollBarTextureConfig::OnBnClickedApply()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	if(m_pGUIObject)
	{
		m_pGUIObject->SetTexture(m_TextureFileName,m_TextureRect);	
		m_pGUIObject->SetBorders(m_WndBorders);
	}
}
