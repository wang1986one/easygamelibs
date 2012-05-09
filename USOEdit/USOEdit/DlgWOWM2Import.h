/****************************************************************************/
/*                                                                          */
/*      文件名:    DlgWOWM2Import.h                                         */
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
/*      文件名:    DlgWOWM2Import.h                                         */
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


// CDlgWOWM2Import 对话框

class CDlgWOWM2Import : public CDialog
{
	DECLARE_DYNAMIC(CDlgWOWM2Import)

public:
	CDlgWOWM2Import(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgWOWM2Import();

// 对话框数据
	enum { IDD = IDD_DIALOG_WOW_M2_IMPORT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_ModelFileName;
	CString m_SkinFileName;
	

protected:
	
	afx_msg void OnBnClickedSelectModelFile();
	afx_msg void OnBnClickedSelectSkinFile();
	virtual BOOL OnInitDialog();
public:
	
	afx_msg void OnBnClickedOk();

};
