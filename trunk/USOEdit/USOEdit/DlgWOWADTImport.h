/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DlgWOWADTImport.h                                        */
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
/*      �ļ���:    DlgWOWADTImport.h                                        */
/*      ��������:  2010��02��09��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once
#include "afxwin.h"


// CDlgWOWADTImport �Ի���

class CDlgWOWADTImport : public CDialog
{
	DECLARE_DYNAMIC(CDlgWOWADTImport)

public:
	CDlgWOWADTImport(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgWOWADTImport();

// �Ի�������
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

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
