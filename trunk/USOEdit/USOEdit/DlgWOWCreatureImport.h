/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DlgWOWCreatureImport.h                                   */
/*      ��������:  2011��10��07��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// DlgWOWCreatureImport �Ի���

class DlgWOWCreatureImport : public CDialog
{
	DECLARE_DYNAMIC(DlgWOWCreatureImport)

public:
	DlgWOWCreatureImport(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~DlgWOWCreatureImport();

	UINT	m_CreatureDisplayID;	
// �Ի�������
	enum { IDD = IDD_DIALOG_WOW_CREATURE_IMPORT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
protected:
	CListCtrl m_lvCreatureList;
	CString m_FilterText;
	virtual BOOL OnInitDialog();
	void FillList(LPCTSTR szFilter);
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedFilter();
};
