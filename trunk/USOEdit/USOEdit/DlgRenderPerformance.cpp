/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DlgRenderPerformance.cpp                                 */
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
/*      �ļ���:    DlgRenderPerformance.cpp                                 */
/*      ��������:  2009��09��11��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
// DlgRenderPerformance.cpp : ʵ���ļ�
//

#include "stdafx.h"



// CDlgRenderPerformance �Ի���

IMPLEMENT_DYNAMIC(CDlgRenderPerformance, CDialog)

CDlgRenderPerformance::CDlgRenderPerformance(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgRenderPerformance::IDD, pParent)
	, m_FrameRate(_T(""))
	, m_ObjectCount(_T(""))
	, m_SubMeshCount(_T(""))
	, m_FaceCount(_T(""))
	, m_VertexCount(_T(""))
	, m_UpdateRate(_T(""))
{
	m_pViewPort=NULL;
}

CDlgRenderPerformance::~CDlgRenderPerformance()
{
}

void CDlgRenderPerformance::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_FRAME_RATE, m_FrameRate);
	DDX_Text(pDX, IDC_EDIT_FRAME_OBJECT_COUNT, m_ObjectCount);
	DDX_Text(pDX, IDC_EDIT_SUBMESH_COUNT, m_SubMeshCount);
	DDX_Text(pDX, IDC_EDIT_FACE_COUNT, m_FaceCount);
	DDX_Text(pDX, IDC_EDIT_VERTEX_COUNT, m_VertexCount);
	DDX_Text(pDX, IDC_EDIT_UPDATE_RATE, m_UpdateRate);
}


BEGIN_MESSAGE_MAP(CDlgRenderPerformance, CDialog)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CDlgRenderPerformance ��Ϣ�������

void CDlgRenderPerformance::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	if(m_pViewPort)
	{
		m_FrameRate.Format(_T("%g"),m_pViewPort->GetFrameRate());
		m_UpdateRate.Format(_T("%g"),m_pViewPort->GetUpdateRate());
		m_ObjectCount.Format(_T("%d"),m_pViewPort->GetRenderObjectCount());
		m_SubMeshCount.Format(_T("%d"),m_pViewPort->GetRenderSubMeshCount());
		m_FaceCount.Format(_T("%d"),m_pViewPort->GetRenderFaceCount());
		m_VertexCount.Format(_T("%d"),m_pViewPort->GetRenderVertexCount());
		UpdateData(false);
	}
	

	CDialog::OnTimer(nIDEvent);
}

BOOL CDlgRenderPerformance::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	SetTimer(1011,FRAME_RATE_STAT_TIME,NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
