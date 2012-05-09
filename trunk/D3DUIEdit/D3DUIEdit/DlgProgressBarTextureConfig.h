/****************************************************************************/
/*                                                                          */
/*      文件名:    DlgProgressBarTextureConfig.h                            */
/*      创建日期:  2011年10月07日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once


// CDlgProgressBarTextureConfig 对话框

class CDlgProgressBarTextureConfig : public CPropertyConfigDialog
{
	DECLARE_DYNAMIC(CDlgProgressBarTextureConfig)

public:
	CDlgProgressBarTextureConfig(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgProgressBarTextureConfig();

// 对话框数据
	enum { IDD = IDD_DIALOG_PROGRESSBAR_TEXTURE };

protected:
	CD3DWnd *		m_pGUIObject;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	void SetGUIObject(CD3DWnd *	pGUIObject)
	{
		m_pGUIObject=pGUIObject;
	}
public:
	afx_msg void OnBnClickedButtonTexture1();
	afx_msg void OnBnClickedButtonTexture2();
};
