/****************************************************************************/
/*                                                                          */
/*      文件名:    DlgLightSetting.h                                        */
/*      创建日期:  2011年10月07日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once
#include "afxwin.h"


// CDlgLightSetting 对话框
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
	CDlgLightSetting(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgLightSetting();

// 对话框数据
	enum { IDD = IDD_DIALOG_LIGHT_SETTING };

	void SetDocument(CUSOEditDoc *	pDoc)
	{
		m_pDoc=pDoc;
	}
	void SetLight(CD3DLight * pLight);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	
	afx_msg void OnBnClickedApply();
	afx_msg void OnBnClickedSelectAmbient();
	afx_msg void OnBnClickedSelectDiffuse();
	afx_msg void OnBnClickedSelectSpecular();
	
	afx_msg void OnBnClickedUseSceneLight();
};
