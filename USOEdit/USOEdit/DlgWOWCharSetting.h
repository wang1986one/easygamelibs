/****************************************************************************/
/*                                                                          */
/*      文件名:    DlgWOWCharSetting.h                                      */
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
/*      文件名:    DlgWOWCharSetting.h                                      */
/*      创建日期:  2009年09月11日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CDlgWOWCharSetting 对话框

class CDlgWOWCharSetting : public CPropertyConfigDialog
{
	DECLARE_DYNAMIC(CDlgWOWCharSetting)

public:
	CDlgWOWCharSetting(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgWOWCharSetting();

// 对话框数据
	enum { IDD = IDD_WOW_CHAR_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
protected:
	CD3DWOWM2CharacterModel *	m_pModel;
	CUSOObject *				m_pUSOObject;
	CComboBox m_cbSkinColor;
	CComboBox m_cbHairType;
	CComboBox m_cbHairColor;
	CComboBox m_cbWhiskerType;
	CComboBox m_cbFaceType;
	CComboBox m_cbAnimation;
	CComboBox m_cbAttachment;
	CSliderCtrl m_slPlaySpeed;
	CString m_PlaySpeedTip;
	BOOL m_IsBald;
	BOOL m_IsLoopPlay;

	CDlgWOWCharEquipmentSetting	m_DlgEquipmentSetting;
public:

	bool Init(CD3DWOWM2CharacterModel * pModel,CUSOObject * pUSOObject);
	
	virtual BOOL OnInitDialog();
	
	afx_msg void OnBnClickedEnableAttachment();
	afx_msg void OnCbnSelchangeComboSkinColor();
	afx_msg void OnCbnSelchangeComboHairType();
	afx_msg void OnCbnSelchangeComboHairColor();
	afx_msg void OnCbnSelchangeComboWhiskerType();
	afx_msg void OnCbnSelchangeComboFaceType();
	afx_msg void OnCbnSelchangeComboAnimation();
	
	
	
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	
	afx_msg void OnBnClickedCheckIsBald();
	afx_msg void OnBnClickedEquipmentSetting();
	
	afx_msg void OnBnClickedPlayAni();
};
