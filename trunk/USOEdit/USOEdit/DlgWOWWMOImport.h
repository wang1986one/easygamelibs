/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DlgWOWWMOImport.h                                        */
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
/*      �ļ���:    DlgWOWWMOImport.h                                        */
/*      ��������:  2010��02��09��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once


// CDlgWOWWMOImport �Ի���

class CDlgWOWWMOImport : public CDialog
{
	DECLARE_DYNAMIC(CDlgWOWWMOImport)

public:
	CDlgWOWWMOImport(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgWOWWMOImport();

// �Ի�������
	enum { IDD = IDD_DIALOG_WMO_IMPORT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_ModelFileName;
	afx_msg void OnBnClickedSelectModelFile();
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
