#pragma once
#include "afxcmn.h"


// CDlgDataObjectModifyFlagEditor 对话框

class CDlgDataObjectModifyFlagEditor : public CDialog
{
	DECLARE_DYNAMIC(CDlgDataObjectModifyFlagEditor)
protected:
	CListCtrl					m_lvDataObjects;
	CListCtrl					m_lvDataObjectMembers;
	vector<STRUCT_DEFINE_INFO>	m_DataObjectList;
	int							m_CurSelectIndex;
public:
	DATA_OBJECT_MODIFY_FLAG			m_DataObjectModifyFlag;
	CDlgDataObjectModifyFlagEditor(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgDataObjectModifyFlagEditor();

// 对话框数据
	enum { IDD = IDD_DIALOG_DATA_OBJECT_MODIFY_FLAG_EDITOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	void FillDataObjectList();
	void FillDataObjectMemberList(size_t Index);
	void FetchDataObjectModifyFlag(size_t Index);
	MODIFY_FLAG_UNIT * FindModifyFlagUnit(DATA_OBJECT_MODIFY_FLAG& DataObjectModifyFlag,LPCTSTR ClassName);
	
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnNMClickListDataObjects(NMHDR *pNMHDR, LRESULT *pResult);
};
