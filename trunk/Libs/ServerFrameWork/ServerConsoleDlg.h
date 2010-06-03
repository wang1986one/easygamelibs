/****************************************************************************/
/*                                                                          */
/*      �ļ���:    ServerConsoleDlg.h                                       */
/*      ��������:  2009��07��06��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
// SuperStarServerDlg.h : ͷ�ļ�
//

#pragma once





#define WM_TRAY_ICON_NOTIFY	WM_USER+1122
// CServerConsoleDlg �Ի���
class CServerConsoleDlg : public CDialog
{
// ����
public:
	CServerConsoleDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CServerConsoleDlg();

	void RegisterTrayIcon(LPCTSTR szTip);
	void UnregisterTrayIcon();

// �Ի�������
	enum { IDD = IDD_SERVER_CONSOLE };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

	
// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnConsoleMsg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnTryIconNotify(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

protected:
	void OnOK();
	void OnCancel();
	//ȡ�ð汾
	void ShowVersion();

protected:
	virtual void OnCommand(LPCTSTR szCommand);
	virtual void OnConsoleMsg(PANEL_MSG * pMsg);
	virtual void OnLogMsg(LPCTSTR szLogMsg);
	void OnTimer(UINT nIDEvent);
	void OnBnClickedExecCommand();
	afx_msg void OnBnClickedCloseServer();

	CEdit m_edMsgWnd;	
	CString m_CycleTime;
	CString m_TCPRecv;
	CString m_TCPSend;
	CString m_UDPRecv;
	CString m_UCPSend;
	CString	m_Command;

	CEasyBuffer m_ServerStatus;
	CDlgServerStatus m_DlgServerStatus;
	
public:
	afx_msg void OnBnClickedShowServerStatus();
};
