/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DlgColorConfig.h                                         */
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
/*      �ļ���:    DlgColorConfig.h                                         */
/*      ��������:  2009��07��06��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once


// CDlgColorConfig �Ի���

class CDlgColorConfig : public CPropertyConfigDialog
{
	DECLARE_DYNAMIC(CDlgColorConfig)

public:
	CDlgColorConfig(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgColorConfig();

// �Ի�������
	enum { IDD = IDD_DIALOG_COLOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	D3DCOLORVALUE	m_D3DColor;

	virtual BOOL OnInitDialog();

protected:
	float m_ColorRed;
	float m_ColorGreen;
	float m_ColorBlue;
	float m_ColorAlpha;
	afx_msg void OnBnClickedButtonSelectColor();

	virtual void OnOK();
};
