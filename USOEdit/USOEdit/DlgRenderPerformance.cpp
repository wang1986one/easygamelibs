/****************************************************************************/
/*                                                                          */
/*      文件名:    DlgRenderPerformance.cpp                                 */
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
/*      文件名:    DlgRenderPerformance.cpp                                 */
/*      创建日期:  2009年09月11日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
// DlgRenderPerformance.cpp : 实现文件
//

#include "stdafx.h"



// CDlgRenderPerformance 对话框

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


// CDlgRenderPerformance 消息处理程序

void CDlgRenderPerformance::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

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

	// TODO:  在此添加额外的初始化

	SetTimer(1011,FRAME_RATE_STAT_TIME,NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
