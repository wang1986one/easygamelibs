/****************************************************************************/
/*                                                                          */
/*      文件名:    DlgTrianglesViewer.h                                     */
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
#include "afxwin.h"


// CDlgTrianglesViewer 对话框

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
	CDlgTrianglesViewer(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgTrianglesViewer();

// 对话框数据
	enum { IDD = IDD_DIALOG_TRIANGLES_VIEWER };

	void Init(CD3DSubMesh * pSubMesh);	

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
