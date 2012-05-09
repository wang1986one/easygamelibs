/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DlgProgressBarTextureConfig.h                            */
/*      ��������:  2011��10��07��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once


// CDlgProgressBarTextureConfig �Ի���

class CDlgProgressBarTextureConfig : public CPropertyConfigDialog
{
	DECLARE_DYNAMIC(CDlgProgressBarTextureConfig)

public:
	CDlgProgressBarTextureConfig(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgProgressBarTextureConfig();

// �Ի�������
	enum { IDD = IDD_DIALOG_PROGRESSBAR_TEXTURE };

protected:
	CD3DWnd *		m_pGUIObject;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	void SetGUIObject(CD3DWnd *	pGUIObject)
	{
		m_pGUIObject=pGUIObject;
	}
public:
	afx_msg void OnBnClickedButtonTexture1();
	afx_msg void OnBnClickedButtonTexture2();
};
