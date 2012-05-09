/****************************************************************************/
/*                                                                          */
/*      文件名:    DlgFileViewer.h                                          */
/*      创建日期:  2011年11月09日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once
#include "afxcmn.h"


// CDlgFileViewer 对话框

class CUSOEditDoc;

class CDlgFileViewer : public CDialog
{
	DECLARE_DYNAMIC(CDlgFileViewer)
protected:
	
	CEasyString			m_CurDir;
	int					m_IsInMPQ;
	CUSOEditDoc *		m_pUSOEditDoc;
	UINT				m_TreeNodeCount;
	UINT				m_TreeDataLen;
	CEasyArray<UINT>	m_Tree;
	CEasyBuffer			m_TreeData;
public:
	enum ITEM_TYPE
	{
		ITEM_TYPE_DIR,
		ITEM_TYPE_M2,
		ITEM_TYPE_WMO,
		ITEM_TYPE_BLP,
	};

	struct MPQ_FILE_TREE
	{
		BYTE						Type;
		CEasyString					Name;
		CEasyArray<MPQ_FILE_TREE>	Childs;
	};

	CDlgFileViewer(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgFileViewer();

	void SetDocument(CUSOEditDoc * pDoc)
	{
		m_pUSOEditDoc=pDoc;
	}

// 对话框数据
	enum { IDD = IDD_DIALOG_FILE_VIEWER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedSelectDir();
	CTreeCtrl m_tcFiles;
	afx_msg void OnTvnSelchangedTreeFile(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnItemexpandingTreeFile(NMHDR *pNMHDR, LRESULT *pResult);

protected:
	void FetchChild(HTREEITEM Item);
	void BrowserDir(LPCTSTR szDir);
	void BrowserMPQ();
	void AddFilePathToTree(HTREEITEM ParentItem,CEasyString FilePath,int FileType);
	void AddFilePathToTree(MPQ_FILE_TREE& FileTree,CEasyString FilePath,int FileType);

	UINT PushTreeData(MPQ_FILE_TREE& FileTree,CEasyArray<UINT>& Tree,CEasyBuffer& TreeData);
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBrowserMpq();
	afx_msg void OnBnClickedClosePreview();
};
