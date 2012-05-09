/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DlgParamEditor.h                                         */
/*      ��������:  2011��10��07��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once
#include "afxwin.h"


// CDlgParamEditor �Ի���

class CDlgParamEditor : public CDialog
{
	DECLARE_DYNAMIC(CDlgParamEditor)

public:
	CDlgParamEditor(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgParamEditor();

// �Ի�������
	enum { IDD = IDD_DIALOG_PARAM_EDITOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
