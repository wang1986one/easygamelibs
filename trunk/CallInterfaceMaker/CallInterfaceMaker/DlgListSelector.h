#pragma once
#include "afxcmn.h"


// CDlgListSelector �Ի���

class CDlgListSelector : public CDialog
{
	DECLARE_DYNAMIC(CDlgListSelector)
protected:
	CListCtrl		m_lvList;
	CString			m_Title;
	vector<CString> m_Items;
public:
	CString		m_SelectedItem;
public:
	CDlgListSelector(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgListSelector();

// �Ի�������
	enum { IDD = IDD_DIALOG_LIST_SELECTOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	
	virtual BOOL OnInitDialog();

	void Init(CString Title,vector<CString> Items);
	afx_msg void OnNMDblclkList(NMHDR *pNMHDR, LRESULT *pResult);
};
