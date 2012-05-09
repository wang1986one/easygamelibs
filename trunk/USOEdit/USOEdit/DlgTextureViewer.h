/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DlgTextureViewer.h                                       */
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
/*      �ļ���:    DlgTextureViewer.h                                       */
/*      ��������:  2010��04��30��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once


// CDlgTextureViewer �Ի���

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
	CDlgTextureViewer(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgTextureViewer();

	void SetRenderThread(CD3DRenderThread * pRenderThread)
	{
		m_pRenderThread=pRenderThread;
	}

// �Ի�������
	enum { IDD = IDD_DIALOG_TEXTURE_VIEWER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	void ShowTexture(CD3DTexture * pTexture);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedEditFx();
	afx_msg void OnBnClickedSaveTexture();
};
