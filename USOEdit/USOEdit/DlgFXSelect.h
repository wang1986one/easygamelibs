/****************************************************************************/
/*                                                                          */
/*      文件名:    DlgFXSelect.h                                            */
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
/*      文件名:    DlgFXSelect.h                                            */
/*      创建日期:  2009年07月06日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once
#include "afxcmn.h"
#include <vector>

// CDlgFXSelect 对话框
typedef std::vector<CD3DFX *> CFXPtrArray;

class CDlgFXSelect : public CPropertyConfigDialog
{
	DECLARE_DYNAMIC(CDlgFXSelect)

public:
	CDlgFXSelect(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgFXSelect();

// 对话框数据
	enum { IDD = IDD_FX_SELECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CListCtrl	m_lvFXList;
	CFXPtrArray m_FXPtrArray;
	CD3DFX *	m_SelectedFX;
	virtual BOOL OnInitDialog();
};
