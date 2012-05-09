/****************************************************************************/
/*                                                                          */
/*      文件名:    DlgRenderPerformance.h                                   */
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
/*      文件名:    DlgRenderPerformance.h                                   */
/*      创建日期:  2009年07月29日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once


// CDlgRenderPerformance 对话框

class CD3DViewPort;

class CDlgRenderPerformance : public CDialog
{
protected:
	CD3DViewPort *			m_pViewPort;
	DECLARE_DYNAMIC(CDlgRenderPerformance)

public:
	CDlgRenderPerformance(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgRenderPerformance();

// 对话框数据
	enum { IDD = IDD_RENDER_PERFORMANCE };

	void SetViewPort(CD3DViewPort * pViewPort)
	{
		m_pViewPort=pViewPort;
	}

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_FrameRate;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL OnInitDialog();
	CString m_ObjectCount;
	CString m_SubMeshCount;
	CString m_FaceCount;
	CString m_VertexCount;
	CString m_UpdateRate;
};
