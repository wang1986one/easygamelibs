/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DlgM2CameraViewer.h                                      */
/*      ��������:  2011��10��07��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once
#include "afxcmn.h"


// CDlgM2CameraViewer �Ի���

class CDlgM2CameraViewer : public CPropertyConfigDialog
{
protected:
	CD3DWOWM2ModelResource *		m_pModelResource;
	CD3DViewPort *					m_pViewPort;
	CListCtrl						m_lvCameraList;
	DECLARE_DYNAMIC(CDlgM2CameraViewer)

public:
	CDlgM2CameraViewer(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgM2CameraViewer();

	void Init(CD3DWOWM2ModelResource * pModelResource,CD3DViewPort * pViewPort);

// �Ի�������
	enum { IDD = IDD_DIALOG_M2_CAMERA_VIEWER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	

protected:
	void FillList();
public:
	afx_msg void OnBnClickedButtonUseSelectedCamera();
};
