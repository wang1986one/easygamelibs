/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DlgConfig.cpp                                            */
/*      ��������:  2012��04��20��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
// DlgConfig.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CallInterfaceMaker.h"
#include "DlgConfig.h"


// CDlgConfig �Ի���

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


// CDlgConfig ��Ϣ�������


BOOL CDlgConfig::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
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
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CDlgConfig::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���
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
