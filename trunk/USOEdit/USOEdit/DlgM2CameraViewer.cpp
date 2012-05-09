/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DlgM2CameraViewer.cpp                                    */
/*      ��������:  2011��11��09��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
// DlgM2CameraViewer.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DlgM2CameraViewer.h"




// CDlgM2CameraViewer �Ի���

IMPLEMENT_DYNAMIC(CDlgM2CameraViewer, CPropertyConfigDialog)

CDlgM2CameraViewer::CDlgM2CameraViewer(CWnd* pParent /*=NULL*/)
	: CPropertyConfigDialog(CDlgM2CameraViewer::IDD, pParent)
{
	m_IsModual=false;
	m_pModelResource=NULL;
}

CDlgM2CameraViewer::~CDlgM2CameraViewer()
{
}

void CDlgM2CameraViewer::DoDataExchange(CDataExchange* pDX)
{
	CPropertyConfigDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_CAMERA, m_lvCameraList);
}


BEGIN_MESSAGE_MAP(CDlgM2CameraViewer, CPropertyConfigDialog)
	ON_BN_CLICKED(IDC_BUTTON_USE_SELECTED_CAMERA, &CDlgM2CameraViewer::OnBnClickedButtonUseSelectedCamera)
END_MESSAGE_MAP()


// CDlgM2CameraViewer ��Ϣ�������

BOOL CDlgM2CameraViewer::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	m_lvCameraList.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);

	m_lvCameraList.InsertColumn(0,_T("����"),LVCFMT_LEFT,50);
	m_lvCameraList.InsertColumn(1,_T("FOV"),LVCFMT_LEFT,50);
	m_lvCameraList.InsertColumn(2,_T("����"),LVCFMT_LEFT,50);
	m_lvCameraList.InsertColumn(3,_T("Զ��"),LVCFMT_LEFT,50);
	m_lvCameraList.InsertColumn(4,_T("λ��"),LVCFMT_LEFT,100);
	m_lvCameraList.InsertColumn(5,_T("Ŀ��"),LVCFMT_LEFT,100);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CDlgM2CameraViewer::Init(CD3DWOWM2ModelResource * pModelResource,CD3DViewPort * pViewPort)
{
	m_pModelResource=pModelResource;
	m_pViewPort=pViewPort;
	FillList();
}

void CDlgM2CameraViewer::FillList()
{
	CEasyString Temp;
	m_lvCameraList.DeleteAllItems();
	for(UINT i=0;i<m_pModelResource->GetCameraCount();i++)
	{
		CD3DWOWM2ModelResource::CAMERA_INFO * pCameraInfo=m_pModelResource->GetCameraInfo(i);
		Temp.Format(_T("%d"),pCameraInfo->Type);
		int Item=m_lvCameraList.InsertItem(i,Temp);
		m_lvCameraList.SetItemData(Item,i);
		if(pCameraInfo->FOV.Animations.GetCount())
		{
			FLOAT FOV;
			GetInterpolationValue(0,true,pCameraInfo->FOV.InterpolationType,0,
				pCameraInfo->FOV.Animations[0],FOV);
			Temp.Format(_T("%g��"),FOV*180/D3DX_PI);
		}
		else
		{
			Temp=_T("��");
		}
		
		m_lvCameraList.SetItemText(Item,1,Temp);
		Temp.Format(_T("%g"),pCameraInfo->NearClipping);
		m_lvCameraList.SetItemText(Item,2,Temp);
		Temp.Format(_T("%g"),pCameraInfo->FarClipping);
		m_lvCameraList.SetItemText(Item,3,Temp);
		Temp.Format(_T("%g,%g,%g"),pCameraInfo->Position.x,pCameraInfo->Position.y,pCameraInfo->Position.z);
		m_lvCameraList.SetItemText(Item,4,Temp);
		Temp.Format(_T("%g,%g,%g"),pCameraInfo->Target.x,pCameraInfo->Target.y,pCameraInfo->Target.z);
		m_lvCameraList.SetItemText(Item,5,Temp);
	}
}
void CDlgM2CameraViewer::OnBnClickedButtonUseSelectedCamera()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	POSITION Pos=m_lvCameraList.GetFirstSelectedItemPosition();
	if(Pos)
	{
		int Item=m_lvCameraList.GetNextSelectedItem(Pos);
		UINT Index=m_lvCameraList.GetItemData(Item);
		CD3DWOWM2ModelResource::CAMERA_INFO * pCameraInfo=m_pModelResource->GetCameraInfo(Index);
		if(pCameraInfo)
		{
			m_pViewPort->GetSystemCamera()->LookAt(pCameraInfo->Position,pCameraInfo->Target,CD3DVector3(0,1,0));
			FLOAT FOV=D3DX_PI*47/180;
			if(pCameraInfo->FOV.Animations.GetCount())
			{
				GetInterpolationValue(0,true,pCameraInfo->FOV.InterpolationType,0,
					pCameraInfo->FOV.Animations[0],FOV);
			}
			
			m_pViewPort->GetSystemCamera()->SetProjectPerspective(FOV,
				(float)CUSOEditApp::GetInstance()->GetViewportSetting().ViewportWidth/
				(float)CUSOEditApp::GetInstance()->GetViewportSetting().ViewportHeight,
				pCameraInfo->NearClipping,
				pCameraInfo->FarClipping);
		}
	}
	
}
