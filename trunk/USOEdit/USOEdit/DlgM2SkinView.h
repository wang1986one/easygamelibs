/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DlgM2SkinView.h                                          */
/*      ��������:  2011��10��07��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once
#include "afxcmn.h"


// CDlgM2SkinView �Ի���

class CDlgM2SkinView : public CPropertyConfigDialog
{
protected:

	CD3DWOWM2Model *	m_pModel;
	CListCtrl			m_lvSkins;

	DECLARE_DYNAMIC(CDlgM2SkinView)

public:
	CDlgM2SkinView(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgM2SkinView();

// �Ի�������
	enum { IDD = IDD_DIALOG_M2_SKIN_VIEW };

	bool Init(CD3DWOWM2Model * pModel);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	
	afx_msg void OnBnClickedButtonChangeSkin();
};
