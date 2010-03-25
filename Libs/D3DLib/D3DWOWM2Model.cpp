/****************************************************************************/
/*                                                                          */
/*      文件名:    D3DWOWM2Model.cpp                                        */
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
	

IMPLEMENT_FILE_CHANNEL_MANAGER(CD3DWOWM2Model)

IMPLEMENT_CLASS_INFO(CD3DWOWM2Model,CD3DBaseDynamicModel);
CD3DWOWM2Model::CD3DWOWM2Model(void)
{
	m_pModelResource=NULL;
	m_CurAnimationID=-1;
	m_CurSubAnimationID=-1;
	m_CurAnimationIndex=-1;
	m_CurAnimationLength=0;
	m_UseSoftSkinMesh=false;

	m_InterimStatus=IS_NONE;
	m_InterimTime=0;
	m_IsPlaying=false;
	m_IsLoop=false;
	m_PlayStartTime=-1;
	m_CurPlayTime=0;
	m_PlaySpeedRate=1.0f;

	m_pCurBoundingBox=NULL;
	//m_pActivedAttachments=NULL;	
}

CD3DWOWM2Model::~CD3DWOWM2Model(void)
{
	Destory();	
}


void CD3DWOWM2Model::Destory()
{
	for(UINT i=0;i<m_BoneMatrices.GetCount();i++)
	{
		if(m_BoneMatrices[i].pAttachObject)
		{	
			SAFE_RELEASE(m_BoneMatrices[i].pAttachObject);
		}
	}
	m_BoneMatrices.Clear();
	m_AdditionalBoneMatrix.Clear();
	m_InterimBoneMatStart.Clear();
	m_InterimBoneMatEnd.Clear();
	m_InterimStatus=IS_NONE;
	m_InterimTime=0;
	SAFE_RELEASE(m_pModelResource);
	m_CurAnimationID=-1;
	m_CurSubAnimationID=-1;
	m_CurAnimationIndex=-1;
	m_CurAnimationLength=0;
	m_UseSoftSkinMesh=false;
	m_InterimStatus=IS_NONE;
	m_InterimTime=0;
	m_IsPlaying=false;
	m_IsLoop=false;
	m_PlayStartTime=-1;
	m_CurPlayTime=0;
	m_PlaySpeedRate=1.0f;

	m_pCurBoundingBox=NULL;	
	CD3DBaseDynamicModel::Destory();
}

bool CD3DWOWM2Model::Reset()
{
	return true;
}

bool CD3DWOWM2Model::Restore()
{
	return true;
}

bool CD3DWOWM2Model::Play(int AnimationID,int SubAnimationID,FLOAT InterimTime,bool IsLoop)
{
	if(m_pModelResource)
	{
		m_CurAnimationIndex=m_pModelResource->GetAnimationIndex(AnimationID,SubAnimationID);
		CD3DWOWM2ModelResource::ANIMATION_SEQUENCE *pAniInfo=m_pModelResource->GetAnimationInfo(m_CurAnimationIndex);
		if(pAniInfo)
		{
			m_CurAnimationID=AnimationID;
			m_CurSubAnimationID=SubAnimationID;
			
			m_CurAnimationLength=pAniInfo->Length/1000.0f;
			m_pCurBoundingBox=&(pAniInfo->BoundingBox);
			m_InterimTime=InterimTime;			
			return Play(IsLoop);
		}
	}
	return false;
}

bool CD3DWOWM2Model::Play(bool IsLoop)
{
	if(m_CurAnimationIndex>=0)
	{
		m_IsLoop=IsLoop;
		m_IsPlaying=true;
		m_PlayStartTime=-1.0f;
		m_CurPlayTime=0;
		if(m_InterimTime>0.000001f)
		{
			m_InterimStatus=IS_INIT;
			m_InterimBoneMatStart.Resize(m_BoneMatrices.GetCount());
			for(UINT i=0;i<m_BoneMatrices.GetCount();i++)
			{
				m_InterimBoneMatStart[i]=m_BoneMatrices[i].Matrix;
			}
		}
		else
		{
			FetchAnimationFrames(0);
		}
	}
	return false;
}
bool CD3DWOWM2Model::Stop()
{
	m_IsPlaying=false;
	return true;
}
bool CD3DWOWM2Model::IsPlaying()
{
	return m_IsPlaying;
}

bool CD3DWOWM2Model::AddBoneTrans(int KeyBone,CD3DMatrix& Matrix)
{
	int Bone=m_pModelResource->GetKeyBone(KeyBone);
	if(Bone>=0)
	{
		ADDITIONAL_BONE_MATRIX Info;
		Info.BoneID=Bone;
		Info.Matrix=Matrix;
		m_AdditionalBoneMatrix.Add(Info);
		return true;
	}
	return false;
}

void CD3DWOWM2Model::ClearAdditionalBoneTrans()
{
	m_AdditionalBoneMatrix.Clear();
}

bool CD3DWOWM2Model::LoadFromFile(LPCTSTR szModelFileName)
{
	CEasyString ModelFileName=szModelFileName;
	CEasyString SkinFileName;
	int Pos=ModelFileName.ReverseFind('.');
	if(Pos>=0)
	{
		SkinFileName=ModelFileName.Left(Pos);
		SkinFileName=SkinFileName+"00.skin";
		return LoadFromFile(ModelFileName,SkinFileName);
	}		
	return false;
}
bool CD3DWOWM2Model::LoadFromFile(LPCTSTR ModelFileName,LPCTSTR SkinFileName)
{
	if(GetDevice()==NULL)
		return false;

	Destory();

	CEasyString ModelName=ModelFileName;
	ModelName+="_"+GetPathFileName(SkinFileName);

	CD3DWOWM2ModelResource* pResource=
		dynamic_cast<CD3DWOWM2ModelResource*>(GetDevice()->GetObjectResourceManager()->GetResource(ModelName));
	if(!pResource)
	{
		pResource=new CD3DWOWM2ModelResource(GetDevice()->GetObjectResourceManager());
		if(!pResource->LoadFromFile(ModelFileName,SkinFileName))
		{
			pResource->Release();
			return false;
		}
		if(!GetDevice()->GetObjectResourceManager()->AddResource(pResource,ModelName))
		{
			pResource->Release();
			return false;
		}
	}
	else
	{
		pResource->AddUseRef();
	}	
	bool Ret=LoadFromResource(pResource);
	SAFE_RELEASE(pResource);
	return Ret;
	
}

bool CD3DWOWM2Model::LoadFromResource(CD3DWOWM2ModelResource * pModelResource)
{
	if(pModelResource==NULL)
		return false;

	Destory();

	m_pModelResource=pModelResource;	
	m_pModelResource->AddUseRef();

	m_BoneMatrices.Resize(m_pModelResource->GetBoneCount());
	m_BoneMatList.Resize(m_pModelResource->GetBoneCount());
	for(UINT i=0;i<m_pModelResource->GetBoneCount();i++)
	{
		m_BoneMatrices[i].Matrix.SetIdentity();
		m_BoneMatList[i].SetIdentity();
	}

	if(m_pModelResource->GetSkinMeshBoneCount()>MAX_BONE_NUM)
	{
		m_UseSoftSkinMesh=true;
		PrepareSoftSkinMesh();
	}
	else
	{
		m_UseSoftSkinMesh=false;
	}

	for(UINT i=0;i<m_pModelResource->GetParticleEmitterCount();i++)
	{
		CD3DWOWM2ModelResource::PARTICLE_EMITTER_INFO * pParticleEmitterInfo=m_pModelResource->GetParticleEmitterInfo(i);
		if(pParticleEmitterInfo)
		{
			m_BoneMatrices[pParticleEmitterInfo->Bone].AttachmentType=CD3DWOWM2ModelResource::MAT_PARTICLE_EMITTER;
			m_BoneMatrices[pParticleEmitterInfo->Bone].AttachmentID=i;			
			m_BoneMatrices[pParticleEmitterInfo->Bone].AttachTransform=
				CD3DMatrix::FromTranslation(pParticleEmitterInfo->Position);			
			CD3DWOWM2BillBoardParticleEmitter * pParticleEmitter=new CD3DWOWM2BillBoardParticleEmitter();

			pParticleEmitter->SetDevice(GetDevice());
			pParticleEmitter->Init(m_pModelResource,i,0);
			pParticleEmitter->SetParent(this);
			pParticleEmitter->SetLocalMatrix(m_BoneMatrices[pParticleEmitterInfo->Bone].AttachTransform);

			m_BoneMatrices[pParticleEmitterInfo->Bone].pAttachObject=pParticleEmitter;
		}
	}

	for(UINT i=0;i<m_pModelResource->GetRibbonEmitterCount();i++)
	{
		CD3DWOWM2ModelResource::RIBBON_EMITTER_INFO * pRibbonEmitterInfo=m_pModelResource->GetRibbonEmitterInfo(i);
		if(pRibbonEmitterInfo)
		{
			m_BoneMatrices[pRibbonEmitterInfo->BoneID].AttachmentType=CD3DWOWM2ModelResource::MAT_RIBBON_EMITTER;
			m_BoneMatrices[pRibbonEmitterInfo->BoneID].AttachmentID=i;
			m_BoneMatrices[pRibbonEmitterInfo->BoneID].AttachTransform=
				CD3DMatrix::FromTranslation(pRibbonEmitterInfo->Position);
			CD3DWOWM2RibbonEmitter * pRibbonEmitter=new CD3DWOWM2RibbonEmitter();

			pRibbonEmitter->SetDevice(GetDevice());
			pRibbonEmitter->Init(m_pModelResource,i);
			pRibbonEmitter->SetParent(this);
			pRibbonEmitter->SetLocalMatrix(m_BoneMatrices[pRibbonEmitterInfo->BoneID].AttachTransform);

			m_BoneMatrices[pRibbonEmitterInfo->BoneID].pAttachObject=pRibbonEmitter;
		}
	}

	SetName(m_pModelResource->GetName());
	return true;
}


bool CD3DWOWM2Model::CloneFrom(CNameObject * pObject,UINT Param)
{
	if(!pObject->IsKindOf(GET_CLASS_INFO(CD3DWOWM2Model)))
		return false;

	Destory();

	if(!CD3DBaseDynamicModel::CloneFrom(pObject,Param))
		return false;

	CD3DWOWM2Model * pSrcObject=(CD3DWOWM2Model *)pObject;
	m_pModelResource=pSrcObject->m_pModelResource;
	if(m_pModelResource)
		m_pModelResource->AddUseRef();

	m_BoneMatrices=pSrcObject->m_BoneMatrices;	
	m_BoneMatList=pSrcObject->m_BoneMatList;
	m_InterimBoneMatStart=pSrcObject->m_InterimBoneMatStart;
	m_InterimBoneMatEnd=pSrcObject->m_InterimBoneMatEnd;
	m_InterimStatus=pSrcObject->m_InterimStatus;
	m_InterimTime=pSrcObject->m_InterimTime;
	m_UseSoftSkinMesh=pSrcObject->m_UseSoftSkinMesh;
	m_CurAnimationID=pSrcObject->m_CurAnimationID;
	m_CurSubAnimationID=pSrcObject->m_CurSubAnimationID;
	m_CurAnimationIndex=pSrcObject->m_CurAnimationIndex;
	m_CurAnimationLength=pSrcObject->m_CurAnimationLength;
	m_IsPlaying=pSrcObject->m_IsPlaying;
	m_IsLoop=pSrcObject->m_IsLoop;
	m_PlayStartTime=pSrcObject->m_PlayStartTime;
	m_CurPlayTime=pSrcObject->m_CurPlayTime;
	m_PlaySpeedRate=pSrcObject->m_PlaySpeedRate;
	m_pCurBoundingBox=pSrcObject->m_pCurBoundingBox;

	for(UINT i=0;i<m_BoneMatrices.GetCount();i++)
	{
		if(m_BoneMatrices[i].pAttachObject)
		{
			m_BoneMatrices[i].pAttachObject=(CD3DDummy *)GetChildByStorageIDRecursive(m_BoneMatrices[i].pAttachObject->GetStorageID());
		}
	}

	return true;
}

void CD3DWOWM2Model::PickResource(CNameObjectSet * pObjectSet,UINT Param)
{
	if(m_pModelResource)
	{
		m_pModelResource->PickResource(pObjectSet);
		pObjectSet->Add(m_pModelResource);
	}
	for(UINT i=0;i<GetChildCount();i++)
	{
		if((!m_ChildList[i]->IsKindOf(GET_CLASS_INFO(CD3DWOWM2BillBoardParticleEmitter)))&&
			(!m_ChildList[i]->IsKindOf(GET_CLASS_INFO(CD3DWOWM2RibbonEmitter))))
		{
			GetChildByIndex(i)->PickResource(pObjectSet,Param);
		}
	}
}

CD3DWOWM2ModelResource * CD3DWOWM2Model::GetModelResource()
{
	return m_pModelResource;
}

CD3DDummy * CD3DWOWM2Model::EnableAttachment(UINT ID)
{
	CD3DWOWM2ModelResource::MODEL_ATTACHMENT * pAttachment=m_pModelResource->GetAttachment(ID);
	if(pAttachment)
	{
		if(m_BoneMatrices[pAttachment->Bone].pAttachObject==NULL)
		{
			CD3DBoundingBox BoundingBox;
			BoundingBox.m_Min=-0.1f;
			BoundingBox.m_Max=0.1f;
			m_BoneMatrices[pAttachment->Bone].AttachmentID=pAttachment->ID;
			m_BoneMatrices[pAttachment->Bone].AttachTransform=CD3DMatrix::FromTranslation(pAttachment->Position);
			m_BoneMatrices[pAttachment->Bone].AttachmentType=CD3DWOWM2ModelResource::MAT_DUMMY;
			CD3DDummy * pDummy=new CD3DDummy;			
			pDummy->SetName(pAttachment->Name);
			pDummy->SetRender(GetRender());
			pDummy->SetBoundingBox(BoundingBox);
			pDummy->SetParent(this);
			pDummy->SetLocalMatrix(m_BoneMatrices[pAttachment->Bone].AttachTransform);
			m_BoneMatrices[pAttachment->Bone].pAttachObject=pDummy;
		}
		return (CD3DDummy *)m_BoneMatrices[pAttachment->Bone].pAttachObject;
	}
	return NULL;
}

bool CD3DWOWM2Model::ToSmartStruct(CSmartStruct& Packet,CUSOFile * pUSOFile,UINT Param)
{
	SAFE_RELEASE(m_pBoundingFrame);

	if(!CD3DObject::ToSmartStruct(Packet,pUSOFile,Param|OPP_WITHOUT_CHILD))
		return false;	

	if((Param&OPP_WITHOUT_CHILD)==0)
	{
		for(UINT i=0;i<m_ChildList.GetCount();i++)
		{
			if((!m_ChildList[i]->IsKindOf(GET_CLASS_INFO(CD3DWOWM2BillBoardParticleEmitter)))&&
				(!m_ChildList[i]->IsKindOf(GET_CLASS_INFO(CD3DWOWM2RibbonEmitter))))
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
	CHECK_SMART_STRUCT_ADD_AND_RETURN(Packet.AddMember(SST_D3DWMM_MODEL_RESOURCE,ResourceID));
	CHECK_SMART_STRUCT_ADD_AND_RETURN(Packet.AddMember(SST_D3DWMM_CUR_ANI_ID,m_CurAnimationID));
	CHECK_SMART_STRUCT_ADD_AND_RETURN(Packet.AddMember(SST_D3DWMM_CUR_SUB_ANI_ID,m_CurSubAnimationID));
	for(UINT i=0;i<m_BoneMatrices.GetCount();i++)
	{
		if(m_BoneMatrices[i].pAttachObject&&m_BoneMatrices[i].AttachmentType==CD3DWOWM2ModelResource::MAT_DUMMY)
		{
			UINT BufferSize;
			void * pBuffer=Packet.PrepareMember(BufferSize);
			CSmartStruct SubPacket(pBuffer,BufferSize,true);

			CHECK_SMART_STRUCT_ADD_AND_RETURN(SubPacket.AddMember(SST_AAI_ATTACHMENT_ID,m_BoneMatrices[i].AttachmentID));
			CHECK_SMART_STRUCT_ADD_AND_RETURN(SubPacket.AddMember(SST_AAI_DUMMY_STORAGE_ID,
				m_BoneMatrices[i].pAttachObject->GetStorageID()));

			if(!Packet.FinishMember(SST_D3DWMM_ATCIVE_ATTACHMENT,SubPacket.GetDataLen()))
				return false;
		}
	}

	
	return true;
}
bool CD3DWOWM2Model::FromSmartStruct(CSmartStruct& Packet,CUSOFile * pUSOFile,UINT Param)
{
	if(!CD3DObject::FromSmartStruct(Packet,pUSOFile,Param))
		return false;
	void * Pos=Packet.GetFirstMemberPosition();
	while(Pos)
	{
		WORD MemberID;
		CSmartValue Value=Packet.GetNextMember(Pos,MemberID);
		switch(MemberID)
		{	
		case SST_D3DWMM_MODEL_RESOURCE:
			{
				int ResourceID=Value;
				m_pModelResource=(CD3DWOWM2ModelResource *)pUSOFile->ResourceIndexToObject(ResourceID,GET_CLASS_INFO(CD3DWOWM2ModelResource));
				if(m_pModelResource)
				{
					m_pModelResource->AddUseRef();

					m_BoneMatrices.Resize(m_pModelResource->GetBoneCount());
					m_BoneMatList.Resize(m_pModelResource->GetBoneCount());

					if(m_pModelResource->GetSkinMeshBoneCount()>MAX_BONE_NUM)
					{
						m_UseSoftSkinMesh=true;
						PrepareSoftSkinMesh();
					}
					else
					{
						m_UseSoftSkinMesh=false;
					}
				}
			}
			break;
		case SST_D3DWMM_CUR_ANI_ID:
			m_CurAnimationID=Value;
			break;
		case SST_D3DWMM_CUR_SUB_ANI_ID:
			m_CurSubAnimationID=Value;
			break;
		case SST_D3DWMM_ATCIVE_ATTACHMENT:
			{
				CSmartStruct AttachmentInfo=Value;
				UINT AttachmentID=AttachmentInfo.GetMember(SST_AAI_ATTACHMENT_ID);
				UINT StorageID=AttachmentInfo.GetMember(SST_AAI_DUMMY_STORAGE_ID);
				CD3DWOWM2ModelResource::MODEL_ATTACHMENT * pAttachment=m_pModelResource->GetAttachment(AttachmentID);
				if(pAttachment)
				{
					CD3DDummy * pDummy=(CD3DDummy *)GetChildByStorageIDRecursive(StorageID);
					if(pDummy)
					{
						CD3DBoundingBox BoundingBox;
						BoundingBox.m_Min=-0.1f;
						BoundingBox.m_Max=0.1f;
						pDummy->SetBoundingBox(BoundingBox);
						m_BoneMatrices[pAttachment->Bone].AttachmentID=pAttachment->ID;
						m_BoneMatrices[pAttachment->Bone].AttachTransform=
							CD3DMatrix::FromTranslation(pAttachment->Position);
						m_BoneMatrices[pAttachment->Bone].pAttachObject=pDummy;				
					}			
				}
			}
			break;
		}
	}
	
	if(m_pModelResource)
	{
		for(UINT i=0;i<m_pModelResource->GetParticleEmitterCount();i++)
		{
			CD3DWOWM2ModelResource::PARTICLE_EMITTER_INFO * pParticleEmitterInfo=m_pModelResource->GetParticleEmitterInfo(i);
			if(pParticleEmitterInfo)
			{
				m_BoneMatrices[pParticleEmitterInfo->Bone].AttachmentType=CD3DWOWM2ModelResource::MAT_PARTICLE_EMITTER;
				m_BoneMatrices[pParticleEmitterInfo->Bone].AttachmentID=i;
				m_BoneMatrices[pParticleEmitterInfo->Bone].AttachTransform=
					//CD3DMatrix::FromScale(pParticleEmitterInfo->Scale)*
					CD3DMatrix::FromTranslation(pParticleEmitterInfo->Position);
				CD3DWOWM2BillBoardParticleEmitter * pParticleEmitter=new CD3DWOWM2BillBoardParticleEmitter();
				pParticleEmitter->SetDevice(GetDevice());
				pParticleEmitter->Init(m_pModelResource,i,4096);
				pParticleEmitter->SetParent(this);

				m_BoneMatrices[pParticleEmitterInfo->Bone].pAttachObject=pParticleEmitter;
			}
		}

		for(UINT i=0;i<m_pModelResource->GetRibbonEmitterCount();i++)
		{
			CD3DWOWM2ModelResource::RIBBON_EMITTER_INFO * pRibbonEmitterInfo=m_pModelResource->GetRibbonEmitterInfo(i);
			if(pRibbonEmitterInfo)
			{
				m_BoneMatrices[pRibbonEmitterInfo->BoneID].AttachmentType=CD3DWOWM2ModelResource::MAT_RIBBON_EMITTER;
				m_BoneMatrices[pRibbonEmitterInfo->BoneID].AttachmentID=i;
				m_BoneMatrices[pRibbonEmitterInfo->BoneID].AttachTransform=
					CD3DMatrix::FromTranslation(pRibbonEmitterInfo->Position);
				CD3DWOWM2RibbonEmitter * pRibbonEmitter=new CD3DWOWM2RibbonEmitter();
				pRibbonEmitter->SetDevice(GetDevice());
				pRibbonEmitter->Init(m_pModelResource,i);
				pRibbonEmitter->SetParent(this);

				m_BoneMatrices[pRibbonEmitterInfo->BoneID].pAttachObject=pRibbonEmitter;
			}
		}
	}

	Play(m_CurAnimationID,m_CurSubAnimationID,0,true);
	return true;
}

UINT CD3DWOWM2Model::GetSmartStructSize(UINT Param)
{
	UINT Size=CD3DObject::GetSmartStructSize(Param|OPP_WITHOUT_CHILD);

	Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(int));
	Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(m_CurAnimationID));
	Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(m_CurSubAnimationID));
	
	for(UINT i=0;i<m_BoneMatrices.GetCount();i++)
	{
		if(m_BoneMatrices[i].pAttachObject&&m_BoneMatrices[i].AttachmentType==CD3DWOWM2ModelResource::MAT_DUMMY)
		{
			Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(m_BoneMatrices[i].AttachmentID));
			Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(m_BoneMatrices[i].pAttachObject->GetStorageID()));
			Size+=SMART_STRUCT_STRUCT_MEMBER_SIZE(0);
		}
	}

	if((Param&OPP_WITHOUT_CHILD)==0)
	{
		for(UINT i=0;i<m_ChildList.GetCount();i++)
		{
			if((!m_ChildList[i]->IsKindOf(GET_CLASS_INFO(CD3DWOWM2BillBoardParticleEmitter)))&&
				(!m_ChildList[i]->IsKindOf(GET_CLASS_INFO(CD3DWOWM2RibbonEmitter))))
			{
				Size+=SMART_STRUCT_STRUCT_MEMBER_SIZE(m_ChildList[i]->GetSmartStructSize(Param));
			}
		}
	}
	return Size;
}

void CD3DWOWM2Model::PrepareRender(CD3DDevice * pDevice,CD3DSubMesh * pSubMesh,CD3DSubMeshMaterial * pMaterial,CD3DLight ** pLight,CD3DCamera * pCamera)
{
	FUNCTION_BEGIN;
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
			pMaterial->GetFX()->SetMatrix("UVMatrix0",pMaterial->GetTextureUVTransform(0));

			//设置全局色
			pMaterial->GetFX()->SetColor("GlobalColor",pMaterial->GetGlobalColor());

			pMaterial->GetFX()->SetMatrix("WorldMatrix",GetWorldMatrix());
			//设置视投影矩阵
			pMaterial->GetFX()->SetMatrix("ViewMatrix",pCamera->GetViewMat());
			pMaterial->GetFX()->SetMatrix("ProjMatrix",pCamera->GetProjectMat());
			pMaterial->GetFX()->SetVector("CameraPos",pCamera->GetWorldMatrix().GetTranslation());

			if((!m_UseSoftSkinMesh)&&m_BoneMatList.GetCount())
			{
				UINT BoneNum=m_pModelResource->GetSkinMeshBoneCount();
				//设置骨骼矩阵
				pMaterial->GetFX()->SetMatrixArray("BoneMatrixArray",&(m_BoneMatList[0]),BoneNum);
			}
		}	
	}
	else
	{

	}
	FUNCTION_END;
}

int CD3DWOWM2Model::GetSubMeshCount()
{
	if(m_pModelResource)
		return m_pModelResource->GetSubMeshCount();
	else
		return NULL;
	
}

CD3DSubMesh * CD3DWOWM2Model::GetSubMesh(int index)
{
	if(m_pModelResource)
		return m_pModelResource->GetSubMesh(index);
	else
	return NULL;
}

CD3DBoundingBox * CD3DWOWM2Model::GetBoundingBox()
{	
	if(m_pModelResource)
	{			
		if(m_pCurBoundingBox) 
			return m_pCurBoundingBox;
		else
			return m_pModelResource->GetBoundingBox();
	}
	return NULL;
}
CD3DBoundingSphere * CD3DWOWM2Model::GetBoundingSphere()
{
	if(m_pModelResource)
		return m_pModelResource->GetBoundingSphere();
	return NULL;
}



void CD3DWOWM2Model::Update(FLOAT Time)
{
	FUNCTION_BEGIN;

	
	//更新世界矩阵
	if(GetParent())
		m_WorldMatrix=m_LocalMatrix*GetParent()->GetWorldMatrix();
	else
		m_WorldMatrix=m_LocalMatrix;

	if(!IsCulled())
	{
		if(m_IsPlaying)	
		{
			if(m_PlayStartTime<0)
			{
				m_PlayStartTime=Time;
			}
			m_CurPlayTime=Time-m_PlayStartTime;
			if(!m_IsLoop)
			{
				if(m_CurPlayTime>m_CurAnimationLength)
				{
					m_CurPlayTime=m_CurAnimationLength;
					m_IsPlaying=false;
				}
			}		
		}	

		UINT CurTime=(UINT)(m_CurPlayTime*1000.0f*m_PlaySpeedRate);

		if(m_pModelResource->HasBoneAni())
		{

			if(m_InterimStatus!=IS_DOING)
			{
				FetchAnimationFrames(CurTime);
				for(UINT i=0;i<m_AdditionalBoneMatrix.GetCount();i++)
				{
					m_BoneMatrices[m_AdditionalBoneMatrix[i].BoneID].Matrix=m_AdditionalBoneMatrix[i].Matrix*m_BoneMatrices[m_AdditionalBoneMatrix[i].BoneID].Matrix;
				}
			}

			if(m_InterimStatus==IS_INIT)
			{		
				m_InterimStatus=IS_DOING;
				m_InterimBoneMatEnd.Resize(m_BoneMatrices.GetCount());
				for(UINT i=0;i<m_BoneMatrices.GetCount();i++)
				{
					m_InterimBoneMatEnd[i]=m_BoneMatrices[i].Matrix;
				}
			}


			if(m_InterimStatus==IS_DOING)
			{
				for(UINT i=0;i<m_BoneMatList.GetCount();i++)
				{
					FLOAT S=m_CurPlayTime/m_InterimTime;
					if(S>1.0f)
						S=1.0f;
					CD3DVector3 ScalingStart=m_InterimBoneMatStart[i].GetScale();
					CD3DVector3 ScalingEnd=m_InterimBoneMatEnd[i].GetScale();
					ScalingEnd=CD3DVector3::Lerp(ScalingStart,ScalingEnd,S);

					CD3DQuaternion RotationStart=m_InterimBoneMatStart[i].GetRotationOnQuaternion();
					CD3DQuaternion RotationEnd=m_InterimBoneMatEnd[i].GetRotationOnQuaternion();
					RotationEnd=CD3DQuaternion::SLerp(RotationStart,RotationEnd,S);

					CD3DVector3 TranslationStart=m_InterimBoneMatStart[i].GetTranslation();
					CD3DVector3 TranslationEnd=m_InterimBoneMatEnd[i].GetTranslation();
					TranslationEnd=CD3DVector3::Lerp(TranslationStart,TranslationEnd,S);

					m_BoneMatList[i]=CD3DMatrix::FromScale(ScalingEnd)*CD3DMatrix::FromRotationQuaternion(RotationEnd)*
						CD3DMatrix::FromTranslation(TranslationEnd);
				}
				if(m_CurPlayTime>m_InterimTime)
				{
					m_InterimStatus=IS_NONE;
					m_PlayStartTime=Time;
					m_CurPlayTime=0.0f;
				}
			}
			else
			{
				for(UINT i=0;i<m_BoneMatList.GetCount();i++)
				{			
					m_BoneMatList[i]=m_BoneMatrices[i].Matrix;			
				}
			}

			for(UINT i=0;i<m_BoneMatList.GetCount();i++)
			{
				if(m_BoneMatrices[i].ParentID>=0)		
				{
					m_BoneMatList[i]=m_BoneMatList[i]*m_BoneMatList[m_BoneMatrices[i].ParentID];
				}
			}

			for(UINT i=0;i<m_BoneMatList.GetCount();i++)
			{
				if(m_BoneMatrices[i].pAttachObject)
				{	
					CD3DMatrix Mat=m_BoneMatrices[i].AttachTransform*m_BoneMatList[i];
					m_BoneMatrices[i].pAttachObject->SetLocalMatrix(Mat);				
				}

				if((m_BoneMatrices[i].Flags&CD3DWOWM2ModelResource::BONE_FLAG_BILLBOARD))
				{
					m_BoneMatList[i]=m_BoneMatList[i]*GetWorldMatrix();


					CD3DMatrix BillBoardMat=CD3DMatrix::FromTranslation(m_BoneMatrices[i].PivotPoint*(-1.0f))*
						GetRender()->GetCamera()->GetWorldMatrixDirect().GetRotation()*
						m_BoneMatList[i].GetRotation().GetInverse()*
						CD3DMatrix::FromTranslation(m_BoneMatrices[i].PivotPoint);		


					m_BoneMatList[i]=BillBoardMat*m_BoneMatList[i];
				}
				else
				{
					m_BoneMatList[i]=m_BoneMatList[i]*GetWorldMatrix();
				}

			}

			if(m_UseSoftSkinMesh&&m_BoneMatList.GetCount())
			{
				CaculateSoftSkinMesh();
			}

		}
		else
		{

			for(UINT i=0;i<m_BoneMatList.GetCount();i++)
			{			
				m_BoneMatList[i]=GetWorldMatrix();
				if((m_BoneMatrices[i].Flags&CD3DWOWM2ModelResource::BONE_FLAG_BILLBOARD))
				{
					CD3DMatrix BillBoardMat=CD3DMatrix::FromTranslation(m_BoneMatrices[i].PivotPoint*(-1.0f))*
						GetRender()->GetCamera()->GetWorldMatrixDirect().GetRotation()*
						m_BoneMatList[i].GetRotation().GetInverse()*
						CD3DMatrix::FromTranslation(m_BoneMatrices[i].PivotPoint);		
					m_BoneMatList[i]=BillBoardMat*m_BoneMatList[i];
				}			
			}

		}


		if(m_pModelResource->HasMaterialAni())
		{

			//设置颜色动画和纹理动画
			for(int i=0;i<GetSubMeshCount();i++)		
			{			
				CD3DSubMeshMaterial * pMaterial=GetSubMeshMaterial(i);
				if(pMaterial)
				{
					if(pMaterial->GetTextureLayerCount()>0)
					{
						D3DCOLORVALUE GlobalColor={1.0f,1.0f,1.0f,1.0f};

						UINT64 TextureProperty=pMaterial->GetTextureProperty(0);
						signed char ColorAniIndex=(signed char)((TextureProperty&CD3DWOWM2ModelResource::SMP_COLOR_ANI_INDEX)>>24);
						signed char TransparencyAniIndex=(signed char)((TextureProperty&CD3DWOWM2ModelResource::SMP_TRANSPARENCY_ANI_INDEX)>>32);

						if(ColorAniIndex>=0)
						{
							m_pModelResource->MakeColorAnimationFrame(ColorAniIndex,CurTime,GlobalColor);
						}
						if(TransparencyAniIndex>=0)
						{
							FLOAT Alpha;
							if(m_pModelResource->MakeTransparencyAnimationFrame(TransparencyAniIndex,CurTime,Alpha))
							{
								GlobalColor.a*=Alpha;
							}
						}
						pMaterial->SetGlobalColor(GlobalColor);

						for(UINT j=0;j<pMaterial->GetTextureLayerCount();j++)
						{
							UINT64 TextureProperty=pMaterial->GetTextureProperty(j);
							signed char UVAniIndex=(signed char)((TextureProperty&CD3DWOWM2ModelResource::SMP_UV_ANI_INDEX)>>40);
							if(UVAniIndex>=0)
							{
								CD3DMatrix UVTransform;
								if(m_pModelResource->MakeTextureUVAniFrame(UVAniIndex,CurTime,UVTransform))
								{
									pMaterial->SetTextureUVTransform(j,UVTransform);
								}
							}
						}
					}
				}			
			}

		}
	}
	
	
	//更新子对象
	for(UINT i=0;i<GetChildCount();i++)
		GetChildByIndex(i)->Update(Time);

	

	FUNCTION_END;
}

void CD3DWOWM2Model::PrepareSoftSkinMesh()
{
	if(m_pModelResource)
	{
		for(int i=0;i<m_pModelResource->GetSubMeshCount();i++)
		{
			CD3DSubMesh * pSubMesh=m_pModelResource->GetSubMesh(i);
			pSubMesh->AllocVertexs();
			BYTE * pModelVertices;
			pSubMesh->GetDXVertexBuffer()->Lock(0,0,(LPVOID *)&pModelVertices,0);
			memcpy(pSubMesh->GetVertexs(),pModelVertices,pSubMesh->GetVertexFormat().VertexSize*pSubMesh->GetVertexCount());
			pSubMesh->GetDXVertexBuffer()->Unlock();
			//if(m_pModelResource->GetSubMesh(i)->GetMaterial().pFX)
			//{
			//	m_pModelResource->GetSubMesh(i)->GetMaterial().pFX->SetActiveTechnique("TecNormal");
			//}
		}
	}
}

void CD3DWOWM2Model::CaculateSoftSkinMesh()
{
	if(m_pModelResource)
	{
		for(int s=0;s<m_pModelResource->GetSubMeshCount();s++)
		{
			CD3DSubMesh * pSubMesh=m_pModelResource->GetSubMesh(s);

			if(pSubMesh->IsVisible()&&(!pSubMesh->IsCulled()))			
			{
				CD3DWOWM2ModelResource::MODEL_VERTEXT * pDestVertices;
				pSubMesh->GetDXVertexBuffer()->Lock(0,0,(LPVOID *)&pDestVertices,0);

				CD3DWOWM2ModelResource::MODEL_VERTEXT * pSrcVertices=(CD3DWOWM2ModelResource::MODEL_VERTEXT *)pSubMesh->GetVertexs();


				for(UINT i=0;i<pSubMesh->GetVertexCount();i++)
				{
					pDestVertices[i].Pos=0.0f;
					pDestVertices[i].Normal=0.0f;
					for(int b=0;b<MAX_VERTEX_BONE_BIND;b++)
					{
						if(pSrcVertices[i].BoneWeight[b]>0.000001f)
						{
							pDestVertices[i].Pos+=pSrcVertices[i].Pos*
								m_BoneMatList[pSrcVertices[i].BoneID[b]]*pSrcVertices[i].BoneWeight[b];
							pDestVertices[i].Normal+=pSrcVertices[i].Normal*
								m_BoneMatList[pSrcVertices[i].BoneID[b]]*pSrcVertices[i].BoneWeight[b];
						}							
					}			
				}
				pSubMesh->GetDXVertexBuffer()->Unlock();
			}
		}		
	}
}


void CD3DWOWM2Model::FetchAnimationFrames(UINT Time)
{
	FUNCTION_BEGIN;
	if(m_pModelResource)
	{
		m_pModelResource->MakeAnimationBoneFrame(m_CurAnimationIndex,Time,
			m_BoneMatrices.GetBuffer(),m_BoneMatrices.GetCount());
	}
	FUNCTION_END;
}



}