/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DlgWOWItemImport.h                                       */
/*      ��������:  2011��10��07��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DlgWOWItemImport.h                                       */
/*      ��������:  2009��09��11��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once
#include "afxwin.h"


// CDlgWOWItemImport �Ի���

class CDlgWOWItemImport : public CDialog
{
protected:


	DECLARE_DYNAMIC(CDlgWOWItemImport)

public:
	CDlgWOWItemImport(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgWOWItemImport();

// �Ի�������
	enum { IDD = IDD_DIALOG_WOW_ITEM_IMPORT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString		m_ModelFileName;
	CString		m_SkinFileName;
	CComboBox	m_cbItemClass;
	CComboBox	m_cbItemSubClass;
	CComboBox	m_cbItem;
	CComboBox	m_cbItemHandType;
	UINT		m_ItemID;
	UINT		m_ItemDisplayID;
	int			m_ItemHandType;
	virtual BOOL OnInitDialog();
protected:
	afx_msg void OnBnClickedSelectModelFile();
	afx_msg void OnBnClickedSelectSkinFile();	
	afx_msg void OnCbnSelchangeComboItemClass();
	afx_msg void OnCbnSelchangeComboItemSubClass();
	afx_msg void OnCbnSelchangeComboItem();

	void ProcessItemClassChange();
	void ProcessItemSubClassChange();
	void ProcessItemChange();
public:
	
	
	afx_msg void OnCbnSelchangeComboItemHandType();
	CString m_FilterText;
};
