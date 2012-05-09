/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DlgBoneViewer.h                                          */
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


// CDlgBoneViewer �Ի���

class CDlgBoneViewer : public CPropertyConfigDialog
{
	DECLARE_DYNAMIC(CDlgBoneViewer)
protected:
	CD3DWOWM2ModelResource *		m_pModelResource;
	CListCtrl						m_lvBones;
	CDlgAniFrameViewer				m_DlgAniFrameViewer;

public:
	CDlgBoneViewer(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgBoneViewer();

// �Ի�������
	enum { IDD = IDD_DIALOG_BONE_VIEWER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void Init(CD3DWOWM2ModelResource * pModelResource);
	
	afx_msg void OnBnClickedButtonViewAniSequence();
};
