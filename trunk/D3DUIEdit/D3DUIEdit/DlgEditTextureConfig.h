/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DlgEditTextureConfig.h                                   */
/*      ��������:  2011��10��07��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once


// CDlgEditTextureConfig �Ի���

class CDlgEditTextureConfig : public CPropertyConfigDialog
{
	DECLARE_DYNAMIC(CDlgEditTextureConfig)

public:
	CDlgEditTextureConfig(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgEditTextureConfig();

// �Ի�������
	enum { IDD = IDD_DIALOG_EDIT_TEXTURE };

protected:
	CD3DEdit *		m_pGUIObject;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:

	void SetGUIObject(CD3DWnd *	pGUIObject)
	{
		m_pGUIObject=(CD3DEdit *)pGUIObject;
	}
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
};
