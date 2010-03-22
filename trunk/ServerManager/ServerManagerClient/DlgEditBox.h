#pragma once


// CDlgEditBox 对话框

class CDlgEditBox : public CDialog
{
	DECLARE_DYNAMIC(CDlgEditBox)

public:
	CDlgEditBox(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgEditBox();

// 对话框数据
	enum { IDD = IDD_DIALOG_EDIT_BOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_Text;
	virtual BOOL OnInitDialog();
};
