/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DlgWOWCharSetting.h                                      */
/*      ��������:  2011��10��07��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DlgWOWCharSetting.h                                      */
/*      ��������:  2009��09��11��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CDlgWOWCharSetting �Ի���

class CDlgWOWCharSetting : public CPropertyConfigDialog
{
	DECLARE_DYNAMIC(CDlgWOWCharSetting)

public:
	CDlgWOWCharSetting(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgWOWCharSetting();

// �Ի�������
	enum { IDD = IDD_WOW_CHAR_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
