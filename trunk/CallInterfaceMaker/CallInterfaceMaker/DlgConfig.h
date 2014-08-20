/****************************************************************************/
/*                                                                          */
/*      文件名:    DlgConfig.h                                              */
/*      创建日期:  2012年04月20日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once
#include "afxcmn.h"


// CDlgConfig 对话框

class CDlgConfig : public CDialog
{
	DECLARE_DYNAMIC(CDlgConfig)

public:
	CDlgConfig(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgConfig();

// 对话框数据
	enum { IDD = IDD_DIALOG_CONFIG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	INTERFACE_CONFIG m_Config;
protected:		
	bool					m_IsModified;
public:


	virtual BOOL OnInitDialog();
	virtual void OnOK();
protected:
	virtual void OnCancel();
	afx_msg void OnEnChangeEdit();
public:
	afx_msg void OnBnClickedButton1DefaultStructGenerateOperations();
	afx_msg void OnBnClickedButtonArrayDefineConfig();
};

	
