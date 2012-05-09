/****************************************************************************/
/*                                                                          */
/*      文件名:    DlgM2CameraViewer.h                                      */
/*      创建日期:  2011年10月07日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once
#include "afxcmn.h"


// CDlgM2CameraViewer 对话框

class CDlgM2CameraViewer : public CPropertyConfigDialog
{
protected:
	CD3DWOWM2ModelResource *		m_pModelResource;
	CD3DViewPort *					m_pViewPort;
	CListCtrl						m_lvCameraList;
	DECLARE_DYNAMIC(CDlgM2CameraViewer)

public:
	CDlgM2CameraViewer(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgM2CameraViewer();

	void Init(CD3DWOWM2ModelResource * pModelResource,CD3DViewPort * pViewPort);

// 对话框数据
	enum { IDD = IDD_DIALOG_M2_CAMERA_VIEWER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	

protected:
	void FillList();
public:
	afx_msg void OnBnClickedButtonUseSelectedCamera();
};
