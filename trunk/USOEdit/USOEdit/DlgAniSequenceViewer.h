/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DlgAniSequenceViewer.h                                   */
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


// CDlgAniSequenceViewer �Ի���

class CDlgAniSequenceViewer : public CPropertyConfigDialog
{
	DECLARE_DYNAMIC(CDlgAniSequenceViewer)
protected:
	CD3DWOWM2ModelResource *		m_pModelResource;
	CListCtrl						m_lvAniSeqs;
	
public:
	CDlgAniSequenceViewer(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgAniSequenceViewer();

// �Ի�������
	enum { IDD = IDD_DIALOG_ANI_SEQUENCE_VIEWER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void Init(CD3DWOWM2ModelResource * pModelResource);
	
};
