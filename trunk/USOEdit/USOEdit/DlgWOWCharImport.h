/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DlgWOWCharImport.h                                       */
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
/*      �ļ���:    DlgWOWCharImport.h                                       */
/*      ��������:  2010��02��09��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once
#include "afxwin.h"


// CDlgWOWCharImport �Ի���

class CDlgWOWCharImport : public CDialog
{
	DECLARE_DYNAMIC(CDlgWOWCharImport)

public:
	CDlgWOWCharImport(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgWOWCharImport();

// �Ի�������
	enum { IDD = IDD_DIALOG_WOW_CHAR_IMPORT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
