/****************************************************************************/
/*                                                                          */
/*      文件名:    DlgPathListSetting.h                                     */
/*      创建日期:  2011年10月07日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
/****************************************************************************/
/*                                                                          */
/*      文件名:    DlgPathListSetting.h                                     */
/*      创建日期:  2009年07月06日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once
#include "afxwin.h"


// CDlgPathListSetting 对话框

class CDlgPathListSetting : public CDialog
{
	DECLARE_DYNAMIC(CDlgPathListSetting)

public:
	CDlgPathListSetting(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgPathListSetting();

// 对话框数据
	enum { IDD = IDD_SET_PATH_LIST };
	enum WORK_MODE
	{
		WORK_MODE_PATH_LIST,
		WORK_MODE_FILE_LIST,
	};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	CString m_PathListText;
	
public:
	CEasyArray<CEasyString> m_PathList;
	int m_WorkMode;
	CString m_Title;	
	afx_msg void OnBnClickedAdd();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnEditCut();
	afx_msg void OnEditCopy();
	afx_msg void OnEditPaste();
	CEdit m_edPathListText;
	afx_msg void OnEditUndo();
protected:
	virtual void OnOK();
};
