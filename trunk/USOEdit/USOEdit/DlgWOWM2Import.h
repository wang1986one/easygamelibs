/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DlgWOWM2Import.h                                         */
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
/*      �ļ���:    DlgWOWM2Import.h                                         */
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


// CDlgWOWM2Import �Ի���

class CDlgWOWM2Import : public CDialog
{
	DECLARE_DYNAMIC(CDlgWOWM2Import)

public:
	CDlgWOWM2Import(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgWOWM2Import();

// �Ի�������
	enum { IDD = IDD_DIALOG_WOW_M2_IMPORT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
