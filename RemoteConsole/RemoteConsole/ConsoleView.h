/****************************************************************************/
/*                                                                          */
/*      文件名:    ConsoleView.h                                            */
/*      创建日期:  2009年02月09日                                           */
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



// CConsoleView 窗体视图

class CConsoleView : public CFormView
{
	DECLARE_DYNCREATE(CConsoleView)

protected:
	CConsoleView();           // 动态创建所使用的受保护的构造函数
	virtual ~CConsoleView();

public:
	enum { IDD = IDD_CONSOLEVIEW };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	CDialogItemSorter	m_ItemSorter;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();

	void PrintLogMsg(LPCTSTR szMsg);

protected:
	afx_msg LRESULT OnConsoleMsg(WPARAM wParam, LPARAM lParam);

	void FillList();
public:
	CEdit m_edLog;
	afx_msg void OnSize(UINT nType, int cx, int cy);

//	afx_msg void OnClose();
	CListCtrl m_lvLinks;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};


