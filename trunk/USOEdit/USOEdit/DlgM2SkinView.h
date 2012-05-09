/****************************************************************************/
/*                                                                          */
/*      文件名:    DlgM2SkinView.h                                          */
/*      创建日期:  2011年10月07日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once
#include "afxcmn.h"


// CDlgM2SkinView 对话框

class CDlgM2SkinView : public CPropertyConfigDialog
{
protected:

	CD3DWOWM2Model *	m_pModel;
	CListCtrl			m_lvSkins;

	DECLARE_DYNAMIC(CDlgM2SkinView)

public:
	CDlgM2SkinView(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgM2SkinView();

// 对话框数据
	enum { IDD = IDD_DIALOG_M2_SKIN_VIEW };

	bool Init(CD3DWOWM2Model * pModel);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	
	afx_msg void OnBnClickedButtonChangeSkin();
};
