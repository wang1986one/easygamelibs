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
	, m_IsExpProtocolFile(FALSE)
	, m_IsExpInterface(FALSE)
	, m_IsExpCallerHeader(FALSE)
	, m_IsExpCallerSource(FALSE)
	, m_IsExpHandlerHeader(FALSE)
	, m_IsExpHandlerSource(FALSE)
	, m_ProtocolFileExt(_T(""))
	, m_InterfaceExt(_T(""))
	, m_CallerHeaderExt(_T(""))
	, m_CallerSourceExt(_T(""))
	, m_HandlerHeaderExt(_T(""))
	, m_HandlerSourceExt(_T(""))
{

}

CDlgConfig::~CDlgConfig()
{
}

void CDlgConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_EXP_PROTOCOL, m_IsExpProtocolFile);
	DDX_Check(pDX, IDC_CHECK_EXP_INTERFACE, m_IsExpInterface);
	DDX_Check(pDX, IDC_EXP_CALL_HEADER, m_IsExpCallerHeader);
	DDX_Check(pDX, IDC_EXP_CALL_SOURCE, m_IsExpCallerSource);
	DDX_Check(pDX, IDC_EXP_HANDLER_HEADER, m_IsExpHandlerHeader);
	DDX_Check(pDX, IDC_EXP_HANDLER_SOURCE, m_IsExpHandlerSource);
	DDX_Text(pDX, IDC_EDIT1, m_ProtocolFileExt);
	DDX_Text(pDX, IDC_EDIT2, m_InterfaceExt);
	DDX_Text(pDX, IDC_EDIT3, m_CallerHeaderExt);
	DDX_Text(pDX, IDC_EDIT4, m_CallerSourceExt);
	DDX_Text(pDX, IDC_EDIT5, m_HandlerHeaderExt);
	DDX_Text(pDX, IDC_EDIT6, m_HandlerSourceExt);
}


BEGIN_MESSAGE_MAP(CDlgConfig, CDialog)
END_MESSAGE_MAP()


// CDlgConfig 消息处理程序


BOOL CDlgConfig::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_IsExpProtocolFile=m_pConfig->ProtocolFileExportConfig.IsExport;
	m_IsExpInterface=m_pConfig->InterfaceExportConfig.IsExport;
	m_IsExpCallerHeader=m_pConfig->CallHeaderExportConfig.IsExport;
	m_IsExpCallerSource=m_pConfig->CallSourceExportConfig.IsExport;
	m_IsExpHandlerHeader=m_pConfig->HandlerHeaderExportConfig.IsExport;
	m_IsExpHandlerSource=m_pConfig->HandlerSourceExportConfig.IsExport;
	m_ProtocolFileExt=m_pConfig->ProtocolFileExportConfig.ExportExt;
	m_InterfaceExt=m_pConfig->InterfaceExportConfig.ExportExt;
	m_CallerHeaderExt=m_pConfig->CallHeaderExportConfig.ExportExt;
	m_CallerSourceExt=m_pConfig->CallSourceExportConfig.ExportExt;
	m_HandlerHeaderExt=m_pConfig->HandlerHeaderExportConfig.ExportExt;
	m_HandlerSourceExt=m_pConfig->HandlerSourceExportConfig.ExportExt;

	UpdateData(false);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgConfig::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	UpdateData(true);

	m_pConfig->ProtocolFileExportConfig.IsExport=m_IsExpProtocolFile;
	m_pConfig->InterfaceExportConfig.IsExport=m_IsExpInterface;
	m_pConfig->CallHeaderExportConfig.IsExport=m_IsExpCallerHeader;
	m_pConfig->CallSourceExportConfig.IsExport=m_IsExpCallerSource;
	m_pConfig->HandlerHeaderExportConfig.IsExport=m_IsExpHandlerHeader;
	m_pConfig->HandlerSourceExportConfig.IsExport=m_IsExpHandlerSource;

	m_pConfig->ProtocolFileExportConfig.ExportExt=m_ProtocolFileExt;
	m_pConfig->InterfaceExportConfig.ExportExt=m_InterfaceExt;
	m_pConfig->CallHeaderExportConfig.ExportExt=m_CallerHeaderExt;
	m_pConfig->CallSourceExportConfig.ExportExt=m_CallerSourceExt;
	m_pConfig->HandlerHeaderExportConfig.ExportExt=m_HandlerHeaderExt;
	m_pConfig->HandlerSourceExportConfig.ExportExt=m_HandlerSourceExt;

	CDialog::OnOK();
}
