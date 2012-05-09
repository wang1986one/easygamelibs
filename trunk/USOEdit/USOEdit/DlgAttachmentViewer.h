/****************************************************************************/
/*                                                                          */
/*      文件名:    DlgAttachmentViewer.h                                    */
/*      创建日期:  2011年10月07日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once
#include "afxcmn.h"


// CDlgAttachmentViewer 对话框

class CDlgAttachmentViewer : public CPropertyConfigDialog
{
protected:
	CListCtrl			m_lvAttachments;
	CD3DWOWM2Model *	m_pModel;
	CUSOObject *		m_pUSOObject;
	DECLARE_DYNAMIC(CDlgAttachmentViewer)

public:
	CDlgAttachmentViewer(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgAttachmentViewer();

// 对话框数据
	enum { IDD = IDD_DIALOG_ATTACHMENT_VIEWER };

	void Init(CD3DWOWM2Model * pModel,CUSOObject * pUSOObject);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	
	afx_msg void OnBnClickedButtonEnableAttachment();
};
