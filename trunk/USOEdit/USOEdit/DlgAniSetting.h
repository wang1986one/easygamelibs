/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DlgAniSetting.h                                          */
/*      ��������:  2011��10��07��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once
#include "afxwin.h"


// CDlgAniSetting �Ի���

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
	CDlgAniSetting(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgAniSetting();

// �Ի�������
	enum { IDD = IDD_DIALOG_ANI_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:

	bool Init(CD3DWOWM2Model * pModel,CUSOObject * pUSOObject);
	
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedPlayAni();
	afx_msg void OnCbnSelchangeComboAnimation();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};
