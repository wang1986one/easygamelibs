/****************************************************************************/
/*                                                                          */
/*      文件名:    RemoteConsoleView.h                                      */
/*      创建日期:  2009年02月09日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
// RemoteConsoleView.h : CRemoteConsoleView 类的接口
//


#pragma once
#include "afxwin.h"


class CRemoteConsoleView : public CFormView
{
protected: // 仅从序列化创建
	CDialogItemSorter	m_ItemSorter;
	CNetLink *			m_pNetLink;
	CServerLogPrinter*	m_pFileLog;

	CRemoteConsoleView();
	DECLARE_DYNCREATE(CRemoteConsoleView)

public:
	enum{ IDD = IDD_REMOTECONSOLE_FORM };

// 属性
public:
	CRemoteConsoleDoc* GetDocument() const;

	void SetNetLink(CNetLink *	pNetLink)
	{
		m_pNetLink=pNetLink;

		CEasyString LogFileName;
		CEasyString ModulePath=GetModulePath(NULL);		

		LogFileName.Format("%s\\Log\\%s",(LPCTSTR)ModulePath,m_pNetLink->GetRemoteAddress().GetIPString());
		m_pFileLog=new CServerLogPrinter(NULL,CServerLogPrinter::LOM_FILE,ILogPrinter::LOG_LEVEL_NORMAL|ILogPrinter::LOG_LEVEL_DEBUG,LogFileName);
	}

	void PrintLogMsg(LPCTSTR szMsg);
	void SetServerStatus(CSmartStruct& ServerStatus);
// 操作
public:

// 重写
	public:
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual void OnInitialUpdate(); // 构造后第一次调用

// 实现
public:
	virtual ~CRemoteConsoleView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	

	
// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedButtonSendCommand();
	afx_msg void OnEditCut();
	afx_msg void OnEditCopy();
	afx_msg void OnEditPaste();
	afx_msg void OnEditUndo();
	CEdit m_edCommand;
	CEdit m_edLog;
	
	afx_msg void OnTimer(UINT nIDEvent);
	int m_STClientCount;
	float m_STCycleTime;
	float m_STTCPRecvFlow;
	float m_STTCPSendFlow;
	float m_STUDPRecvFlow;
	float m_STUDPSendFlow;
};

#ifndef _DEBUG  // RemoteConsoleView.cpp 的调试版本
inline CRemoteConsoleDoc* CRemoteConsoleView::GetDocument() const
   { return reinterpret_cast<CRemoteConsoleDoc*>(m_pDocument); }
#endif

