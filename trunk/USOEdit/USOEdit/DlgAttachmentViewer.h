/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DlgAttachmentViewer.h                                    */
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


// CDlgAttachmentViewer �Ի���

class CDlgAttachmentViewer : public CPropertyConfigDialog
{
protected:
	CListCtrl			m_lvAttachments;
	CD3DWOWM2Model *	m_pModel;
	CUSOObject *		m_pUSOObject;
	DECLARE_DYNAMIC(CDlgAttachmentViewer)

public:
	CDlgAttachmentViewer(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgAttachmentViewer();

// �Ի�������
	enum { IDD = IDD_DIALOG_ATTACHMENT_VIEWER };

	void Init(CD3DWOWM2Model * pModel,CUSOObject * pUSOObject);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	
	afx_msg void OnBnClickedButtonEnableAttachment();
};
