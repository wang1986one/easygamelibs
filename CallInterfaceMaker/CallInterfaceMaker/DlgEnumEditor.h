#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CDlgEnumEditor 对话框

class CDlgEnumEditor : public CDialog
{
	DECLARE_DYNAMIC(CDlgEnumEditor)
protected:
	
	CComboBox				m_cbBaseStruct;
	CListCtrl				m_lvMemberList;	
public:
	ENUM_DEFINE_INFO				m_EnumDefineInfo;
	ENUM_MEMBER_INFO				m_EnumMemberInfo;
	CDlgEnumEditor(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgEnumEditor();

// 对话框数据
	enum { IDD = IDD_DIALOG_ENUM_EDITOR };

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

	afx_msg void OnBnClickedOk();
	
public:
	afx_msg void OnBnClickedButtonMemberMoveUp();
	afx_msg void OnBnClickedButtonMemberMoveDown();
	
	afx_msg void OnNMClickListMemberList(NMHDR *pNMHDR, LRESULT *pResult);
};
