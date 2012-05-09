/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DlgConfig.h                                              */
/*      ��������:  2012��04��20��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once


// CDlgConfig �Ի���

class CDlgConfig : public CDialog
{
	DECLARE_DYNAMIC(CDlgConfig)

public:
	CDlgConfig(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgConfig();

// �Ի�������
	enum { IDD = IDD_DIALOG_CONFIG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
