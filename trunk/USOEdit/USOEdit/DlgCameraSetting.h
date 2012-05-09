/****************************************************************************/
/*                                                                          */
/*      文件名:    DlgCameraSetting.h                                       */
/*      创建日期:  2011年10月07日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
/****************************************************************************/
/*                                                                          */
/*      文件名:    DlgCameraSetting.h                                       */
/*      创建日期:  2010年02月09日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once
#include "afxwin.h"


// CDlgCameraSetting 对话框
class CD3DViewPort;

class CDlgCameraSetting : public CDialog
{
	DECLARE_DYNAMIC(CDlgCameraSetting)

public:
	CDlgCameraSetting(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgCameraSetting();

// 对话框数据
	enum { IDD = IDD_DIALOG_CAMERA_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
protected:
	CD3DViewPort *	m_pD3DView;
	int				m_Mode;
	int				m_ProjectMode;
	float			m_MoveStep;
	float			m_RotateStep;
	float			m_NearFace;
	float			m_FarFace;
	float			m_CameraPosX;
	float			m_CameraPosY;
	float			m_CameraPosZ;
	float			m_CameraRotateX;
	float			m_CameraRotateY;

	CButton			m_rbSceneMode;
	CButton			m_rbObjectMode;
	CButton			m_rbProjectModePerspective;
	CButton			m_cbProjectModeOrtho;

	afx_msg void OnBnClickedApply();
	afx_msg void OnBnClickedRadioSceneMode();
	afx_msg void OnBnClickedRadioObjectMode();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedRadioProjectModePerspective();
	afx_msg void OnBnClickedRadioProjectModeOrtho();
	void ShowSetting();
public:
	void SetD3DView(CD3DViewPort * pD3DView);
	
	
	afx_msg void OnBnClickedSetPos();
};
