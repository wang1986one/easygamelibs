/****************************************************************************/
/*                                                                          */
/*      文件名:    DlgButtonTextureConfig.cpp                               */
/*      创建日期:  2011年10月07日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
// DlgButtonTextureConfig.cpp : 实现文件
//

#include "stdafx.h"
#include "D3DUIEdit.h"
#include "DlgButtonTextureConfig.h"
#include ".\dlgbuttontextureconfig.h"
#include "DlgTextureConfig.h"

// CDlgButtonTextureConfig 对话框

IMPLEMENT_DYNAMIC(CDlgButtonTextureConfig, CPropertyConfigDialog)
CDlgButtonTextureConfig::CDlgButtonTextureConfig(CWnd* pParent /*=NULL*/)
	: CPropertyConfigDialog(CDlgButtonTextureConfig::IDD, pParent)
	, m_NormalTextureFileName(_T(""))
	, m_ActiveTextureFileName(_T(""))
	, m_PressTextureFileName(_T(""))
	, m_DisableTextureFileName(_T(""))
{
}

CDlgButtonTextureConfig::~CDlgButtonTextureConfig()
{
	m_pGUIObject=NULL;
}

void CDlgButtonTextureConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_NormalTextureFileName);
	DDX_Text(pDX, IDC_EDIT15, m_ActiveTextureFileName);
	DDX_Text(pDX, IDC_EDIT2, m_PressTextureFileName);
	DDX_Text(pDX, IDC_EDIT17, m_DisableTextureFileName);
	DDX_Text(pDX, IDC_EDIT3,m_NormalTextOffset.x);
	DDX_Text(pDX, IDC_EDIT4,m_NormalTextOffset.y);
	DDX_Text(pDX, IDC_EDIT5,m_ActiveTextOffset.x);
	DDX_Text(pDX, IDC_EDIT6,m_ActiveTextOffset.y);
	DDX_Text(pDX, IDC_EDIT7,m_PressTextOffset.x);
	DDX_Text(pDX, IDC_EDIT8,m_PressTextOffset.y);
	DDX_Text(pDX, IDC_EDIT9,m_DisableTextOffset.x);
	DDX_Text(pDX, IDC_EDIT10,m_DisableTextOffset.y);
}


BEGIN_MESSAGE_MAP(CDlgButtonTextureConfig, CPropertyConfigDialog)
	ON_BN_CLICKED(IDAPPLY, OnBnClickedApply)
	ON_BN_CLICKED(IDC_BUTTON_SET_TEXTURE, OnBnClickedButtonSetTexture)
	ON_BN_CLICKED(IDC_BUTTON_SET_TEXTURE2, OnBnClickedButtonSetTexture2)
	ON_BN_CLICKED(IDC_BUTTON_SET_TEXTURE3, OnBnClickedButtonSetTexture3)
	ON_BN_CLICKED(IDC_BUTTON_SET_TEXTURE4, OnBnClickedButtonSetTexture4)
END_MESSAGE_MAP()


// CDlgButtonTextureConfig 消息处理程序

BOOL CDlgButtonTextureConfig::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	
	CD3DButton * pButton=(CD3DButton *)m_pGUIObject;
	if(pButton)
	{
		CEasyString Temp;
		pButton->GetTexture(BUTTON_STATUS_NORMAL,Temp,m_NormalTextureRect,m_NormalTextOffset);
		m_NormalTextureFileName=Temp;
		pButton->GetTexture(BUTTON_STATUS_ACTIVE,Temp,m_ActiveTextureRect,m_ActiveTextOffset);
		m_ActiveTextureFileName=Temp;
		pButton->GetTexture(BUTTON_STATUS_PRESS,Temp,m_PressTextureRect,m_PressTextOffset);
		m_PressTextureFileName=Temp;
		pButton->GetTexture(BUTTON_STATUS_DISBALE,Temp,m_DisableTextureRect,m_DisableTextOffset);
		m_DisableTextureFileName=Temp;
		pButton->GetBorders(m_WndBorders);
	}
	
	UpdateData(false);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgButtonTextureConfig::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	OnBnClickedApply();
	CDialog::OnOK();
}

void CDlgButtonTextureConfig::OnBnClickedApply()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	
	CD3DButton * pButton=(CD3DButton *)m_pGUIObject;
	if(pButton)
	{
		pButton->SetTexture(BUTTON_STATUS_NORMAL,m_NormalTextureFileName,m_NormalTextureRect,m_NormalTextOffset);
		pButton->SetTexture(BUTTON_STATUS_ACTIVE,m_ActiveTextureFileName,m_ActiveTextureRect,m_ActiveTextOffset);
		pButton->SetTexture(BUTTON_STATUS_PRESS,m_PressTextureFileName,m_PressTextureRect,m_PressTextOffset);
		pButton->SetTexture(BUTTON_STATUS_DISBALE,m_DisableTextureFileName,m_DisableTextureRect,m_DisableTextOffset);
		pButton->SetBorders(m_WndBorders);
	}
	
}

void CDlgButtonTextureConfig::OnBnClickedButtonSetTexture()
{
	// TODO: 在此添加控件通知处理程序代码
	CDlgTextureConfig dlg;

	dlg.m_TextureFileName=m_NormalTextureFileName;
	dlg.m_TextureRect=m_NormalTextureRect;
	memcpy(dlg.m_WndBorders,m_WndBorders,sizeof(WIN_BORDERS));
	if(dlg.DoModal()==IDOK)
	{
		m_NormalTextureFileName=dlg.m_TextureFileName;
		m_NormalTextureRect=dlg.m_TextureRect;
		memcpy(m_WndBorders,dlg.m_WndBorders,sizeof(WIN_BORDERS));
	}
}

void CDlgButtonTextureConfig::OnBnClickedButtonSetTexture2()
{
	// TODO: 在此添加控件通知处理程序代码
	CDlgTextureConfig dlg;

	dlg.m_TextureFileName=m_ActiveTextureFileName;
	dlg.m_TextureRect=m_ActiveTextureRect;
	memcpy(dlg.m_WndBorders,m_WndBorders,sizeof(WIN_BORDERS));
	if(dlg.DoModal()==IDOK)
	{
		m_ActiveTextureFileName=dlg.m_TextureFileName;
		m_ActiveTextureRect=dlg.m_TextureRect;
		memcpy(m_WndBorders,dlg.m_WndBorders,sizeof(WIN_BORDERS));
	}
}

void CDlgButtonTextureConfig::OnBnClickedButtonSetTexture3()
{
	// TODO: 在此添加控件通知处理程序代码
	CDlgTextureConfig dlg;

	dlg.m_TextureFileName=m_PressTextureFileName;
	dlg.m_TextureRect=m_PressTextureRect;
	memcpy(dlg.m_WndBorders,m_WndBorders,sizeof(WIN_BORDERS));
	if(dlg.DoModal()==IDOK)
	{
		m_PressTextureFileName=dlg.m_TextureFileName;
		m_PressTextureRect=dlg.m_TextureRect;
		memcpy(m_WndBorders,dlg.m_WndBorders,sizeof(WIN_BORDERS));
	}
}

void CDlgButtonTextureConfig::OnBnClickedButtonSetTexture4()
{
	// TODO: 在此添加控件通知处理程序代码
	CDlgTextureConfig dlg;

	dlg.m_TextureFileName=m_DisableTextureFileName;
	dlg.m_TextureRect=m_DisableTextureRect;
	memcpy(dlg.m_WndBorders,m_WndBorders,sizeof(WIN_BORDERS));
	if(dlg.DoModal()==IDOK)
	{
		m_DisableTextureFileName=dlg.m_TextureFileName;
		m_DisableTextureRect=dlg.m_TextureRect;
		memcpy(m_WndBorders,dlg.m_WndBorders,sizeof(WIN_BORDERS));
	}
}
