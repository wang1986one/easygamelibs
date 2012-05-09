/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DlgComboTextureConfig.h                                  */
/*      ��������:  2011��10��07��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once


// CDlgComboTextureConfig �Ի���

class CDlgComboTextureConfig : public CPropertyConfigDialog
{
	DECLARE_DYNAMIC(CDlgComboTextureConfig)

public:
	CDlgComboTextureConfig(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgComboTextureConfig();

// �Ի�������
	enum { IDD = IDD_DIALOG_COMBO_TEXTURE };

protected:
	CD3DCombo *		m_pGUIObject;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
