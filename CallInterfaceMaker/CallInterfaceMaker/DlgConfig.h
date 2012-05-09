/****************************************************************************/
/*                                                                          */
/*      文件名:    DlgConfig.h                                              */
/*      创建日期:  2012年04月20日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once


// CDlgConfig 对话框

class CDlgConfig : public CDialog
{
	DECLARE_DYNAMIC(CDlgConfig)

public:
	CDlgConfig(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgConfig();

// 对话框数据
	enum { IDD = IDD_DIALOG_CONFIG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	INTERFACE_CONFIG * m_pConfig;
protected:
	BOOL m_IsExpProtocolFile;
	BOOL m_IsExpInterface;
	BOOL m_IsExpCallerHeader;
	BOOL m_IsExpCallerSource;
	BOOL m_IsExpHandlerHeader;
	BOOL m_IsExpHandlerSource;
	CString m_ProtocolFileExt;
	CString m_InterfaceExt;
	CString m_CallerHeaderExt;
	CString m_CallerSourceExt;
	CString m_HandlerHeaderExt;
	CString m_HandlerSourceExt;
public:
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
};
