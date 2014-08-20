#pragma once
#include "afxcmn.h"


// CDlgDataObjectModifyFlagManager �Ի���

class CDlgDataObjectModifyFlagManager : public CDialog
{
	DECLARE_DYNAMIC(CDlgDataObjectModifyFlagManager)
protected:
	CListCtrl							m_lvList;
public:
	vector<DATA_OBJECT_MODIFY_FLAG>		m_DataObjectModifyFlags;
	CDlgDataObjectModifyFlagManager(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgDataObjectModifyFlagManager();

// �Ի�������
	enum { IDD = IDD_DIALOG_DATA_OBJECT_MODIFY_FLAG_MANAGER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
protected:
	void FillList();
	void SelectItemByName(LPCTSTR szName);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonNew();
	afx_msg void OnBnClickedButtonEdit();
	afx_msg void OnBnClickedButtonDel();
	afx_msg void OnNMDblclkList(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnBnClickedButtonMoveUp();
	afx_msg void OnBnClickedButtonMoveDown();
	afx_msg void OnBnClickedButtonMoveSort();

	MODIFY_FLAG_UNIT * FindModifyFlagUnit(DATA_OBJECT_MODIFY_FLAG& DataObjectModifyFlag,LPCTSTR ClassName);
};
