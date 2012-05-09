/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DlgSenceTreeView.h                                       */
/*      ��������:  2011��10��07��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once
#include "afxcmn.h"


// CDlgSenceTreeView �Ի���

class CDlgSenceTreeView : public CDialog
{
	DECLARE_DYNAMIC(CDlgSenceTreeView)
	CD3DViewPort *		m_pView;
	CUSOEditDoc *		m_pDoc;
	CTreeCtrl			m_tvSenceTree;
public:
	CDlgSenceTreeView(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgSenceTreeView();

// �Ի�������
	enum { IDD = IDD_DIALOG_SENCE_TREE_VIEW };

	void Init(CUSOEditDoc * pDoc,CD3DViewPort * pView)
	{
		m_pDoc=pDoc;
		m_pView=pView;
	}

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
