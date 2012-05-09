/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DlgSimpleListTextureConfig.h                             */
/*      ��������:  2011��10��07��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once


// CDlgSimpleListTextureConfig �Ի���

class CDlgSimpleListTextureConfig : public CPropertyConfigDialog
{
	DECLARE_DYNAMIC(CDlgSimpleListTextureConfig)

public:
	CDlgSimpleListTextureConfig(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgSimpleListTextureConfig();

// �Ի�������
	enum { IDD = IDD_DIALOG_SIMPLELIST_TEXTURE };

protected:
	CD3DSimpleList *		m_pGUIObject;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	void SetGUIObject(CD3DWnd *	pGUIObject)
	{
		m_pGUIObject=(CD3DSimpleList *)pGUIObject;
	}

	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
};
