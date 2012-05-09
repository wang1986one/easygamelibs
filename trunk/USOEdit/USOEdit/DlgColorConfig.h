/****************************************************************************/
/*                                                                          */
/*      文件名:    DlgColorConfig.h                                         */
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
/*      文件名:    DlgColorConfig.h                                         */
/*      创建日期:  2009年07月06日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once


// CDlgColorConfig 对话框

class CDlgColorConfig : public CPropertyConfigDialog
{
	DECLARE_DYNAMIC(CDlgColorConfig)

public:
	CDlgColorConfig(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgColorConfig();

// 对话框数据
	enum { IDD = IDD_DIALOG_COLOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	D3DCOLORVALUE	m_D3DColor;

	virtual BOOL OnInitDialog();

protected:
	float m_ColorRed;
	float m_ColorGreen;
	float m_ColorBlue;
	float m_ColorAlpha;
	afx_msg void OnBnClickedButtonSelectColor();

	virtual void OnOK();
};
