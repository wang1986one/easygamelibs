/****************************************************************************/
/*                                                                          */
/*      文件名:    DlgWOWCharImport.h                                       */
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
/*      文件名:    DlgWOWCharImport.h                                       */
/*      创建日期:  2010年02月09日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once
#include "afxwin.h"


// CDlgWOWCharImport 对话框

class CDlgWOWCharImport : public CDialog
{
	DECLARE_DYNAMIC(CDlgWOWCharImport)

public:
	CDlgWOWCharImport(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgWOWCharImport();

// 对话框数据
	enum { IDD = IDD_DIALOG_WOW_CHAR_IMPORT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:	
	int m_Race;	
	int m_Sex;
	

protected:
	CComboBox m_cbRace;
	CComboBox m_cbSex;		
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnCbnSelchangeCombo1();	
	afx_msg void OnBnClickedOk();
protected:
	void ProcessRaceChange();	
};
