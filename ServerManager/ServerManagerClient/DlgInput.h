#pragma once


// CDlgInput 对话框

class CDlgInput : public CDialog
{
	DECLARE_DYNAMIC(CDlgInput)

public:
	CDlgInput(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgInput();

// 对话框数据
	enum { IDD = IDD_DIALOG_INPUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_InputText;
	CString m_Title;
	virtual BOOL OnInitDialog();
};
