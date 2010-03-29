/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DlgTrannferProgress.h                                    */
/*      ��������:  2010��02��09��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once
#include "afxcmn.h"


// CDlgTrannferProgress �Ի���

class CDlgTrannferProgress : public CDialog
{
protected:
	CProgressCtrl m_pgTransfer;
	CString m_TransferRate;
	CEasyTimer m_TansferTimer;

	DECLARE_DYNAMIC(CDlgTrannferProgress)

public:
	CDlgTrannferProgress(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgTrannferProgress();

// �Ի�������
	enum { IDD = IDD_DIALOG_TRANSFER_PROGRESS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	void InitProgress(int TotalSize);
	void UpdateProgress(int FinishSize);
	
};
