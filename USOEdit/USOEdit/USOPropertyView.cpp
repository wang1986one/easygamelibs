/****************************************************************************/
/*                                                                          */
/*      文件名:    USOPropertyView.cpp                                      */
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
/*      文件名:    USOPropertyView.cpp                                      */
/*      创建日期:  2010年02月09日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
// USOPropertyView.cpp : CUSOPropertyView 类的实现
//

#include "stdafx.h"




enum USO_OBJECT_PROPERTY
{
	USOP_NONE,
	USOP_NAME,
	USOP_TYPE,
	USOP_FLAG,
	USOP_X,
	USOP_Y,
	USOP_Z,
	USOP_PITCH,
	USOP_YAW,
	USOP_ROLL,
	USOP_SCALE_X,
	USOP_SCALE_Y,
	USOP_SCALE_Z,
	USOP_VISIBLE,	
	USOP_BOUNDING_BOX,
	USOP_BOUNDING_BOX_SIZE,
	USOP_STM_IS_GROUND,
	USOP_DYM_SKIN_SETTING,
	USOP_SKIN_NAME,
	USOP_ANI_NAME,

	USOP_SUBMESH_ID,
	USOP_SUBMESH_NAME,
	USOP_SUBMESH_PROPERTY,
	USOP_SUBMESH_VISIBLE,
	USOP_SUBMESH_DIFFUSE,
	USOP_SUBMESH_AMBIENT,
	USOP_SUBMESH_SPECULAR,
	USOP_SUBMESH_EMISSIVE,
	USOP_SUBMESH_POWER,
	USOP_SUBMESH_TRIANGLES,
	USOP_SUBMESH_FX,
	USOP_SUBMESH_TEXTURE,

	USOP_FX_CONTENT,

	
	USOP_WOWM2_ANIMATION,
	USOP_WOWM2_CHAR_SETTING,	
	USOP_WOWM2_ATTACJMENTS,
	USOP_WOWM2_SKINS,

	USOP_WOWM2_VERSION,
	USOP_WOWM2_RES_FLAG,
	USOP_WOWM2_RES_ANI_SEQUENCE,
	USOP_WOWM2_RES_BONE,
	USOP_WOWM2_RES_CAMERA,

	USOP_WOWWMO_DOODAD_SET,
	USOP_WOWWMO_GROUP_FLAG,
	USOP_WOWWMO_GROUP_PORTAL_COUNT,

	USOP_WOW_PARTICLE_FLAG,
	USOP_WOW_PARTICLE_PARAM,

};

// CUSOPropertyView

IMPLEMENT_DYNCREATE(CUSOPropertyView, CTreeView)

BEGIN_MESSAGE_MAP(CUSOPropertyView, CPropertyView)
	ON_MESSAGE(WM_PROPERTY_GRID_EDIT_START,OnPropertyEditStart)
	ON_MESSAGE(WM_PROPERTY_GRID_EDIT_END,OnPropertyEditEnd)
	ON_MESSAGE(WM_PROPERTY_GRID_VALUE_CHANGED,OnPropertyChanged)
END_MESSAGE_MAP()


// CUSOPropertyView 构造/析构

CUSOPropertyView::CUSOPropertyView()
{
	// TODO: 在此处添加构造代码
	m_pUSOObject=NULL;
}

CUSOPropertyView::~CUSOPropertyView()
{
}

BOOL CUSOPropertyView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改 CREATESTRUCT cs 来修改窗口类或样式

	return CPropertyView::PreCreateWindow(cs);
}

void CUSOPropertyView::OnInitialUpdate()
{
	CPropertyView::OnInitialUpdate();

	// TODO: 调用 GetTreeCtrl() 直接访问 TreeView 的树控件，
	// 从而可以用项填充 TreeView。

	m_DlgWOWCharSetting.Create(m_DlgWOWCharSetting.IDD,AfxGetMainWnd());
	m_DlgAniSetting.Create(m_DlgAniSetting.IDD,AfxGetMainWnd());
	m_DlgTrianglesViewer.Create(m_DlgTrianglesViewer.IDD,AfxGetMainWnd());
	m_DlgBoneViewer.Create(m_DlgBoneViewer.IDD,AfxGetMainWnd());
	m_DlgAniSequenceViewer.Create(m_DlgAniSequenceViewer.IDD,AfxGetMainWnd());
	m_DlgParticleEmitterParamViewer.Create(m_DlgParticleEmitterParamViewer.IDD,AfxGetMainWnd());
	m_DlgM2CameraViewer.Create(m_DlgM2CameraViewer.IDD,AfxGetMainWnd());
	m_DlgAttachmentViewer.Create(m_DlgAttachmentViewer.IDD,AfxGetMainWnd());
	m_DlgM2SkinView.Create(m_DlgM2SkinView.IDD,AfxGetMainWnd());
}


// CUSOPropertyView 诊断

#ifdef _DEBUG
void CUSOPropertyView::AssertValid() const
{
	CPropertyView::AssertValid();
}

void CUSOPropertyView::Dump(CDumpContext& dc) const
{
	CPropertyView::Dump(dc);
}

CUSOEditDoc* CUSOPropertyView::GetDocument() // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CUSOEditDoc)));
	return (CUSOEditDoc*)m_pDocument;
}
#endif //_DEBUG


// CUSOPropertyView 消息处理程序


void CUSOPropertyView::SetUSOObject(CUSOObject * pUSOObject)
{
	m_pUSOObject=pUSOObject;
	RefreshObjectProperty();
}

void CUSOPropertyView::RefreshObjectProperty()
{
	m_PropertyGrid.Clear();
	if(m_pUSOObject==NULL)
		return;	

	PROPERTY_INFO info;	
	CString Temp;
	CRect Rect;


	CNameObject * pBindObject=m_pUSOObject->GetBindObject();
	if(pBindObject==NULL)
		return;

	m_PropertyGrid.SetRedraw(false);

	

	info.PropertyType=PROT_NONE;
	info.Name=_T("类型");
	info.ID=USOP_NAME;	
	info.Value=m_pUSOObject->GetTypeName();
	m_PropertyGrid.AddProperty(&info);	

	

	if(pBindObject->IsKindOf(GET_CLASS_INFO(CD3DObject)))
	{
		CD3DObject * pObject=(CD3DObject *)pBindObject;

		info.PropertyType=PROT_EDIT;
		info.Name=_T("名称");
		info.ID=USOP_NAME;
		info.Value=pObject->GetName();
		m_PropertyGrid.AddProperty(&info);	

		info.PropertyType=PROT_NONE;
		info.Name=_T("Flag");
		info.ID=USOP_FLAG;
		Temp.Format(_T("0x%llX"),pObject->GetFlag());
		info.Value=Temp;
		m_PropertyGrid.AddProperty(&info);	

		


		CD3DVector3 Trans;

		Trans=pObject->GetLocalMatrix().GetTranslation();

		info.PropertyType=PROT_EDIT;
		info.Name=_T("X");
		info.ID=USOP_X;
		info.Value=Trans.x;
		m_PropertyGrid.AddProperty(&info);

		info.PropertyType=PROT_EDIT;
		info.Name=_T("Y");
		info.ID=USOP_Y;
		info.Value=Trans.y;
		m_PropertyGrid.AddProperty(&info);

		info.PropertyType=PROT_EDIT;
		info.Name=_T("Z");
		info.ID=USOP_Z;
		info.Value=Trans.z;
		m_PropertyGrid.AddProperty(&info);

		CD3DQuaternion Rotation=pObject->GetLocalMatrix().GetRotationOnQuaternion();
		FLOAT Pitch,Yaw,Roll;
		Rotation.GetYawPitchRoll(Yaw,Pitch,Roll);
		Pitch=Pitch*180/PI;
		Yaw=Yaw*180/PI;
		Roll=Roll*180/PI;

		info.PropertyType=PROT_EDIT;
		info.Name=_T("Pitch");
		info.ID=USOP_PITCH;
		info.Value=Pitch;
		m_PropertyGrid.AddProperty(&info);

		info.PropertyType=PROT_EDIT;
		info.Name=_T("Yaw");
		info.ID=USOP_YAW;
		info.Value=Yaw;
		m_PropertyGrid.AddProperty(&info);

		info.PropertyType=PROT_EDIT;
		info.Name=_T("Roll");
		info.ID=USOP_ROLL;
		info.Value=Roll;
		m_PropertyGrid.AddProperty(&info);	


		CD3DVector3 Scale;

		Scale=pObject->GetLocalMatrix().GetScale();

		info.PropertyType=PROT_EDIT;
		info.Name=_T("X轴缩放");
		info.ID=USOP_SCALE_X;
		info.Value=Scale.x;
		m_PropertyGrid.AddProperty(&info);	

		info.PropertyType=PROT_EDIT;
		info.Name=_T("Y轴缩放");
		info.ID=USOP_SCALE_Y;
		info.Value=Scale.y;
		m_PropertyGrid.AddProperty(&info);

		info.PropertyType=PROT_EDIT;
		info.Name=_T("Z轴缩放");
		info.ID=USOP_SCALE_Z;
		info.Value=Scale.z;
		m_PropertyGrid.AddProperty(&info);

		
		info.PropertyType=PROT_BOOL;
		info.Name=_T("可见性");
		info.ID=USOP_VISIBLE;
		info.Value=pObject->IsVisible();
		m_PropertyGrid.AddProperty(&info);	

		if(pObject->GetBoundingBox())
		{
			info.PropertyType=PROT_NONE;
			info.Name=_T("包围盒");
			info.ID=USOP_BOUNDING_BOX_SIZE;
			Temp.Format(_T("(%g,%g,%g)-(%g,%g,%g)"),
				pObject->GetBoundingBox()->m_Min.x,
				pObject->GetBoundingBox()->m_Min.y,
				pObject->GetBoundingBox()->m_Min.z,
				pObject->GetBoundingBox()->m_Max.x,
				pObject->GetBoundingBox()->m_Max.y,
				pObject->GetBoundingBox()->m_Max.z);
			info.Value=Temp;
			m_PropertyGrid.AddProperty(&info);		

			info.PropertyType=PROT_NONE;
			info.Name=_T("包围盒大小");
			info.ID=USOP_BOUNDING_BOX_SIZE;
			CD3DVector3 Size=(pObject->GetBoundingBox()->m_Max)-(pObject->GetBoundingBox()->m_Min);
			Temp.Format(_T("(%g,%g,%g)"),Size.x,Size.y,Size.z);
			info.Value=Temp;
			m_PropertyGrid.AddProperty(&info);		
		}		
	}

		
	if(pBindObject->IsKindOf(GET_CLASS_INFO(CD3DObjectResource)))
	{
		CD3DObjectResource * pResource=(CD3DObjectResource *)pBindObject;

		info.PropertyType=PROT_EDIT;
		info.Name=_T("名称");
		info.ID=USOP_NAME;
		info.Value=pResource->GetName();
		m_PropertyGrid.AddProperty(&info);	

		if(pResource->IsKindOf(GET_CLASS_INFO(CD3DWOWM2ModelResource)))
		{
			CD3DWOWM2ModelResource * pM2Resource=(CD3DWOWM2ModelResource *)pResource;

			info.PropertyType=PROT_NONE;
			info.Name=_T("Version");
			info.ID=USOP_WOWM2_VERSION;
			Temp.Format(_T("0x%X"),pM2Resource->GetVersion());
			info.Value=Temp;
			m_PropertyGrid.AddProperty(&info);
			

			info.PropertyType=PROT_NONE;
			info.Name=_T("M2Flag");
			info.ID=USOP_WOWM2_RES_FLAG;
			Temp.Format(_T("0x%X"),pM2Resource->GetFlag());
			info.Value=Temp;
			m_PropertyGrid.AddProperty(&info);


			info.PropertyType=PROT_CUSTOM;
			info.Name=_T("动画序列");
			info.ID=USOP_WOWM2_RES_ANI_SEQUENCE;
			info.Value=_T("查看");
			info.pConfigDialog=&m_DlgAniSequenceViewer;
			m_DlgAniSequenceViewer.Init(pM2Resource);
			m_PropertyGrid.AddProperty(&info);


			info.PropertyType=PROT_CUSTOM;
			info.Name=_T("骨骼");
			info.ID=USOP_WOWM2_RES_BONE;
			info.Value=_T("查看");
			info.pConfigDialog=&m_DlgBoneViewer;
			m_DlgBoneViewer.Init(pM2Resource);
			m_PropertyGrid.AddProperty(&info);


			info.PropertyType=PROT_CUSTOM;
			info.Name=_T("摄像机");
			info.ID=USOP_WOWM2_RES_CAMERA;
			info.Value=_T("查看");
			info.pConfigDialog=&m_DlgM2CameraViewer;
			m_DlgM2CameraViewer.Init(pM2Resource,GetDocument()->GetD3DViewPort());
			m_PropertyGrid.AddProperty(&info);

	
		}
		
	}
		
	if(pBindObject->IsKindOf(GET_CLASS_INFO(CD3DWOWM2Model)))
	{
		CD3DWOWM2Model * pModel=(CD3DWOWM2Model *)pBindObject;	

		

		info.PropertyType=PROT_CUSTOM;
		info.Name=_T("动画设置");
		info.ID=USOP_WOWM2_ANIMATION;
		info.Value=_T("<编辑>");
		info.pConfigDialog=&m_DlgAniSetting;
		m_DlgAniSetting.Init(pModel,m_pUSOObject);
		m_PropertyGrid.AddProperty(&info);		

		if(pBindObject->IsKindOf(GET_CLASS_INFO(CD3DWOWM2CharacterModel)))
		{
			
			CD3DWOWM2CharacterModel * pCharModel=(CD3DWOWM2CharacterModel *)pBindObject;	

			info.PropertyType=PROT_CUSTOM;
			info.Name=_T("角色设置");
			info.ID=USOP_WOWM2_CHAR_SETTING;
			info.Value=_T("<编辑>");
			info.pConfigDialog=&m_DlgWOWCharSetting;
			m_DlgWOWCharSetting.Init(pCharModel,m_pUSOObject);
			m_PropertyGrid.AddProperty(&info);	
			
		}


		info.PropertyType=PROT_CUSTOM;
		info.Name=_T("挂载点");
		info.ID=USOP_WOWM2_ATTACJMENTS;
		info.Value=_T("<编辑>");
		info.pConfigDialog=&m_DlgAttachmentViewer;
		m_DlgAttachmentViewer.Init(pModel,m_pUSOObject);
		m_PropertyGrid.AddProperty(&info);		


		info.PropertyType=PROT_CUSTOM;
		info.Name=_T("Skin");
		info.ID=USOP_WOWM2_SKINS;
		info.Value=_T("<编辑>");
		info.pConfigDialog=&m_DlgM2SkinView;
		m_DlgM2SkinView.Init(pModel);
		m_PropertyGrid.AddProperty(&info);		


		
		
	}
	
	if(pBindObject->IsKindOf(GET_CLASS_INFO(CD3DWOWWMOModel)))
	{
		CD3DWOWWMOModel * pModel=(CD3DWOWWMOModel *)pBindObject;

		info.PropertyType=PROT_SPIN_NUMBER;
		info.Name=_T("附件集");
		info.ID=USOP_WOWWMO_DOODAD_SET;
		info.Value=pModel->GetCurDoodadSet();
		info.Define.Format(_T("[Define]\r\nMin=0\r\nMax=%d\r\n"),pModel->GetDoodadSetCount()-1);
		m_PropertyGrid.AddProperty(&info);	

		
	}
	if(pBindObject->IsKindOf(GET_CLASS_INFO(CD3DWOWWMOGroupModel)))
	{
		CD3DWOWWMOGroupModel * pModel=(CD3DWOWWMOGroupModel *)pBindObject;

		Temp.Format(_T("0x%X"),
			pModel->GetGroupInfo()->Flags);
		
		info.PropertyType=PROT_NONE;
		info.Name=_T("Flag");
		info.ID=USOP_WOWWMO_GROUP_FLAG;
		info.Value=(LPCTSTR)Temp;
		m_PropertyGrid.AddProperty(&info);	

		info.PropertyType=PROT_NONE;
		info.Name=_T("入口数量");
		info.ID=USOP_WOWWMO_GROUP_PORTAL_COUNT;
		info.Value=pModel->GetGroupInfo()->PortalList.GetCount();
		m_PropertyGrid.AddProperty(&info);	
	}

	if(pBindObject->IsKindOf(GET_CLASS_INFO(CD3DBillBoardParticleEmitter)))
	{
		CD3DBillBoardParticleEmitter * pModel=(CD3DBillBoardParticleEmitter *)pBindObject;

		Temp.Format(_T("0x%X"),
			pModel->GetModelResource()->GetParticleEmitterInfo()->Flags);

		info.PropertyType=PROT_EDIT;
		info.Name=_T("Flag");
		info.ID=USOP_WOW_PARTICLE_FLAG;
		info.Value=(LPCTSTR)Temp;
		m_PropertyGrid.AddProperty(&info);	

		CD3DParticleEmitterResource * pModelResource=pModel->GetModelResource();
		if(pModel)
		{
			info.PropertyType=PROT_CUSTOM;
			info.Name=_T("粒子参数");
			info.ID=USOP_WOW_PARTICLE_PARAM;
			info.Value=_T("查看");
			info.pConfigDialog=&m_DlgParticleEmitterParamViewer;
			m_DlgParticleEmitterParamViewer.Init(pModelResource);

			m_PropertyGrid.AddProperty(&info);	
		}		
	}		

	if(pBindObject->IsKindOf(GET_CLASS_INFO(CD3DSubMesh)))
	{
		CD3DSubMesh * pSubMesh=(CD3DSubMesh *)pBindObject;

		info.PropertyType=PROT_NUMBER;
		info.Name=_T("ID");
		info.ID=USOP_SUBMESH_ID;
		info.Value=pSubMesh->GetID();					
		m_PropertyGrid.AddProperty(&info);	

		info.PropertyType=PROT_EDIT;
		info.Name=_T("名称");
		info.ID=USOP_SUBMESH_NAME;
		info.Value=pSubMesh->GetName();					
		m_PropertyGrid.AddProperty(&info);	


		info.PropertyType=PROT_NONE;
		info.Name=_T("属性");
		info.ID=USOP_SUBMESH_PROPERTY;
		Temp.Format(_T("0x%X"),pSubMesh->GetProperty());
		info.Value=Temp;					
		m_PropertyGrid.AddProperty(&info);		

		
		info.PropertyType=PROT_BOOL;
		info.Name=_T("可见");
		info.ID=USOP_SUBMESH_VISIBLE;
		info.Value=pSubMesh->IsVisible();				
		m_PropertyGrid.AddProperty(&info);

		info.PropertyType=PROT_CUSTOM;
		info.Name=_T("Diffuse");
		info.ID=USOP_SUBMESH_DIFFUSE;
		info.Value=_T("<编辑>");
		info.pConfigDialog=&m_DlgDiffuseConfig;
		m_DlgDiffuseConfig.m_D3DColor=pSubMesh->GetMaterial().GetMaterial().Diffuse;
		m_PropertyGrid.AddProperty(&info);

		info.PropertyType=PROT_CUSTOM;
		info.Name=_T("Ambient");
		info.ID=USOP_SUBMESH_AMBIENT;
		info.Value=_T("<编辑>");
		info.pConfigDialog=&m_DlgAmbientConfig;
		m_DlgAmbientConfig.m_D3DColor=pSubMesh->GetMaterial().GetMaterial().Ambient;
		m_PropertyGrid.AddProperty(&info);

		info.PropertyType=PROT_CUSTOM;
		info.Name=_T("Specular;");
		info.ID=USOP_SUBMESH_SPECULAR;
		info.Value=_T("<编辑>");
		info.pConfigDialog=&m_DlgSpecularConfig;
		m_DlgSpecularConfig.m_D3DColor=pSubMesh->GetMaterial().GetMaterial().Specular;
		m_PropertyGrid.AddProperty(&info);

		info.PropertyType=PROT_CUSTOM;
		info.Name=_T("Emissive");
		info.ID=USOP_SUBMESH_EMISSIVE;
		info.Value=_T("<编辑>");
		info.pConfigDialog=&m_DlgEmissiveConfig;
		m_DlgEmissiveConfig.m_D3DColor=pSubMesh->GetMaterial().GetMaterial().Emissive;
		m_PropertyGrid.AddProperty(&info);

		info.PropertyType=PROT_NUMBER;
		info.Name=_T("Power");
		info.ID=USOP_SUBMESH_POWER;
		info.Value=pSubMesh->GetMaterial().GetMaterial().Power;					
		m_PropertyGrid.AddProperty(&info);	
	

		info.PropertyType=PROT_CUSTOM;
		info.Name=_T("三角面;");
		info.ID=USOP_SUBMESH_TRIANGLES;
		info.Value=_T("<编辑>");
		info.pConfigDialog=&m_DlgTrianglesViewer;		
		m_DlgTrianglesViewer.Init(pSubMesh);
		m_PropertyGrid.AddProperty(&info);


		info.PropertyType=PROT_CUSTOM;
		info.Name=_T("FX:");
		info.ID=USOP_SUBMESH_FX;
		info.Value=_T("<编辑>");
		info.pConfigDialog=&m_DlgFXSelect;
		GetDocument()->GetFXList(m_DlgFXSelect.m_FXPtrArray);
		m_DlgFXSelect.m_SelectedFX=pSubMesh->GetMaterial().GetFX();
		m_PropertyGrid.AddProperty(&info);

		
		info.PropertyType=PROT_NONE;
		info.Name=_T("纹理;");
		info.ID=USOP_SUBMESH_TEXTURE;
		info.Value=_T("<编辑>");				
		m_PropertyGrid.AddProperty(&info);
				
		
	}	

	if(pBindObject->IsKindOf(GET_CLASS_INFO(CD3DFX)))
	{		

		CD3DFX * pFX=(CD3DFX *)pBindObject;

		info.PropertyType=PROT_EDIT;
		info.Name=_T("名称");
		info.ID=USOP_NAME;
		info.Value=pFX->GetName();
		m_PropertyGrid.AddProperty(&info);

		info.PropertyType=PROT_CUSTOM;
		info.Name=_T("内容");
		info.ID=USOP_FX_CONTENT;
		info.Value=_T("<编辑>");
		info.pConfigDialog=&m_DlgFXEditor;
		m_DlgFXEditor.m_FxContent=pFX->GetFXContent();
		
		m_PropertyGrid.AddProperty(&info);
	}

	

	m_PropertyGrid.SetRedraw(true);
}

LRESULT CUSOPropertyView::OnPropertyEditStart( WPARAM wParam, LPARAM lParam)
{
	PROPERTY_INFO * pInfo=(PROPERTY_INFO *)lParam;
	((CMainFrame *)AfxGetMainWnd())->EnableAccel(false);
	return TRUE;
}

LRESULT CUSOPropertyView::OnPropertyEditEnd( WPARAM wParam, LPARAM lParam)
{
	PROPERTY_INFO * pInfo=(PROPERTY_INFO *)lParam;
	((CMainFrame *)AfxGetMainWnd())->EnableAccel(true);
	return TRUE;
}


LRESULT CUSOPropertyView::OnPropertyChanged( WPARAM wParam, LPARAM lParam)
{

	CAutoLock Lock(GetDocument()->GetD3DViewPort()->GetRender()->GetRenderLock());

	PROPERTY_CHANGE_INFO * pChangeInfo=(PROPERTY_CHANGE_INFO *)lParam;
	PROPERTY_INFO * pInfo=pChangeInfo->pInfo;

	if(m_pUSOObject==NULL)
		return FALSE;

	CNameObject * pBindObject=m_pUSOObject->GetBindObject();
	if(pBindObject==NULL)
		return FALSE;



	if(pBindObject->IsKindOf(GET_CLASS_INFO(CD3DObject)))
	{
		CD3DObject * pObject=(CD3DObject *)m_pUSOObject->GetBindObject();
		switch(pInfo->ID)
		{	
		case USOP_NAME:
			{
				pObject->SetName(pInfo->Value);
				m_pUSOObject->UpdateTreeItem();
			}
			break;
		case USOP_X:
			{
				CD3DMatrix Mat=pObject->GetLocalMatrix();
				CD3DVector3 Trans;
				Trans=Mat.GetTranslation();
				Trans.x=pInfo->Value;
				Mat.SetTranslation(Trans);
				pObject->SetLocalMatrix(Mat);
			}
			break;
		case USOP_Y:
			{
				CD3DMatrix Mat=pObject->GetLocalMatrix();
				CD3DVector3 Trans;
				Trans=Mat.GetTranslation();
				Trans.y=pInfo->Value;
				Mat.SetTranslation(Trans);
				pObject->SetLocalMatrix(Mat);
			}
			break;
		case USOP_Z:
			{
				CD3DMatrix Mat=pObject->GetLocalMatrix();
				CD3DVector3 Trans;
				Trans=Mat.GetTranslation();
				Trans.z=pInfo->Value;
				Mat.SetTranslation(Trans);
				pObject->SetLocalMatrix(Mat);
			}
			break;
		case USOP_PITCH:
			{
				CD3DMatrix Mat=pObject->GetLocalMatrix();
				CD3DQuaternion Rotation=Mat.GetRotationOnQuaternion();
				FLOAT Pitch,Yaw,Roll;
				Rotation.GetYawPitchRoll(Yaw,Pitch,Roll);
				Pitch=(FLOAT)pInfo->Value*PI/180;			
				Mat.SetRotation(CD3DMatrix::FromRotationYawPitchRoll(Yaw,Pitch,Roll));
				pObject->SetLocalMatrix(Mat);
			}
			break;
		case USOP_YAW:
			{
				CD3DMatrix Mat=pObject->GetLocalMatrix();
				CD3DQuaternion Rotation=Mat.GetRotationOnQuaternion();
				FLOAT Pitch,Yaw,Roll;
				Rotation.GetYawPitchRoll(Yaw,Pitch,Roll);
				Yaw=(FLOAT)pInfo->Value*PI/180;			
				Mat.SetRotation(CD3DMatrix::FromRotationYawPitchRoll(Yaw,Pitch,Roll));
				pObject->SetLocalMatrix(Mat);
			}
			break;
		case USOP_ROLL:
			{
				CD3DMatrix Mat=pObject->GetLocalMatrix();
				CD3DQuaternion Rotation=Mat.GetRotationOnQuaternion();
				FLOAT Pitch,Yaw,Roll;
				Rotation.GetYawPitchRoll(Yaw,Pitch,Roll);
				Roll=(FLOAT)pInfo->Value*PI/180;			
				Mat.SetRotation(CD3DMatrix::FromRotationYawPitchRoll(Yaw,Pitch,Roll));
				pObject->SetLocalMatrix(Mat);
			}
			break;
		case USOP_SCALE_X:
			{
				CD3DMatrix Mat=pObject->GetLocalMatrix();
				CD3DVector3 Scale=Mat.GetScale();				
				Scale.x=(FLOAT)pInfo->Value;
				Mat.SetScale(Scale);
				pObject->SetLocalMatrix(Mat);
			}
			break;
		case USOP_SCALE_Y:
			{
				CD3DMatrix Mat=pObject->GetLocalMatrix();
				CD3DVector3 Scale=Mat.GetScale();				
				Scale.y=(FLOAT)pInfo->Value;
				Mat.SetScale(Scale);
				pObject->SetLocalMatrix(Mat);
			}
			break;
		case USOP_SCALE_Z:
			{
				CD3DMatrix Mat=pObject->GetLocalMatrix();
				CD3DVector3 Scale=Mat.GetScale();				
				Scale.z=(FLOAT)pInfo->Value;
				Mat.SetScale(Scale);
				pObject->SetLocalMatrix(Mat);
			}
			break;
		case USOP_VISIBLE:
			{
				pObject->SetVisible(pInfo->Value);		
			}
			break;
		}
	}	
	
	if(pBindObject->IsKindOf(GET_CLASS_INFO(CD3DObjectResource)))
	{
		CD3DObjectResource * pResource=(CD3DObjectResource *)pBindObject;

		switch(pInfo->ID)
		{	
		case USOP_NAME:
			{
				pResource->SetName(pInfo->Value);
				GetDocument()->UpdateAllTreeItem();
			}
			break;
		}
	}
	
		
	if(pBindObject->IsKindOf(GET_CLASS_INFO(CD3DWOWM2Model)))
	{
		CD3DWOWM2Model * pModel=(CD3DWOWM2Model *)pBindObject;		

		if(pBindObject->IsKindOf(GET_CLASS_INFO(CD3DWOWM2CharacterModel)))
		{			
			CD3DWOWM2CharacterModel * pCharModel=(CD3DWOWM2CharacterModel *)pBindObject;			
		}
	}
	if(pBindObject->IsKindOf(GET_CLASS_INFO(CD3DWOWWMOModel)))
	{
		CD3DWOWWMOModel * pModel=(CD3DWOWWMOModel *)pBindObject;
		switch(pInfo->ID)
		{
		case USOP_WOWWMO_DOODAD_SET:
			pModel->ShowDoodads(pInfo->Value);		
			m_pUSOObject->ReBindChilds();
			break;		
		}		
	}
	
	if(pBindObject->IsKindOf(GET_CLASS_INFO(CD3DSubMesh)))
	{
		CD3DSubMesh * pSubMesh=(CD3DSubMesh *)pBindObject;	
		
		switch(pInfo->ID)
		{	
		case USOP_SUBMESH_ID:
			pSubMesh->SetID(pInfo->Value);
			break;
		case USOP_SUBMESH_NAME:
			pSubMesh->SetName(pInfo->Value);
			break;
		case USOP_SUBMESH_VISIBLE:
			pSubMesh->SetVisible(pInfo->Value);
			break;		
		case USOP_SUBMESH_DIFFUSE:
			pSubMesh->GetMaterial().GetMaterial().Diffuse=m_DlgDiffuseConfig.m_D3DColor;
			break;
		case USOP_SUBMESH_AMBIENT:
			pSubMesh->GetMaterial().GetMaterial().Ambient=m_DlgAmbientConfig.m_D3DColor;
			break;
		case USOP_SUBMESH_SPECULAR:
			pSubMesh->GetMaterial().GetMaterial().Specular=m_DlgSpecularConfig.m_D3DColor;
			break;
		case USOP_SUBMESH_EMISSIVE:
			pSubMesh->GetMaterial().GetMaterial().Emissive=m_DlgEmissiveConfig.m_D3DColor;
			break;
		case USOP_SUBMESH_POWER:
			pSubMesh->GetMaterial().GetMaterial().Power=pInfo->Value;
			break;
		case USOP_SUBMESH_FX:
			{
				if(pSubMesh->GetMaterial().GetFX()!=m_DlgFXSelect.m_SelectedFX)
				{
					pSubMesh->GetMaterial().SetFX(m_DlgFXSelect.m_SelectedFX);
					m_DlgFXSelect.m_SelectedFX->AddUseRef();
					m_pUSOObject->ReBindChilds();
				}
			}
			break;
		}
	}
	if(pBindObject->IsKindOf(GET_CLASS_INFO(CD3DFX)))
	{
		CD3DFX * pFX=(CD3DFX *)pBindObject;

		switch(pInfo->ID)
		{	
		case USOP_NAME:
			{
				pFX->SetName(pInfo->Value);
				GetDocument()->UpdateAllTreeItem();
			}
			break;
		case USOP_FX_CONTENT:
			{
				CD3DFX * pTempFX=new CD3DFX(pFX->GetManager());
				if(pTempFX->LoadFromMemory((LPCTSTR)m_DlgFXEditor.m_FxContent,m_DlgFXEditor.m_FxContent.GetLength()+1))
				{
					pFX->LoadFromMemory((LPCTSTR)m_DlgFXEditor.m_FxContent,m_DlgFXEditor.m_FxContent.GetLength()+1);
				}				
				SAFE_RELEASE(pTempFX);
			}
			break;
		}
	}
	GetDocument()->SetModifiedFlag(true);
	return TRUE;
}