/****************************************************************************/
/*                                                                          */
/*      文件名:    DlgCameraSetting.cpp                                     */
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
/*      文件名:    DlgCameraSetting.cpp                                     */
/*      创建日期:  2010年02月09日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
// DlgCameraSetting.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgCameraSetting.h"




// CDlgCameraSetting 对话框

IMPLEMENT_DYNAMIC(CDlgCameraSetting, CDialog)

CDlgCameraSetting::CDlgCameraSetting(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCameraSetting::IDD, pParent)	
	, m_MoveStep(0)
	, m_RotateStep(0)
	, m_NearFace(0)
	, m_FarFace(0)
	, m_CameraPosX(0)
{
	m_pD3DView=NULL;
	m_Mode=0;
	m_ProjectMode=0;
}

CDlgCameraSetting::~CDlgCameraSetting()
{
}

void CDlgCameraSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_MoveStep);
	DDX_Text(pDX, IDC_EDIT7, m_RotateStep);
	DDX_Control(pDX, IDC_RADIO_SCENE_MODE, m_rbSceneMode);
	DDX_Control(pDX, IDC_RADIO_OBJECT_MODE, m_rbObjectMode);
	DDX_Control(pDX, IDC_RADIO_PROJECT_MODE_PERSPECTIVE, m_rbProjectModePerspective);
	DDX_Control(pDX, IDC_RADIO_PROJECT_MODE_ORTHO, m_cbProjectModeOrtho);
	DDX_Text(pDX, IDC_NEAR_FACE, m_NearFace);
	DDX_Text(pDX, IDC_FAR_FACE, m_FarFace);
	DDX_Text(pDX, IDC_POS_X, m_CameraPosX);
	DDX_Text(pDX, IDC_POS_Y, m_CameraPosY);
	DDX_Text(pDX, IDC_POS_Z, m_CameraPosZ);
	DDX_Text(pDX, IDC_ROTATE_X, m_CameraRotateX);
	DDX_Text(pDX, IDC_ROTATE_Y, m_CameraRotateY);
}


BEGIN_MESSAGE_MAP(CDlgCameraSetting, CDialog)
	ON_BN_CLICKED(IDC_APPLY, &CDlgCameraSetting::OnBnClickedApply)
	ON_BN_CLICKED(IDC_RADIO_SCENE_MODE, &CDlgCameraSetting::OnBnClickedRadioSceneMode)
	ON_BN_CLICKED(IDC_RADIO_OBJECT_MODE, &CDlgCameraSetting::OnBnClickedRadioObjectMode)
	ON_BN_CLICKED(IDC_RADIO_PROJECT_MODE_PERSPECTIVE, &CDlgCameraSetting::OnBnClickedRadioProjectModePerspective)
	ON_BN_CLICKED(IDC_RADIO_PROJECT_MODE_ORTHO, &CDlgCameraSetting::OnBnClickedRadioProjectModeOrtho)
	ON_BN_CLICKED(IDC_SET_POS, &CDlgCameraSetting::OnBnClickedSetPos)
END_MESSAGE_MAP()


// CDlgCameraSetting 消息处理程序

void CDlgCameraSetting::OnBnClickedApply()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	CUSOEditApp::GetInstance()->GetCameraSetting().CameraMode=m_Mode;
	CUSOEditApp::GetInstance()->GetCameraSetting().CameraProjectMode=m_ProjectMode;
	CUSOEditApp::GetInstance()->GetCameraSetting().CameraMoveStep=m_MoveStep;
	CUSOEditApp::GetInstance()->GetCameraSetting().CameraRotateStep=m_RotateStep;
	CUSOEditApp::GetInstance()->GetCameraSetting().CameraNearFace=m_NearFace;
	CUSOEditApp::GetInstance()->GetCameraSetting().CameraFarFace=m_FarFace;
	if(m_pD3DView)
	{
		m_pD3DView->ApplyCameraChange(false);		
	}

	

	CUSOEditApp::GetInstance()->SaveSetting();
}


void CDlgCameraSetting::SetD3DView(CD3DViewPort * pD3DView)
{
	m_pD3DView=pD3DView;
	
}

void CDlgCameraSetting::ShowSetting()
{
	UpdateData(true);
	m_rbSceneMode.SetCheck(m_Mode==CAMERA_MODE_SCENE);
	m_rbObjectMode.SetCheck(m_Mode==CAMERA_MODE_OBJECT);
	m_rbProjectModePerspective.SetCheck(m_ProjectMode==CAMERA_PROJECT_MODE_PERSPECTIVE);
	m_cbProjectModeOrtho.SetCheck(m_ProjectMode==CAMERA_PROJECT_MODE_ORTHO);
	m_pD3DView->GetCameraPos(m_CameraPosX,m_CameraPosY,m_CameraPosZ);
	m_pD3DView->GetCameraRotate(m_CameraRotateX,m_CameraRotateY);
	m_CameraRotateX=m_CameraRotateX*180/D3DX_PI;
	m_CameraRotateY=m_CameraRotateY*180/D3DX_PI;
	UpdateData(false);
}
void CDlgCameraSetting::OnBnClickedRadioSceneMode()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Mode=CAMERA_MODE_SCENE;
	ShowSetting();	
}

void CDlgCameraSetting::OnBnClickedRadioObjectMode()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Mode=CAMERA_MODE_OBJECT;
	ShowSetting();
}

BOOL CDlgCameraSetting::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_Mode=CUSOEditApp::GetInstance()->GetCameraSetting().CameraMode;
	m_ProjectMode=CUSOEditApp::GetInstance()->GetCameraSetting().CameraProjectMode;
	m_MoveStep=CUSOEditApp::GetInstance()->GetCameraSetting().CameraMoveStep;
	m_RotateStep=CUSOEditApp::GetInstance()->GetCameraSetting().CameraRotateStep;
	m_NearFace=CUSOEditApp::GetInstance()->GetCameraSetting().CameraNearFace;
	m_FarFace=CUSOEditApp::GetInstance()->GetCameraSetting().CameraFarFace;
	UpdateData(false);
	ShowSetting();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgCameraSetting::OnBnClickedRadioProjectModePerspective()
{
	// TODO: 在此添加控件通知处理程序代码
	m_ProjectMode=CAMERA_PROJECT_MODE_PERSPECTIVE;
	ShowSetting();
}

void CDlgCameraSetting::OnBnClickedRadioProjectModeOrtho()
{
	// TODO: 在此添加控件通知处理程序代码
	m_ProjectMode=CAMERA_PROJECT_MODE_ORTHO;
	ShowSetting();
}


void CDlgCameraSetting::OnBnClickedSetPos()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	if(m_pD3DView)
	{
		float CameraRotateX=m_CameraRotateX*D3DX_PI/180;
		float CameraRotateY=m_CameraRotateY*D3DX_PI/180;
		m_pD3DView->SetCameraPos(m_CameraPosX,m_CameraPosY,m_CameraPosZ);
		m_pD3DView->SetCameraRotate(CameraRotateX,CameraRotateY);

		m_pD3DView->UpdateCamera();
	}

}
