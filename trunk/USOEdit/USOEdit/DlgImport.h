/****************************************************************************/
/*                                                                          */
/*      文件名:    DlgImport.h                                              */
/*      创建日期:  2011年10月07日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
/****************************************************************************/
/*                                                                          */
/*      文件名:    DlgImport.h                                              */
/*      创建日期:  2009年07月06日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once
#include "afxwin.h"


// CDlgImport 对话框
enum IMPORT_TYPE
{
	IMP_NONE,
	IMP_SN2,
	IMP_SMDL,
	IMP_SKN,
	IMP_MDA,
	IMP_ACT,
	IMP_MDL,
	IMP_USO,
	IMP_X,
	IMP_FX,
	IMP_M2,
};

class CDlgImport : public CDialog
{
	DECLARE_DYNAMIC(CDlgImport)

public:
	CDlgImport(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgImport();

// 对话框数据
	enum { IDD = IDD_IMPORT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_ImportType;
	CString m_ImportFilePath;
	CString m_TextureDir;
	CString m_FxDir;
	CString m_ModelDir;
	BOOL m_UseTextureDir;
	BOOL m_UseFxDir;
	BOOL m_UseModelDir;
	CString m_DefualtFile;
	CString m_FileFliter;
	CString m_DefualtSkinFile;
	CString m_SkinFileFliter;
	afx_msg void OnBnClickedCheckUseTexturePath();
	afx_msg void OnBnClickedCheckUseFxPath();
	afx_msg void OnBnClickedCheckUseModelPath();
	afx_msg void OnBnClickedSelectFile();
	afx_msg void OnBnClickedSelectTexturePath();
	afx_msg void OnBnClickedSelectFxPath();	
	afx_msg void OnBnClickedSelectModelPath();
protected:
	virtual void OnOK();
public:
	CEdit m_edTextureDir;
	CEdit m_edFxDir;
	CEdit m_edModelDir;
	virtual BOOL OnInitDialog();
	CButton m_chkInverseImport;
	BOOL m_IsInverseImport;
	CButton m_btSelectSkinFile;
	CEdit m_edSkinFileName;
	CString m_SkinFileName;
	
		
	
	afx_msg void OnBnClickedSelectSkinFile();
};
