/****************************************************************************/
/*                                                                          */
/*      文件名:    DlgTrannferProgress.h                                    */
/*      创建日期:  2010年02月09日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once
#include "afxcmn.h"


// CDlgTrannferProgress 对话框

class CDlgTrannferProgress : public CDialog
{
protected:
	CProgressCtrl m_pgTransfer;
	CString m_TransferRate;
	CEasyTimer m_TansferTimer;

	DECLARE_DYNAMIC(CDlgTrannferProgress)

public:
	CDlgTrannferProgress(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgTrannferProgress();

// 对话框数据
	enum { IDD = IDD_DIALOG_TRANSFER_PROGRESS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	void InitProgress(int TotalSize);
	void UpdateProgress(int FinishSize);
	
};
