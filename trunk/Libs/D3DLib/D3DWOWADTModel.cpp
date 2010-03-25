/****************************************************************************/
/*                                                                          */
/*      文件名:    D3DWOWADTModel.cpp                                       */
/*      创建日期:  2010年02月09日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#include "StdAfx.h"
namespace D3DLib{


IMPLEMENT_FILE_CHANNEL_MANAGER(CD3DWOWADTModel)

IMPLEMENT_CLASS_INFO(CD3DWOWADTModel,CD3DBaseStaticModel);

CD3DWOWADTModel::CD3DWOWADTModel(void)
{
	m_pModelResource=NULL;
}

CD3DWOWADTModel::~CD3DWOWADTModel(void)
{
	Destory();
}

void CD3DWOWADTModel::Destory()
{	
	for(UINT i=0;i<m_M2ObjectList.GetCount();i++)
	{
		SAFE_RELEASE(m_M2ObjectList[i]);
	}
	m_M2ObjectList.Clear();
	for(UINT i=0;i<m_WMOObjectList.GetCount();i++)
	{
		SAFE_RELEASE(m_WMOObjectList[i]);
	}
	m_WMOObjectList.Clear();
	SAFE_RELEASE(m_pModelResource);
	CD3DBaseStaticModel::Destory();
}

bool CD3DWOWADTModel::Reset()
{
	return true;
}

bool CD3DWOWADTModel::Restore()
{
	return true;
}

bool CD3DWOWADTModel::LoadFromFile(LPCTSTR ModelFileName)
{
	if(GetDevice()==NULL)
		return false;

	Destory();


	CD3DWOWADTModelResource* pResource=
		dynamic_cast<CD3DWOWADTModelResource*>(GetDevice()->GetObjectResourceManager()->GetResource(ModelFileName));
	if(!pResource)
	{
		pResource=new CD3DWOWADTModelResource(GetDevice()->GetObjectResourceManager());
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

	if(!LoadObjects())
		return true;
	

	return true;
}

void CD3DWOWADTModel::PickResource(CNameObjectSet * pObjectSet,UINT Param)
{
	CD3DBaseStaticModel::PickResource(pObjectSet,Param);
	if(m_pModelResource)
	{
		m_pModelResource->PickResource(pObjectSet);
		pObjectSet->Add(m_pModelResource);
	}
}

bool CD3DWOWADTModel::ToSmartStruct(CSmartStruct& Packet,CUSOFile * pUSOFile,UINT Param)
{
	SAFE_RELEASE(m_pBoundingFrame);

	if(!CD3DBaseStaticModel::ToSmartStruct(Packet,pUSOFile,Param))
		return false;		

	int ResourceID=pUSOFile->ResourceObjectToIndex(m_pModelResource);
	CHECK_SMART_STRUCT_ADD_AND_RETURN(Packet.AddMember(SST_D3DWAM_MODEL_RESOURCE,ResourceID));	

	return true;
}
bool CD3DWOWADTModel::FromSmartStruct(CSmartStruct& Packet,CUSOFile * pUSOFile,UINT Param)
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
		case SST_D3DWAM_MODEL_RESOURCE:
			{
				int ResourceID=Value;
				m_pModelResource=(CD3DWOWADTModelResource *)pUSOFile->ResourceIndexToObject(ResourceID,GET_CLASS_INFO(CD3DWOWADTModelResource));
				if(m_pModelResource)
				{
					m_pModelResource->AddUseRef();
				}
			}
			break;	
		}
	}

	LoadObjects();
	return true;
}

UINT CD3DWOWADTModel::GetSmartStructSize(UINT Param)
{
	UINT Size=CD3DBaseStaticModel::GetSmartStructSize(Param);

	Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(int));

	return Size;
}

void CD3DWOWADTModel::PrepareRender(CD3DDevice * pDevice,CD3DSubMesh * pSubMesh,CD3DSubMeshMaterial * pMaterial,CD3DLight ** pLight,CD3DCamera * pCamera)
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
			for(UINT i=0;i<pMaterial->GetTextureLayerCount();i++)
			{
				char szTexName[64];
				sprintf_s(szTexName,64,"TexLay%d",i);
				pMaterial->GetFX()->SetTexture(szTexName,pMaterial->GetTexture(i));
			}

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

int CD3DWOWADTModel::GetSubMeshCount()
{
	if(m_pModelResource)
		return m_pModelResource->GetSubMeshCount();
	return 0;
}
CD3DSubMesh * CD3DWOWADTModel::GetSubMesh(int index)
{
	if(m_pModelResource)
		return m_pModelResource->GetSubMesh(index);
	return NULL;
}
CD3DSubMeshMaterial * CD3DWOWADTModel::GetSubMeshMaterial(int index)
{
	if(m_pModelResource)
		return &(m_pModelResource->GetSubMesh(index)->GetMaterial());
	return NULL;
}

CD3DBoundingBox * CD3DWOWADTModel::GetBoundingBox()
{
	if(m_pModelResource)
		return m_pModelResource->GetBoundingBox();
	return NULL;
}
CD3DBoundingSphere * CD3DWOWADTModel::GetBoundingSphere()
{
	if(m_pModelResource)
		return m_pModelResource->GetBoundingSphere();
	return NULL;
}


bool CD3DWOWADTModel::LoadObjects()
{
	for(UINT i=0;i<m_pModelResource->GetM2ObjectCount();i++)
	{
		CD3DWOWADTModelResource::M2_OBJECT_INFO * pInfo=m_pModelResource->GetM2ObjectInfo(i);
		if(pInfo)
		{
			CD3DWOWM2Model * pObject=new CD3DWOWM2Model();
			pObject->SetDevice(GetDevice());
			if(!pObject->LoadFromResource(pInfo->pModelResource))
			{
				PrintSystemLog(0,"地图对象装载失败");
				SAFE_RELEASE(pObject);
				return false;
			}			
			CD3DMatrix Mat=CD3DMatrix::FromScale(pInfo->Scale,pInfo->Scale,pInfo->Scale)*
				CD3DMatrix::FromRotationQuaternion(pInfo->Orientation)*
				CD3DMatrix::FromTranslation(pInfo->Position);
			pObject->SetLocalMatrix(Mat);
			pObject->SetParent(this);
			pObject->GetModelResource()->SetBoneAniCache(5,30);
			pObject->Play(0,0,0,true);
			CEasyString ObjectName;
			ObjectName.Format("%u",
				pInfo->ID);
			pObject->SetName(ObjectName);
			m_M2ObjectList.Add(pObject);
		}
	}

	for(UINT i=0;i<m_pModelResource->GetWMOObjectCount();i++)
	{
		CD3DWOWADTModelResource::WMO_OBJECT_INFO * pInfo=m_pModelResource->GetWMOObjectInfo(i);
		if(pInfo)
		{
			CD3DWOWWMOModel * pObject=new CD3DWOWWMOModel();
			pObject->SetDevice(GetDevice());
			if(!pObject->LoadFromResource(pInfo->pModelResource))
			{
				PrintSystemLog(0,"地图对象装载失败");
				SAFE_RELEASE(pObject);
				return false;
			}			
			CD3DMatrix Mat=CD3DMatrix::FromRotationQuaternion(pInfo->Orientation)*
				CD3DMatrix::FromTranslation(pInfo->Position);
			pObject->SetLocalMatrix(Mat);
			pObject->SetParent(this);			
			pObject->ShowDoodads(pInfo->DoodadSet);
			CEasyString ObjectName;
			ObjectName.Format("%u",
				pInfo->ID);
			pObject->SetName(ObjectName);
			
			m_WMOObjectList.Add(pObject);
		}
	}
	return true;
}

}