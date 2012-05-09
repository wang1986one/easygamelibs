/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DlgRenderPerformance.h                                   */
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
/*      �ļ���:    DlgRenderPerformance.h                                   */
/*      ��������:  2009��07��29��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once


// CDlgRenderPerformance �Ի���

class CD3DViewPort;

class CDlgRenderPerformance : public CDialog
{
protected:
	CD3DViewPort *			m_pViewPort;
	DECLARE_DYNAMIC(CDlgRenderPerformance)

public:
	CDlgRenderPerformance(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgRenderPerformance();

// �Ի�������
	enum { IDD = IDD_RENDER_PERFORMANCE };

	void SetViewPort(CD3DViewPort * pViewPort)
	{
		m_pViewPort=pViewPort;
	}

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
