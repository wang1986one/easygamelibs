/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DlgButtonTextureConfig.h                                 */
/*      ��������:  2011��10��07��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once


// CDlgButtonTextureConfig �Ի���

class CDlgButtonTextureConfig : public CPropertyConfigDialog
{
	DECLARE_DYNAMIC(CDlgButtonTextureConfig)

public:
	CDlgButtonTextureConfig(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgButtonTextureConfig();

// �Ի�������
	enum { IDD = IDD_DIALOG_BOTTON_TEXTURE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

	CD3DWnd *		m_pGUIObject;
	CString			m_NormalTextureFileName;
	CRect			m_NormalTextureRect;
	FLOAT_POINT		m_NormalTextOffset;

	CString			m_ActiveTextureFileName;
	CRect			m_ActiveTextureRect;
	FLOAT_POINT		m_ActiveTextOffset;

	CString			m_PressTextureFileName;
	CRect			m_PressTextureRect;
	FLOAT_POINT		m_PressTextOffset;

	CString			m_DisableTextureFileName;
	CRect			m_DisableTextureRect;
	FLOAT_POINT		m_DisableTextOffset;

	WIN_BORDERS		m_WndBorders;
public:
	void SetGUIObject(CD3DWnd *	pGUIObject)
	{
		m_pGUIObject=pGUIObject;
	}

	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
public:
	afx_msg void OnBnClickedApply();
	
	afx_msg void OnBnClickedButtonSetTexture();
	afx_msg void OnBnClickedButtonSetTexture2();
	afx_msg void OnBnClickedButtonSetTexture3();
	afx_msg void OnBnClickedButtonSetTexture4();
};
