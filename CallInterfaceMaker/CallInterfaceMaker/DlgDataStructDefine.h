#pragma once
#include "afxcmn.h"


// CDlgDataStructDefine 对话框

class CDlgDataStructDefine : public CDialog
{
	DECLARE_DYNAMIC(CDlgDataStructDefine)
protected:
	CListCtrl				m_lvList;
public:
	DATA_STRUCT_DEFINES *						m_pDataStructDefines;

	CDlgDataStructDefine(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgDataStructDefine();

// 对话框数据
	enum { IDD = IDD_DIALOG_DATA_STRUCT_DEFINE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void FillList();
	void SelectItemByName(LPCTSTR szName,LPCTSTR szSubName);
	afx_msg void OnBnClickedButtonNewConst();
	afx_msg void OnBnClickedButtonNewEnum();
	afx_msg void OnBnClickedButtonNewStruct();
	afx_msg void OnBnClickedButtonEdit();
	afx_msg void OnBnClickedButtonDel();
	afx_msg void OnBnClickedButtonMoveUp();
	afx_msg void OnBnClickedButtonOveDown();
	afx_msg void OnBnClickedDataObjectModifyFlagsManager();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonBaseTypeDef();
	
	afx_msg void OnNMDblclkList(NMHDR *pNMHDR, LRESULT *pResult);
};
