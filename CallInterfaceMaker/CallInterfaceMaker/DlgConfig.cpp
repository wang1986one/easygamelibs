/****************************************************************************/
/*                                                                          */
/*      文件名:    DlgConfig.cpp                                            */
/*      创建日期:  2012年04月20日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
// DlgConfig.cpp : 实现文件
//

#include "stdafx.h"
#include "CallInterfaceMaker.h"
#include "DlgConfig.h"


// CDlgConfig 对话框

IMPLEMENT_DYNAMIC(CDlgConfig, CDialog)

CDlgConfig::CDlgConfig(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgConfig::IDD, pParent)	
{
	m_IsModified=false;
}

CDlgConfig::~CDlgConfig()
{
}

void CDlgConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_EXP_PROTOCOL, m_Config.InterfaceHeaderExportConfig.IsExport);
	DDX_Check(pDX, IDC_CHECK_EXP_STRUCT_DEFINE, m_Config.StructExportConfig.IsExport);
	DDX_Check(pDX, IDC_CHECK_EXP_ENUM, m_Config.EnumExportConfig.IsExport);
	DDX_Check(pDX, IDC_CHECK_EXP_CONST, m_Config.ConstExportConfig.IsExport);
	DDX_Check(pDX, IDC_CHECK_EXP_DOHEADER, m_Config.DataStructHeaderExportConfig.IsExport);
	DDX_Check(pDX, IDC_CHECK_EXP_INTERFACE, m_Config.InterfaceExportConfig.IsExport);
	DDX_Check(pDX, IDC_EXP_CALL_HEADER, m_Config.CallHeaderExportConfig.IsExport);
	DDX_Check(pDX, IDC_EXP_CALL_SOURCE, m_Config.CallSourceExportConfig.IsExport);
	DDX_Check(pDX, IDC_EXP_HANDLER_HEADER, m_Config.HandlerHeaderExportConfig.IsExport);
	DDX_Check(pDX, IDC_EXP_HANDLER_SOURCE, m_Config.HandlerSourceExportConfig.IsExport);
	DDX_Check(pDX, IDC_EXP_DATA_OBJECT_HEADER, m_Config.DataObjectHeaderExportConfig.IsExport);
	DDX_Check(pDX, IDC_EXP_DATA_OBJECT_SOURCE, m_Config.DataObjectSourceExportConfig.IsExport);
	DDX_Check(pDX, IDC_EXP_DATA_OBJECT_MODIFY_FLAGS, m_Config.DataObjectModifyFlagsExportConfig.IsExport);

	DDX_Text(pDX, IDC_EDIT_PROTOCOL_FILE_EXT, m_Config.InterfaceHeaderExportConfig.ExportExt);
	DDX_Text(pDX, IDC_EDIT_STRUCT_DEFINE_EXT, m_Config.StructExportConfig.ExportExt);
	DDX_Text(pDX, IDC_EDIT_ENUM_EXT, m_Config.EnumExportConfig.ExportExt);
	DDX_Text(pDX, IDC_EDIT_CONST_EXT, m_Config.ConstExportConfig.ExportExt);
	DDX_Text(pDX, IDC_EDIT_CONST_DOHEADER_EXT, m_Config.DataStructHeaderExportConfig.ExportExt);
	DDX_Text(pDX, IDC_EDIT_INTERFACE_EXT, m_Config.InterfaceExportConfig.ExportExt);
	DDX_Text(pDX, IDC_EDIT_MSG_CALLER_HEADER_EXT, m_Config.CallHeaderExportConfig.ExportExt);
	DDX_Text(pDX, IDC_EDIT_MSG_CALLER_SOURCE_EXT, m_Config.CallSourceExportConfig.ExportExt);
	DDX_Text(pDX, IDC_EDIT_MSG_HANDLER_HEADER_EXT, m_Config.HandlerHeaderExportConfig.ExportExt);
	DDX_Text(pDX, IDC_EDIT_MSG_HANDLER_SOURCE_EXT, m_Config.HandlerSourceExportConfig.ExportExt);
	DDX_Text(pDX, IDC_EDIT_DATA_OBJECT_HEADER_EXT, m_Config.DataObjectHeaderExportConfig.ExportExt);
	DDX_Text(pDX, IDC_EDIT_DATA_OBJECT_SOURCE_EXT, m_Config.DataObjectSourceExportConfig.ExportExt);
	DDX_Text(pDX, IDC_EDIT_DATA_OBJECT_MODIFY_FLAGS_EXT, m_Config.DataObjectModifyFlagsExportConfig.ExportExt);
	
}


BEGIN_MESSAGE_MAP(CDlgConfig, CDialog)

	ON_EN_CHANGE(IDC_EDIT_PROTOCOL_FILE_EXT, &CDlgConfig::OnEnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT_STRUCT_DEFINE_EXT, &CDlgConfig::OnEnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT_INTERFACE_EXT, &CDlgConfig::OnEnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT_MSG_CALLER_HEADER_EXT, &CDlgConfig::OnEnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT_MSG_CALLER_SOURCE_EXT, &CDlgConfig::OnEnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT_MSG_HANDLER_HEADER_EXT, &CDlgConfig::OnEnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT_MSG_HANDLER_SOURCE_EXT, &CDlgConfig::OnEnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT_DATA_OBJECT_HEADER_EXT, &CDlgConfig::OnEnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT_DATA_OBJECT_SOURCE_EXT, &CDlgConfig::OnEnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT_DATA_OBJECT_MODIFY_FLAGS_EXT, &CDlgConfig::OnEnChangeEdit)
	
	ON_BN_CLICKED(IDC_BUTTON1_DEFAULT_STRUCT_GENERATE_OPERATIONS, &CDlgConfig::OnBnClickedButton1DefaultStructGenerateOperations)
	ON_BN_CLICKED(IDC_BUTTON_ARRAY_DEFINE_CONFIG, &CDlgConfig::OnBnClickedButtonArrayDefineConfig)
END_MESSAGE_MAP()


// CDlgConfig 消息处理程序


BOOL CDlgConfig::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	UpdateData(false);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgConfig::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类


	CDialog::OnOK();
}


void CDlgConfig::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	
	if(m_IsModified)
	{
		if(AfxMessageBox("是否要放弃已作出的更改？",MB_YESNO)==IDYES)
		{
			CDialog::OnCancel();
		}
	}
	else
	{
		CDialog::OnCancel();
	}
}
void CDlgConfig::OnEnChangeEdit()
{
	m_IsModified=true;
}
void CDlgConfig::OnBnClickedButton1DefaultStructGenerateOperations()
{
	// TODO: 在此添加控件通知处理程序代码
	CDlgGenerateOperationsEditor Dlg;
	Dlg.m_GenerateOperations=m_Config.DefaultStructGenerateOperations;
	if(Dlg.DoModal()==IDOK)
	{
		m_Config.DefaultStructGenerateOperations=Dlg.m_GenerateOperations;
	}
}

void CDlgConfig::OnBnClickedButtonArrayDefineConfig()
{
	// TODO: 在此添加控件通知处理程序代码
	CDlgGenerateOperationsEditor Dlg;
	Dlg.m_GenerateOperations=m_Config.ArrayDefineConfig;
	Dlg.m_ShowIndexOperation=true;
	if(Dlg.DoModal()==IDOK)
	{
		m_Config.ArrayDefineConfig=Dlg.m_GenerateOperations;
	}
}
