/****************************************************************************/
/*                                                                          */
/*      文件名:    DlgAniSequenceViewer.h                                   */
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


// CDlgAniSequenceViewer 对话框

class CDlgAniSequenceViewer : public CPropertyConfigDialog
{
	DECLARE_DYNAMIC(CDlgAniSequenceViewer)
protected:
	CD3DWOWM2ModelResource *		m_pModelResource;
	CListCtrl						m_lvAniSeqs;
	
public:
	CDlgAniSequenceViewer(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgAniSequenceViewer();

// 对话框数据
	enum { IDD = IDD_DIALOG_ANI_SEQUENCE_VIEWER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void Init(CD3DWOWM2ModelResource * pModelResource);
	
};
