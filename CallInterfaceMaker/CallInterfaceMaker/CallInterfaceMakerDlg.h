/****************************************************************************/
/*                                                                          */
/*      文件名:    CallInterfaceMakerDlg.h                                  */
/*      创建日期:  2012年04月22日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
// CallInterfaceMakerDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"


// CCallInterfaceMakerDlg 对话框
class CCallInterfaceMakerDlg : public CDialog
{
// 构造
protected:
	CListCtrl								m_lvInterfaceList;
	vector<CALLER_INTERFACE>				m_InterfaceList;

	DATA_STRUCT_DEFINES						m_DataStructDefines;


	UINT									m_InterfaceIDSeed;
	CString									m_InterfacesFileName;
	CString									m_InterfacePrefix;
	CString									m_InterfacePostFix;
	CString									m_Name;
	INTERFACE_CONFIG						m_InterfaceConfig;
	RUN_ENV_INFO							m_RunEnvInfo;
	CString									m_TypeDefFileName;

	CString									m_InterfaceHeaderTemplate;
	CString									m_InterfaceTemplate;

	CString									m_ParamTemple;
	CString									m_ParamDescriptionTemple;

	CString									m_PackSizeTemple;

	CString									m_MethodHeaderTemple;
	CString									m_MethodHeaderPureTailTemple;
	CString									m_MethodCallerSourceTemple;
	CString									m_MsgPackMethodHeaderTemple;
	CString									m_MsgPackMethodSourceTemple;
	CString									m_MsgHandlerMethodHeaderTemple;
	CString									m_MsgHandlerMethodSourceTemple;
	CString									m_DataObjectDefineHeaderTemple;
	CString									m_DataObjectDefineSourceTemple;

	CString									m_MsgMapInitTemple;

	CString									m_MsgCallerHeaderTemplate;
	CString									m_MsgCallerSourceTemplate;
	
	CString									m_MsgHandlerHeaderTemplate;
	CString									m_MsgHandlerSourceTemplate;	

	CString									m_StructDefineHeaderTemplate;
	CString									m_EnumDefineHeaderTemplate;
	CString									m_SystemEnumDefineHeaderTemplate;
	CString									m_StructDefineTemplate;
	CString									m_StructMemberTemplate;
	CString									m_DataObjectModifyFlagsTemple;
	CString									m_DataObjectModifyFlagDefineTemple;
	CString									m_DataObjectModifyFlagUnitTemple;
	CString									m_PackOperationUnitTemple;
	CString									m_UnpackOperationUnitTemple;

	CString									m_EnumDefineTemplate;
	CString									m_EnumMemberDefineTemplate;
	CString									m_EnumStrValueDefineTemplate;

	CString									m_ConstDefineTemplate;
	CString									m_MacroDefineTemplate;
	CString									m_ConstDefineHeaderTemplate;

	CString									m_StringConstDefineTemplate;

	vector<TYPE_DEFINE>						m_CurVarTypeList;

	bool									m_SelectAll;
public:
	CCallInterfaceMakerDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CCallInterfaceMakerDlg();

// 对话框数据
	enum { IDD = IDD_CALLINTERFACEMAKER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
	bool LoadTypeDef();	
	bool LoadTemples();
	bool LoadConfig();
	bool SaveConfig();
	bool LoadEnv();
	bool SaveEnv();
public:
	bool SaveTypeDef();
	void MakeCurVarType(bool IncludeDataObject);
	UINT GetVarTypeCount();
	TYPE_DEFINE * GetVarType(UINT Index);
	TYPE_DEFINE * FindVarType(LPCTSTR TypeName);
	void GetDataObjectDefineList(vector<STRUCT_DEFINE_INFO>& DataObjectList);
	INTERFACE_CONFIG& GetConfig()
	{
		return m_InterfaceConfig;
	}
	RUN_ENV_INFO& GetRunEnv()
	{
		return m_RunEnvInfo;
	}	

	void GetDataStructPacketList(int Type,vector<CString>& NameList);

	bool MoveDataStruct(int Type,CString SourcePacket,CString TargetPacket,CString Name);
	
protected:
	virtual void OnCancel();
	virtual void OnOK();

	bool Save(LPCTSTR FileName);
	bool Load(LPCTSTR FileName);

	bool SaveStructDefine(xml_node& Structs,vector<STRUCT_DEFINE_INFO>& StructDefineList);
	bool LoadStructDefine(xml_node& Structs,vector<STRUCT_DEFINE_INFO>& StructDefineList);

	bool SaveEnumDefine(xml_node& Enums,vector<ENUM_DEFINE_INFO>& EnumDefineList);
	bool LoadEnumDefine(xml_node& Enums,vector<ENUM_DEFINE_INFO>& EnumDefineList);

	bool SaveConstDefine(xml_node& Consts,vector<CONST_DEFINE_INFO>& ConstDefineList);
	bool LoadConstDefine(xml_node& Consts,vector<CONST_DEFINE_INFO>& ConstDefineList);

	bool SaveDataObjectModifyFlag(xml_node& ModifyFlags,vector<DATA_OBJECT_MODIFY_FLAG>& DataObjectModifyFlags);
	bool LoadDataObjectModifyFlag(xml_node& ModifyFlags,vector<DATA_OBJECT_MODIFY_FLAG>& DataObjectModifyFlags);

	bool SaveGenerateOperations(xml_node& XmlNode,GENERATE_OPERATIONS_INFO& GenerateOperations);
	bool LoadGenerateOperations(xml_node& XmlNode,GENERATE_OPERATIONS_INFO& GenerateOperations);

	CString EncodeString(CString Str);
	CString DecodeString(CString Str);

	bool ExportDataStructDefineHeader(LPCTSTR ExportDir,LPCTSTR FileName);
	bool ExportInterfaceHeader(LPCTSTR ExportDir,LPCTSTR FileName);
	bool ExportStructs(LPCTSTR ExportDir,LPCTSTR FileName,vector<STRUCT_DEFINE_INFO>& StructList);
	bool ExportSystemEnums(LPCTSTR ExportDir,LPCTSTR FileName);
	bool ExportEnums(LPCTSTR ExportDir,LPCTSTR FileName,vector<ENUM_DEFINE_INFO>& EnumList,LPCTSTR ListName,bool IsSystem);
	bool ExportConsts(LPCTSTR ExportDir,LPCTSTR FileName,vector<CONST_DEFINE_INFO>& ConstList,LPCTSTR ListName);
	bool ExportDataStructDefines(LPCTSTR ExportDir);
	bool ExportInterfaces(LPCTSTR ExportDir);
	

	bool ExportMsgCaller(LPCTSTR ExportDir,CALLER_INTERFACE& InterfaceInfo);
	bool ExportMsgHandler(LPCTSTR ExportDir,CALLER_INTERFACE& InterfaceInfo);
	bool ExportDataObject(LPCTSTR ExportDir);
	bool ExportDataObjectModifyFlags(LPCTSTR ExportDir);

	

	CString MakeMethods(vector<INTERFACE_METHOD> MethodList,LPCTSTR szLineSpace,bool IsPure);
	CString MakeMethodSSTIDEnumDefines(CALLER_INTERFACE& InterfaceInfo,LPCTSTR szLineSpace);
	CString MakeStructSSTIDEnumDefines(STRUCT_DEFINE_INFO& StructInfo,LPCTSTR szLineSpace);
	CString MakeMsgPackMethods(vector<INTERFACE_METHOD>& MethodList,LPCTSTR szLineSpace);
	CString MakeParams(vector<METHOD_PARAM>& ParamList,bool IsDefine,bool IncludeDefaultValue);
	CString MakeParamDescriptions(vector<METHOD_PARAM>& ParamList,LPCTSTR szLineSpace);
	CString MakeParamDefines(vector<METHOD_PARAM>& ParamList,LPCTSTR szLineSpace);

	CString MakeMethodsSource(CALLER_INTERFACE& InterfaceInfo,LPCTSTR szLineSpace);
	CString MakeMsgPackMethodsSource(CALLER_INTERFACE& InterfaceInfo,LPCTSTR szLineSpace);

	CString MakeInitOperations(INTERFACE_METHOD& MethodInfo,LPCTSTR szLineSpace);
	CString MakePackOperations(INTERFACE_METHOD& MethodInfo,CString SSTIDEnumName,LPCTSTR szLineSpace);
	CString MakeUnpackOperations(INTERFACE_METHOD& MethodInfo,CString SSTIDEnumName,LPCTSTR szLineSpace);
	CString MakePackSizes(INTERFACE_METHOD& MethodInfo,LPCTSTR szLineSpace);

	CString MakeInitOperations(STRUCT_DEFINE_INFO& StructInfo,LPCTSTR szLineSpace);
	CString MakePackOperations(STRUCT_DEFINE_INFO& StructInfo,CString SSTIDEnumName,LPCTSTR szLineSpace);
	CString MakeUnpackOperations(STRUCT_DEFINE_INFO& StructInfo,CString SSTIDEnumName,LPCTSTR szLineSpace);
	CString MakePackSizes(STRUCT_DEFINE_INFO& StructInfo,LPCTSTR szLineSpace);

	CString MakePackOperation(CString Type,CString Name,CString ParentName,CString ParentShortName,CString SSTIDEnumName,bool IsInDataObject,bool IsMFCheck,bool IsArray,LPCTSTR szLineSpace);
	CString MakeUnpackOperation(CString Type,CString Name,CString ParentName,CString ParentShortName,CString SSTIDEnumName,bool IsInDataObject,bool IsMonitorUpdate,bool IsArray,LPCTSTR szLineSpace);


	CString MakeMsgHandlerMethods(CALLER_INTERFACE& InterfaceInfo,LPCTSTR szLineSpace);
	CString MakeMsgHandlerMethodsSource(CALLER_INTERFACE& InterfaceInfo,LPCTSTR szLineSpace);

	CString MakeMsgMapInits(CALLER_INTERFACE& InterfaceInfo,LPCTSTR szLineSpace);

	CString MakeStructDefines(vector<STRUCT_DEFINE_INFO>& StructDefineList,LPCTSTR szLineSpace);
	CString MakeStructMembers(STRUCT_DEFINE_INFO& StructInfo,LPCTSTR szLineSpace);

	CString MakeCloneOperations(STRUCT_DEFINE_INFO& StructInfo,LPCTSTR szLineSpace,LPCTSTR szLeftValue,LPCTSTR szRightValue);

	bool MakeDataObjectFlagIndicesEnum(ENUM_DEFINE_INFO& EnumInfo);
	bool MakeDataObjectSSTIDsEnum(ENUM_DEFINE_INFO& EnumInfo);
	CString MakeDataObjectModifyFlagEnumDefine(STRUCT_DEFINE_INFO& StructInfo,LPCTSTR szLineSpace);
	CString MakeDataObjectMembersGetModifyFlag(STRUCT_DEFINE_INFO& StructInfo,LPCTSTR szLineSpace);
	CString MakeDataDataObjectMembersIsModified(STRUCT_DEFINE_INFO& StructInfo,LPCTSTR szLineSpace);
	CString MakeDataDataObjectMembersClearModifyFlag(STRUCT_DEFINE_INFO& StructInfo,LPCTSTR szLineSpace);
	
	CString MakeDataObjectSetMethodsDeclare(STRUCT_DEFINE_INFO& StructInfo,LPCTSTR szLineSpace);
	CString MakeDataObjectSetMethodsDefine(STRUCT_DEFINE_INFO& StructInfo,LPCTSTR szLineSpace);
	CString MakeDataObjectGetMethodsDeclare(STRUCT_DEFINE_INFO& StructInfo,LPCTSTR szLineSpace);
	CString MakeDataObjectGetMethodsDefine(STRUCT_DEFINE_INFO& StructInfo,LPCTSTR szLineSpace);

	CString MakeDataObjectModifyFlags(LPCTSTR szLineSpace);
	CString MakeDataObjectModifyFlagData(DATA_OBJECT_MODIFY_FLAG& ModifyFlag,LPCTSTR szLineSpace);


	CString MakeEnumDefine(ENUM_DEFINE_INFO& EnumInfo,LPCTSTR szLineSpace);
	CString MakeEnumDefines(vector<ENUM_DEFINE_INFO>& EnumDefineList,LPCTSTR szLineSpace);

	CString MakeEnumStrValue(ENUM_DEFINE_INFO& EnumInfo,LPCTSTR szLineSpace);
	CString MakeEnumStrValues(vector<ENUM_DEFINE_INFO>& EnumDefineList,LPCTSTR szLineSpace);

	CString MakeConstDefines(vector<CONST_DEFINE_INFO>& ConstDefineList,LPCTSTR szLineSpace);

	CString MakeStringConst(LPCTSTR StrValue);


	DATA_OBJECT_MODIFY_FLAG MakeDataObjectFullModifyFlag(STRUCT_DEFINE_INFO& StructInfo,vector<STRUCT_DEFINE_INFO>& DataObjectList);

	bool HaveMemberByType(STRUCT_DEFINE_INFO& StructInfo,CString Type);
	STRUCT_DEFINE_INFO * GetStructDefineInfo(CString Type);
	

	CString GetLineSpace(CString& TempleStr,LPCTSTR szWord);
	int GetHeadLen(CString& TempleStr,LPCTSTR szWord);
	CString ToComment(CString& Content,LPCTSTR szLineSpace);

	CString ClassNameToFileName(LPCTSTR szClassName);
	CString ClassNameToUpper(LPCTSTR szClassName);

	bool IsBigChar(TCHAR c);

	void RemoveBlock(CString& Template,LPCTSTR szBlockStart,LPCTSTR szBlockEnd);

	CString RestoreFileToTemplate(LPCTSTR szFileName,const CString& szTemplate);
	CString RestoreToTemplate(LPCTSTR szInput,const CString& szTemplate);

	
	
public:
	
	
	
	
	afx_msg void OnNMDblclkInterfaceList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonConfig();
	afx_msg void OnBnClickedButtonSelectAllInterface();
	afx_msg void OnBnClickedButtonDataStructDef();
};


inline UINT CCallInterfaceMakerDlg::GetVarTypeCount()
{
	return m_CurVarTypeList.size();
}
inline TYPE_DEFINE * CCallInterfaceMakerDlg::GetVarType(UINT Index)
{
	if(Index<m_CurVarTypeList.size())
	{
		return &m_CurVarTypeList.at(Index);
	}
	return NULL;
}

inline TYPE_DEFINE * CCallInterfaceMakerDlg::FindVarType(LPCTSTR TypeName)
{
	for(UINT i=0;i<m_CurVarTypeList.size();i++)
	{
		if(m_CurVarTypeList[i].Name==TypeName)
		{
			return &m_CurVarTypeList[i];
		}
	}
	return NULL;
}