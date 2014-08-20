#pragma once
#include "afxcmn.h"


// CDlgEnumList 对话框

class CDlgEnumList : public CDialog
{
	DECLARE_DYNAMIC(CDlgEnumList)
protected:
	CListCtrl	m_lvEnum;
public:
	CString							m_ListName;
	vector<ENUM_DEFINE_INFO>		m_EnumDefineList;
	

	CDlgEnumList(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgEnumList();

// 对话框数据
	enum { IDD = IDD_DIALOG_ENUM_LIST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
protected:
	void FillList();
	void SelectItemByName(LPCTSTR szName);
	void FillItem(int Item,ENUM_DEFINE_INFO& EnumDefine,size_t Index);
	virtual BOOL OnInitDialog();
	

	afx_msg void OnBnClickedButtonNew();
	afx_msg void OnBnClickedButtonEdit();
	afx_msg void OnBnClickedButtonDel();
public:
	afx_msg void OnNMDblclkListStruct(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonMoveUp();
	afx_msg void OnBnClickedButtonOveDown();
	
	afx_msg void OnBnClickedButtonClone();
	afx_msg void OnBnClickedButtonMove();
};
