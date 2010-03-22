#pragma once


// CDlgConnect 对话框

class CDlgConnect : public CDialog
{
	DECLARE_DYNAMIC(CDlgConnect)

public:
	CDlgConnect(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgConnect();

// 对话框数据
	enum { IDD = IDD_DIALOG_CONNECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_Address;
	UINT m_Port;
};
