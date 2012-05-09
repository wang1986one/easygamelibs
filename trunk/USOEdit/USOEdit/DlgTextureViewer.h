/****************************************************************************/
/*                                                                          */
/*      文件名:    DlgTextureViewer.h                                       */
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
/*      文件名:    DlgTextureViewer.h                                       */
/*      创建日期:  2010年04月30日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once


// CDlgTextureViewer 对话框

class CDlgTextureViewer : public CDialog
{
	DECLARE_DYNAMIC(CDlgTextureViewer)
protected:
	//CD3DDevice 			m_D3DDevice;

	CD3DRenderThread *	m_pRenderThread;
	CD3DSwapChain		m_SwapChain;
	CD3DUIRender 		m_Render;

	CTextureViewRect *	m_pTextureViewRect;

public:
	CDlgTextureViewer(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgTextureViewer();

	void SetRenderThread(CD3DRenderThread * pRenderThread)
	{
		m_pRenderThread=pRenderThread;
	}

// 对话框数据
	enum { IDD = IDD_DIALOG_TEXTURE_VIEWER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	void ShowTexture(CD3DTexture * pTexture);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedEditFx();
	afx_msg void OnBnClickedSaveTexture();
};
