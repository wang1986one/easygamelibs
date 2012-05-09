/****************************************************************************/
/*                                                                          */
/*      文件名:    DlgTextureConfig.h                                       */
/*      创建日期:  2011年10月07日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once
#include "D3DViewBox.h"
#include "afxcmn.h"

// CDlgTextureConfig 对话框
enum BORDER_LINES
{
	LINE_BORDER_TOP,
	LINE_BORDER_BOTTOM,
	LINE_TOP,
	LINE_BOTTOM,
	LINE_BORDER_LEFT,
	LINE_BORDER_RIGHT,
	LINE_BORDER_TOP_LEFT,
	LINE_BORDER_TOP_RIGHT,
	LINE_BORDER_BOTTOM_LEFT,
	LINE_BORDER_BOTTOM_RIGHT,	
	LINE_LEFT,
	LINE_RIGHT,
	LINE_MAX
};

class CDlgTextureConfig : public CPropertyConfigDialog
{
	DECLARE_DYNAMIC(CDlgTextureConfig)

public:
	CDlgTextureConfig(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgTextureConfig();

// 对话框数据
	enum { IDD = IDD_DIALOG_TEXTURE };

protected:
	CD3DViewBox*	m_pD3DViewBox;	
	IUITexture *	m_pTexture;
	IUIWndRect *	m_pTextureRect;
	CD3DWnd *		m_pLines[LINE_MAX];
	
	
	CSpinButtonCtrl m_ZoomSpin;
	int				m_ShowZoom;

	CD3DWnd *		m_pGUIObject;


	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

	void LoadTexture();
public:
	CString			m_TextureFileName;	
	CRect			m_TextureRect;
	WIN_BORDERS		m_WndBorders;

	virtual BOOL OnInitDialog();
	virtual BOOL DestroyWindow();	
	
	

	afx_msg void OnBnClickedButtonSelectFile();
	afx_msg void OnEnChangeEdit2();
	afx_msg LRESULT OnGUIWndResized(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnGUIWndResizing(WPARAM wParam,LPARAM lParam);
	afx_msg void OnBnClickedApply();

	void ShowTexture();
	void BulidLines();

	void SetGUIObject(CD3DWnd *	pGUIObject)
	{
		m_pGUIObject=pGUIObject;
	}
	
protected:
	virtual void OnOK();
public:	
	
	afx_msg void OnBnClickedButtonEditBorders();
};
