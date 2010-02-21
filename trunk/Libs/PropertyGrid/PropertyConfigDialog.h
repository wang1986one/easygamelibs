#pragma once


// CPropertyConfigDialog 对话框
namespace PropertyGrid
{
class CPropertyConfigDialog : public CDialog
{
	DECLARE_DYNAMIC(CPropertyConfigDialog)

public:
	CPropertyConfigDialog(UINT nIDTemplate, CWnd* pParentWnd = NULL);   // 标准构造函数
	virtual ~CPropertyConfigDialog();

// 对话框数据
	bool IsModual()
	{
		return m_IsModual;
	}

protected:
	bool	m_IsModual;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
}