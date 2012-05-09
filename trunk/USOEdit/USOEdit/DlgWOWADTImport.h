/****************************************************************************/
/*                                                                          */
/*      文件名:    DlgWOWADTImport.h                                        */
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
/*      文件名:    DlgWOWADTImport.h                                        */
/*      创建日期:  2010年02月09日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once
#include "afxwin.h"


// CDlgWOWADTImport 对话框

class CDlgWOWADTImport : public CDialog
{
	DECLARE_DYNAMIC(CDlgWOWADTImport)

public:
	CDlgWOWADTImport(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgWOWADTImport();

// 对话框数据
	enum { IDD = IDD_DIALOG_ADT_IMPORT };

protected:
	CComboBox		m_cbMap;
	CComboBox		m_cbArea;
	BOOL			m_IsImportAllArea;
	CString			m_ModelFileName;
	

	CD3DDevice *	m_pD3DDevice;
	HBITMAP			m_hBMPMiniMap;
	CStatic			m_stMiniMap;
	CDlgMapView		m_DlgMapView;
	CD3DBoundingBox	m_SenceBox;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString							m_WDLFileName;
	CEasyArray<CEasyString>			m_AreaFileNameList;
	bool							m_IsBigAlphaMask;
	UINT							m_MapID;
	void SetD3DDevice(CD3DDevice * pD3DDevice)
	{
		m_pD3DDevice=pD3DDevice;
	}
	CD3DBoundingBox& GetSenceBox()
	{
		return m_SenceBox;
	}
	afx_msg void OnBnClickedSelectModelFile();
	afx_msg void OnBnClickedOk();
	
	afx_msg void OnCbnSelchangeComboMap();
	afx_msg void OnCbnSelchangeComboArea();
	virtual BOOL OnInitDialog();
	
	afx_msg void OnBnClickedShowFullMap();
};
