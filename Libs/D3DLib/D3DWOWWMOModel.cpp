#include "StdAfx.h"


namespace D3DLib{

IMPLEMENT_FILE_CHANNEL_MANAGER(CD3DWOWWMOModel)

IMPLEMENT_CLASS_INFO(CD3DWOWWMOModel,CD3DBaseStaticModel);

CD3DWOWWMOModel::CD3DWOWWMOModel(void)
{
	m_pModelResource=NULL;
	m_CurDoodadSet=-1;
}

CD3DWOWWMOModel::~CD3DWOWWMOModel(void)
{
	Destory();
}

void CD3DWOWWMOModel::Destory()
{
	ClearDoodads();
	SAFE_RELEASE(m_pModelResource);
	CD3DBaseStaticModel::Destory();
}

bool CD3DWOWWMOModel::Reset()
{
	return true;
}

bool CD3DWOWWMOModel::Restore()
{
	return true;
}

bool CD3DWOWWMOModel::LoadFromFile(LPCTSTR ModelFileName)
{
	if(GetDevice()==NULL)
		return false;

	Destory();
	

	CD3DWOWWMOModelResource* pResource=
		dynamic_cast<CD3DWOWWMOModelResource*>(GetDevice()->GetObjectResourceManager()->GetResource(ModelFileName));
	if(!pResource)
	{
		pResource=new CD3DWOWWMOModelResource(GetDevice()->GetObjectResourceManager());
		if(!pResource->LoadFromFile(ModelFileName))
		{
			pResource->Release();
			return false;
		}
		if(!GetDevice()->GetObjectResourceManager()->AddResource(pResource,ModelFileName))
		{
			pResource->Release();
			return false;
		}
	}
	else
	{
		pResource->AddUseRef();
	}	

	m_pModelResource=pResource;	
	SetName(ModelFileName);
	
	ShowDoodads(0);

	return true;
}

bool CD3DWOWWMOModel::LoadFromResource(CD3DWOWWMOModelResource * pModelResource)
{
	if(GetDevice()==NULL)
		return false;

	Destory();

	m_pModelResource=pModelResource;
	m_pModelResource->AddUseRef();
	SetName(m_pModelResource->GetName());

	ShowDoodads(0);

	return true;
}

UINT CD3DWOWWMOModel::GetDoodadSetCount()
{
	if(m_pModelResource)
	{
		return m_pModelResource->GetDoodadSetCount();
	}
	return 0;
}

bool CD3DWOWWMOModel::ShowDoodads(UINT SetIndex)
{
	
	if(m_pModelResource)
	{
		ClearDoodads();
		CD3DWOWWMOModelResource::DOODAD_SET_INFO * pSetInfo=m_pModelResource->GetDoodadSetInfo(SetIndex);
		if(pSetInfo)
		{
			m_CurDoodadSet=SetIndex;
			for(UINT i=pSetInfo->StartDoodad;i<pSetInfo->StartDoodad+pSetInfo->DoodadCount;i++)
			{
				CD3DWOWWMOModelResource::DOODAD_INFO * pDoodadInfo=m_pModelResource->GetDoodadInfo(i);
				if(pDoodadInfo)
				{
					if(pDoodadInfo->pDoodadModel)
					{
						CD3DWOWM2Model * pDoodadModel=new CD3DWOWM2Model;
						pDoodadModel->SetDevice(GetDevice());

						if(pDoodadModel->LoadFromResource(pDoodadInfo->pDoodadModel))
						{
							pDoodadModel->SetParent(this);
							CD3DMatrix Mat=CD3DMatrix::FromScale(pDoodadInfo->Scaling,pDoodadInfo->Scaling,pDoodadInfo->Scaling)*
								CD3DMatrix::FromRotationQuaternion(pDoodadInfo->Rotation)*
								CD3DMatrix::FromTranslation(pDoodadInfo->Translation);
							if(GetRender())
								GetRender()->AddObjectRecursive(pDoodadModel);
							pDoodadModel->SetLocalMatrix(Mat);							
							pDoodadModel->Play(0,0,0,true);
							m_DoodadList.Add(pDoodadModel);
						}
						else
						{
							SAFE_RELEASE(pDoodadModel);
						}
					}
				}
			}
			return true;
		}
	}
	return false;
}

void CD3DWOWWMOModel::ClearDoodads()
{
	for(UINT i=0;i<m_DoodadList.GetCount();i++)
	{
		SAFE_RELEASE(m_DoodadList[i]);
	}
	m_DoodadList.Clear();
	m_CurDoodadSet=-1;
}

void CD3DWOWWMOModel::PickResource(CNameObjectSet * pObjectSet,UINT Param)
{
	CD3DBaseStaticModel::PickResource(pObjectSet,Param);
	if(m_pModelResource)
	{
		m_pModelResource->PickResource(pObjectSet);
		pObjectSet->Add(m_pModelResource);
	}
	
}


bool CD3DWOWWMOModel::ToSmartStruct(CSmartStruct& Packet,CUSOFile * pUSOFile,UINT Param)
{
	SAFE_RELEASE(m_pBoundingFrame);

	if(!CD3DBaseStaticModel::ToSmartStruct(Packet,pUSOFile,Param))
		return false;		

	int ResourceID=pUSOFile->ResourceObjectToIndex(m_pModelResource);
	CHECK_SMART_STRUCT_ADD_AND_RETURN(Packet.AddMember(SST_D3DWWM_MODEL_RESOURCE,ResourceID));
	CHECK_SMART_STRUCT_ADD_AND_RETURN(Packet.AddMember(SST_D3DWWM_CUR_DOODAD_SET,m_CurDoodadSet));
	
	return true;
}
bool CD3DWOWWMOModel::FromSmartStruct(CSmartStruct& Packet,CUSOFile * pUSOFile,UINT Param)
{
	Destory();

	if(!CD3DBaseStaticModel::FromSmartStruct(Packet,pUSOFile,Param))
		return false;
	void * Pos=Packet.GetFirstMemberPosition();
	while(Pos)
	{
		WORD MemberID;
		CSmartValue Value=Packet.GetNextMember(Pos,MemberID);
		switch(MemberID)
		{	
		case SST_D3DWWM_MODEL_RESOURCE:
			{
				int ResourceID=Value;
				m_pModelResource=(CD3DWOWWMOModelResource *)pUSOFile->ResourceIndexToObject(ResourceID,GET_CLASS_INFO(CD3DWOWWMOModelResource));
				if(m_pModelResource)
				{
					m_pModelResource->AddUseRef();
				}
			}
			break;
		case SST_D3DWWM_CUR_DOODAD_SET:
			m_CurDoodadSet=Value;
			break;		
		}
	}

	ShowDoodads(m_CurDoodadSet);
	return true;
}

UINT CD3DWOWWMOModel::GetSmartStructSize(UINT Param)
{
	UINT Size=CD3DBaseStaticModel::GetSmartStructSize(Param);

	Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(int));
	Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(m_CurDoodadSet));	

	return Size;
}

void CD3DWOWWMOModel::PrepareRender(CD3DDevice * pDevice,CD3DSubMesh * pSubMesh,CD3DSubMeshMaterial * pMaterial,CD3DLight ** pLight,CD3DCamera * pCamera)
{
	if(pSubMesh&&pMaterial)
	{	

		if(pMaterial->GetFX())
		{
			//设置灯光
			if(pLight[0])
			{			
				D3DLIGHT9 Light;
				pLight[0]->GetCurLight(Light);
				pMaterial->GetFX()->SetVector("LightDir",CD3DVector4(Light.Direction));
				pMaterial->GetFX()->SetColor("LightAmbient",Light.Ambient);
				pMaterial->GetFX()->SetColor("LightDiffuse",Light.Diffuse);
				pMaterial->GetFX()->SetColor("LightSpecular",Light.Specular);
			}
			//设置材质
			D3DMATERIAL9 * pD3DMaterial;
			if(pSubMesh->IsSelected())
				pD3DMaterial=&SELECTED_SUBMESH_MATERIAL;
			else
				pD3DMaterial=&(pMaterial->GetMaterial());
			pMaterial->GetFX()->SetColor("MaterialAmbient",pD3DMaterial->Ambient);
			pMaterial->GetFX()->SetColor("MaterialDiffuse",pD3DMaterial->Diffuse);
			pMaterial->GetFX()->SetColor("MaterialSpecular",pD3DMaterial->Specular);
			pMaterial->GetFX()->SetColor("MaterialEmissive",pD3DMaterial->Emissive);
			pMaterial->GetFX()->SetFloat("MaterialPower",pD3DMaterial->Power);

			//设置纹理
			pMaterial->GetFX()->SetTexture("TexLay0",pMaterial->GetTexture(0));
			pMaterial->GetFX()->SetTexture("TexLay1",pMaterial->GetTexture(1));

			CD3DMatrix Mat=GetWorldMatrix()*pCamera->GetViewMat()*pCamera->GetProjectMat();

			pMaterial->GetFX()->SetMatrix("WorldMatrix",GetWorldMatrix());
			pMaterial->GetFX()->SetMatrix("WorldViewPrjMatrix",Mat);
			pMaterial->GetFX()->SetVector("CameraPos",pCamera->GetWorldMatrix().GetTranslation());
		}	
	}
	else
	{

	}
}


int CD3DWOWWMOModel::GetSubMeshCount()
{
	if(m_pModelResource)
		return m_pModelResource->GetSubMeshCount();
	return 0;
}
CD3DSubMesh * CD3DWOWWMOModel::GetSubMesh(int index)
{
	if(m_pModelResource)
		return m_pModelResource->GetSubMesh(index);
	return NULL;
}
CD3DSubMeshMaterial * CD3DWOWWMOModel::GetSubMeshMaterial(int index)
{
	if(m_pModelResource)
		return &(m_pModelResource->GetSubMesh(index)->GetMaterial());
	return NULL;
}

CD3DBoundingBox * CD3DWOWWMOModel::GetBoundingBox()
{
	if(m_pModelResource)
		return m_pModelResource->GetBoundingBox();
	return NULL;
}
CD3DBoundingSphere * CD3DWOWWMOModel::GetBoundingSphere()
{
	if(m_pModelResource)
		return m_pModelResource->GetBoundingSphere();
	return NULL;
}



}