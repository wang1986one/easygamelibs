/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DlgTrianglesViewer.h                                     */
/*      ��������:  2011��10��07��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CDlgTrianglesViewer �Ի���

class CDlgTrianglesViewer : public CPropertyConfigDialog
{
	DECLARE_DYNAMIC(CDlgTrianglesViewer)
protected:
	enum VIEW_MODE
	{
		VIEW_MODE_VERTEX,
		VIEW_MODE_TRIANGLE,
	};
	enum VERTEXT_ELEMENT
	{
		VE_POS,
		VE_POSW,
		VE_FLOAT,
		VE_FLOAT2,
		VE_FLOAT3,
		VE_FLOAT4,
		VE_UBYTE4,
		VE_NORMAL,
		VE_PSIZE,
		VE_DIFFUSE,
		VE_SPECULAR,
		VE_TEX,
	};
	struct VERTEX_FORMAT
	{
		int	Type;
		int Index;
	};
	CD3DSubMesh *				m_pSubMesh;
	UINT						m_VertexCount;
	UINT						m_TriangleCount;
	CListCtrl					m_lvVertexList;
	CButton 					m_rdVertexMode;
	CButton 					m_rdTriangleMode;
	UINT						m_ViewMode;
	CEasyArray<VERTEX_FORMAT>	m_VertextFormat;
public:
	CDlgTrianglesViewer(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgTrianglesViewer();

// �Ի�������
	enum { IDD = IDD_DIALOG_TRIANGLES_VIEWER };

	void Init(CD3DSubMesh * pSubMesh);	

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	
	afx_msg void OnBnClickedRadioVertexMode();
	afx_msg void OnBnClickedRadioTriangleMode();
protected:
	void FillList();
	void MakeColumns();
	void InsertLine(int Index,BYTE * pVertexData,UINT VertexSize);
	void ParseVertexFormat();
	
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonModify();
	afx_msg void OnNMClickVertextList(NMHDR *pNMHDR, LRESULT *pResult);
};
