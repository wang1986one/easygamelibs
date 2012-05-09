/****************************************************************************/
/*                                                                          */
/*      �ļ���:    CallInterfaceMakerDlg.h                                  */
/*      ��������:  2012��04��22��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
// CallInterfaceMakerDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"


// CCallInterfaceMakerDlg �Ի���
class CCallInterfaceMakerDlg : public CDialog
{
// ����
protected:
	CListCtrl						m_lvInterfaceList;
	vector<CALLER_INTERFACE>		m_InterfaceList;
	vector<PARAM_TYPE>				m_ParamTypeList;
	CString							m_InterfacesFileName;
	CString							m_InterfacePrefix;
	CString							m_InterfacePostFix;
	CString							m_Name;
	INTERFACE_CONFIG				m_InterfaceConfig;
	CString							m_TypeDefFileName;

	CString							m_ProtocolFileTemplate;
	CString							m_InterfaceTemplate;

	CString							m_ParamTemple;
	CString							m_ParamDescriptionTemple;

	CString							m_MethodHeaderTemple;
	CString							m_MethodHeaderPureTailTemple;
	CString							m_MethodCallerSourceTemple;
	CString							m_MsgPackMethodHeaderTemple;
	CString							m_MsgPackMethodSourceTemple;
	CString							m_MsgHandlerMethodHeaderTemple;
	CString							m_MsgHandlerMethodSourceTemple;

	CString							m_MsgMapInitTemple;
	CString							m_MethodSSTIDEnumDefineTemple;

	CString							m_MsgCallerHeaderTemplate;
	CString							m_MsgCallerSourceTemplate;
	
	CString							m_MsgHandlerHeaderTemplate;
	CString							m_MsgHandlerSourceTemplate;	

	CString							m_ID;
public:
	CCallInterfaceMakerDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_CALLINTERFACEMAKER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
protected:
	
	afx_msg void OnBnClickedButtonAddInterface();
	afx_msg void OnBnClickedButtonEditInterface();
	afx_msg void OnBnClickedButtonDelInterface();
	afx_msg void OnBnClickedButtonEditPrefix();
	afx_msg void OnBnClickedButtonEditPostfix();
	afx_msg void OnBnClickedButtonLoad();
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedButtonSaveAs();
	afx_msg void OnBnClickedButtonSaveExport();

	void AddListItem(CALLER_INTERFACE& InterfaceInfo,UINT Index);
	void EditListItem(int Item,CALLER_INTERFACE& InterfaceInfo);
	bool IsListItemChecked(UINT Index);
	void FillListItem();
	bool LoadTypeDef(LPCTSTR FileName);
	bool SaveTypeDef(LPCTSTR FileName);
	bool LoadTemples();
	bool LoadConfig();
	bool SaveConfig();
public:
	UINT GetParamTypeCount();
	PARAM_TYPE * GetParamType(UINT Index);
	UINT FindTypeIndex(LPCTSTR Type);
	
protected:
	virtual void OnCancel();
	virtual void OnOK();

	bool Save(LPCTSTR FileName);
	bool Load(LPCTSTR FileName);

	CString EncodeString(CString Str);
	CString DecodeString(CString Str);

	bool ExportProtocolFile(LPCTSTR ExportDir,LPCTSTR FileName);
	bool ExportInterfaces(LPCTSTR ExportDir);

	bool ExportMsgCaller(LPCTSTR ExportDir,CALLER_INTERFACE& InterfaceInfo);
	bool ExportMsgHandler(LPCTSTR ExportDir,CALLER_INTERFACE& InterfaceInfo);

	

	CString MakeMethods(vector<INTERFACE_METHOD> MethodList,bool IsPure);
	CString MakeMethodSSTIDEnumDefines(CALLER_INTERFACE& InterfaceInfo);
	CString MakeMsgPackMethods(vector<INTERFACE_METHOD> MethodList);
	CString MakeParams(vector<METHOD_PARAM>& ParamList,bool IsDefine,bool IncludeDefaultValue);
	CString MakeParamDescriptions(vector<METHOD_PARAM>& ParamList);
	CString MakeParamDefines(vector<METHOD_PARAM>& ParamList);

	CString MakeMethodsSource(CALLER_INTERFACE& InterfaceInfo);
	CString MakeMsgPackMethodsSource(CALLER_INTERFACE& InterfaceInfo);

	CString MakePackOperations(INTERFACE_METHOD MethodInfo);
	CString MakeUnpackOperations(INTERFACE_METHOD MethodInfo);
	CString MakePackSizes(INTERFACE_METHOD MethodInfo);


	CString MakeMsgHandlerMethods(CALLER_INTERFACE& InterfaceInfo);
	CString MakeMsgHandlerMethodsSource(CALLER_INTERFACE& InterfaceInfo);

	CString MakeMsgMapInits(CALLER_INTERFACE& InterfaceInfo);

	
	
public:
	
	
	
	
	afx_msg void OnBnClickedButtonTypeDef();
	afx_msg void OnNMDblclkInterfaceList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonConfig();
};


inline UINT CCallInterfaceMakerDlg::GetParamTypeCount()
{
	return m_ParamTypeList.size();
}
inline PARAM_TYPE * CCallInterfaceMakerDlg::GetParamType(UINT Index)
{
	if(Index<m_ParamTypeList.size())
	{
		return &m_ParamTypeList.at(Index);
	}
	return NULL;
}

inline UINT CCallInterfaceMakerDlg::FindTypeIndex(LPCTSTR Type)
{
	for(UINT i=0;i<m_ParamTypeList.size();i++)
	{
		if(m_ParamTypeList[i].Name==Type)
		{
			return i;
		}
	}
	return -1;
}