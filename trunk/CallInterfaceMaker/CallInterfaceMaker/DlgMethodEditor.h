/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DlgMethodEditor.h                                        */
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


// CDlgMethodEditor �Ի���

class CDlgMethodEditor : public CDialog
{
	DECLARE_DYNAMIC(CDlgMethodEditor)
protected:
	CListCtrl					m_lvParamList;
	vector<METHOD_PARAM>		m_ParamList;
	CString						m_Name;
	CString						m_Description;
	UINT						m_ID;
	UINT						m_ParamIDSeed;
public:
	CDlgMethodEditor(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgMethodEditor();

// �Ի�������
	enum { IDD = IDD_DIALOG_METHOD_EDITOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
protected:
	void AddListItem(METHOD_PARAM& ParamInfo,UINT Index);
	void EditListItem(int Item,METHOD_PARAM& ParamInfo);
	void FillListItem();
	void SelectItemByName(LPCTSTR szName);
	virtual void OnOK();
	virtual void OnCancel();
	
	afx_msg void OnBnClickedButtonAddParam();
	afx_msg void OnBnClickedButtonEditParam();
	afx_msg void OnBnClickedButtonDelParam();
public:
	void SetData(INTERFACE_METHOD& MethodInfo);
	void SetID(UINT ID);
	void GetData(INTERFACE_METHOD& MethodInfo);
	
	
	
	afx_msg void OnBnClickedButtonParamUp();
	afx_msg void OnBnClickedButtonParamDown();
	afx_msg void OnNMDblclkListParam(NMHDR *pNMHDR, LRESULT *pResult);
};

inline void CDlgMethodEditor::SetData(INTERFACE_METHOD& MethodInfo)
{
	m_Name=MethodInfo.Name;
	m_ParamList=MethodInfo.ParamList;
	m_Description=MethodInfo.Description;
	m_ID=MethodInfo.ID;
	m_ParamIDSeed=MethodInfo.ParamIDSeed;
}
inline void CDlgMethodEditor::SetID(UINT ID)
{
	m_ID=ID;
}
inline void CDlgMethodEditor::GetData(INTERFACE_METHOD& MethodInfo)
{
	MethodInfo.Name=m_Name;
	MethodInfo.ParamList=m_ParamList;
	MethodInfo.Description=m_Description;
	MethodInfo.ID=m_ID;
	MethodInfo.ParamIDSeed=m_ParamIDSeed;
}	