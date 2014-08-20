#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CDlgStructEditor 对话框

class CDlgStructEditor : public CDialog
{
	DECLARE_DYNAMIC(CDlgStructEditor)
protected:
	
	CComboBox				m_cbBaseStruct;
	CListCtrl				m_lvMemberList;
	
public:
	STRUCT_DEFINE_INFO				m_StructDefineInfo;
	CDlgStructEditor(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgStructEditor();

// 对话框数据
	enum { IDD = IDD_DIALOG_STRUCT_EDITOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
