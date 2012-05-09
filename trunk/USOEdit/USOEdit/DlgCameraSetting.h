/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DlgCameraSetting.h                                       */
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
/*      �ļ���:    DlgCameraSetting.h                                       */
/*      ��������:  2010��02��09��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once
#include "afxwin.h"


// CDlgCameraSetting �Ի���
class CD3DViewPort;

class CDlgCameraSetting : public CDialog
{
	DECLARE_DYNAMIC(CDlgCameraSetting)

public:
	CDlgCameraSetting(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgCameraSetting();

// �Ի�������
	enum { IDD = IDD_DIALOG_CAMERA_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
