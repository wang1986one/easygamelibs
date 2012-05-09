/****************************************************************************/
/*                                                                          */
/*      文件名:    DlgFontConfig.h                                          */
/*      创建日期:  2011年10月07日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once
#include "Resource.h"
#include "afxwin.h"

// CDlgFontConfig 对话框

class CDlgFontConfig : public CPropertyConfigDialog
{
	DECLARE_DYNAMIC(CDlgFontConfig)

public:
	CDlgFontConfig(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgFontConfig();

// 对话框数据
	enum { IDD = IDD_DIALOG_FONT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
protected:
	CD3DWnd *		m_pGUIObject;
	LOGFONT			m_Font;
	CString			m_FontString;
	int				m_CharSpace;
	int				m_LineSpace;
	DWORD			m_FontColor;
	int				m_ShadowWidth;
	DWORD			m_ShadowColor;
	BOOL			m_IsAlignLeft;
	BOOL			m_IsAlignCenter;
	BOOL			m_IsAlignRight;
	BOOL			m_IsAlignTop;
	BOOL			m_IsAlignVCenter;
	BOOL			m_IsAlignBottom;
	BOOL			m_IsWorkBreak;
	BOOL			m_ShadowLeft;
	BOOL			m_ShadowTop;
	BOOL			m_ShadowRight;
	BOOL			m_ShadowBottom;
	BOOL			m_ShadowTopLeft;
	BOOL			m_ShadowTopRight;
	BOOL			m_ShadowBottomLeft;
	BOOL			m_ShadowBottomRight;
	float			m_FontScale;

public:
	virtual BOOL OnInitDialog();

	CString LogFontToString(LOGFONT& Font);

	void SetGUIObject(CD3DWnd *	pGUIObject)
	{
		m_pGUIObject=pGUIObject;
	}
	afx_msg void OnBnClickedButtonSelectFont();
	afx_msg void OnBnClickedButtonSelectFontColor();
	afx_msg void OnBnClickedButtonSelectShadowColor();	
	afx_msg void OnBnClickedApply();
protected:
	virtual void OnOK();
public:	
	
	
	
	
};
