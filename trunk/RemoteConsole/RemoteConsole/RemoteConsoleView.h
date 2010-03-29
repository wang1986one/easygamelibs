/****************************************************************************/
/*                                                                          */
/*      �ļ���:    RemoteConsoleView.h                                      */
/*      ��������:  2009��02��09��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
// RemoteConsoleView.h : CRemoteConsoleView ��Ľӿ�
//


#pragma once
#include "afxwin.h"


class CRemoteConsoleView : public CFormView
{
protected: // �������л�����
	CDialogItemSorter	m_ItemSorter;
	CNetLink *			m_pNetLink;
	CServerLogPrinter*	m_pFileLog;

	CRemoteConsoleView();
	DECLARE_DYNCREATE(CRemoteConsoleView)

public:
	enum{ IDD = IDD_REMOTECONSOLE_FORM };

// ����
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
// ����
public:

// ��д
	public:
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual void OnInitialUpdate(); // ������һ�ε���

// ʵ��
public:
	virtual ~CRemoteConsoleView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	

	
// ���ɵ���Ϣӳ�亯��
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

#ifndef _DEBUG  // RemoteConsoleView.cpp �ĵ��԰汾
inline CRemoteConsoleDoc* CRemoteConsoleView::GetDocument() const
   { return reinterpret_cast<CRemoteConsoleDoc*>(m_pDocument); }
#endif

