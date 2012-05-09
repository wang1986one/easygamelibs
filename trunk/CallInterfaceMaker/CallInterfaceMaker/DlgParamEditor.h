/****************************************************************************/
/*                                                                          */
/*      文件名:    DlgParamEditor.h                                         */
/*      创建日期:  2011年10月07日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once
#include "afxwin.h"


// CDlgParamEditor 对话框

class CDlgParamEditor : public CDialog
{
	DECLARE_DYNAMIC(CDlgParamEditor)

public:
	CDlgParamEditor(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgParamEditor();

// 对话框数据
	enum { IDD = IDD_DIALOG_PARAM_EDITOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
	virtual void OnCancel();
protected:	
	CString			m_Name;
	CComboBox		m_cbType;
	int				m_Type;
	int				m_Length;
	CString			m_Description;
	CString			m_DefaultValue;
	BOOL			m_CanNull;
	BOOL			m_IsReference;
	UINT			m_ID;
public:
	void SetID(UINT ID);
	void SetData(METHOD_PARAM& ParamInfo);
	void GetData(METHOD_PARAM& ParamInfo);
	
	
	
	
	
};

inline void CDlgParamEditor::SetID(UINT ID)
{
	m_ID=ID;
}

inline void CDlgParamEditor::SetData(METHOD_PARAM& ParamInfo)
{
	m_Name=ParamInfo.Name;
	m_Type=ParamInfo.Type;
	m_Length=ParamInfo.Length;
	m_DefaultValue=ParamInfo.DefaultValue;
	m_Description=ParamInfo.Description;
	m_CanNull=ParamInfo.CanNull;	
	m_IsReference=ParamInfo.IsReference;
	m_ID=ParamInfo.ID;
}

inline void CDlgParamEditor::GetData(METHOD_PARAM& ParamInfo)
{
	ParamInfo.Name=m_Name;
	ParamInfo.Type=m_Type;
	ParamInfo.Length=m_Length;
	ParamInfo.DefaultValue=m_DefaultValue;
	ParamInfo.Description=m_Description;
	ParamInfo.CanNull=m_CanNull;
	ParamInfo.IsReference=m_IsReference;
	ParamInfo.ID=m_ID;
}
