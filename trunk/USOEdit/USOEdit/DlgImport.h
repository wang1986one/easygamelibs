/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DlgImport.h                                              */
/*      ��������:  2011��10��07��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DlgImport.h                                              */
/*      ��������:  2009��07��06��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once
#include "afxwin.h"


// CDlgImport �Ի���
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
	CDlgImport(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgImport();

// �Ի�������
	enum { IDD = IDD_IMPORT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
