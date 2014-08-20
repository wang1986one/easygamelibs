#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CDlgStructEditor �Ի���

class CDlgStructEditor : public CDialog
{
	DECLARE_DYNAMIC(CDlgStructEditor)
protected:
	
	CComboBox				m_cbBaseStruct;
	CListCtrl				m_lvMemberList;
	
public:
	STRUCT_DEFINE_INFO				m_StructDefineInfo;
	CDlgStructEditor(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgStructEditor();

// �Ի�������
	enum { IDD = IDD_DIALOG_STRUCT_EDITOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
protected:
	void FillList();
	void SelectItemByName(LPCTSTR szName);
	virtual BOOL OnInitDialog();
	
	afx_msg void OnBnClickedButtonAddMember();
	afx_msg void OnBnClickedButtonEditMember();
	afx_msg void OnBnClickedButtonDelMember();

	afx_msg void OnNMDblclkListMemberList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedOk();
	
public:
	afx_msg void OnBnClickedButtonMemberMoveUp();
	afx_msg void OnBnClickedButtonMemberMoveDown();
	afx_msg void OnBnClickedButtonMemberGenerateOperations();
};
