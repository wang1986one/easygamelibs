/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DlgLightSetting.h                                        */
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


// CDlgLightSetting �Ի���
class CUSOEditDoc;

class CDlgLightSetting : public CDialog
{
	DECLARE_DYNAMIC(CDlgLightSetting)
protected:
	CUSOEditDoc *	m_pDoc;
	CD3DLight *		m_pLight;
	CComboBox		m_cbLightType;
	D3DLIGHT9		m_LightData;
	UINT			m_TimeHour;
	UINT			m_TimeMinute;
public:
	CDlgLightSetting(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgLightSetting();

// �Ի�������
	enum { IDD = IDD_DIALOG_LIGHT_SETTING };

	void SetDocument(CUSOEditDoc *	pDoc)
	{
		m_pDoc=pDoc;
	}
	void SetLight(CD3DLight * pLight);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	
	afx_msg void OnBnClickedApply();
	afx_msg void OnBnClickedSelectAmbient();
	afx_msg void OnBnClickedSelectDiffuse();
	afx_msg void OnBnClickedSelectSpecular();
	
	afx_msg void OnBnClickedUseSceneLight();
};
