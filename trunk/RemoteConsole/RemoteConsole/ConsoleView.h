/****************************************************************************/
/*                                                                          */
/*      �ļ���:    ConsoleView.h                                            */
/*      ��������:  2009��02��09��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once
#include "afxwin.h"
#include "afxcmn.h"



// CConsoleView ������ͼ

class CConsoleView : public CFormView
{
	DECLARE_DYNCREATE(CConsoleView)

protected:
	CConsoleView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
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

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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


