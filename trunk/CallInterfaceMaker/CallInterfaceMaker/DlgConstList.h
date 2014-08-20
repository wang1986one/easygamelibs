#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CDlgConstList �Ի���

class CDlgConstList : public CDialog
{
	DECLARE_DYNAMIC(CDlgConstList)
protected:
	CListCtrl			m_lvList;
	CComboBox			m_cbType;
	CONST_DEFINE_INFO	m_ConstDefine;
public:
	CString								m_ListName;
	vector<CONST_DEFINE_INFO>			m_ConstDefineList;
	
	

	CDlgConstList(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgConstList();

// �Ի�������
	enum { IDD = IDD_DIALOG_CONST_LIST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
protected:
	void FillList();
	void SelectItemByName(LPCTSTR szName);
	void FillItem(int Item,CONST_DEFINE_INFO& ConstDefine,size_t Index);
	virtual BOOL OnInitDialog();
	

	afx_msg void OnBnClickedButtonNew();
	afx_msg void OnBnClickedButtonEdit();
	afx_msg void OnBnClickedButtonDel();
public:
	afx_msg void OnBnClickedButtonMoveUp();
	afx_msg void OnBnClickedButtonOveDown();
	afx_msg void OnNMClickListEnum(NMHDR *pNMHDR, LRESULT *pResult);
	
	
	afx_msg void OnBnClickedButtonClone();
	afx_msg void OnBnClickedButtonMove();
};
