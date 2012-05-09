/****************************************************************************/
/*                                                                          */
/*      文件名:    DlgInterfaceEdtor.h                                      */
/*      创建日期:  2011年10月07日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CDlgInterfaceEdtor 对话框

class CDlgInterfaceEdtor : public CDialog
{
	DECLARE_DYNAMIC(CDlgInterfaceEdtor)
protected:
	vector<INTERFACE_METHOD>		m_MethodList;
	CListCtrl						m_lvMethodList;
	CString							m_Name;
	CString							m_Description;
	int								m_InterfaceType;
	CButton							m_rdIsCall;
	CButton							m_rdIsCallBack;
	CString							m_ID;
public:
	CDlgInterfaceEdtor(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgInterfaceEdtor();

// 对话框数据
	enum { IDD = IDD_DIALOG_INTERFACE_EDITOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
protected:
	afx_msg void OnBnClickedButtonAddMethod();
	afx_msg void OnBnClickedButtonEditMethod();
	afx_msg void OnBnClickedButtonDelMethod();

	virtual void OnOK();
	virtual void OnCancel();

	void AddListItem(INTERFACE_METHOD& MethodInfo,UINT Index);
	void EditListItem(int Item,INTERFACE_METHOD& MethodInfo);
	void FillListItem();


public:
	void SetData(CALLER_INTERFACE& InterfaceInfo);
	void GetData(CALLER_INTERFACE& InterfaceInfo);	
	
	afx_msg void OnNMDblclkListMethod(NMHDR *pNMHDR, LRESULT *pResult);
};

inline void CDlgInterfaceEdtor::SetData(CALLER_INTERFACE& InterfaceInfo)
{
	m_Name=InterfaceInfo.Name;
	m_MethodList=InterfaceInfo.MethodList;
	m_Description=InterfaceInfo.Description;
	m_InterfaceType=InterfaceInfo.Type;
	m_ID=InterfaceInfo.ID;
}
inline void CDlgInterfaceEdtor::GetData(CALLER_INTERFACE& InterfaceInfo)
{
	InterfaceInfo.Name=m_Name;
	InterfaceInfo.MethodList=m_MethodList;
	InterfaceInfo.Description=m_Description;
	InterfaceInfo.Type=m_InterfaceType;
	InterfaceInfo.ID=m_ID;
}