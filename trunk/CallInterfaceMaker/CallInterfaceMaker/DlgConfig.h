/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DlgConfig.h                                              */
/*      ��������:  2012��04��20��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once
#include "afxcmn.h"


// CDlgConfig �Ի���

class CDlgConfig : public CDialog
{
	DECLARE_DYNAMIC(CDlgConfig)

public:
	CDlgConfig(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgConfig();

// �Ի�������
	enum { IDD = IDD_DIALOG_CONFIG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	INTERFACE_CONFIG m_Config;
protected:		
	bool					m_IsModified;
public:


	virtual BOOL OnInitDialog();
	virtual void OnOK();
protected:
	virtual void OnCancel();
	afx_msg void OnEnChangeEdit();
public:
	afx_msg void OnBnClickedButton1DefaultStructGenerateOperations();
	afx_msg void OnBnClickedButtonArrayDefineConfig();
};

	
