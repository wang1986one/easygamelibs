/****************************************************************************/
/*                                                                          */
/*      文件名:    DlgScrollBarTextureConfig.h                              */
/*      创建日期:  2011年10月07日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once


// CDlgScrollBarTextureConfig 对话框

class CDlgScrollBarTextureConfig : public CPropertyConfigDialog
{
	DECLARE_DYNAMIC(CDlgScrollBarTextureConfig)

public:
	CDlgScrollBarTextureConfig(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgScrollBarTextureConfig();

// 对话框数据
	enum { IDD = IDD_DIALOG_SCROLLBAR_TEXTURE };

protected:
	CD3DWnd *		m_pGUIObject;
	CString			m_TextureFileName;
	CRect			m_TextureRect;
	WIN_BORDERS		m_WndBorders;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	
	afx_msg void OnBnClickedButtonSelectTexture();
	afx_msg void OnBnClickedButtonDec();
	afx_msg void OnBnClickedButtonInc();
	afx_msg void OnBnClickedButtonTrack();
	virtual BOOL OnInitDialog();

	void SetGUIObject(CD3DWnd *	pGUIObject)
	{
		m_pGUIObject=pGUIObject;
	}
protected:
	virtual void OnOK();
public:
	afx_msg void OnBnClickedApply();
};
