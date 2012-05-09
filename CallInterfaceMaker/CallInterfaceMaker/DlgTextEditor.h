/****************************************************************************/
/*                                                                          */
/*      文件名:    DlgTextEditor.h                                          */
/*      创建日期:  2011年10月07日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once


// CDlgTextEditor 对话框

class CDlgTextEditor : public CDialog
{
	DECLARE_DYNAMIC(CDlgTextEditor)

public:
	CDlgTextEditor(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgTextEditor();

// 对话框数据
	enum { IDD = IDD_DIALOG_TEXT_EDITOR };

protected:
	CString m_Text;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	void SetText(LPCTSTR szText)
	{
		m_Text=szText;
	}
	LPCTSTR GetText()
	{
		return m_Text;
	}
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
	virtual void OnCancel();
};
