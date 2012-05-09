/****************************************************************************/
/*                                                                          */
/*      文件名:    DlgColorConfig.cpp                                       */
/*      创建日期:  2011年10月07日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
/****************************************************************************/
/*                                                                          */
/*      文件名:    DlgColorConfig.cpp                                       */
/*      创建日期:  2009年09月11日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
// DlgColorConfig.cpp : 实现文件
//

#include "stdafx.h"




// CDlgColorConfig 对话框

IMPLEMENT_DYNAMIC(CDlgColorConfig, CPropertyConfigDialog)
CDlgColorConfig::CDlgColorConfig(CWnd* pParent /*=NULL*/)
	: CPropertyConfigDialog(CDlgColorConfig::IDD, pParent)
	, m_ColorRed(0)
	, m_ColorGreen(0)
	, m_ColorBlue(0)
	, m_ColorAlpha(0)
{
}

CDlgColorConfig::~CDlgColorConfig()
{
}

void CDlgColorConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_ColorRed);
	DDV_MinMaxFloat(pDX, m_ColorRed, 0, 1);
	DDX_Text(pDX, IDC_EDIT15, m_ColorGreen);
	DDV_MinMaxFloat(pDX, m_ColorGreen, 0, 1);
	DDX_Text(pDX, IDC_EDIT2, m_ColorBlue);
	DDV_MinMaxFloat(pDX, m_ColorBlue, 0, 1);
	DDX_Text(pDX, IDC_EDIT17, m_ColorAlpha);
	DDV_MinMaxFloat(pDX, m_ColorAlpha, 0, 1);
	
}


BEGIN_MESSAGE_MAP(CDlgColorConfig, CPropertyConfigDialog)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_COLOR, OnBnClickedButtonSelectColor)
END_MESSAGE_MAP()


// CDlgColorConfig 消息处理程序

void CDlgColorConfig::OnBnClickedButtonSelectColor()
{
	// TODO: 在此添加控件通知处理程序代码
	CColorDialog dlg(RGB(m_ColorRed,m_ColorGreen,m_ColorBlue));

	if(dlg.DoModal()==IDOK)
	{
		UpdateData(true);
		m_ColorRed=GetRValue(dlg.GetColor())/255.0f;
		m_ColorGreen=GetGValue(dlg.GetColor())/255.0f;
		m_ColorBlue=GetBValue(dlg.GetColor())/255.0f;
		UpdateData(false);
	}
}

BOOL CDlgColorConfig::OnInitDialog()
{
	CPropertyConfigDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_ColorAlpha=m_D3DColor.a;
	m_ColorRed=m_D3DColor.r;
	m_ColorGreen=m_D3DColor.g;
	m_ColorBlue=m_D3DColor.b;

	UpdateData(false);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgColorConfig::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	UpdateData(true);
	m_D3DColor.a=m_ColorAlpha;
	m_D3DColor.r=m_ColorRed;
	m_D3DColor.g=m_ColorGreen;
	m_D3DColor.b=m_ColorBlue;
	
	CPropertyConfigDialog::OnOK();
}
