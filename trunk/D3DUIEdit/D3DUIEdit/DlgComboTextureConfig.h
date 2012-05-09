/****************************************************************************/
/*                                                                          */
/*      文件名:    DlgComboTextureConfig.h                                  */
/*      创建日期:  2011年10月07日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once


// CDlgComboTextureConfig 对话框

class CDlgComboTextureConfig : public CPropertyConfigDialog
{
	DECLARE_DYNAMIC(CDlgComboTextureConfig)

public:
	CDlgComboTextureConfig(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgComboTextureConfig();

// 对话框数据
	enum { IDD = IDD_DIALOG_COMBO_TEXTURE };

protected:
	CD3DCombo *		m_pGUIObject;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	void SetGUIObject(CD3DWnd *	pGUIObject)
	{
		m_pGUIObject=(CD3DCombo *)pGUIObject;
	}

	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
};
