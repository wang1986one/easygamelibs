/****************************************************************************/
/*                                                                          */
/*      文件名:    DlgSenceTreeView.h                                       */
/*      创建日期:  2011年10月07日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once
#include "afxcmn.h"


// CDlgSenceTreeView 对话框

class CDlgSenceTreeView : public CDialog
{
	DECLARE_DYNAMIC(CDlgSenceTreeView)
	CD3DViewPort *		m_pView;
	CUSOEditDoc *		m_pDoc;
	CTreeCtrl			m_tvSenceTree;
public:
	CDlgSenceTreeView(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgSenceTreeView();

// 对话框数据
	enum { IDD = IDD_DIALOG_SENCE_TREE_VIEW };

	void Init(CUSOEditDoc * pDoc,CD3DViewPort * pView)
	{
		m_pDoc=pDoc;
		m_pView=pView;
	}

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonRefresh();
	afx_msg void OnBnClickedButtonRenderSelected();

protected:
	void FillTree(CD3DSceneRender::TREE_NODE * pRoot,HTREEITEM hTreeRoot);
public:
	
	afx_msg void OnBnClickedButtonHideCutObject();
	afx_msg void OnBnClickedButtonShowObjectNode();

protected:
	HTREEITEM FindTree(HTREEITEM hRoot,CD3DSceneRender::TREE_NODE * pNode);
public:
	afx_msg void OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult);
};
