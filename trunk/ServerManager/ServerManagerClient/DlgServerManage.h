/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DlgServerManage.h                                        */
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


// CDlgServerManage �Ի���

class CDlgServerManage : public CDialog
{
protected:
	CEasyArray<SERVER_INFO>		m_ServerList;
	CListCtrl					m_lvServerList;
	CString						m_IP;
	UINT						m_Port;


	DECLARE_DYNAMIC(CDlgServerManage)

public:
	CDlgServerManage(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgServerManage();

// �Ի�������
	enum { IDD = IDD_DIALOG_SERVER_MANAGE };

	void SetServerList(CEasyArray<SERVER_INFO>& ServerList)
	{
		m_ServerList=ServerList;
	}
	CEasyArray<SERVER_INFO>& GetServerList()
	{
		return m_ServerList;
	}

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

	
	afx_msg void OnBnClickedAddServer();
	afx_msg void OnBnClickedDelServer();
	afx_msg void OnBnClickedEditServer();
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
public:
//	afx_msg void OnHdnItemdblclickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
};
