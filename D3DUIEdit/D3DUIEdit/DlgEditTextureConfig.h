/****************************************************************************/
/*                                                                          */
/*      文件名:    DlgEditTextureConfig.h                                   */
/*      创建日期:  2011年10月07日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once


// CDlgEditTextureConfig 对话框

class CDlgEditTextureConfig : public CPropertyConfigDialog
{
	DECLARE_DYNAMIC(CDlgEditTextureConfig)

public:
	CDlgEditTextureConfig(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgEditTextureConfig();

// 对话框数据
	enum { IDD = IDD_DIALOG_EDIT_TEXTURE };

protected:
	CD3DEdit *		m_pGUIObject;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:

	void SetGUIObject(CD3DWnd *	pGUIObject)
	{
		m_pGUIObject=(CD3DEdit *)pGUIObject;
	}
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
};
