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
	CD3DBaseStaticModel::Destory();

	//for(UINT i=0;i<m_M2ObjectList.GetCount();i++)
	//{
	//	SAFE_RELEASE(m_M2ObjectList[i]);
	//}
	m_M2ObjectList.Clear();
	//for(UINT i=0;i<m_WMOObjectList.GetCount();i++)
	//{
	//	SAFE_RELEASE(m_WMOObjectList[i]);
	//}
	m_WMOObjectList.Clear();
	SAFE_RELEASE(m_pModelResource);
	
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


	SetName(ModelFileName);

	bool Ret=LoadFromResource(pResource);
	SAFE_RELEASE(pResource);
	return Ret;
}

bool CD3DWOWADTModel::LoadFromResource(CD3DWOWADTModelResource * pModelResource)
{
	if(GetDevice()==NULL)
		return false;

	Destory();

	m_pModelResource=pModelResource;	
	pModelResource->AddUseRef();
	

	if(!LoadObjects())
		return false;
	BuildBoundingBox();
	return true;
}

void CD3DWOWADTModel::PickResource(CUSOResourceManager * pResourceManager,UINT Param)
{
	CD3DBaseStaticModel::PickResource(pResourceManager,Param);
	if(m_pModelResource)
	{
		m_pModelResource->PickResource(pResourceManager);
		pResourceManager->AddResource(m_pModelResource);
	}
}

bool CD3DWOWADTModel::ToSmartStruct(CSmartStruct& Packet,CUSOResourceManager * pResourceManager,UINT Param)
{
	SAFE_RELEASE(m_pBoundingFrame);

	if(!CD3DBaseStaticModel::ToSmartStruct(Packet,pResourceManager,Param|OPP_WITHOUT_CHILD))
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
				if(!m_ChildList[i]->ToSmartStruct(ChildPacket,pResourceManager,Param))
					return false;
				if(!Packet.FinishMember(SST_TO_CHILD,ChildPacket.GetDataLen()))
					return false;
			}
		}
	}
	

	CHECK_SMART_STRUCT_ADD_AND_RETURN(Packet.AddMember(SST_D3DWAM_MODEL_RESOURCE,m_pModelResource->GetName()));	

	return true;
}
bool CD3DWOWADTModel::FromSmartStruct(CSmartStruct& Packet,CUSOResourceManager * pResourceManager,UINT Param)
{
	if(!CD3DBaseStaticModel::FromSmartStruct(Packet,pResourceManager,Param))
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
				LPCTSTR szResourceName=Value;
				CD3DWOWADTModelResource * pModelResource=(CD3DWOWADTModelResource *)pResourceManager->FindResource(szResourceName,GET_CLASS_INFO(CD3DWOWADTModelResource));
				if(pModelResource)
				{
					if(!LoadFromResource(pModelResource))
						return false;
				}
			}
			break;	
		}
	}

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

	Size+=SMART_STRUCT_STRING_MEMBER_SIZE(m_pModelResource->GetNameLength());

	return Size;
}

void CD3DWOWADTModel::OnPrepareRender(CD3DBaseRender * pRender,CD3DFX * pFX,CEasyArray<CD3DLight *>& LightList,CD3DCamera * pCamera)
{
	//设置灯光
	if(LightList.GetCount())
	{		
		D3DLIGHT9	Light;
		char		szParamName[32];
		pFX->SetInt("LightCount",LightList.GetCount());
		for(UINT i=0;i<LightList.GetCount();i++)
		{
			LightList[i]->GetCurLight(Light);
			sprintf_s(szParamName,32,"LightType[%d]",i);
			pFX->SetInt(szParamName,Light.Type);
			sprintf_s(szParamName,32,"LightPos[%d]",i);
			pFX->SetVector(szParamName,CD3DVector3(Light.Position));
			sprintf_s(szParamName,32,"LightDir[%d]",i);
			pFX->SetVector(szParamName,CD3DVector3(Light.Direction));
			sprintf_s(szParamName,32,"LightAmbient[%d]",i);
			pFX->SetColor(szParamName,Light.Ambient);
			sprintf_s(szParamName,32,"LightDiffuse[%d]",i);
			pFX->SetColor(szParamName,Light.Diffuse);
			sprintf_s(szParamName,32,"LightSpecular[%d]",i);
			pFX->SetColor(szParamName,Light.Specular);
			sprintf_s(szParamName,32,"LightRange[%d]",i);
			pFX->SetFloat(szParamName,Light.Range);
			sprintf_s(szParamName,32,"LightAtn0[%d]",i);
			pFX->SetFloat(szParamName,Light.Attenuation0);
			sprintf_s(szParamName,32,"LightAtn1[%d]",i);
			pFX->SetFloat(szParamName,Light.Attenuation1);
			sprintf_s(szParamName,32,"LightAtn2[%d]",i);
			pFX->SetFloat(szParamName,Light.Attenuation2);
			//sprintf_s(szParamName,32,"LightFalloff[%d]",i);
			//pFX->SetFloat(szParamName,Light.Falloff);
			//sprintf_s(szParamName,32,"LightTheta[%d]",i);
			//pFX->SetFloat(szParamName,Light.Theta);
			//sprintf_s(szParamName,32,"LightPhi[%d]",i);
			//pFX->SetFloat(szParamName,Light.Phi);

		}

	}
	////设置雾
	//CD3DSceneRender * pRender=(CD3DSceneRender *)GetRender();
	//if(pRender)
	//{
	//	pFX->SetColor("FogColor",pRender->GetFogColor());
	//	pFX->SetFloat("FogNear",pRender->GetFogNear());
	//	pFX->SetFloat("FogFar",pRender->GetFogFar());
	//}

	CD3DMatrix Mat=GetWorldMatrixR();
	pFX->SetMatrix("WorldMatrix",Mat);


	Mat=GetWorldMatrixR()*pCamera->GetViewMatR();
	pFX->SetMatrix("WorldViewMatrix",Mat);
	//pFX->SetMatrix("PrjMatrix",pCamera->GetProjectMatR());

	//pFX->SetVector("CameraPos",pCamera->GetWorldMatrixR().GetTranslation());
	//pFX->SetFloat("CameraNear",pCamera->GetNear());
	//pFX->SetFloat("CameraFar",pCamera->GetFar());
}
void CD3DWOWADTModel::OnPrepareRenderSubMesh(CD3DBaseRender * pRender,CD3DFX * pFX,CD3DSubMesh * pSubMesh,CD3DSubMeshMaterial * pMaterial,CEasyArray<CD3DLight *>& LightList,CD3DCamera * pCamera)
{
	////设置材质
	//D3DMATERIAL9 * pD3DMaterial;
	//if(pSubMesh->IsSelected())
	//	pD3DMaterial=&SELECTED_SUBMESH_MATERIAL;
	//else
	//	pD3DMaterial=&(pMaterial->GetMaterial());
	//pFX->SetColor("MaterialAmbient",pD3DMaterial->Ambient);
	//pFX->SetColor("MaterialDiffuse",pD3DMaterial->Diffuse);
	//pFX->SetColor("MaterialSpecular",pD3DMaterial->Specular);
	//pFX->SetColor("MaterialEmissive",pD3DMaterial->Emissive);
	//pFX->SetFloat("MaterialPower",pD3DMaterial->Power);

	//设置纹理

	if(pSubMesh->GetProperty()&CD3DWOWADTModelResource::SMP_IS_WATER)
	{
		CD3DSceneRender * pRender=(CD3DSceneRender *)GetRender();
		pFX->SetTexture("TexLay0",pMaterial->GetTexture(0));
		pFX->SetTexture("TexLay1",pRender->GetDepthTexture());
	}
	else
	{
		for(UINT i=0;i<pMaterial->GetTextureLayerCount();i++)
		{
			char szTexName[64];
			sprintf_s(szTexName,64,"TexLay%d",i);
			if(pMaterial->GetTextureProperty(i)&CD3DWOWADTModelResource::TP_SHADOW_MAP)
			{
				pFX->SetTexture("TexShadow",pMaterial->GetTexture(i));
			}
			else
			{
				pFX->SetTexture(szTexName,pMaterial->GetTexture(i));
			}					
		}
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
CD3DSubMeshMaterial * CD3DWOWADTModel::GetSubMeshMaterial(UINT index)
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

bool CD3DWOWADTModel::GetHeightByXZ(const CD3DVector3& Pos,FLOAT MinHeight,FLOAT MaxHeight,FLOAT& Height,FLOAT& WaterHeight,bool IncludeChild)
{
	if(GetBoundingBox())
	{		
		CD3DBoundingBox WMOBBox=(*GetBoundingBox())*GetWorldMatrix();
		if(WMOBBox.m_Min.x<=Pos.x&&Pos.x<=WMOBBox.m_Max.x&&
			WMOBBox.m_Min.z<=Pos.z&&Pos.z<=WMOBBox.m_Max.z)
		{
			FLOAT FinalHeight=WMOBBox.m_Min.y,FinalWaterHeight=WMOBBox.m_Min.y;
			bool HaveHeight=false;
			if(m_pModelResource)
			{
				FLOAT ADTHeight,ADTWaterHeight;
				CD3DVector3 LocalPos=Pos*GetWorldMatrix().GetInverse();
				if(m_pModelResource->GetHeightByXZ(LocalPos.x,LocalPos.z,ADTHeight,ADTWaterHeight))
				{
					//PrintSystemLog(0,"%g,%s",ADTHeight,m_pModelResource->GetName());
					HaveHeight=true;
					if(ADTHeight>FinalHeight)
						FinalHeight=ADTHeight;
					if(ADTWaterHeight>FinalWaterHeight)
						FinalWaterHeight=ADTWaterHeight;
				}
			}
			if(IncludeChild)
			{
				MinHeight=WMOBBox.m_Min.y;
				if(HaveHeight)
				{
					MinHeight=FinalHeight;
				}

				for(UINT i=0;i<m_WMOObjectList.GetCount();i++)
				{
					FLOAT WMOHeight,WMOWaterHeight;
					if(m_WMOObjectList[i]->GetHeightByXZ(Pos,WMOBBox.m_Min.y,MaxHeight,WMOHeight,WMOWaterHeight))
					{
						//PrintSystemLog(0,"%g,%s",WMOHeight,m_WMOObjectList[i]->GetModelResource()->GetName());
						HaveHeight=true;
						if(WMOHeight>FinalHeight)
							FinalHeight=WMOHeight;
						if(WMOWaterHeight>FinalWaterHeight)
							FinalWaterHeight=WMOWaterHeight;
					}
				}
			}
			if(HaveHeight)
			{
				Height=FinalHeight;
				WaterHeight=FinalWaterHeight;
			}

			return HaveHeight;
		}
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
	return true;
}

void CD3DWOWADTModel::BuildBoundingBox()
{
	m_BoundingBox=*m_pModelResource->GetBoundingBox();
	for(UINT i=0;i<m_M2ObjectList.GetCount();i++)
	{
		if(m_M2ObjectList[i]->GetBoundingBox()->IsValid())
			m_BoundingBox.Merge((*m_M2ObjectList[i]->GetBoundingBox())*m_M2ObjectList[i]->GetLocalMatrix());		
	}	
	for(UINT i=0;i<m_WMOObjectList.GetCount();i++)
	{
		if(m_WMOObjectList[i]->GetBoundingBox()->IsValid())
			m_BoundingBox.Merge((*m_WMOObjectList[i]->GetBoundingBox())*m_WMOObjectList[i]->GetLocalMatrix());
	}
}

}