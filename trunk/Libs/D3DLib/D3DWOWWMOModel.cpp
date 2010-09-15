/****************************************************************************/
/*                                                                          */
/*      文件名:    D3DWOWWMOModel.cpp                                       */
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
	for(UINT i=0;i<m_GroupList.GetCount();i++)
	{
		SAFE_RELEASE(m_GroupList[i]);
	}
	m_GroupList.Clear();
	SAFE_RELEASE(m_pModelResource);
	CD3DBaseStaticModel::Destory();
}



bool CD3DWOWWMOModel::LoadFromFile(LPCTSTR ModelFileName)
{
	if(GetDevice()==NULL)
		return false;

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

	LoadFromResource(pResource);
	//SetName(ModelFileName);
	
	SAFE_RELEASE(pResource);

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

	BuildGroups();
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
						CD3DWOWDoodadModel * pDoodadModel=new CD3DWOWDoodadModel;
						pDoodadModel->SetDevice(GetDevice());

						if(pDoodadModel->LoadFromResource(pDoodadInfo->pDoodadModel))
						{
							if(pDoodadInfo->GroupIndex<m_GroupList.GetCount())
							{
								pDoodadModel->SetParent(m_GroupList[pDoodadInfo->GroupIndex]);
							}
							else
								pDoodadModel->SetParent(this);
							CD3DMatrix Mat=CD3DMatrix::FromScale(pDoodadInfo->Scaling,pDoodadInfo->Scaling,pDoodadInfo->Scaling)*
								CD3DMatrix::FromRotationQuaternion(pDoodadInfo->Rotation)*
								CD3DMatrix::FromTranslation(pDoodadInfo->Translation);
							if(GetRender())
								GetRender()->AddObject(pDoodadModel);
							pDoodadModel->SetLocalMatrix(Mat);
							pDoodadModel->SetGroupIndex(pDoodadInfo->GroupIndex);
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

void CD3DWOWWMOModel::DoPortalCull(CD3DCamera * pCamera)
{	
	if(pCamera)
	{
		CD3DVector3 CameraPos1=pCamera->GetWorldMatrix().GetTranslation();
		CD3DVector3 CameraPos=CD3DVector3(0.0f,0.0f,0.0f)*
			pCamera->GetProjectMat().GetInverse()*pCamera->GetViewMat().GetInverse();

		CEasyArray<CD3DWOWWMOGroupModel *> Groups;
		CEasyArray<CD3DObject *> Objects(100,100);

		
		GetGroupByPos(CameraPos,Groups);

		//Doodad裁剪
		//先剔除所有Doodad,和group，但不包括摄像机所在的group
		//if(Groups.GetCount())
		{
			for(UINT i=0;i<m_DoodadList.GetCount();i++)
			{
				if(m_DoodadList[i]->GetBoundingBox())
				{
					bool IsIn=false;
					for(UINT g=0;g<Groups.GetCount();g++)
					{
						if(m_DoodadList[i]->GetGroupIndex()==Groups[g]->GetGroupIndex())
						{
							IsIn=true;
							break;
						}

					}
					if(!IsIn)
					{
						m_DoodadList[i]->AddFlagRecursive(CD3DObject::OBJECT_FLAG_PREPARE_RENDERED);
						Objects.Add(m_DoodadList[i]);
					}
				}
			}				
			for(UINT i=0;i<m_GroupList.GetCount();i++)
			{
				bool IsIn=false;
				for(UINT g=0;g<Groups.GetCount();g++)
				{
					if(m_GroupList[i]==Groups[g])
					{
						IsIn=true;
						break;
					}

				}
				if(!IsIn)
				{
					m_GroupList[i]->AddFlagRecursive(CD3DObject::OBJECT_FLAG_PREPARE_RENDERED);
					Objects.Add(m_GroupList[i]);
				}
			}
		}

		for(UINT g=0;g<Groups.GetCount();g++)
		{
			CD3DWOWWMOGroupModel * pGroup=Groups[g];
			CD3DWOWWMOModelResource::GROUP_INFO * pGroupInfo=NULL;		
			pGroupInfo=pGroup->GetGroupInfo();
			if(pGroupInfo)
			{
				//if(pGroupInfo->Flags&WMOGF_OUTDOOR)
				{
					CD3DMatrix WorldViewMat=pGroup->GetWorldMatrix()*pCamera->GetViewMat();
					CD3DMatrix WorldScaleRotationMat=pGroup->GetWorldMatrix().GetScaleRotation();

					//根据每一个Portal添加可见的Doodad
					for(UINT i=0;i<pGroupInfo->PortalList.GetCount();i++)
					{
						CD3DVector3 CameraDir=pGroupInfo->PortalList[i].Center*pGroup->GetWorldMatrix()-CameraPos1;
						CD3DVector3 PortalNormal=pGroupInfo->PortalList[i].Normal*WorldScaleRotationMat;
						if(CameraDir.Dot(PortalNormal)>0)
						{
							FLOAT MaxX=-3.4E+38f ,MaxY=-3.4E+38f ,MinX=3.4E+38f ,MinY=3.4E+38f,MinZ=3.4E+38f;
							UINT ZOutCount=0;
							for(UINT j=0;j<pGroupInfo->PortalList[i].Vertices.GetCount();j++)
							{
								CD3DVector3 Point=pGroupInfo->PortalList[i].Vertices[j]*WorldViewMat;								
								if(Point.z<pCamera->GetNear())
								{
									Point.z=pCamera->GetNear();
									ZOutCount++;
								}
								Point=Point*pCamera->GetProjectMat();
								if(Point.x>MaxX)
									MaxX=Point.x;
								if(Point.x<MinX)
									MinX=Point.x;
								if(Point.y>MaxY)
									MaxY=Point.y;
								if(Point.y<MinY)
									MinY=Point.y;
								if(Point.z<MinZ)
									MinZ=Point.z;
							}
							if(MaxX<-1.0f||MaxY<-1.0f||MinX>1.0f||MinY>1.0f||
								ZOutCount>=pGroupInfo->PortalList[i].Vertices.GetCount())//||MinZ>1.0f)
							{
								//Portal在视野外，不需进行裁剪
								continue;
							}
							if(MaxX>1.0f)
								MaxX=1.0f;
							if(MaxY>1.0f)
								MaxY=1.0f;
							if(MinX<-1.0f)
								MinX=-1.0f;
							if(MinY<-1.0f)
								MinY=-1.0f;
							if(MinZ<0.0f)
								MinZ=0.0f;

							CD3DFrustum Frustum=CD3DFrustum::FromCustomViewFace(pCamera->GetViewMat(),pCamera->GetProjectMat(),MaxX,MaxY,1.0f,MinX,MinY,MinZ);
							for(int j=(int)Objects.GetCount()-1;j>=0;j--)
							{
								CD3DBoundingBox BBox=(*Objects[j]->GetBoundingBox())*Objects[j]->GetWorldMatrix();
								int Relation=Frustum.BoxLocation(BBox);
								if(Relation!=CD3DFrustum::LR_OUT)
								{
									Objects[j]->RemoveFlagRecursive(CD3DObject::OBJECT_FLAG_PREPARE_RENDERED);
									Objects.Delete(j);
								}
							}							
						}
					}				
				}
			}
		}
	}
}

void CD3DWOWWMOModel::PickResource(CNameObjectSet * pObjectSet,UINT Param)
{
	//CD3DBaseStaticModel::PickResource(pObjectSet,Param);
	if(m_pModelResource)
	{
		m_pModelResource->PickResource(pObjectSet);
		pObjectSet->Add(m_pModelResource);
	}
	for(UINT i=0;i<GetChildCount();i++)
	{
		if(!GetChildByIndex(i)->IsKindOf(GET_CLASS_INFO(CD3DWOWWMOGroupModel)))
			GetChildByIndex(i)->PickResource(pObjectSet,Param);
	}
}


bool CD3DWOWWMOModel::ToSmartStruct(CSmartStruct& Packet,CUSOFile * pUSOFile,UINT Param)
{
	SAFE_RELEASE(m_pBoundingFrame);

	if(!CD3DBaseStaticModel::ToSmartStruct(Packet,pUSOFile,Param|OPP_WITHOUT_CHILD))
		return false;

	if((Param&OPP_WITHOUT_CHILD)==0)
	{
		for(UINT i=0;i<m_ChildList.GetCount();i++)
		{
			if((!m_ChildList[i]->IsKindOf(GET_CLASS_INFO(CD3DWOWWMOGroupModel)))&&
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

	BuildGroups();
	ShowDoodads(m_CurDoodadSet);
	return true;
}

UINT CD3DWOWWMOModel::GetSmartStructSize(UINT Param)
{
	UINT Size=CD3DBaseStaticModel::GetSmartStructSize(Param|OPP_WITHOUT_CHILD);

	if((Param&OPP_WITHOUT_CHILD)==0)
	{
		for(UINT i=0;i<m_ChildList.GetCount();i++)
		{
			if((!m_ChildList[i]->IsKindOf(GET_CLASS_INFO(CD3DWOWWMOGroupModel)))&&
				(!m_ChildList[i]->IsKindOf(GET_CLASS_INFO(CD3DWOWDoodadModel))))
			{
				Size+=SMART_STRUCT_STRUCT_MEMBER_SIZE(m_ChildList[i]->GetSmartStructSize(Param));
			}
		}
	}

	Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(int));
	Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(m_CurDoodadSet));	

	return Size;
}

//void CD3DWOWWMOModel::PrepareRender(CD3DDevice * pDevice,CD3DSubMesh * pSubMesh,CD3DSubMeshMaterial * pMaterial,CEasyArray<CD3DLight *>& LightList,CD3DCamera * pCamera)
//{
//	if(pSubMesh&&pMaterial)
//	{	
//
//		if(pMaterial->GetFX())
//		{
//			//设置灯光
//			if(LightList.GetCount())
//			{		
//				D3DLIGHT9	Light;
//				char		szParamName[32];
//				pMaterial->GetFX()->SetInt("LightCount",LightList.GetCount());
//				for(UINT i=0;i<LightList.GetCount();i++)
//				{
//					LightList[i]->GetCurLight(Light);
//					sprintf_s(szParamName,32,"LightType[%d]",i);
//					pMaterial->GetFX()->SetInt(szParamName,Light.Type);
//					sprintf_s(szParamName,32,"LightPos[%d]",i);
//					pMaterial->GetFX()->SetVector(szParamName,CD3DVector3(Light.Position));
//					sprintf_s(szParamName,32,"LightDir[%d]",i);
//					pMaterial->GetFX()->SetVector(szParamName,CD3DVector3(Light.Direction));
//					sprintf_s(szParamName,32,"LightAmbient[%d]",i);
//					pMaterial->GetFX()->SetColor(szParamName,Light.Ambient);
//					sprintf_s(szParamName,32,"LightDiffuse[%d]",i);
//					pMaterial->GetFX()->SetColor(szParamName,Light.Diffuse);
//					sprintf_s(szParamName,32,"LightSpecular[%d]",i);
//					pMaterial->GetFX()->SetColor(szParamName,Light.Specular);
//					sprintf_s(szParamName,32,"LightRange[%d]",i);
//					pMaterial->GetFX()->SetFloat(szParamName,Light.Range);
//					sprintf_s(szParamName,32,"LightAtn0[%d]",i);
//					pMaterial->GetFX()->SetFloat(szParamName,Light.Attenuation0);
//					sprintf_s(szParamName,32,"LightAtn1[%d]",i);
//					pMaterial->GetFX()->SetFloat(szParamName,Light.Attenuation1);
//					sprintf_s(szParamName,32,"LightAtn2[%d]",i);
//					pMaterial->GetFX()->SetFloat(szParamName,Light.Attenuation2);
//					sprintf_s(szParamName,32,"LightFalloff[%d]",i);
//					pMaterial->GetFX()->SetFloat(szParamName,Light.Falloff);
//					sprintf_s(szParamName,32,"LightTheta[%d]",i);
//					pMaterial->GetFX()->SetFloat(szParamName,Light.Theta);
//					sprintf_s(szParamName,32,"LightPhi[%d]",i);
//					pMaterial->GetFX()->SetFloat(szParamName,Light.Phi);
//
//				}
//
//			}
//			//设置材质
//			D3DMATERIAL9 * pD3DMaterial;
//			if(pSubMesh->IsSelected())
//				pD3DMaterial=&SELECTED_SUBMESH_MATERIAL;
//			else
//				pD3DMaterial=&(pMaterial->GetMaterial());
//			pMaterial->GetFX()->SetColor("MaterialAmbient",pD3DMaterial->Ambient);
//			pMaterial->GetFX()->SetColor("MaterialDiffuse",pD3DMaterial->Diffuse);
//			pMaterial->GetFX()->SetColor("MaterialSpecular",pD3DMaterial->Specular);
//			pMaterial->GetFX()->SetColor("MaterialEmissive",pD3DMaterial->Emissive);
//			pMaterial->GetFX()->SetFloat("MaterialPower",pD3DMaterial->Power);
//
//			//设置纹理
//			pMaterial->GetFX()->SetTexture("TexLay0",pMaterial->GetTexture(0));
//			pMaterial->GetFX()->SetTexture("TexLay1",pMaterial->GetTexture(1));
//
//			CD3DMatrix Mat=GetWorldMatrix()*pCamera->GetViewMat()*pCamera->GetProjectMat();
//
//			pMaterial->GetFX()->SetMatrix("WorldMatrix",GetWorldMatrix());
//			pMaterial->GetFX()->SetMatrix("WorldViewPrjMatrix",Mat);
//			pMaterial->GetFX()->SetVector("CameraPos",pCamera->GetWorldMatrix().GetTranslation());
//			pMaterial->GetFX()->SetFloat("CameraNear",pCamera->GetNear());
//			pMaterial->GetFX()->SetFloat("CameraFar",pCamera->GetFar());
//		}	
//	}
//	else
//	{
//
//	}
//}

void CD3DWOWWMOModel::Update(FLOAT Time)
{
	CD3DBaseStaticModel::Update(Time);
	//if(GetRender())
	//{
	//	CD3DCamera * pCamera=GetRender()->GetCamera();

	//	for(UINT i=0;i<m_DoodadList.GetCount();i++)
	//	{			
	//		CD3DBoundingBox BBox=(*m_DoodadList[i]->GetBoundingBox())*m_DoodadList[i]->GetWorldMatrix();
	//		FLOAT Size=CD3DVector3(BBox.m_Max-BBox.m_Min).Length();
	//		FLOAT Dis=CD3DVector3(m_DoodadList[i]->GetWorldMatrix().GetTranslation()-pCamera->GetWorldMatrix().GetTranslation()).Length();
	//		if(Size<50)
	//		{
	//			if(Dis>300)
	//			{
	//				m_DoodadList[i]->SetVisibleRecursive(false);
	//			}
	//			else
	//			{
	//				m_DoodadList[i]->SetVisibleRecursive(true);
	//			}
	//		}			
	//	}



	//}
}


int CD3DWOWWMOModel::GetSubMeshCount()
{
	//if(m_pModelResource)
	//	return m_pModelResource->GetSubMeshCount();
	return 0;
}
CD3DSubMesh * CD3DWOWWMOModel::GetSubMesh(UINT index)
{
	//if(m_pModelResource)
	//	return m_pModelResource->GetSubMesh(index);
	return NULL;
}
CD3DSubMeshMaterial * CD3DWOWWMOModel::GetSubMeshMaterial(int index)
{
	//if(m_pModelResource)
	//	return &(m_pModelResource->GetSubMesh(index)->GetMaterial());
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

void CD3DWOWWMOModel::BuildGroups()
{
	if(m_pModelResource)
	{
		for(UINT i=0;i<m_pModelResource->GetGroupCount();i++)
		{
			CD3DWOWWMOGroupModel * pGroup=new CD3DWOWWMOGroupModel();
			pGroup->SetDevice(GetDevice());
			if(pGroup->LoadFromResource(m_pModelResource,i))
			{
				pGroup->SetParent(this);
				m_GroupList.Add(pGroup);
			}
			else
			{
				SAFE_RELEASE(pGroup);
			}		
		}
	}
}

void CD3DWOWWMOModel::GetGroupByPos(const CD3DVector3& Pos,CEasyArray<CD3DWOWWMOGroupModel *>& Groups)
{
	CEasyArray<CD3DWOWWMOGroupModel *> ExteriorGroups;
	
	for(UINT i=0;i<m_GroupList.GetCount();i++)
	{
		CD3DWOWWMOModelResource::GROUP_INFO * pGroupInfo=m_GroupList[i]->GetGroupInfo();
		if(pGroupInfo->Flags&WMOGF_INDOOR)
		{
			CD3DBoundingBox BBox=pGroupInfo->BoundingBox*m_GroupList[i]->GetWorldMatrix();
			if(BBox.CheckRelation(Pos)==CD3DBoundingBox::RELATION_TYPE_INCLUDE)
			{
				Groups.Add(m_GroupList[i]);
			}
		}
		if(pGroupInfo->Flags&WMOGF_OUTDOOR)
		{
			ExteriorGroups.Add(m_GroupList[i]);
		}
	}
	
	if(Groups.GetCount()==0)
	{
		Groups.Add(ExteriorGroups);
	}
	
}

}