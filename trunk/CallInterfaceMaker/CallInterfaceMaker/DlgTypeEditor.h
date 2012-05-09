/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DlgTypeEditor.h                                          */
/*      ��������:  2012��04��22��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CDlgTypeEditor �Ի���

class CDlgTypeEditor : public CDialog
{
	DECLARE_DYNAMIC(CDlgTypeEditor)
protected:
	CTabCtrl	m_tabMain;
	CListCtrl	m_lvTypeList;
	CString		m_TypeName;
	CString		m_CType;
	BOOL		m_IsFixLength;
	BOOL		m_NeedNullCheck;
	CEdit		m_edConstructOperation;
	CEdit		m_edEvaluateOperation;
	CEdit		m_edSizeCaculateOperation;
	CEdit		m_edPackOperation;
	CEdit		m_edUnpackOperation;
	CEdit		m_edNULLCheckOperation;
	CEdit		m_edReferenceDefine;
	bool		m_IsModified;
	int			m_CurSelectItem;
public:
	vector<PARAM_TYPE>		m_TypeList;


	CDlgTypeEditor(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgTypeEditor();

// �Ի�������
	enum { IDD = IDD_DIALOG_TYPE_EDIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	void ShowItemData(int Item);
	void FetchItemData(int Item);
	void FillList();

	DECLARE_MESSAGE_MAP()
public:
	
	virtual BOOL OnInitDialog();
	
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);	
	
	afx_msg void OnEnChangeEdit();	
	afx_msg void OnBnClickedCheckIsFixLength();
	afx_msg void OnBnClickedCheckNeedNullCheck();
	afx_msg void OnNMClickListTypes(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedNew();
	afx_msg void OnBnClickedDel();
};
