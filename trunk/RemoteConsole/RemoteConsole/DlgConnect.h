#pragma once


// CDlgConnect �Ի���

class CDlgConnect : public CDialog
{
	DECLARE_DYNAMIC(CDlgConnect)

public:
	CDlgConnect(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgConnect();

// �Ի�������
	enum { IDD = IDD_DIALOG_CONNECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_Address;
	UINT m_Port;
};
