/****************************************************************************/
/*                                                                          */
/*      文件名:    D3DViewPort.cpp                                          */
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
/*      文件名:    D3DViewPort.cpp                                          */
/*      创建日期:  2010年02月09日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
// D3DViewPort.cpp : 实现文件
//

#include "stdafx.h"
#include "D3DViewPort.h"

// CD3DViewPort



IMPLEMENT_DYNAMIC(CD3DViewPort, CWnd)
CD3DViewPort::CD3DViewPort()
{
	m_pD3DDevice=NULL;
	m_pUIRender=NULL;
	m_pRender=NULL;
	m_pObjectCreator=NULL;
	m_pGUI=NULL;	
	m_IsRotation=false;
	m_IsTransform=false;
	m_ObjectTransMode=OBJECT_TRANS_NONE;
	m_HaveTrans=false;
	m_pSelectedObject=NULL;
	m_UpdateStartTime=0;	
	m_FrameCount=0;
	m_FrameRate=0;
	m_pSystemLight=NULL;
	m_pSystemCamera=NULL;
	
}

CD3DViewPort::~CD3DViewPort()
{
	
	((CUSOEditApp *)AfxGetApp())->DeleteD3DViewPort(this);	

	m_pRenderThread->SafeTerminate();
	SAFE_RELEASE(m_pRenderThread);
	SAFE_RELEASE(m_pSystemCamera);
	SAFE_RELEASE(m_pSystemLight);
	SAFE_RELEASE(m_pGUI);
	SAFE_RELEASE(m_pObjectCreator);
	SAFE_RELEASE(m_pUIRender);
	SAFE_RELEASE(m_pRender);	
	SAFE_RELEASE(m_pD3DDevice);		
}


BEGIN_MESSAGE_MAP(CD3DViewPort, CWnd)	
	
	ON_WM_KILLFOCUS()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()



// CD3DViewPort 消息处理程序

BOOL CD3DViewPort::Create(CWnd * pParent)
{
	if(!CWnd::Create(NULL,_T("ViewPort"),WS_CHILD|WS_VISIBLE,
		CRect(0,0,
		CUSOEditApp::GetInstance()->GetViewportSetting().ViewportWidth,
		CUSOEditApp::GetInstance()->GetViewportSetting().ViewportHeight),
		pParent,100101))
		return FALSE;

	CD3DTexture::EnableSaveOrgInfo(true);

	m_pD3DDevice=new CD3DDevice();

	D3DDEVICE_PARAMS Params;
	Params.hFocusWindow=GetSafeHwnd();
	//Params.Adapter=1;
	//Params.DeviceType=D3DDEVTYPE_REF;
	Params.PresentParams.PresentationInterval=D3DPRESENT_INTERVAL_ONE;
	//Params.BehaviorFlags=D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	Params.PresentParams.EnableAutoDepthStencil = TRUE;
	Params.PresentParams.AutoDepthStencilFormat = D3DFMT_D32F_LOCKABLE;
	Params.PresentParams.BackBufferCount=3;
	Params.IsUseMultiThreadRender=true;
	if(!m_pD3DDevice->Create(Params))
	{
		Params.BehaviorFlags=D3DCREATE_SOFTWARE_VERTEXPROCESSING;
		Params.PresentParams.AutoDepthStencilFormat = D3DFMT_D24X8;
		if(!m_pD3DDevice->Create(Params))
		{
			AfxMessageBox(_T("设备创建失败"));
			return false;
		}
	}

	m_pUIRender=new CD3DUIRender();
	m_pRender=new CD3DSceneRender();

	m_pUIRender->Create(m_pD3DDevice);

	//m_pRender->ShowNodeFrame(true);

	CD3DBoundingBox SceneBox(0,-BLZ_ADT_MAP_AREA_SIZE*(31.5f),-BLZ_ADT_MAP_AREA_SIZE*64,
		BLZ_ADT_MAP_AREA_SIZE*64,BLZ_ADT_MAP_AREA_SIZE*(33.5f),0);
	m_pRender->Create(m_pD3DDevice,CD3DSceneRender::TREE_MODE_OC,SceneBox,3,13);

	

	m_pObjectCreator=new CD3DGUIObjectCreator(m_pUIRender);

	m_pGUI=new CD3DGUI(m_pObjectCreator,GetSafeHwnd());
	
	((CUSOEditApp *)AfxGetApp())->AddD3DViewPort(this);


	m_pSystemLight=new CD3DLight();
	m_pSystemLight->SetDirect(1.0f,-1.0f,0.0f);
	m_pSystemLight->SetAmbient(0.5f,0.5f,0.5f,1.0f);
	m_pSystemLight->SetDiffuse(1.0f,1.0f,1.0f,1.0f);
	m_pSystemLight->SetSpecular(0.8f,0.8f,0.8f,1.0f);		
	m_pSystemLight->SetName(_T("系统灯光"));
	m_pRender->SetGlobalLight(m_pSystemLight);
	
	

	CD3DMatrix Mat;
	Mat.SetIdentity();



	m_pSystemCamera=new CD3DCamera();			
	m_pSystemCamera->SetName(_T("系统摄像机"));
	m_pRender->SetCamera(m_pSystemCamera);
	m_pRender->AddRootObject(m_pSystemCamera);


	ApplyCameraChange(true);
	
	CD3DTexture::ClearPath();
	CD3DTexture::AddPathList(((CUSOEditApp *)AfxGetApp())->GetTexturePathList());
	CD3DFX::ClearPath();
	CD3DFX::AddPathList(((CUSOEditApp *)AfxGetApp())->GetFXPathList());


	CD3DTexture::AddPath(GetModulePath(NULL));
	CD3DFX::AddPath(GetModulePath(NULL));

	m_UpdateStartTime=CEasyTimer::GetTime();
	m_FrameRateTimer.SaveTime();


	m_pRender->CreateDepthTexture();


	m_pRenderThread=new CD3DRenderThread();
	m_pRenderThread->SetDevice(m_pD3DDevice);
	m_pRenderThread->SetClearColor(0,D3DCOLOR_XRGB(128,128,128));
	m_pRenderThread->AddRender(0,m_pRender);
	m_pRenderThread->AddRender(0,m_pUIRender);

	m_pRenderThread->Start();
		
	//CBLZM2Loader Loader;

	//Loader.LoadFromFile("Human\\Male\\HumanMale.M2");
	//Loader.LoadSkinFromFile("Human\\Male\\HumanMale00.skin");

	//UINT SubMeshCount;
	//CD3DSubMesh * pSubMesh=Loader.MakeStaticModelSubMest(m_pD3DDevice,SubMeshCount);

	return TRUE;
}

void CD3DViewPort::Update()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	FUNCTION_BEGIN;

	UINT CurTime=CEasyTimer::GetTime();
	m_UpdateControlTimer.SaveTime(CurTime);
	float time=(CurTime-m_UpdateStartTime)/1000.0f;
	m_pGUI->Update();
	//m_pUIRender->Update(time);
	m_pRender->Update(time);	
	m_pD3DDevice->Update(time);

	//m_pD3DDevice->StartRenderToTexture(m_pRender->GetDepthTexture());
	//m_pD3DDevice->Clear(D3DCOLOR_XRGB(0xff,0xff,0xff),D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER);
	//m_pRender->RenderDepthTexture();
	//m_pD3DDevice->EndRenderToTexture();

	//m_pD3DDevice->StartRender(m_ClearColor);
	//m_pRender->Render();
	//m_pUIRender->Render();
	//m_pD3DDevice->EndRender();

	m_FrameCount++;
	FUNCTION_END;
	if(m_FrameRateTimer.IsTimeOut(FRAME_RATE_STAT_TIME))
	{
		m_FrameRateTimer.SaveTime(CurTime);
		m_FrameRate=(FLOAT)m_FrameCount*1000.0f/FRAME_RATE_STAT_TIME;
		m_FrameCount=0;

		CPerformanceStatistician::GetInstance()->PrintPerformanceStat(LOG_IMPORTANT_CHANNEL);
		CPerformanceStatistician::GetInstance()->PrintPerformanceStatUnit(LOG_IMPORTANT_CHANNEL);
		CPerformanceStatistician::GetInstance()->ResetPerformanceStat();
	}

	if(!m_UpdateControlTimer.IsTimeOut(10))
		Sleep(1);
}


BOOL CD3DViewPort::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类

	if(pMsg->message==WM_MOUSEWHEEL)
	{	
	
		POINT point;
		point.x=LOWORD(pMsg->lParam);
		point.y=HIWORD(pMsg->lParam);
		ScreenToClient(&point);
		pMsg->lParam=point.y<<16|point.x;
	}

	switch(pMsg->message)
	{
	case WM_KEYDOWN:
		{
			switch(pMsg->wParam)
			{
			case VK_CONTROL:
				m_ObjectTransMode=OBJECT_TRANS_MODE1;
				break;			
			case VK_SHIFT:
				m_ObjectTransMode=OBJECT_TRANS_MODE2;
				break;
			}
		}
		break;
	case WM_KEYUP:
		{
			switch(pMsg->wParam)
			{
			case VK_CONTROL:
			case VK_SHIFT:
				m_ObjectTransMode=OBJECT_TRANS_NONE;
				break;		
			}
		}
		break;
	case WM_LBUTTONDOWN:
		{
			SetFocus();
			m_StartX=GET_X_LPARAM(pMsg->lParam);
			m_StartY=GET_Y_LPARAM(pMsg->lParam);			
			m_IsRotation=true;			
		}
		break;
	case WM_LBUTTONUP:
		{
			m_IsRotation=false;			
			if(!m_HaveTrans)
			{			
				int X=GET_X_LPARAM(pMsg->lParam);
				int Y=GET_Y_LPARAM(pMsg->lParam);
				CD3DVector3 Point,Dir;
				m_pRender->GetCamera()->GetPickRay(X,Y,
					CUSOEditApp::GetInstance()->GetViewportSetting().ViewportWidth,
					CUSOEditApp::GetInstance()->GetViewportSetting().ViewportHeight,Point,Dir);
				FLOAT Dis;
				if(m_pSystemLight->PickObject(Point,Dir,Dis))
					m_pSelectedObject=m_pSystemLight;
				else
					m_pSelectedObject=GetDocument()->PickObject(Point,Dir);
				if(m_pSelectedObject)
				{
					CUSOObject * pObject=(CUSOObject *)m_pSelectedObject->GetParam();
					if(pObject)
					{
						if(pObject->GetTreeCtrl())
							pObject->GetTreeCtrl()->SelectItem(pObject->GetTreeItem());
					}
					else
					{
						GetDocument()->GetTreeView()->GetTreeCtrl().SelectItem(NULL);
					}
				}
				else
				{
					GetDocument()->GetTreeView()->GetTreeCtrl().SelectItem(NULL);
				}
			}
			m_HaveTrans=false;
		}
		break;
	case WM_RBUTTONDOWN:
		{
			m_StartX=GET_X_LPARAM(pMsg->lParam);
			m_StartY=GET_Y_LPARAM(pMsg->lParam);
			m_IsTransform=true;
		}
		break;
	case WM_RBUTTONUP:
		{
			m_IsTransform=false;
			m_HaveTrans=false;
		}
		break;
	case WM_MOUSEMOVE:
		
		if(m_IsRotation)
		{
			m_HaveTrans=true;

			int dx=GET_X_LPARAM(pMsg->lParam)-m_StartX;
			int dy=GET_Y_LPARAM(pMsg->lParam)-m_StartY;

			if(m_ObjectTransMode==OBJECT_TRANS_MODE1&&m_pSelectedObject)
			{
				FLOAT RY=-D3DX_PI*dx*CUSOEditApp::GetInstance()->GetCameraSetting().CameraRotateStep;

				CD3DMatrix Mat=m_pSelectedObject->GetLocalMatrix();

				Mat=Mat.GetScaleRotation()*CD3DMatrix::FromRotationY(RY)*Mat.GetTranslationMatrix();

				m_pSelectedObject->SetLocalMatrix(Mat);		
			}
			else if(m_ObjectTransMode==OBJECT_TRANS_MODE2&&m_pSelectedObject)
			{
				FLOAT RX=-D3DX_PI*dy*CUSOEditApp::GetInstance()->GetCameraSetting().CameraRotateStep;
				FLOAT RZ=-D3DX_PI*dx*CUSOEditApp::GetInstance()->GetCameraSetting().CameraRotateStep;

				CD3DMatrix Mat=m_pSelectedObject->GetLocalMatrix();

				Mat=Mat.GetScaleRotation()*CD3DMatrix::FromRotationX(RX)*CD3DMatrix::FromRotationZ(RZ)*Mat.GetTranslationMatrix();

				m_pSelectedObject->SetLocalMatrix(Mat);		
			}
			else
			{
				if(CUSOEditApp::GetInstance()->GetCameraSetting().CameraMode==CAMERA_MODE_SCENE)
				{
					m_CameraRotationX+=-D3DX_PI*dy*CUSOEditApp::GetInstance()->GetCameraSetting().CameraRotateStep;
					m_CameraRotationY+=-D3DX_PI*dx*CUSOEditApp::GetInstance()->GetCameraSetting().CameraRotateStep;
				}
				else
				{
					m_CameraRotationX+=D3DX_PI*dy*CUSOEditApp::GetInstance()->GetCameraSetting().CameraRotateStep;
					m_CameraRotationY+=D3DX_PI*dx*CUSOEditApp::GetInstance()->GetCameraSetting().CameraRotateStep;
				}

				UpdateCamera();
			}			

			m_StartX=GET_X_LPARAM(pMsg->lParam);
			m_StartY=GET_Y_LPARAM(pMsg->lParam);

			
		}
		if(m_IsTransform)
		{
			m_HaveTrans=true;

			int dx=GET_X_LPARAM(pMsg->lParam)-m_StartX;
			int dy=GET_Y_LPARAM(pMsg->lParam)-m_StartY;
			
			if(m_ObjectTransMode==OBJECT_TRANS_MODE1&&m_pSelectedObject)
			{
				CD3DVector3 Trans;

				Trans.z=-dy*CUSOEditApp::GetInstance()->GetCameraSetting().CameraMoveStep;
				Trans.y=0;
				Trans.x=dx*CUSOEditApp::GetInstance()->GetCameraSetting().CameraMoveStep;
				Trans*=m_pRender->GetCamera()->GetWorldMatrix().GetRotation();
				Trans.y=0;

				CD3DMatrix Mat=m_pSelectedObject->GetLocalMatrix();

				Mat*=CD3DMatrix::FromTranslation(Trans);

				CD3DVector3 Pos=Mat.GetTranslation();

				if(GetDocument()->GetHeightByXZ(Pos,Pos.y))
				{
					Mat.SetTranslation(Pos);
				}

				m_pSelectedObject->SetLocalMatrix(Mat);				
			}
			else if(m_ObjectTransMode==OBJECT_TRANS_MODE2&&m_pSelectedObject)
			{
				CD3DVector3 Trans;

				Trans.z=0;
				Trans.y=-dy*CUSOEditApp::GetInstance()->GetCameraSetting().CameraMoveStep;
				Trans.x=0;
				Trans*=m_pRender->GetCamera()->GetWorldMatrix().GetRotation();
				Trans.x=0;
				Trans.z=0;		

				CD3DMatrix Mat=m_pSelectedObject->GetLocalMatrix();

				Mat*=CD3DMatrix::FromTranslation(Trans);	

				m_pSelectedObject->SetLocalMatrix(Mat);				
			}
			else
			{
				CD3DVector3 Trans;

				Trans.x=-dx*CUSOEditApp::GetInstance()->GetCameraSetting().CameraMoveStep;
				Trans.y=dy*CUSOEditApp::GetInstance()->GetCameraSetting().CameraMoveStep;
				Trans.z=0;

				if(CUSOEditApp::GetInstance()->GetCameraSetting().CameraMode==CAMERA_MODE_SCENE)
				{
					Trans*=m_pRender->GetCamera()->GetWorldMatrix().GetRotation();
				}
				

				m_CameraXTransform+=Trans.x;
				m_CameraYTransform+=Trans.y;
				m_CameraZTransform+=Trans.z;

				UpdateCamera();
			}

			m_StartX=GET_X_LPARAM(pMsg->lParam);
			m_StartY=GET_Y_LPARAM(pMsg->lParam);

			
		}
		
		break;

	case WM_MOUSEWHEEL:
		{
			CD3DVector3 Trans;

			if(CUSOEditApp::GetInstance()->GetCameraSetting().CameraMode==CAMERA_MODE_SCENE)
			{
				Trans.x=0;
				Trans.y=0;
				Trans.z=-GET_WHEEL_DELTA_WPARAM(pMsg->wParam)*CUSOEditApp::GetInstance()->GetCameraSetting().CameraMoveStep*50.0f/WHEEL_DELTA;

				Trans*=m_pRender->GetCamera()->GetWorldMatrix().GetRotation();

			}
			else
			{
				Trans.x=m_CameraXTransform;
				Trans.y=m_CameraYTransform;
				Trans.z=m_CameraZTransform;

				Trans.Normalize();

				Trans*=GET_WHEEL_DELTA_WPARAM(pMsg->wParam)*CUSOEditApp::GetInstance()->GetCameraSetting().CameraMoveStep*10.0f/WHEEL_DELTA;
			}
			
			m_CameraXTransform+=Trans.x;
			m_CameraYTransform+=Trans.y;
			m_CameraZTransform+=Trans.z;
			

			UpdateCamera();

		}
		break;
	}

	if(m_pGUI)
	{
		if(m_pGUI->HandleMessage(pMsg->message, pMsg->wParam, pMsg->lParam ))
			return true;
	}
	
	return CWnd::PreTranslateMessage(pMsg);
}

CUSOEditDoc * CD3DViewPort::GetDocument()
{
	return ((CD3DView *)GetParent())->GetDocument();
}

void CD3DViewPort::ApplyCameraChange(bool IsReset)
{	
	switch(CUSOEditApp::GetInstance()->GetCameraSetting().CameraProjectMode)
	{
	case CAMERA_PROJECT_MODE_PERSPECTIVE:
		{			
			m_pRender->GetCamera()->SetProjectPerspective(D3DX_PI*47/180,
				(float)CUSOEditApp::GetInstance()->GetViewportSetting().ViewportWidth/
				(float)CUSOEditApp::GetInstance()->GetViewportSetting().ViewportHeight,
				CUSOEditApp::GetInstance()->GetCameraSetting().CameraNearFace,
				CUSOEditApp::GetInstance()->GetCameraSetting().CameraFarFace);		

			if(IsReset)
			{
				if(CUSOEditApp::GetInstance()->GetCameraSetting().CameraMode==CAMERA_MODE_SCENE)
				{
					m_CameraXTransform=0;
					m_CameraYTransform=0;
					m_CameraZTransform=-2.0f;
					m_CameraRotationX=0;
					m_CameraRotationY=0;
				}
				else
				{
					m_CameraXTransform=0;
					m_CameraYTransform=1.0f;
					m_CameraZTransform=-3.0f;
					m_CameraRotationX=0;
					m_CameraRotationY=0;
				}
			}

			
		}
		break;
	case CAMERA_PROJECT_MODE_ORTHO:
		{
			
			CUSOObject * pRootObject=GetDocument()->GetRootObject();
			CD3DBoundingBox BoundingBox=GetSceneBoundingBox(pRootObject);
			float Width=BoundingBox.m_Max.x-BoundingBox.m_Min.x;
			float Height=BoundingBox.m_Max.z-BoundingBox.m_Min.z;
			if(Width/Height<(float)CUSOEditApp::GetInstance()->GetViewportSetting().ViewportWidth/
				(float)CUSOEditApp::GetInstance()->GetViewportSetting().ViewportHeight)
			{
				Width=Height*(float)CUSOEditApp::GetInstance()->GetViewportSetting().ViewportWidth/
					(float)CUSOEditApp::GetInstance()->GetViewportSetting().ViewportHeight;
			}
			else
			{
				Height=Width*(float)CUSOEditApp::GetInstance()->GetViewportSetting().ViewportHeight/
					(float)CUSOEditApp::GetInstance()->GetViewportSetting().ViewportWidth;
			}
			CD3DVector3 Pos;
			Pos.x=(BoundingBox.m_Max.x+BoundingBox.m_Min.x)/2;
			Pos.y=(BoundingBox.m_Max.y+BoundingBox.m_Min.y)/2+(BoundingBox.m_Max.y-BoundingBox.m_Min.y);
			Pos.z=(BoundingBox.m_Max.z+BoundingBox.m_Min.z)/2;
			m_pRender->GetCamera()->SetProjectOrtho(Width,Height,
				CUSOEditApp::GetInstance()->GetCameraSetting().CameraNearFace,
				CUSOEditApp::GetInstance()->GetCameraSetting().CameraFarFace);
			//m_pRender->GetCamera()->SetPos(Pos);
			//m_pRender->GetCamera()->SetRotation(0,PI/2,0);



			m_CameraXTransform=Pos.x;
			m_CameraYTransform=Pos.y;
			m_CameraZTransform=Pos.z;
			m_CameraRotationX=PI/2;
			m_CameraRotationY=0;

		}

		break;
	}

	UpdateCamera();	
	

	m_IsRotation=false;
	m_IsTransform=false;
	
}

void CD3DViewPort::MoveCamera(FLOAT X,FLOAT Y,FLOAT Z,FLOAT RotateX,FLOAT RotateY)
{
	m_CameraXTransform=X;
	m_CameraYTransform=Y;
	m_CameraZTransform=Z;

	m_CameraRotationX=RotateX;
	m_CameraRotationY=RotateY;

	UpdateCamera();
}

void CD3DViewPort::UpdateCamera()
{
	CD3DMatrix Mat;
	
	
	Mat.SetIdentity();

	if(CUSOEditApp::GetInstance()->GetCameraSetting().CameraMode==CAMERA_MODE_SCENE)
	{
		Mat*=CD3DMatrix::FromRotationX(m_CameraRotationX);
		Mat*=CD3DMatrix::FromRotationY(m_CameraRotationY);

		Mat*=CD3DMatrix::FromTranslation(CD3DVector3(m_CameraXTransform,m_CameraYTransform,m_CameraZTransform));
	}
	else
	{
		Mat*=CD3DMatrix::FromTranslation(CD3DVector3(m_CameraXTransform,m_CameraYTransform,m_CameraZTransform));


		Mat*=CD3DMatrix::FromRotationX(m_CameraRotationX);
		Mat*=CD3DMatrix::FromRotationY(m_CameraRotationY);
	}
	
	

	
	m_pRender->GetCamera()->SetLocalMatrix(Mat);

}


CD3DBoundingBox CD3DViewPort::GetSceneBoundingBox(CUSOObject * pSceneRoot)
{
	CD3DBoundingBox BoundingBox;
	bool IsEmpty=true;
	

	CNameObject * pObject=pSceneRoot->GetBindObject();
	if(pObject)
	{
		if(pObject->IsKindOf(GET_CLASS_INFO(CD3DObject)))
		{
			CD3DObject * pD3DObject=(CD3DObject *)pObject;
			if(pD3DObject->GetBoundingBox())
			{
 				BoundingBox=((*(pD3DObject->GetBoundingBox()))*pD3DObject->GetWorldMatrix());
				IsEmpty=false;
			}
		}
	}
	for(int i=0;i<pSceneRoot->GetChildCount();i++)
	{
		CD3DBoundingBox ChildBoundingBox=GetSceneBoundingBox(pSceneRoot->GetChildByIndex(i));
		CD3DVector3 Size=ChildBoundingBox.GetSize();
		if(Size.x!=0||Size.y!=0||Size.z!=0)
		{
			if(IsEmpty)
			{
				BoundingBox=ChildBoundingBox;
				IsEmpty=false;
			}
			else
			{
				BoundingBox+=ChildBoundingBox;
			}
		}
		
	}
	return BoundingBox;
}

void CD3DViewPort::SaveDepthTexture(LPCTSTR szFileName)
{
	D3DXSaveTextureToFile(szFileName,D3DXIFF_PNG,m_pRender->GetDepthTexture()->GetD3DTexture(),NULL);
}
void CD3DViewPort::OnKillFocus(CWnd* pNewWnd)
{
	CWnd::OnKillFocus(pNewWnd);

	// TODO: 在此处添加消息处理程序代码
	//m_IsRotation=false;
	//m_IsTransform=false;
	//m_ObjectTransMode=OBJECT_TRANS_NONE;
	//m_HaveTrans=false;
	m_pRenderThread->SetClearColor(0,D3DCOLOR_XRGB(128,128,128));
	
}

void CD3DViewPort::OnSetFocus(CWnd* pOldWnd)
{
	CWnd::OnSetFocus(pOldWnd);

	// TODO: 在此处添加消息处理程序代码
	m_pRenderThread->SetClearColor(0,D3DCOLOR_XRGB(64,64,128));
}
