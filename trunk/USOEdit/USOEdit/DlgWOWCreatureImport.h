/****************************************************************************/
/*                                                                          */
/*      文件名:    DlgWOWCreatureImport.h                                   */
/*      创建日期:  2011年10月07日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// DlgWOWCreatureImport 对话框

class DlgWOWCreatureImport : public CDialog
{
	DECLARE_DYNAMIC(DlgWOWCreatureImport)

public:
	DlgWOWCreatureImport(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~DlgWOWCreatureImport();

	UINT	m_CreatureDisplayID;	
// 对话框数据
	enum { IDD = IDD_DIALOG_WOW_CREATURE_IMPORT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
protected:
	CListCtrl m_lvCreatureList;
	CString m_FilterText;
	virtual BOOL OnInitDialog();
	void FillList(LPCTSTR szFilter);
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedFilter();
};
