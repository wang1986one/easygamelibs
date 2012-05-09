/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DlgScrollBarTextureConfig.h                              */
/*      ��������:  2011��10��07��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once


// CDlgScrollBarTextureConfig �Ի���

class CDlgScrollBarTextureConfig : public CPropertyConfigDialog
{
	DECLARE_DYNAMIC(CDlgScrollBarTextureConfig)

public:
	CDlgScrollBarTextureConfig(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgScrollBarTextureConfig();

// �Ի�������
	enum { IDD = IDD_DIALOG_SCROLLBAR_TEXTURE };

protected:
	CD3DWnd *		m_pGUIObject;
	CString			m_TextureFileName;
	CRect			m_TextureRect;
	WIN_BORDERS		m_WndBorders;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	
	afx_msg void OnBnClickedButtonSelectTexture();
	afx_msg void OnBnClickedButtonDec();
	afx_msg void OnBnClickedButtonInc();
	afx_msg void OnBnClickedButtonTrack();
	virtual BOOL OnInitDialog();

	void SetGUIObject(CD3DWnd *	pGUIObject)
	{
		m_pGUIObject=pGUIObject;
	}
protected:
	virtual void OnOK();
public:
	afx_msg void OnBnClickedApply();
};
