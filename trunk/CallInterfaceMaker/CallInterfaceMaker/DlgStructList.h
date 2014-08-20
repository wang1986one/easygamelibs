#pragma once
#include "afxcmn.h"


// CDlgStructList 对话框

class CDlgStructList : public CDialog
{
	DECLARE_DYNAMIC(CDlgStructList)
protected:
	CListCtrl	m_lvStruct;
public:
	CString							m_ListName;
	vector<STRUCT_DEFINE_INFO>		m_StructDefineList;
	

	CDlgStructList(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgStructList();

// 对话框数据
	enum { IDD = IDD_DIALOG_STRUCT_LIST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
protected:
	void FillList();
	void SelectItemByName(LPCTSTR szName);
	void FillItem(int Item,STRUCT_DEFINE_INFO& StructDefine,size_t Index);
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
