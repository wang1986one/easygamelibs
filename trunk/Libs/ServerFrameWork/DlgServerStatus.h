#pragma once
#include "afxcmn.h"


// CDlgServerStatus �Ի���

class CDlgServerStatus : public CDialog
{
	DECLARE_DYNAMIC(CDlgServerStatus)

public:
	CDlgServerStatus(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgServerStatus();

// �Ի�������
	enum { IDD = IDD_SERVER_STATUS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_lvServerStatus;
	virtual BOOL OnInitDialog();

	void FlushStatus(CSmartStruct& ServerStatus);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
