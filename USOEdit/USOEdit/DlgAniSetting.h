/****************************************************************************/
/*                                                                          */
/*      文件名:    DlgAniSetting.h                                          */
/*      创建日期:  2011年10月07日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once
#include "afxwin.h"


// CDlgAniSetting 对话框

class CDlgAniSetting : public CPropertyConfigDialog
{
	DECLARE_DYNAMIC(CDlgAniSetting)
protected:
	CD3DWOWM2Model *	m_pModel;
	CUSOObject *		m_pUSOObject;
	CComboBox			m_cbAnimation;
	CSliderCtrl			m_slPlaySpeed;
	CString				m_PlaySpeedTip;
	BOOL				m_IsLoopPlay;

public:
	CDlgAniSetting(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgAniSetting();

// 对话框数据
	enum { IDD = IDD_DIALOG_ANI_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:

	bool Init(CD3DWOWM2Model * pModel,CUSOObject * pUSOObject);
	
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedPlayAni();
	afx_msg void OnCbnSelchangeComboAnimation();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};
