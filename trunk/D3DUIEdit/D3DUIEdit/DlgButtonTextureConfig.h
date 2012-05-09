/****************************************************************************/
/*                                                                          */
/*      文件名:    DlgButtonTextureConfig.h                                 */
/*      创建日期:  2011年10月07日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once


// CDlgButtonTextureConfig 对话框

class CDlgButtonTextureConfig : public CPropertyConfigDialog
{
	DECLARE_DYNAMIC(CDlgButtonTextureConfig)

public:
	CDlgButtonTextureConfig(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgButtonTextureConfig();

// 对话框数据
	enum { IDD = IDD_DIALOG_BOTTON_TEXTURE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

	CD3DWnd *		m_pGUIObject;
	CString			m_NormalTextureFileName;
	CRect			m_NormalTextureRect;
	FLOAT_POINT		m_NormalTextOffset;

	CString			m_ActiveTextureFileName;
	CRect			m_ActiveTextureRect;
	FLOAT_POINT		m_ActiveTextOffset;

	CString			m_PressTextureFileName;
	CRect			m_PressTextureRect;
	FLOAT_POINT		m_PressTextOffset;

	CString			m_DisableTextureFileName;
	CRect			m_DisableTextureRect;
	FLOAT_POINT		m_DisableTextOffset;

	WIN_BORDERS		m_WndBorders;
public:
	void SetGUIObject(CD3DWnd *	pGUIObject)
	{
		m_pGUIObject=pGUIObject;
	}

	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
public:
	afx_msg void OnBnClickedApply();
	
	afx_msg void OnBnClickedButtonSetTexture();
	afx_msg void OnBnClickedButtonSetTexture2();
	afx_msg void OnBnClickedButtonSetTexture3();
	afx_msg void OnBnClickedButtonSetTexture4();
};
