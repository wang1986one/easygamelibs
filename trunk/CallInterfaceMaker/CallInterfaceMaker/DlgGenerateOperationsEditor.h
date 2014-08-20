#pragma once


// CDlgGenerateOperationsEditor �Ի���

class CDlgGenerateOperationsEditor : public CDialog
{
	DECLARE_DYNAMIC(CDlgGenerateOperationsEditor)
protected:
	CEdit					m_edOperation;
	CTabCtrl				m_tbOperations;
	bool					m_IsModified;
public:
	GENERATE_OPERATIONS_INFO	m_GenerateOperations;
	bool						m_ShowIndexOperation;
	CDlgGenerateOperationsEditor(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgGenerateOperationsEditor();

// �Ի�������
	enum { IDD = IDD_DIALOG_GENERATE_OPERATIONS_EDITOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	
protected:
	virtual void OnCancel();
	virtual void OnOK();

	afx_msg void OnTcnSelchangingTab(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeEdit();

	void ShowItemData();
	void FetchItemData();
public:
	
};
