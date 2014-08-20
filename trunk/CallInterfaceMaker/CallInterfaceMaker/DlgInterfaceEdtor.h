/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DlgInterfaceEdtor.h                                      */
/*      ��������:  2011��10��07��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CDlgInterfaceEdtor �Ի���

class CDlgInterfaceEdtor : public CDialog
{
	DECLARE_DYNAMIC(CDlgInterfaceEdtor)
protected:
	CALLER_INTERFACE				m_InterfaceInfo;
	CListCtrl						m_lvMethodList;
	CButton							m_rdIsCall;
	CButton							m_rdIsCallBack;
public:
	CDlgInterfaceEdtor(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgInterfaceEdtor();

// �Ի�������
	enum { IDD = IDD_DIALOG_INTERFACE_EDITOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
	void SelectItemByName(LPCTSTR szName);

public:
	void SetData(CALLER_INTERFACE& InterfaceInfo);
	void SetID(UINT ID);
	void GetData(CALLER_INTERFACE& InterfaceInfo);	
	
	afx_msg void OnNMDblclkListMethod(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonMoveUp();
	afx_msg void OnBnClickedButtonMoveDown();
};

inline void CDlgInterfaceEdtor::SetData(CALLER_INTERFACE& InterfaceInfo)
{
	m_InterfaceInfo=InterfaceInfo;	
}
inline void CDlgInterfaceEdtor::SetID(UINT ID)
{
	m_InterfaceInfo.ID=ID;
}
inline void CDlgInterfaceEdtor::GetData(CALLER_INTERFACE& InterfaceInfo)
{
	InterfaceInfo=m_InterfaceInfo;
}