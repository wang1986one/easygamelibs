/****************************************************************************/
/*                                                                          */
/*      文件名:    DlgFontConfig.cpp                                        */
/*      创建日期:  2011年10月07日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
// DlgFontConfig.cpp : 实现文件
//

#include "stdafx.h"
#include "D3DUIEdit.h"
#include "DlgFontConfig.h"
#include ".\dlgfontconfig.h"
#include "DlgColorConfig.h"

// CDlgFontConfig 对话框

IMPLEMENT_DYNAMIC(CDlgFontConfig, CPropertyConfigDialog)
CDlgFontConfig::CDlgFontConfig(CWnd* pParent /*=NULL*/)
	: CPropertyConfigDialog(CDlgFontConfig::IDD, pParent)
	, m_FontString(_T(""))
	, m_CharSpace(0)
	, m_LineSpace(0)
	, m_FontColor(0)
	, m_ShadowWidth(0)
	, m_ShadowColor(0)		
	, m_IsWorkBreak(FALSE)
	, m_ShadowLeft(FALSE)
	, m_FontScale(1.0f)
{
	m_pGUIObject=NULL;
	ZeroMemory(&m_Font,sizeof(m_Font));
}

CDlgFontConfig::~CDlgFontConfig()
{
}

void CDlgFontConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_FontString);
	DDX_Text(pDX, IDC_EDIT15, m_CharSpace);
	DDV_MinMaxInt(pDX, m_CharSpace, 0, 999999);
	DDX_Text(pDX, IDC_EDIT16, m_LineSpace);
	DDV_MinMaxInt(pDX, m_LineSpace, 0, 999999);
	DDX_Text(pDX, IDC_EDIT17, m_FontColor);
	DDX_Text(pDX, IDC_EDIT18, m_ShadowWidth);
	DDX_Text(pDX, IDC_EDIT19, m_ShadowColor);
	DDX_Check(pDX,IDC_RADIO2, m_IsAlignLeft);
	DDX_Check(pDX,IDC_RADIO3, m_IsAlignCenter);
	DDX_Check(pDX,IDC_RADIO4, m_IsAlignRight);
	DDX_Check(pDX,IDC_RADIO5, m_IsAlignTop);
	DDX_Check(pDX,IDC_RADIO6, m_IsAlignVCenter);
	DDX_Check(pDX,IDC_RADIO7, m_IsAlignBottom);
	DDX_Check(pDX, IDC_CHECK9, m_IsWorkBreak);
	DDX_Check(pDX, IDC_CHECK1, m_ShadowLeft);
	DDX_Check(pDX, IDC_CHECK2, m_ShadowTop);
	DDX_Check(pDX, IDC_CHECK3, m_ShadowRight);
	DDX_Check(pDX, IDC_CHECK4, m_ShadowBottom);
	DDX_Check(pDX, IDC_CHECK5, m_ShadowTopLeft);
	DDX_Check(pDX, IDC_CHECK6, m_ShadowTopRight);
	DDX_Check(pDX, IDC_CHECK7, m_ShadowBottomLeft);
	DDX_Check(pDX, IDC_CHECK8, m_ShadowBottomRight);
	DDX_Text(pDX, IDC_EDIT20, m_FontScale);
}


BEGIN_MESSAGE_MAP(CDlgFontConfig, CPropertyConfigDialog)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_FONT, OnBnClickedButtonSelectFont)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_FONT_COLOR, OnBnClickedButtonSelectFontColor)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_SHADOW_COLOR, OnBnClickedButtonSelectShadowColor)	
	ON_BN_CLICKED(IDAPPLY, OnBnClickedApply)	
END_MESSAGE_MAP()


// CDlgFontConfig 消息处理程序

BOOL CDlgFontConfig::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	
	CD3DWnd * pWnd=m_pGUIObject;
	if(pWnd)
	{
		m_Font=*(pWnd->GetFont());
		m_FontColor=pWnd->GetFontColor();
		m_CharSpace=pWnd->GetFontCharSpace();
		m_LineSpace=pWnd->GetFontLineSpace();
		m_ShadowWidth=pWnd->GetFontShadowWidth();
		m_ShadowColor=pWnd->GetFontShadowColor();

		m_IsAlignLeft=!((pWnd->GetFontAlign()&D3DFONT_CENTER)||
			(pWnd->GetFontAlign()&D3DFONT_RIGHT));

		m_IsAlignCenter=(pWnd->GetFontAlign()&D3DFONT_CENTER)==D3DFONT_CENTER;
		m_IsAlignRight=(pWnd->GetFontAlign()&D3DFONT_RIGHT)==D3DFONT_RIGHT;

		m_IsAlignTop=!((pWnd->GetFontAlign()&D3DFONT_VCENTER)||
			(pWnd->GetFontAlign()&D3DFONT_BOTTOM));

		m_IsAlignVCenter=(pWnd->GetFontAlign()&D3DFONT_VCENTER)==D3DFONT_VCENTER;
		m_IsAlignBottom=(pWnd->GetFontAlign()&D3DFONT_BOTTOM)==D3DFONT_BOTTOM;
		m_IsWorkBreak=(pWnd->GetFontAlign()&D3DFONT_WORDBREAK)==D3DFONT_WORDBREAK;

		m_ShadowLeft=(pWnd->GetFontShadowMode()&FONT_SHADOW_LEFT)==FONT_SHADOW_LEFT;
		m_ShadowTop=(pWnd->GetFontShadowMode()&FONT_SHADOW_TOP)==FONT_SHADOW_TOP;
		m_ShadowRight=(pWnd->GetFontShadowMode()&FONT_SHADOW_RIGHT)==FONT_SHADOW_RIGHT;
		m_ShadowBottom=(pWnd->GetFontShadowMode()&FONT_SHADOW_BOTTOM)==FONT_SHADOW_BOTTOM;
		m_ShadowTopLeft=(pWnd->GetFontShadowMode()&FONT_SHADOW_TOP_LEFT)==FONT_SHADOW_TOP_LEFT;
		m_ShadowTopRight=(pWnd->GetFontShadowMode()&FONT_SHADOW_TOP_RIGHT)==FONT_SHADOW_TOP_RIGHT;
		m_ShadowBottomLeft=(pWnd->GetFontShadowMode()&FONT_SHADOW_BOTTOM_LEFT)==FONT_SHADOW_BOTTOM_LEFT;
		m_ShadowBottomRight=(pWnd->GetFontShadowMode()&FONT_SHADOW_BOTTOM_RIGHT)==FONT_SHADOW_BOTTOM_RIGHT;

		m_FontScale=pWnd->GetFontScale();

	}
	
	m_FontString=LogFontToString(m_Font);

	UpdateData(false);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

CString CDlgFontConfig::LogFontToString(LOGFONT& Font)
{
	CString Name;
	Name.Format(_T("%s:%d:%d%d%d%d%d%d%d"),
		Font.lfFaceName,
		Font.lfHeight,
		Font.lfWidth,	
		Font.lfWeight,
		Font.lfCharSet,
		Font.lfItalic,
		Font.lfOutPrecision,
		Font.lfPitchAndFamily,
		Font.lfQuality
		);
	return Name;
}
void CDlgFontConfig::OnBnClickedButtonSelectFont()
{
	// TODO: 在此添加控件通知处理程序代码

	CEasyString FontNameCN=GetFontNameCN(&m_Font);
	if(FontNameCN.GetLength())
	{
		_tcscpy_s(m_Font.lfFaceName,LF_FACESIZE,FontNameCN);
	}
	
	CFontDialog dlg(&m_Font);

	if(dlg.DoModal()==IDOK)
	{
		UpdateData(true);
		dlg.GetCurrentFont(&m_Font);
		CEasyString FontNameEN=GetFontNameEN(&m_Font);
		if(FontNameEN.GetLength())
		{
			_tcscpy_s(m_Font.lfFaceName,LF_FACESIZE,FontNameEN);
		}
		m_Font.lfHeight=abs(m_Font.lfHeight);
		m_FontString=LogFontToString(m_Font);
		UpdateData(false);
	}	
}

void CDlgFontConfig::OnBnClickedButtonSelectFontColor()
{
	// TODO: 在此添加控件通知处理程序代码
	CDlgColorConfig dlg;
	dlg.m_D3DColor=m_FontColor;

	if(dlg.DoModal()==IDOK)
	{
		UpdateData(true);
		m_FontColor=dlg.m_D3DColor;
		UpdateData(false);
	}
}

void CDlgFontConfig::OnBnClickedButtonSelectShadowColor()
{
	// TODO: 在此添加控件通知处理程序代码
	CDlgColorConfig dlg;
	dlg.m_D3DColor=m_ShadowColor;

	if(dlg.DoModal()==IDOK)
	{
		UpdateData(true);
		m_ShadowColor=dlg.m_D3DColor;
		UpdateData(false);
	}	
}

void CDlgFontConfig::OnBnClickedApply()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	
	CD3DWnd * pWnd=m_pGUIObject;
	if(pWnd)
	{
		pWnd->SetFont(&m_Font);
		pWnd->SetFontColor(m_FontColor);
		pWnd->SetFontCharSpace(m_CharSpace);
		pWnd->SetFontLineSpace(m_LineSpace);
		pWnd->SetFontShadowWidth(m_ShadowWidth);
		pWnd->SetFontShadowColor(m_ShadowColor);

		int FontAlign=0;			

		if(m_IsAlignCenter)
			FontAlign|=D3DFONT_CENTER;
		if(m_IsAlignRight)
			FontAlign|=D3DFONT_RIGHT;
		if(m_IsAlignVCenter)
			FontAlign|=D3DFONT_VCENTER;
		if(m_IsAlignBottom)
			FontAlign|=D3DFONT_BOTTOM;
		if(m_IsWorkBreak)
			FontAlign|=D3DFONT_WORDBREAK;
		pWnd->SetFontAlign(FontAlign);
		
		int FontShadow=0;

		if(m_ShadowLeft)
			FontShadow|=FONT_SHADOW_LEFT;
		if(m_ShadowTop)
			FontShadow|=FONT_SHADOW_TOP;
		if(m_ShadowRight)
			FontShadow|=FONT_SHADOW_RIGHT;
		if(m_ShadowBottom)
			FontShadow|=FONT_SHADOW_BOTTOM;

		if(m_ShadowTopLeft)
			FontShadow|=FONT_SHADOW_TOP_LEFT;
		if(m_ShadowTopRight)
			FontShadow|=FONT_SHADOW_TOP_RIGHT;
		if(m_ShadowBottomLeft)
			FontShadow|=FONT_SHADOW_BOTTOM_LEFT;
		if(m_ShadowBottomRight)
			FontShadow|=FONT_SHADOW_BOTTOM_RIGHT;

		pWnd->SetFontShadowMode(FontShadow);
		pWnd->SetFontScale(m_FontScale);

	}
	
}

void CDlgFontConfig::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	OnBnClickedApply();
	CPropertyConfigDialog::OnOK();
}


