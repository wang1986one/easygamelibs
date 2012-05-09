/****************************************************************************/
/*                                                                          */
/*      文件名:    DlgWOWItemImport.h                                       */
/*      创建日期:  2011年10月07日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
/****************************************************************************/
/*                                                                          */
/*      文件名:    DlgWOWItemImport.h                                       */
/*      创建日期:  2009年09月11日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once
#include "afxwin.h"


// CDlgWOWItemImport 对话框

class CDlgWOWItemImport : public CDialog
{
protected:


	DECLARE_DYNAMIC(CDlgWOWItemImport)

public:
	CDlgWOWItemImport(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgWOWItemImport();

// 对话框数据
	enum { IDD = IDD_DIALOG_WOW_ITEM_IMPORT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString		m_ModelFileName;
	CString		m_SkinFileName;
	CComboBox	m_cbItemClass;
	CComboBox	m_cbItemSubClass;
	CComboBox	m_cbItem;
	CComboBox	m_cbItemHandType;
	UINT		m_ItemID;
	UINT		m_ItemDisplayID;
	int			m_ItemHandType;
	virtual BOOL OnInitDialog();
protected:
	afx_msg void OnBnClickedSelectModelFile();
	afx_msg void OnBnClickedSelectSkinFile();	
	afx_msg void OnCbnSelchangeComboItemClass();
	afx_msg void OnCbnSelchangeComboItemSubClass();
	afx_msg void OnCbnSelchangeComboItem();

	void ProcessItemClassChange();
	void ProcessItemSubClassChange();
	void ProcessItemChange();
public:
	
	
	afx_msg void OnCbnSelchangeComboItemHandType();
	CString m_FilterText;
};
