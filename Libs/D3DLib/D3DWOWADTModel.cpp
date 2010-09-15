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

bool CD3DWOWADTModel::LoadFromFile(LPCTSTR ModelFileName,bool IsBigAlphaMask)
{
	if(GetDevice()==NULL)
		return false;

	Destory();


	CD3DWOWADTModelResource* pResource=
		dynamic_cast<CD3DWOWADTModelResource*>(GetDevice()->GetObjectResourceManager()->GetResource(ModelFileName));
	if(!pResource)
	{
		pResource=new CD3DWOWADTModelResource(GetDevice()->GetObjectResourceManager());
		if(!pResource->LoadFromFile(ModelFileName,IsBigAlphaMask))
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

	if(!CD3DBaseStaticModel::ToSmartStruct(Packet,pUSOFile,Param|OPP_WITHOUT_CHILD))
		return false;	

	if((Param&OPP_WITHOUT_CHILD)==0)
	{
		for(UINT i=0;i<m_ChildList.GetCount();i++)
		{
			if((!m_ChildList[i]->IsKindOf(GET_CLASS_INFO(CD3DWOWWMOModel)))&&
				(!m_ChildList[i]->IsKindOf(GET_CLASS_INFO(CD3DWOWDoodadModel))))
			{
				UINT BufferSize;
				void * pBuffer=Packet.PrepareMember(BufferSize);
				CSmartStruct ChildPacket(pBuffer,BufferSize,true);
				if(!m_ChildList[i]->ToSmartStruct(ChildPacket,pUSOFile,Param))
					return false;
				if(!Packet.FinishMember(SST_TO_CHILD,ChildPacket.GetDataLen()))
					return false;
			}
		}
	}
	

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
	UINT Size=CD3DBaseStaticModel::GetSmartStructSize(Param|OPP_WITHOUT_CHILD);

	if((Param&OPP_WITHOUT_CHILD)==0)
	{
		for(UINT i=0;i<m_ChildList.GetCount();i++)
		{
			if((!m_ChildList[i]->IsKindOf(GET_CLASS_INFO(CD3DWOWWMOModel)))&&
				(!m_ChildList[i]->IsKindOf(GET_CLASS_INFO(CD3DWOWDoodadModel))))
			{
				Size+=SMART_STRUCT_STRUCT_MEMBER_SIZE(m_ChildList[i]->GetSmartStructSize(Param));
			}
		}
	}

	Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(int));

	return Size;
}

void CD3DWOWADTModel::PrepareRender(CD3DDevice * pDevice,CD3DSubMesh * pSubMesh,CD3DSubMeshMaterial * pMaterial,CEasyArray<CD3DLight *>& LightList,CD3DCamera * pCamera)
{
	if(pSubMesh&&pMaterial)
	{	

		if(pMaterial->GetFX())
		{
			//设置灯光
			if(LightList.GetCount())
			{		
				D3DLIGHT9	Light;
				char		szParamName[32];
				pMaterial->GetFX()->SetInt("LightCount",LightList.GetCount());
				for(UINT i=0;i<LightList.GetCount();i++)
				{
					LightList[i]->GetCurLight(Light);
					sprintf_s(szParamName,32,"LightType[%d]",i);
					pMaterial->GetFX()->SetInt(szParamName,Light.Type);
					sprintf_s(szParamName,32,"LightPos[%d]",i);
					pMaterial->GetFX()->SetVector(szParamName,CD3DVector3(Light.Position));
					sprintf_s(szParamName,32,"LightDir[%d]",i);
					pMaterial->GetFX()->SetVector(szParamName,CD3DVector3(Light.Direction));
					sprintf_s(szParamName,32,"LightAmbient[%d]",i);
					pMaterial->GetFX()->SetColor(szParamName,Light.Ambient);
					sprintf_s(szParamName,32,"LightDiffuse[%d]",i);
					pMaterial->GetFX()->SetColor(szParamName,Light.Diffuse);
					sprintf_s(szParamName,32,"LightSpecular[%d]",i);
					pMaterial->GetFX()->SetColor(szParamName,Light.Specular);
					sprintf_s(szParamName,32,"LightRange[%d]",i);
					pMaterial->GetFX()->SetFloat(szParamName,Light.Range);
					sprintf_s(szParamName,32,"LightAtn0[%d]",i);
					pMaterial->GetFX()->SetFloat(szParamName,Light.Attenuation0);
					sprintf_s(szParamName,32,"LightAtn1[%d]",i);
					pMaterial->GetFX()->SetFloat(szParamName,Light.Attenuation1);
					sprintf_s(szParamName,32,"LightAtn2[%d]",i);
					pMaterial->GetFX()->SetFloat(szParamName,Light.Attenuation2);
					//sprintf_s(szParamName,32,"LightFalloff[%d]",i);
					//pMaterial->GetFX()->SetFloat(szParamName,Light.Falloff);
					//sprintf_s(szParamName,32,"LightTheta[%d]",i);
					//pMaterial->GetFX()->SetFloat(szParamName,Light.Theta);
					//sprintf_s(szParamName,32,"LightPhi[%d]",i);
					//pMaterial->GetFX()->SetFloat(szParamName,Light.Phi);

				}
				
			}
			//设置雾
			pMaterial->GetFX()->SetColor("FogColor",GetRender()->GetFogColor());
			pMaterial->GetFX()->SetFloat("FogNear",GetRender()->GetFogNear());
			pMaterial->GetFX()->SetFloat("FogFar",GetRender()->GetFogFar());

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
			if(pSubMesh->GetProperty()&CD3DWOWADTModelResource::SMP_IS_WATER)
			{
				pMaterial->GetFX()->SetTexture("TexLay0",pMaterial->GetTexture(0));
				pMaterial->GetFX()->SetTexture("TexLay1",GetRender()->GetDepthTexture());
			}
			else
			{
				for(UINT i=0;i<pMaterial->GetTextureLayerCount();i++)
				{
					char szTexName[64];
					sprintf_s(szTexName,64,"TexLay%d",i);
					if(pMaterial->GetTextureProperty(i)&CD3DWOWADTModelResource::TP_SHADOW_MAP)
					{
						pMaterial->GetFX()->SetTexture("TexShadow",pMaterial->GetTexture(i));
					}
					else
					{
						pMaterial->GetFX()->SetTexture(szTexName,pMaterial->GetTexture(i));
					}					
				}
			}

			
			

			CD3DMatrix Mat=GetWorldMatrix();
			pMaterial->GetFX()->SetMatrix("WorldMatrix",Mat);

			
			Mat=GetWorldMatrix()*pCamera->GetViewMat();
			pMaterial->GetFX()->SetMatrix("WorldViewMatrix",Mat);
			pMaterial->GetFX()->SetMatrix("PrjMatrix",pCamera->GetProjectMat());

			pMaterial->GetFX()->SetVector("CameraPos",pCamera->GetWorldMatrix().GetTranslation());
			pMaterial->GetFX()->SetFloat("CameraNear",pCamera->GetNear());
			pMaterial->GetFX()->SetFloat("CameraFar",pCamera->GetFar());
		}	
	}
	else
	{

	}
}

void CD3DWOWADTModel::Update(FLOAT Time)
{
	CD3DBaseStaticModel::Update(Time);
	
	//if(GetRender())
	//{
	//	CD3DCamera * pCamera=GetRender()->GetCamera();

	//	for(UINT i=0;i<m_M2ObjectList.GetCount();i++)
	//	{
	//	
	//		CD3DBoundingBox BBox=(*m_M2ObjectList[i]->GetBoundingBox())*m_M2ObjectList[i]->GetWorldMatrix();
	//		FLOAT Size=CD3DVector3(BBox.m_Max-BBox.m_Min).Length();
	//		FLOAT Dis=CD3DVector3(m_M2ObjectList[i]->GetWorldMatrix().GetTranslation()-pCamera->GetWorldMatrix().GetTranslation()).Length();
	//		if(Size<50)
	//		{
	//			if(Dis>300)
	//			{
	//				m_M2ObjectList[i]->SetVisibleRecursive(false);
	//			}
	//			else
	//			{
	//				m_M2ObjectList[i]->SetVisibleRecursive(true);
	//			}
	//		}
	//		
	//	}
	//	

	//	
	//}
}

int CD3DWOWADTModel::GetSubMeshCount()
{
	if(m_pModelResource)
		return m_pModelResource->GetSubMeshCount();
	return 0;
}
CD3DSubMesh * CD3DWOWADTModel::GetSubMesh(UINT index)
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
	return &m_BoundingBox;
}
CD3DBoundingSphere * CD3DWOWADTModel::GetBoundingSphere()
{
	if(m_pModelResource)
		return m_pModelResource->GetBoundingSphere();
	return NULL;
}

bool CD3DWOWADTModel::GetHeightByXZ(FLOAT x,FLOAT z,FLOAT& Height,FLOAT& WaterHeight)
{
	if(m_pModelResource)
	{
		CD3DVector3 Pos=GetWorldMatrix().GetTranslation();
		return m_pModelResource->GetHeightByXZ(x-Pos.x,z-Pos.z,Height,WaterHeight);
	}
	return false;
}


bool CD3DWOWADTModel::LoadObjects()
{
	for(UINT i=0;i<m_pModelResource->GetM2ObjectCount();i++)
	{
		CD3DWOWADTModelResource::M2_OBJECT_INFO * pInfo=m_pModelResource->GetM2ObjectInfo(i);
		if(pInfo)
		{
			CD3DWOWDoodadModel * pObject=new CD3DWOWDoodadModel();
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
	BuildBoundingBox();
	return true;
}

void CD3DWOWADTModel::BuildBoundingBox()
{
	m_BoundingBox=*m_pModelResource->GetBoundingBox();
	for(UINT i=0;i<m_M2ObjectList.GetCount();i++)
	{
		if(m_M2ObjectList[i]->GetBoundingBox()->IsValid())
			m_BoundingBox.Merge((*m_M2ObjectList[i]->GetBoundingBox())*m_M2ObjectList[i]->GetLocalMatrix());
		if(!m_BoundingBox.IsValid())
			int err=1;
	}	
	for(UINT i=0;i<m_WMOObjectList.GetCount();i++)
	{
		if(m_WMOObjectList[i]->GetBoundingBox()->IsValid())
			m_BoundingBox.Merge((*m_WMOObjectList[i]->GetBoundingBox())*m_WMOObjectList[i]->GetLocalMatrix());
	}
}

}