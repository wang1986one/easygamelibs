/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DlgFileViewer.h                                          */
/*      ��������:  2011��11��09��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once
#include "afxcmn.h"


// CDlgFileViewer �Ի���

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

	CDlgFileViewer(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgFileViewer();

	void SetDocument(CUSOEditDoc * pDoc)
	{
		m_pUSOEditDoc=pDoc;
	}

// �Ի�������
	enum { IDD = IDD_DIALOG_FILE_VIEWER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
