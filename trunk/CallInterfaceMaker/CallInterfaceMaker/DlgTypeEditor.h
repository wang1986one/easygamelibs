/****************************************************************************/
/*                                                                          */
/*      文件名:    DlgTypeEditor.h                                          */
/*      创建日期:  2012年04月22日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CDlgTypeEditor 对话框

class CDlgTypeEditor : public CDialog
{
	DECLARE_DYNAMIC(CDlgTypeEditor)
protected:
	CTabCtrl	m_tabMain;
	CListCtrl	m_lvTypeList;
	CString		m_TypeName;
	CString		m_CType;
	BOOL		m_IsFixLength;
	BOOL		m_NeedNullCheck;
	CEdit		m_edConstructOperation;
	CEdit		m_edEvaluateOperation;
	CEdit		m_edSizeCaculateOperation;
	CEdit		m_edPackOperation;
	CEdit		m_edUnpackOperation;
	CEdit		m_edNULLCheckOperation;
	CEdit		m_edReferenceDefine;
	bool		m_IsModified;
	int			m_CurSelectItem;
public:
	vector<PARAM_TYPE>		m_TypeList;


	CDlgTypeEditor(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgTypeEditor();

// 对话框数据
	enum { IDD = IDD_DIALOG_TYPE_EDIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	void ShowItemData(int Item);
	void FetchItemData(int Item);
	void FillList();

	DECLARE_MESSAGE_MAP()
public:
	
	virtual BOOL OnInitDialog();
	
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);	
	
	afx_msg void OnEnChangeEdit();	
	afx_msg void OnBnClickedCheckIsFixLength();
	afx_msg void OnBnClickedCheckNeedNullCheck();
	afx_msg void OnNMClickListTypes(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedNew();
	afx_msg void OnBnClickedDel();
};
