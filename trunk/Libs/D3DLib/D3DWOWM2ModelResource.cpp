/****************************************************************************/
/*                                                                          */
/*      文件名:    D3DWOWM2ModelResource.cpp                                */
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



IMPLEMENT_CLASS_INFO(CD3DWOWM2ModelResource,CD3DObjectResource);




CD3DWOWM2ModelResource::CD3DWOWM2ModelResource(void)
{
	m_HasBoneAni=false;
	m_HasMaterialAni=false;
	m_SkinMeshBoneCount=0;
	m_BoneAniCacheSize=0;
	m_BoneAniCacheFreq=0;
}

CD3DWOWM2ModelResource::CD3DWOWM2ModelResource(CD3DObjectResourceManager* pManager)
	:CD3DObjectResource(pManager)
{
	m_HasBoneAni=false;
	m_HasMaterialAni=false;
	m_SkinMeshBoneCount=0;
	m_BoneAniCacheSize=0;
	m_BoneAniCacheFreq=0;
}

CD3DWOWM2ModelResource::~CD3DWOWM2ModelResource(void)
{
	Destory();
}

void CD3DWOWM2ModelResource::Destory()
{
	for(UINT i=0;i<m_SubMeshList.GetCount();i++)
	{
		SAFE_RELEASE(m_SubMeshList[i]);
	}
	for(UINT i=0;i<m_ParticleEmitterInfos.GetCount();i++)
	{
		SAFE_RELEASE(m_ParticleEmitterInfos[i].pTexture);
		SAFE_RELEASE(m_ParticleEmitterInfos[i].pFX);
	}
	for(UINT i=0;i<m_RibbonEmitterInfos.GetCount();i++)
	{
		for(UINT j=0;j<m_RibbonEmitterInfos[i].Textures.GetCount();j++)
		{
			SAFE_RELEASE(m_RibbonEmitterInfos[i].Textures[j]);
		}
	
		SAFE_RELEASE(m_RibbonEmitterInfos[i].pFX);
	}
	m_SubMeshList.Clear();	
	m_AnimationSequence.Clear();
	m_Bones.Clear();	
	m_SkinMeshBoneCount=0;
	m_KeyBoneIndex.Clear();
	m_GlobalSequences.Clear();
	m_Attachments.Clear();	
	m_ParticleEmitterInfos.Clear();
	m_RibbonEmitterInfos.Clear();
	m_BoneAniCache.Clear();
	m_GlobalBoneAniCache.Clear();
	m_SkinMeshBoneCount=0;
	m_BoneAniCacheSize=0;
	m_BoneAniCacheFreq=0;
	CD3DObjectResource::Destory();
}

bool CD3DWOWM2ModelResource::Reset()
{
	bool Ret=true;
	for(UINT i=0;i<m_SubMeshList.GetCount();i++)
	{		
		Ret=Ret&&m_SubMeshList[i]->Reset();
	}	
	return Ret&&CD3DObjectResource::Reset();
}
bool CD3DWOWM2ModelResource::Restore()
{
	bool Ret=true;
	for(UINT i=0;i<m_SubMeshList.GetCount();i++)
	{		
		Ret=Ret&&m_SubMeshList[i]->Restore();
	}	
	return Ret&&CD3DObjectResource::Restore();
}

bool CD3DWOWM2ModelResource::LoadFromFile(LPCTSTR szModelFileName)
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

bool CD3DWOWM2ModelResource::LoadFromFile(LPCTSTR ModelFileName,LPCTSTR SkinFileName)
{
	IFileAccessor * pFile;


	CEasyBuffer ModelData;
	CEasyBuffer SkinData;

	pFile=CD3DWOWM2Model::CreateFileAccessor();
	if(pFile==NULL)
		return false;
	if(!pFile->Open(ModelFileName,IFileAccessor::modeRead))
	{
		pFile->Release();
		return false;	
	}
	UINT FileSize=(UINT)pFile->GetSize();	
	ModelData.Create(FileSize);	
	pFile->Read(ModelData.GetBuffer(),FileSize);
	ModelData.SetUsedSize(FileSize);
	pFile->Close();
	

	if(!pFile->Open(SkinFileName,IFileAccessor::modeRead))
	{
		pFile->Release();
		return false;	
	}
	FileSize=(UINT)pFile->GetSize();	
	SkinData.Create(FileSize);	
	pFile->Read(SkinData.GetBuffer(),FileSize);
	SkinData.SetUsedSize(FileSize);
	pFile->Release();


	CEasyString ModelName=ModelFileName;
	ModelName+="_"+GetPathFileName(SkinFileName);

	CEasyString ModelPath=ModelFileName;
	int Pos=ModelPath.ReverseFind('.');
	if(Pos>=0)
	{
		ModelPath=ModelPath.Left(Pos);
	}



	BLZ_M2_HEADER * pHeader=(BLZ_M2_HEADER *)ModelData.GetBuffer();
	if(pHeader->Tag!=BLZ_M2_HEADER_TAG)
		return false;

	if(!LoadColorAnimation((BYTE *)ModelData.GetBuffer()))
		return false;

	if(!LoadTransparencyAnimation((BYTE *)ModelData.GetBuffer()))
		return false;

	if(!LoadTextureUVAnimation((BYTE *)ModelData.GetBuffer()))
		return false;

	if(!LoadBones((BYTE *)ModelData.GetBuffer(),ModelPath))
		return false;

	if(!LoadAttachments((BYTE *)ModelData.GetBuffer()))
		return false;
	

	M2_MODEL_VERTEXT * pVertices=(M2_MODEL_VERTEXT *)((BYTE *)ModelData.GetBuffer()+pHeader->VerticesOffset);

	

	SKIN_HEADER * pSkinHeader=(SKIN_HEADER *)SkinData.GetBuffer();
	if(pSkinHeader->Tag!=BLZ_SKIN_HEADER_TAG)
		return false;


	SKIN_SUB_MESH * pSubMesh=(SKIN_SUB_MESH *)((BYTE *)SkinData.GetBuffer()+pSkinHeader->SubmeshesOffset);

	

	SKIN_VERTEX_PROPERTIES * pVertexProperties=(SKIN_VERTEX_PROPERTIES *)((BYTE *)SkinData.GetBuffer()+pSkinHeader->PropertiesOffset);

	UINT SubMeshCount=pSkinHeader->SubmeshesCount;
	WORD * pIndices=(WORD *)((BYTE *)SkinData.GetBuffer()+pSkinHeader->IndicesOffset);
	WORD * pTriangles=(WORD *)((BYTE *)SkinData.GetBuffer()+pSkinHeader->TrianglesOffset);

	int TotalVertexCount=0;
	for(UINT i=0;i<SubMeshCount;i++)
	{		
		CD3DSubMesh * pD3DSubMesh=new CD3DSubMesh;


		pD3DSubMesh->GetVertexFormat().FVF=D3DFVF_XYZB5|D3DFVF_NORMAL|D3DFVF_TEX1|D3DFVF_LASTBETA_UBYTE4;
		pD3DSubMesh->GetVertexFormat().VertexSize=sizeof(MODEL_VERTEXT);
		pD3DSubMesh->GetVertexFormat().IndexSize=sizeof(WORD);
		pD3DSubMesh->SetPrimitiveType(D3DPT_TRIANGLELIST);
		pD3DSubMesh->SetPrimitiveCount(pSubMesh[i].TrianglesCount/3);		
		pD3DSubMesh->SetVertexCount(pSubMesh[i].VerticesCount);
		pD3DSubMesh->SetIndexCount(pSubMesh[i].TrianglesCount);

		
		pD3DSubMesh->AllocDXIndexBuffer(m_pManager->GetDevice());

		CEasyArray<WORD> VertexIndexList;

		WORD * pStaticModelIndices=NULL;
		pD3DSubMesh->GetDXIndexBuffer()->Lock(0,0,(LPVOID *)&pStaticModelIndices,0);
		for(UINT j=0;j<pSubMesh[i].TrianglesCount;j++)
		{
			UINT Triangle=((pSubMesh[i].StartTriangle+j)/3)*3+(2-(pSubMesh[i].StartTriangle+j)%3);
			//UINT Triangle=pSubMesh[i].StartTriangle+j;

			WORD Index=pTriangles[Triangle];
			if(Index>=pSkinHeader->IndicesCount)
			{
				PrintSystemLog(0,"Error Triangle");
			}
			WORD VertexIndex=pIndices[Index];
			if(VertexIndex>=pHeader->VerticesCount)
			{
				PrintSystemLog(0,"Error Indice");
			}			
			pStaticModelIndices[j]=RebuildVertexIndex(VertexIndexList,VertexIndex);
		}
		pD3DSubMesh->GetDXIndexBuffer()->Unlock();

		if(pD3DSubMesh->GetVertexCount()==VertexIndexList.GetCount())
		{
			pD3DSubMesh->AllocDXVertexBuffer(m_pManager->GetDevice());

			MODEL_VERTEXT * pModelVertices=NULL;
			pD3DSubMesh->GetDXVertexBuffer()->Lock(0,0,(LPVOID *)&pModelVertices,0);
			for(UINT j=0;j<VertexIndexList.GetCount();j++)
			{
				WORD VertexIndex=VertexIndexList[j];
				pModelVertices[j].Pos=BLZTranslationToD3D(pVertices[VertexIndex].Pos);
				for(int b=0;b<MAX_VERTEX_BONE_BIND;b++)
				{
					pModelVertices[j].BoneWeight[b]=pVertices[VertexIndex].BoneWeight[b]/255.0f;					
					pModelVertices[j].BoneID[b]=pVertices[VertexIndex].BoneIndex[b];				
					if(pModelVertices[j].BoneID[b]+1>(BYTE)m_SkinMeshBoneCount)
					{
						m_SkinMeshBoneCount=pModelVertices[j].BoneID[b]+1;
					}					
				}			
				pModelVertices[j].Normal=BLZTranslationToD3D(pVertices[VertexIndex].Normal);
				pModelVertices[j].TextureCoord=pVertices[VertexIndex].Tex;			
				TotalVertexCount++;
			}

			pD3DSubMesh->GetDXVertexBuffer()->Unlock();
		}
		else
		{
			PrintSystemLog(0,"Error VertexCount");
		}

		pD3DSubMesh->SetID(pSubMesh[i].ID);
		CEasyString SubMeshName;
		int Part=pSubMesh[i].ID/100;
		int Type=pSubMesh[i].ID%100;
		SubMeshName.Format("%02d-%02d",Part,Type);
		pD3DSubMesh->SetName(SubMeshName);


		
		
		m_SubMeshList.Add(pD3DSubMesh);
	}

	for(UINT i=0;i<m_SubMeshList.GetCount();i++)
	{
		MakeSubmeshMaterial((BYTE *)ModelData.GetBuffer(),(BYTE *)SkinData.GetBuffer(),i,m_SubMeshList[i]);		
	}

	//std::sort(m_SubMeshList.begin(),m_SubMeshList.end(),CD3DSubMesh::SortByName);	

	CheckAni();
	

	CreateBounding();

	
	if(!LoadRibbonEmitters((BYTE *)ModelData.GetBuffer()))
		return false;

	if(!LoadParticleEmitters((BYTE *)ModelData.GetBuffer()))
		return false;

	return true;

}



void CD3DWOWM2ModelResource::PickResource(CNameObjectSet * pObjectSet,UINT Param)
{
	CD3DObjectResource::PickResource(pObjectSet,Param);
	for(UINT i=0;i<m_ParticleEmitterInfos.GetCount();i++)
	{
		if(m_ParticleEmitterInfos[i].pTexture)
			pObjectSet->Add(m_ParticleEmitterInfos[i].pTexture);
		if(m_ParticleEmitterInfos[i].pFX)
			pObjectSet->Add(m_ParticleEmitterInfos[i].pFX);
		
	}
	for(UINT i=0;i<m_RibbonEmitterInfos.GetCount();i++)
	{
		for(UINT j=0;j<m_RibbonEmitterInfos[i].Textures.GetCount();j++)
		{
			if(m_RibbonEmitterInfos[i].Textures[j])
				pObjectSet->Add(m_RibbonEmitterInfos[i].Textures[j]);
		}
		if(m_RibbonEmitterInfos[i].pFX)
			pObjectSet->Add(m_RibbonEmitterInfos[i].pFX);
	}
}

bool CD3DWOWM2ModelResource::ToSmartStruct(CSmartStruct& Packet,CUSOFile * pUSOFile,UINT Param)
{
	//PrintBoneInfo();

	if(!CD3DObjectResource::ToSmartStruct(Packet,pUSOFile,Param))
		return false;	

	for(UINT i=0;i<m_AnimationSequence.GetCount();i++)
	{
		UINT BufferSize;
		void * pBuffer=Packet.PrepareMember(BufferSize);
		CSmartStruct SubPacket(pBuffer,BufferSize,true);
		CHECK_SMART_STRUCT_ADD_AND_RETURN(SubPacket.AddMember(SST_BAS_ANIMATION_ID,m_AnimationSequence[i].AnimationID));
		CHECK_SMART_STRUCT_ADD_AND_RETURN(SubPacket.AddMember(SST_BAS_ANIMATION_NAME,(LPCTSTR)m_AnimationSequence[i].AnimationName));
		CHECK_SMART_STRUCT_ADD_AND_RETURN(SubPacket.AddMember(SST_BAS_SUBANIMATION_ID,m_AnimationSequence[i].SubAnimationID));
		CHECK_SMART_STRUCT_ADD_AND_RETURN(SubPacket.AddMember(SST_BAS_INDEX,m_AnimationSequence[i].Index));
		CHECK_SMART_STRUCT_ADD_AND_RETURN(SubPacket.AddMember(SST_BAS_LENGTH,m_AnimationSequence[i].Length));
		CHECK_SMART_STRUCT_ADD_AND_RETURN(SubPacket.AddMember(SST_BAS_MOVING_SPEED,m_AnimationSequence[i].MovingSpeed));
		CHECK_SMART_STRUCT_ADD_AND_RETURN(SubPacket.AddMember(SST_BAS_FLAGS,m_AnimationSequence[i].Flags));
		CHECK_SMART_STRUCT_ADD_AND_RETURN(SubPacket.AddMember(SST_BAS_FLAGS2,m_AnimationSequence[i].Flags2));
		CHECK_SMART_STRUCT_ADD_AND_RETURN(SubPacket.AddMember(SST_BAS_PLAYBACK_SPEED,m_AnimationSequence[i].PlaybackSpeed));
		CHECK_SMART_STRUCT_ADD_AND_RETURN(SubPacket.AddMember(SST_BAS_BOUNDING_BOX,(char *)&(m_AnimationSequence[i].BoundingBox),
			sizeof(m_AnimationSequence[i].BoundingBox)));
		CHECK_SMART_STRUCT_ADD_AND_RETURN(SubPacket.AddMember(SST_BAS_BOUNDING_SPHERE,(char *)&(m_AnimationSequence[i].BoundingSphere),
			sizeof(m_AnimationSequence[i].BoundingSphere)));
		if(!Packet.FinishMember(SST_D3DWMMR_BONE_ANIMATION_SEQUENCE,SubPacket.GetDataLen()))
			return false;
	}
	for(UINT i=0;i<m_ColorAnimations.GetCount();i++)
	{
		UINT BufferSize;
		void * pBuffer=Packet.PrepareMember(BufferSize);
		CSmartStruct SubPacket(pBuffer,BufferSize,true);
		
		if(!PackAnimationBlock<CD3DVector3>(m_ColorAnimations[i].ColorAnimations,SubPacket,SST_CA_COLOR_ANIMATION))
			return false;
		
		if(!PackAnimationBlock<FLOAT>(m_ColorAnimations[i].AlphaAnimations,SubPacket,SST_CA_ALPHA_ANIMATION))
			return false;
			
		if(!Packet.FinishMember(SST_D3DWMMR_COLOR_ANIMATION,SubPacket.GetDataLen()))
			return false;
	}
	for(UINT i=0;i<m_TransparencyAnimations.GetCount();i++)
	{
		UINT BufferSize;
		void * pBuffer=Packet.PrepareMember(BufferSize);
		CSmartStruct SubPacket(pBuffer,BufferSize,true);		
			
		if(!PackAnimationBlock<FLOAT>(m_TransparencyAnimations[i].AlphaAnimations,SubPacket,SST_TA_ALPHA_ANIMATION))
			return false;
		
		if(!Packet.FinishMember(SST_D3DWMMR_TRANSPARENCY_ANIMATION,SubPacket.GetDataLen()))
			return false;
	}

	for(UINT i=0;i<m_TextureUVAnimations.GetCount();i++)
	{
		UINT BufferSize;
		void * pBuffer=Packet.PrepareMember(BufferSize);
		CSmartStruct SubPacket(pBuffer,BufferSize,true);		
		
		if(!PackAnimationBlock<CD3DVector3>(m_TextureUVAnimations[i].Translations,SubPacket,SST_TUVA_TRANSLATIONS))
			return false;
		
		if(!PackAnimationBlock<CD3DQuaternion>(m_TextureUVAnimations[i].Rotations,SubPacket,SST_TUVA_ROTATIONS))
			return false;
	
		if(!PackAnimationBlock<CD3DVector3>(m_TextureUVAnimations[i].Scalings,SubPacket,SST_TUVA_SCALINGS))
			return false;
			
		if(!Packet.FinishMember(SST_D3DWMMR_TEXTURE_UV_ANIMATION,SubPacket.GetDataLen()))
			return false;
	}

	for(UINT i=0;i<m_Bones.GetCount();i++)
	{
		UINT BufferSize;
		void * pBuffer=Packet.PrepareMember(BufferSize);
		CSmartStruct SubPacket(pBuffer,BufferSize,true);		

		CHECK_SMART_STRUCT_ADD_AND_RETURN(SubPacket.AddMember(SST_BI_ANIMATION_SEQ,m_Bones[i].AnimationSeq));
		CHECK_SMART_STRUCT_ADD_AND_RETURN(SubPacket.AddMember(SST_BI_FLAGS,m_Bones[i].Flags));
		CHECK_SMART_STRUCT_ADD_AND_RETURN(SubPacket.AddMember(SST_BI_PARENT_BONE,m_Bones[i].ParentBone));
		CHECK_SMART_STRUCT_ADD_AND_RETURN(SubPacket.AddMember(SST_BI_GEOSET_ID,m_Bones[i].GeosetID));
		CHECK_SMART_STRUCT_ADD_AND_RETURN(SubPacket.AddMember(SST_BI_PIVOT_POINT,(char *)(&m_Bones[i].PivotPoint),
			sizeof(m_Bones[i].PivotPoint)));
		
		if(!PackAnimationBlock<CD3DVector3>(m_Bones[i].Translations,SubPacket,SST_BI_TRANSLATIONS))
			return false;
		
		if(!PackAnimationBlock<CD3DQuaternion>(m_Bones[i].Rotations,SubPacket,SST_BI_ROTATIONS))
			return false;
		
		if(!PackAnimationBlock<CD3DVector3>(m_Bones[i].Scalings,SubPacket,SST_BI_SCALINGS))
			return false;
			
		if(!Packet.FinishMember(SST_D3DWMMR_BONE,SubPacket.GetDataLen()))
			return false;
	}
	CHECK_SMART_STRUCT_ADD_AND_RETURN(Packet.AddMember(SST_D3DWMMR_SKIN_MESH_BONE_COUNT,m_SkinMeshBoneCount));
	CHECK_SMART_STRUCT_ADD_AND_RETURN(Packet.AddMember(SST_D3DWMMR_KEY_BONE,
		(char *)m_KeyBoneIndex.GetBuffer(),m_KeyBoneIndex.GetCount()*sizeof(short)));
	CHECK_SMART_STRUCT_ADD_AND_RETURN(Packet.AddMember(SST_D3DWMMR_GLOBAL_SEQUENCE,
		(char *)m_GlobalSequences.GetBuffer(),m_GlobalSequences.GetCount()*sizeof(UINT)));
	for(UINT i=0;i<m_Attachments.GetCount();i++)
	{
		UINT BufferSize;
		void * pBuffer=Packet.PrepareMember(BufferSize);
		CSmartStruct SubPacket(pBuffer,BufferSize,true);	
		CHECK_SMART_STRUCT_ADD_AND_RETURN(SubPacket.AddMember(SST_AI_ID,m_Attachments[i].ID));
		CHECK_SMART_STRUCT_ADD_AND_RETURN(SubPacket.AddMember(SST_AI_BONE,m_Attachments[i].Bone));
		CHECK_SMART_STRUCT_ADD_AND_RETURN(SubPacket.AddMember(SST_AI_NAME,m_Attachments[i].Name));
		CHECK_SMART_STRUCT_ADD_AND_RETURN(SubPacket.AddMember(SST_AI_POSITION,
			(char *)(&m_Attachments[i].Position),sizeof(m_Attachments[i].Position)));
		if(!Packet.FinishMember(SST_D3DWMMR_ATTACHMENT,SubPacket.GetDataLen()))
			return false;
	}

	for(UINT i=0;i<m_ParticleEmitterInfos.GetCount();i++)
	{
		UINT BufferSize;
		void * pBuffer=Packet.PrepareMember(BufferSize);
		CSmartStruct SubPacket(pBuffer,BufferSize,true);	

		CHECK_SMART_STRUCT_ADD_AND_RETURN(SubPacket.AddMember(SST_PEI_FLAGS,m_ParticleEmitterInfos[i].Flags));
		CHECK_SMART_STRUCT_ADD_AND_RETURN(SubPacket.AddMember(SST_PEI_POSITION,
			(char *)(&m_ParticleEmitterInfos[i].Position),sizeof(m_ParticleEmitterInfos[i].Position)));
		CHECK_SMART_STRUCT_ADD_AND_RETURN(SubPacket.AddMember(SST_PEI_BONE,m_ParticleEmitterInfos[i].Bone));
		int ResourceID=pUSOFile->ResourceObjectToIndex(m_ParticleEmitterInfos[i].pTexture);
		CHECK_SMART_STRUCT_ADD_AND_RETURN(SubPacket.AddMember(SST_PEI_TEXTURE,ResourceID));
		CHECK_SMART_STRUCT_ADD_AND_RETURN(SubPacket.AddMember(SST_PEI_BLENDING_TYPE,m_ParticleEmitterInfos[i].BlendingType));
		CHECK_SMART_STRUCT_ADD_AND_RETURN(SubPacket.AddMember(SST_PEI_EMITTER_TYPE,m_ParticleEmitterInfos[i].EmitterType));
		CHECK_SMART_STRUCT_ADD_AND_RETURN(SubPacket.AddMember(SST_PEI_PARTICLE_COLOR_REF,m_ParticleEmitterInfos[i].ParticleColorRef));
		CHECK_SMART_STRUCT_ADD_AND_RETURN(SubPacket.AddMember(SST_PEI_PARTICLE_TYPE,m_ParticleEmitterInfos[i].ParticleType));
		CHECK_SMART_STRUCT_ADD_AND_RETURN(SubPacket.AddMember(SST_PEI_HEAD_OR_TAIL,m_ParticleEmitterInfos[i].HeadOrTail));
		CHECK_SMART_STRUCT_ADD_AND_RETURN(SubPacket.AddMember(SST_PEI_TEXTURE_TILE_ROTATION,m_ParticleEmitterInfos[i].TextureTileRotation));
		CHECK_SMART_STRUCT_ADD_AND_RETURN(SubPacket.AddMember(SST_PEI_TEXTURE_ROWS,m_ParticleEmitterInfos[i].TextureRows));
		CHECK_SMART_STRUCT_ADD_AND_RETURN(SubPacket.AddMember(SST_PEI_TEXTURE_COLS,m_ParticleEmitterInfos[i].TextureCols));
		CHECK_SMART_STRUCT_ADD_AND_RETURN(SubPacket.AddMember(SST_PEI_SCALE,
			(char *)&(m_ParticleEmitterInfos[i].Scale),sizeof(m_ParticleEmitterInfos[i].Scale)));
		CHECK_SMART_STRUCT_ADD_AND_RETURN(SubPacket.AddMember(SST_PEI_SLOWDOWN,m_ParticleEmitterInfos[i].Slowdown));
		CHECK_SMART_STRUCT_ADD_AND_RETURN(SubPacket.AddMember(SST_PEI_ROTATION,m_ParticleEmitterInfos[i].Rotation));
		CHECK_SMART_STRUCT_ADD_AND_RETURN(SubPacket.AddMember(SST_PEI_TRANS,
			(char *)&(m_ParticleEmitterInfos[i].Trans),sizeof(m_ParticleEmitterInfos[i].Trans)));
		ResourceID=pUSOFile->ResourceObjectToIndex(m_ParticleEmitterInfos[i].pFX);
		CHECK_SMART_STRUCT_ADD_AND_RETURN(SubPacket.AddMember(SST_PEI_FX,ResourceID));
		
		
		if(!PackAnimationBlock<FLOAT>(m_ParticleEmitterInfos[i].Speed,SubPacket,SST_PEI_SPEED))
			return false;
		if(!PackAnimationBlock<FLOAT>(m_ParticleEmitterInfos[i].SpeedVariation,SubPacket,SST_PEI_SPEED_VARIATION))
			return false;
		if(!PackAnimationBlock<FLOAT>(m_ParticleEmitterInfos[i].VerticalRange,SubPacket,SST_PEI_VERTICAL_RANGE))
			return false;
		if(!PackAnimationBlock<FLOAT>(m_ParticleEmitterInfos[i].HorizontalRange,SubPacket,SST_PEI_HORIZONTAL_RANGE))
			return false;
		if(!PackAnimationBlock<FLOAT>(m_ParticleEmitterInfos[i].Gravity,SubPacket,SST_PEI_GRAVITY))
			return false;
		if(!PackAnimationBlock<FLOAT>(m_ParticleEmitterInfos[i].LifeSpan,SubPacket,SST_PEI_LIFE_SPAN))
			return false;
		if(!PackAnimationBlock<FLOAT>(m_ParticleEmitterInfos[i].EmissionRate,SubPacket,SST_PEI_EMISSION_RATE))
			return false;
		if(!PackAnimationBlock<FLOAT>(m_ParticleEmitterInfos[i].EmissionAreaLength,SubPacket,SST_PEI_EMISSION_AREA_LENGTH))
			return false;
		if(!PackAnimationBlock<FLOAT>(m_ParticleEmitterInfos[i].EmissionAreaWidth,SubPacket,SST_PEI_EMISSION_AREA_WIDTH))
			return false;
		if(!PackAnimationBlock<FLOAT>(m_ParticleEmitterInfos[i].Decelerate,SubPacket,SST_PEI_DECELERATE))
			return false;

		if(!PackFakeAnimationBlock<CD3DVector3>(m_ParticleEmitterInfos[i].ParticleColor,SubPacket,SST_PEI_PARTICLE_COLOR))
			return false;
		if(!PackFakeAnimationBlock<FLOAT>(m_ParticleEmitterInfos[i].ParticleOpacity,SubPacket,SST_PEI_PARTICLE_OPACITY))
			return false;
		if(!PackFakeAnimationBlock<CD3DVector2>(m_ParticleEmitterInfos[i].ParticleSizes,SubPacket,SST_PEI_PARTICLE_SIZES))
			return false;
		if(!PackFakeAnimationBlock<short>(m_ParticleEmitterInfos[i].Intensity,SubPacket,SST_PEI_INTENSITY))
			return false;
			

		if(!Packet.FinishMember(SST_D3DWMMR_PARTICLE_EMITTER,SubPacket.GetDataLen()))
			return false;
	}
	for(UINT i=0;i<m_RibbonEmitterInfos.GetCount();i++)
	{
		UINT BufferSize;
		void * pBuffer=Packet.PrepareMember(BufferSize);
		CSmartStruct SubPacket(pBuffer,BufferSize,true);	

		CHECK_SMART_STRUCT_ADD_AND_RETURN(SubPacket.AddMember(SST_REI_BONE_ID,m_RibbonEmitterInfos[i].BoneID));
		CHECK_SMART_STRUCT_ADD_AND_RETURN(SubPacket.AddMember(SST_REI_POSITION,
			(char *)(&m_RibbonEmitterInfos[i].Position),sizeof(m_RibbonEmitterInfos[i].Position)));

		for(UINT j=0;j<m_RibbonEmitterInfos[i].Textures.GetCount();j++)
		{
			int ResourceID=pUSOFile->ResourceObjectToIndex(m_RibbonEmitterInfos[i].Textures[j]);
			CHECK_SMART_STRUCT_ADD_AND_RETURN(SubPacket.AddMember(SST_REI_TEXTURE,ResourceID));
		}
		int ResourceID=pUSOFile->ResourceObjectToIndex(m_RibbonEmitterInfos[i].pFX);
		CHECK_SMART_STRUCT_ADD_AND_RETURN(SubPacket.AddMember(SST_REI_FX,ResourceID));
		CHECK_SMART_STRUCT_ADD_AND_RETURN(SubPacket.AddMember(SST_REI_IS_TRANSPARENT,(BYTE)m_RibbonEmitterInfos[i].IsTransparent));
		CHECK_SMART_STRUCT_ADD_AND_RETURN(SubPacket.AddMember(SST_REI_RESOLUTION,m_RibbonEmitterInfos[i].Resolution));
		CHECK_SMART_STRUCT_ADD_AND_RETURN(SubPacket.AddMember(SST_REI_LENGTH,m_RibbonEmitterInfos[i].Length));
		CHECK_SMART_STRUCT_ADD_AND_RETURN(SubPacket.AddMember(SST_REI_EMISSION_ANGLE,m_RibbonEmitterInfos[i].Emissionangle));
		if(!PackAnimationBlock<CD3DVector3>(m_RibbonEmitterInfos[i].Color,SubPacket,SST_REI_COLOR))
			return false;
		if(!PackAnimationBlock<FLOAT>(m_RibbonEmitterInfos[i].Opacity,SubPacket,SST_REI_OPACITY))
			return false;
		if(!PackAnimationBlock<FLOAT>(m_RibbonEmitterInfos[i].Above,SubPacket,SST_REI_ABOVE))
			return false;
		if(!PackAnimationBlock<FLOAT>(m_RibbonEmitterInfos[i].Below,SubPacket,SST_REI_BELOW))
			return false;


		if(!Packet.FinishMember(SST_D3DWMMR_RIBBON_EMITTER,SubPacket.GetDataLen()))
			return false;
	}
	return true;
}

bool CD3DWOWM2ModelResource::FromSmartStruct(CSmartStruct& Packet,CUSOFile * pUSOFile,UINT Param)
{
	if(!CD3DObjectResource::FromSmartStruct(Packet,pUSOFile,Param))
		return false;

	UINT AniSeqCount=0;
	UINT ColorAniCount=0;
	UINT TraAniCount=0;
	UINT UVAniCount=0;
	UINT BoneCount=0;	
	UINT AttachmentCount=0;
	UINT PECount=0;
	UINT RECount=0;
	void * Pos=Packet.GetFirstMemberPosition();
	while(Pos)
	{
		WORD MemberID;
		CSmartValue Value=Packet.GetNextMember(Pos,MemberID);
		switch(MemberID)
		{	
		case SST_D3DWMMR_BONE_ANIMATION_SEQUENCE:
			AniSeqCount++;
			break;
		case SST_D3DWMMR_COLOR_ANIMATION:
			ColorAniCount++;
			break;
		case SST_D3DWMMR_TRANSPARENCY_ANIMATION:
			TraAniCount++;
			break;
		case SST_D3DWMMR_TEXTURE_UV_ANIMATION:
			UVAniCount++;
			break;
		case SST_D3DWMMR_BONE:
			BoneCount++;
			break;	
		case SST_D3DWMMR_ATTACHMENT:
			AttachmentCount++;
			break;
		case SST_D3DWMMR_PARTICLE_EMITTER:
			PECount++;
			break;
		case SST_D3DWMMR_RIBBON_EMITTER:
			RECount++;
			break;
		}
	}	
	m_AnimationSequence.Resize(AniSeqCount);
	m_ColorAnimations.Resize(ColorAniCount);
	m_TransparencyAnimations.Resize(TraAniCount);
	m_TextureUVAnimations.Resize(UVAniCount);
	m_Bones.Resize(BoneCount);	
	m_Attachments.Resize(AttachmentCount);
	m_ParticleEmitterInfos.Resize(PECount);
	m_RibbonEmitterInfos.Resize(RECount);

	AniSeqCount=0;
	ColorAniCount=0;
	TraAniCount=0;
	UVAniCount=0;
	BoneCount=0;	
	AttachmentCount=0;
	PECount=0;
	RECount=0;
	Pos=Packet.GetFirstMemberPosition();
	while(Pos)
	{
		WORD MemberID;
		CSmartStruct Value=Packet.GetNextMember(Pos,MemberID);
		switch(MemberID)
		{		
		case SST_D3DWMMR_COLOR_ANIMATION:
			{
				CSmartStruct AniPacket=Value.GetMember(SST_CA_COLOR_ANIMATION);
				int AniCount=GetPacketAniBlockCount(AniPacket);
				m_ColorAnimations[ColorAniCount].ColorAnimations.Animations.Resize(AniCount);

				AniPacket=Value.GetMember(SST_CA_ALPHA_ANIMATION);				
				AniCount=GetPacketAniBlockCount(AniPacket);				
				m_ColorAnimations[ColorAniCount].AlphaAnimations.Animations.Resize(AniCount);
			}
			ColorAniCount++;
			break;
		case SST_D3DWMMR_TRANSPARENCY_ANIMATION:
			{
				CSmartStruct AniPacket=Value.GetMember(SST_TA_ALPHA_ANIMATION);
				int AniCount=GetPacketAniBlockCount(AniPacket);
				m_TransparencyAnimations[TraAniCount].AlphaAnimations.Animations.Resize(AniCount);
			}
			TraAniCount++;
			break;
		case SST_D3DWMMR_TEXTURE_UV_ANIMATION:
			{
				CSmartStruct AniPacket=Value.GetMember(SST_TUVA_TRANSLATIONS);
				int AniCount=GetPacketAniBlockCount(AniPacket);
				m_TextureUVAnimations[UVAniCount].Translations.Animations.Resize(AniCount);

				AniPacket=Value.GetMember(SST_TUVA_SCALINGS);
				AniCount=GetPacketAniBlockCount(AniPacket);
				m_TextureUVAnimations[UVAniCount].Rotations.Animations.Resize(AniCount);

				AniPacket=Value.GetMember(SST_TUVA_ROTATIONS);
				AniCount=GetPacketAniBlockCount(AniPacket);
				m_TextureUVAnimations[UVAniCount].Scalings.Animations.Resize(AniCount);
			}
			UVAniCount++;
			break;
		case SST_D3DWMMR_BONE:
			{
				CSmartStruct AniPacket=Value.GetMember(SST_BI_TRANSLATIONS);
				int AniCount=GetPacketAniBlockCount(AniPacket);
				m_Bones[BoneCount].Translations.Animations.Resize(AniCount);

				AniPacket=Value.GetMember(SST_BI_ROTATIONS);
				AniCount=GetPacketAniBlockCount(AniPacket);
				m_Bones[BoneCount].Rotations.Animations.Resize(AniCount);

				AniPacket=Value.GetMember(SST_BI_SCALINGS);
				AniCount=GetPacketAniBlockCount(AniPacket);
				m_Bones[BoneCount].Scalings.Animations.Resize(AniCount);
			}
			BoneCount++;
			break;		
		case SST_D3DWMMR_PARTICLE_EMITTER:
			{
				CSmartStruct AniPacket=Value.GetMember(SST_PEI_SPEED);
				int AniCount=GetPacketAniBlockCount(AniPacket);
				m_ParticleEmitterInfos[PECount].Speed.Animations.Resize(AniCount);

				AniPacket=Value.GetMember(SST_PEI_SPEED_VARIATION);
				AniCount=GetPacketAniBlockCount(AniPacket);
				m_ParticleEmitterInfos[PECount].SpeedVariation.Animations.Resize(AniCount);

				AniPacket=Value.GetMember(SST_PEI_VERTICAL_RANGE);
				AniCount=GetPacketAniBlockCount(AniPacket);
				m_ParticleEmitterInfos[PECount].VerticalRange.Animations.Resize(AniCount);

				AniPacket=Value.GetMember(SST_PEI_HORIZONTAL_RANGE);
				AniCount=GetPacketAniBlockCount(AniPacket);
				m_ParticleEmitterInfos[PECount].HorizontalRange.Animations.Resize(AniCount);

				AniPacket=Value.GetMember(SST_PEI_GRAVITY);
				AniCount=GetPacketAniBlockCount(AniPacket);
				m_ParticleEmitterInfos[PECount].Gravity.Animations.Resize(AniCount);

				AniPacket=Value.GetMember(SST_PEI_LIFE_SPAN);
				AniCount=GetPacketAniBlockCount(AniPacket);
				m_ParticleEmitterInfos[PECount].LifeSpan.Animations.Resize(AniCount);

				AniPacket=Value.GetMember(SST_PEI_EMISSION_RATE);
				AniCount=GetPacketAniBlockCount(AniPacket);
				m_ParticleEmitterInfos[PECount].EmissionRate.Animations.Resize(AniCount);

				AniPacket=Value.GetMember(SST_PEI_EMISSION_AREA_LENGTH);
				AniCount=GetPacketAniBlockCount(AniPacket);
				m_ParticleEmitterInfos[PECount].EmissionAreaLength.Animations.Resize(AniCount);

				AniPacket=Value.GetMember(SST_PEI_EMISSION_AREA_WIDTH);
				AniCount=GetPacketAniBlockCount(AniPacket);
				m_ParticleEmitterInfos[PECount].EmissionAreaWidth.Animations.Resize(AniCount);

				AniPacket=Value.GetMember(SST_PEI_DECELERATE);
				AniCount=GetPacketAniBlockCount(AniPacket);
				m_ParticleEmitterInfos[PECount].Decelerate.Animations.Resize(AniCount);
			}
			PECount++;
			break;
		case SST_D3DWMMR_RIBBON_EMITTER:
			{
				void * SubPos=Value.GetFirstMemberPosition();
				int TexCount=0;
				while(SubPos)
				{
					WORD SubMemberID;
					CSmartStruct SubValue=Value.GetNextMember(SubPos,SubMemberID);
					switch(SubMemberID)
					{					
					case SST_REI_TEXTURE:
						TexCount++;
						break;
					case SST_REI_COLOR:
						{
							int AniCount=GetPacketAniBlockCount(SubValue);
							m_RibbonEmitterInfos[RECount].Color.Animations.Resize(AniCount);
						}
						break;
					case SST_REI_OPACITY:
						{
							int AniCount=GetPacketAniBlockCount(SubValue);
							m_RibbonEmitterInfos[RECount].Opacity.Animations.Resize(AniCount);
						}
						break;
					case SST_REI_ABOVE:
						{
							int AniCount=GetPacketAniBlockCount(SubValue);
							m_RibbonEmitterInfos[RECount].Above.Animations.Resize(AniCount);
						}
						break;
					case SST_REI_BELOW:
						{
							int AniCount=GetPacketAniBlockCount(SubValue);
							m_RibbonEmitterInfos[RECount].Below.Animations.Resize(AniCount);
						}
						break;
					}
				}
				m_RibbonEmitterInfos[RECount].Textures.Resize(TexCount);				
			}
			RECount++;
			break;
		}
	}

	AniSeqCount=0;
	ColorAniCount=0;
	TraAniCount=0;
	UVAniCount=0;
	BoneCount=0;	
	AttachmentCount=0;
	PECount=0;
	RECount=0;
	Pos=Packet.GetFirstMemberPosition();
	while(Pos)
	{
		WORD MemberID;
		CSmartValue Value=Packet.GetNextMember(Pos,MemberID);
		switch(MemberID)
		{	
		case SST_D3DWMMR_BONE_ANIMATION_SEQUENCE:
			{
				CSmartStruct SubPacket=Value;
				
				m_AnimationSequence[AniSeqCount].AnimationID=SubPacket.GetMember(SST_BAS_ANIMATION_ID);
				m_AnimationSequence[AniSeqCount].AnimationName=(LPCTSTR)SubPacket.GetMember(SST_BAS_ANIMATION_NAME);
				m_AnimationSequence[AniSeqCount].SubAnimationID=SubPacket.GetMember(SST_BAS_SUBANIMATION_ID);
				m_AnimationSequence[AniSeqCount].Index=SubPacket.GetMember(SST_BAS_INDEX);
				m_AnimationSequence[AniSeqCount].Length=SubPacket.GetMember(SST_BAS_LENGTH);
				m_AnimationSequence[AniSeqCount].MovingSpeed=SubPacket.GetMember(SST_BAS_MOVING_SPEED);
				m_AnimationSequence[AniSeqCount].Flags=SubPacket.GetMember(SST_BAS_FLAGS);
				m_AnimationSequence[AniSeqCount].Flags2=SubPacket.GetMember(SST_BAS_FLAGS2);
				m_AnimationSequence[AniSeqCount].PlaybackSpeed=SubPacket.GetMember(SST_BAS_PLAYBACK_SPEED);
				memcpy(&m_AnimationSequence[AniSeqCount].BoundingBox,(LPCTSTR)SubPacket.GetMember(SST_BAS_BOUNDING_BOX),
					sizeof(m_AnimationSequence[AniSeqCount].BoundingBox));
				memcpy(&m_AnimationSequence[AniSeqCount].BoundingSphere,(LPCTSTR)SubPacket.GetMember(SST_BAS_BOUNDING_SPHERE),
					sizeof(m_AnimationSequence[AniSeqCount].BoundingSphere));				
			}
			AniSeqCount++;
			break;
		case SST_D3DWMMR_COLOR_ANIMATION:
			{
				CSmartStruct SubPacket=Value;
				if(!UnpackAnimationBlock<CD3DVector3>(m_ColorAnimations[ColorAniCount].ColorAnimations,SubPacket,SST_CA_COLOR_ANIMATION))
					return false;
				if(!UnpackAnimationBlock<FLOAT>(m_ColorAnimations[ColorAniCount].AlphaAnimations,SubPacket,SST_CA_ALPHA_ANIMATION))
					return false;
			}
			ColorAniCount++;
			break;
		case SST_D3DWMMR_TRANSPARENCY_ANIMATION:
			{
				CSmartStruct SubPacket=Value;				
				if(!UnpackAnimationBlock<FLOAT>(m_TransparencyAnimations[TraAniCount].AlphaAnimations,SubPacket,SST_TA_ALPHA_ANIMATION))
					return false;
			}
			TraAniCount++;
			break;
		case SST_D3DWMMR_TEXTURE_UV_ANIMATION:
			{
				CSmartStruct SubPacket=Value;
				if(!UnpackAnimationBlock<CD3DVector3>(m_TextureUVAnimations[UVAniCount].Translations,SubPacket,SST_TUVA_TRANSLATIONS))
					return false;
				if(!UnpackAnimationBlock<CD3DQuaternion>(m_TextureUVAnimations[UVAniCount].Rotations,SubPacket,SST_TUVA_ROTATIONS))
					return false;
				if(!UnpackAnimationBlock<CD3DVector3>(m_TextureUVAnimations[UVAniCount].Scalings,SubPacket,SST_TUVA_SCALINGS))
					return false;
			}
			UVAniCount++;
			break;
		case SST_D3DWMMR_BONE:
			{
				CSmartStruct SubPacket=Value;
				m_Bones[BoneCount].AnimationSeq=SubPacket.GetMember(SST_BI_ANIMATION_SEQ);
				m_Bones[BoneCount].Flags=SubPacket.GetMember(SST_BI_FLAGS);
				m_Bones[BoneCount].ChildCount=0;
				m_Bones[BoneCount].ParentBone=SubPacket.GetMember(SST_BI_PARENT_BONE);
				if(m_Bones[BoneCount].ParentBone>=0)
				{
					m_Bones[m_Bones[BoneCount].ParentBone].ChildCount++;
				}
				m_Bones[BoneCount].GeosetID=SubPacket.GetMember(SST_BI_GEOSET_ID);
				memcpy(&m_Bones[BoneCount].PivotPoint,(LPCTSTR)SubPacket.GetMember(SST_BI_PIVOT_POINT),
					sizeof(m_Bones[BoneCount].PivotPoint));
				if(!UnpackAnimationBlock<CD3DVector3>(m_Bones[BoneCount].Translations,SubPacket,SST_BI_TRANSLATIONS))
					return false;
				if(!UnpackAnimationBlock<CD3DQuaternion>(m_Bones[BoneCount].Rotations,SubPacket,SST_BI_ROTATIONS))
					return false;
				if(!UnpackAnimationBlock<CD3DVector3>(m_Bones[BoneCount].Scalings,SubPacket,SST_BI_SCALINGS))
					return false;
			}
			BoneCount++;
			break;
		case SST_D3DWMMR_SKIN_MESH_BONE_COUNT:
			m_SkinMeshBoneCount=Value;
			break;
		case SST_D3DWMMR_KEY_BONE:
			m_KeyBoneIndex.Resize(Value.GetLength()/sizeof(short));
			memcpy(m_KeyBoneIndex.GetBuffer(),(LPCTSTR)Value,
				sizeof(short)*m_KeyBoneIndex.GetCount());
			break;
		case SST_D3DWMMR_GLOBAL_SEQUENCE:
			m_GlobalSequences.Resize(Value.GetLength()/sizeof(UINT));
			memcpy(m_GlobalSequences.GetBuffer(),(LPCTSTR)Value,
				sizeof(UINT)*m_GlobalSequences.GetCount());
			break;
		case SST_D3DWMMR_ATTACHMENT:
			{
				CSmartStruct SubPacket=Value;
				m_Attachments[AttachmentCount].ID=SubPacket.GetMember(SST_AI_ID);
				m_Attachments[AttachmentCount].Bone=SubPacket.GetMember(SST_AI_BONE);
				m_Attachments[AttachmentCount].Name=(LPCTSTR)SubPacket.GetMember(SST_AI_NAME);				
				memcpy(&m_Attachments[AttachmentCount].Position,(LPCTSTR)SubPacket.GetMember(SST_AI_POSITION),
					sizeof(m_Attachments[AttachmentCount].Position));				
			}
			AttachmentCount++;
			break;
		case SST_D3DWMMR_PARTICLE_EMITTER:
			{
				CSmartStruct SubPacket=Value;
				m_ParticleEmitterInfos[PECount].Flags=SubPacket.GetMember(SST_PEI_FLAGS);
				memcpy(&m_ParticleEmitterInfos[PECount].Position,(LPCTSTR)SubPacket.GetMember(SST_PEI_POSITION),
					sizeof(m_ParticleEmitterInfos[PECount].Position));
				m_ParticleEmitterInfos[PECount].Bone=SubPacket.GetMember(SST_PEI_BONE);
				int ResourceID=SubPacket.GetMember(SST_PEI_TEXTURE);
				m_ParticleEmitterInfos[PECount].pTexture=
					(CD3DTexture *)pUSOFile->ResourceIndexToObject(ResourceID,GET_CLASS_INFO(CD3DTexture));
				if(m_ParticleEmitterInfos[PECount].pTexture)
				{
					m_ParticleEmitterInfos[PECount].pTexture->AddUseRef();
				}
				m_ParticleEmitterInfos[PECount].BlendingType=SubPacket.GetMember(SST_PEI_BLENDING_TYPE);
				m_ParticleEmitterInfos[PECount].EmitterType=SubPacket.GetMember(SST_PEI_EMITTER_TYPE);
				m_ParticleEmitterInfos[PECount].ParticleColorRef=SubPacket.GetMember(SST_PEI_PARTICLE_COLOR_REF);
				m_ParticleEmitterInfos[PECount].ParticleType=SubPacket.GetMember(SST_PEI_PARTICLE_TYPE);
				m_ParticleEmitterInfos[PECount].HeadOrTail=SubPacket.GetMember(SST_PEI_HEAD_OR_TAIL);
				m_ParticleEmitterInfos[PECount].TextureTileRotation=SubPacket.GetMember(SST_PEI_TEXTURE_TILE_ROTATION);
				m_ParticleEmitterInfos[PECount].TextureRows=SubPacket.GetMember(SST_PEI_TEXTURE_ROWS);
				m_ParticleEmitterInfos[PECount].TextureCols=SubPacket.GetMember(SST_PEI_TEXTURE_COLS);
				memcpy(&m_ParticleEmitterInfos[PECount].Scale,(LPCTSTR)SubPacket.GetMember(SST_PEI_SCALE),
					sizeof(m_ParticleEmitterInfos[PECount].Scale));
				m_ParticleEmitterInfos[PECount].Slowdown=SubPacket.GetMember(SST_PEI_SLOWDOWN);
				m_ParticleEmitterInfos[PECount].Rotation=SubPacket.GetMember(SST_PEI_ROTATION);
				memcpy(&m_ParticleEmitterInfos[PECount].Trans,(LPCTSTR)SubPacket.GetMember(SST_PEI_TRANS),
					sizeof(m_ParticleEmitterInfos[PECount].Trans));
				ResourceID=SubPacket.GetMember(SST_PEI_FX);
				m_ParticleEmitterInfos[PECount].pFX=
					(CD3DFX *)pUSOFile->ResourceIndexToObject(ResourceID,GET_CLASS_INFO(CD3DFX));
				if(m_ParticleEmitterInfos[PECount].pFX)
				{
					m_ParticleEmitterInfos[PECount].pFX->AddUseRef();
				}
				if(!UnpackAnimationBlock<FLOAT>(m_ParticleEmitterInfos[PECount].Speed,SubPacket,SST_PEI_SPEED))
					return false;
				if(!UnpackAnimationBlock<FLOAT>(m_ParticleEmitterInfos[PECount].SpeedVariation,SubPacket,SST_PEI_SPEED_VARIATION))
					return false;
				if(!UnpackAnimationBlock<FLOAT>(m_ParticleEmitterInfos[PECount].VerticalRange,SubPacket,SST_PEI_VERTICAL_RANGE))
					return false;
				if(!UnpackAnimationBlock<FLOAT>(m_ParticleEmitterInfos[PECount].HorizontalRange,SubPacket,SST_PEI_HORIZONTAL_RANGE))
					return false;
				if(!UnpackAnimationBlock<FLOAT>(m_ParticleEmitterInfos[PECount].Gravity,SubPacket,SST_PEI_GRAVITY))
					return false;
				if(!UnpackAnimationBlock<FLOAT>(m_ParticleEmitterInfos[PECount].LifeSpan,SubPacket,SST_PEI_LIFE_SPAN))
					return false;
				if(!UnpackAnimationBlock<FLOAT>(m_ParticleEmitterInfos[PECount].EmissionRate,SubPacket,SST_PEI_EMISSION_RATE))
					return false;
				if(!UnpackAnimationBlock<FLOAT>(m_ParticleEmitterInfos[PECount].EmissionAreaLength,SubPacket,SST_PEI_EMISSION_AREA_LENGTH))
					return false;
				if(!UnpackAnimationBlock<FLOAT>(m_ParticleEmitterInfos[PECount].EmissionAreaWidth,SubPacket,SST_PEI_EMISSION_AREA_WIDTH))
					return false;
				if(!UnpackAnimationBlock<FLOAT>(m_ParticleEmitterInfos[PECount].Decelerate,SubPacket,SST_PEI_DECELERATE))
					return false;

				if(!UnpackFakeAnimationBlock<CD3DVector3>(m_ParticleEmitterInfos[PECount].ParticleColor,SubPacket,SST_PEI_PARTICLE_COLOR))
					return false;
				if(!UnpackFakeAnimationBlock<FLOAT>(m_ParticleEmitterInfos[PECount].ParticleOpacity,SubPacket,SST_PEI_PARTICLE_OPACITY))
					return false;
				if(!UnpackFakeAnimationBlock<CD3DVector2>(m_ParticleEmitterInfos[PECount].ParticleSizes,SubPacket,SST_PEI_PARTICLE_SIZES))
					return false;
				if(!UnpackFakeAnimationBlock<short>(m_ParticleEmitterInfos[PECount].Intensity,SubPacket,SST_PEI_INTENSITY))
					return false;
						
			}
			PECount++;
			break;
		case SST_D3DWMMR_RIBBON_EMITTER:
			{
				CSmartStruct SubPacket=Value;
				
				int TexIndex=0;
				void * SubPos=SubPacket.GetFirstMemberPosition();
				while(SubPos)
				{
					WORD SubMemberID;
					CSmartValue SubValue=SubPacket.GetNextMember(SubPos,SubMemberID);
					switch(SubMemberID)
					{
					case SST_REI_BONE_ID:
						m_RibbonEmitterInfos[RECount].BoneID=SubValue;
						break;
					case SST_REI_POSITION:
						memcpy(&m_RibbonEmitterInfos[RECount].Position,(LPCTSTR)SubValue,
							sizeof(m_RibbonEmitterInfos[RECount].Position));
						break;
					case SST_REI_TEXTURE:
						{
							int ResourceID=SubValue;
							m_RibbonEmitterInfos[RECount].Textures[TexIndex]=
								(CD3DTexture *)pUSOFile->ResourceIndexToObject(ResourceID,GET_CLASS_INFO(CD3DTexture));
							if(m_RibbonEmitterInfos[RECount].Textures[TexIndex])
							{
								m_RibbonEmitterInfos[RECount].Textures[TexIndex]->AddUseRef();
							}
						}
						break;
					case SST_REI_FX:
						{
							int ResourceID=SubValue;
							m_RibbonEmitterInfos[RECount].pFX=
								(CD3DFX *)pUSOFile->ResourceIndexToObject(ResourceID,GET_CLASS_INFO(CD3DFX));
							if(m_RibbonEmitterInfos[RECount].pFX)
							{
								m_RibbonEmitterInfos[RECount].pFX->AddUseRef();
							}
						}
						break;
					case SST_REI_IS_TRANSPARENT:
						m_RibbonEmitterInfos[RECount].IsTransparent=(BYTE)SubValue!=0;
						break;
					case SST_REI_RESOLUTION:
						m_RibbonEmitterInfos[RECount].Resolution=SubValue;
						break;
					case SST_REI_LENGTH:
						m_RibbonEmitterInfos[RECount].Length=SubValue;
						break;
					case SST_REI_EMISSION_ANGLE:
						m_RibbonEmitterInfos[RECount].Emissionangle=SubValue;
						break;								
					}
				}
				if(!UnpackAnimationBlock<CD3DVector3>(m_RibbonEmitterInfos[RECount].Color,SubPacket,SST_REI_COLOR))
					return false;
				if(!UnpackAnimationBlock<FLOAT>(m_RibbonEmitterInfos[RECount].Opacity,SubPacket,SST_REI_OPACITY))
					return false;
				if(!UnpackAnimationBlock<FLOAT>(m_RibbonEmitterInfos[RECount].Above,SubPacket,SST_REI_ABOVE))
					return false;
				if(!UnpackAnimationBlock<FLOAT>(m_RibbonEmitterInfos[RECount].Below,SubPacket,SST_REI_BELOW))
					return false;
			}
			RECount++;
			break;
		}
	}	
	CheckAni();
	//PrintBoneInfo();
	
	return true;
}

UINT CD3DWOWM2ModelResource::GetSmartStructSize(UINT Param)
{
	UINT Size=CD3DObjectResource::GetSmartStructSize(Param);
	//for(int i=0;i<m_DummyCount;i++)
	//{
	//	Size+=SMART_STRUCT_STRING_MEMBER_SIZE(strlen(m_pDummys[i].szName));
	//	Size+=SMART_STRUCT_STRING_MEMBER_SIZE(sizeof(m_pDummys[i].TransformMatrix));
	//}

	for(UINT i=0;i<m_AnimationSequence.GetCount();i++)
	{
		Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(m_AnimationSequence[i].AnimationID));		
		Size+=SMART_STRUCT_STRING_MEMBER_SIZE(m_AnimationSequence[i].AnimationName.GetLength());
		Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(m_AnimationSequence[i].SubAnimationID));
		Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(m_AnimationSequence[i].Index));
		Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(m_AnimationSequence[i].Length));
		Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(m_AnimationSequence[i].MovingSpeed));
		Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(m_AnimationSequence[i].Flags));
		Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(m_AnimationSequence[i].Flags2));
		Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(m_AnimationSequence[i].PlaybackSpeed));
		Size+=SMART_STRUCT_STRING_MEMBER_SIZE(sizeof(m_AnimationSequence[i].BoundingBox));
		Size+=SMART_STRUCT_STRING_MEMBER_SIZE(sizeof(m_AnimationSequence[i].BoundingSphere));
		Size+=SMART_STRUCT_STRUCT_MEMBER_SIZE(0);
	}
	for(UINT i=0;i<m_ColorAnimations.GetCount();i++)
	{
		Size+=GetAnimationBlockSmartStructSize<CD3DVector3>(m_ColorAnimations[i].ColorAnimations);
		Size+=GetAnimationBlockSmartStructSize<FLOAT>(m_ColorAnimations[i].AlphaAnimations);
		Size+=SMART_STRUCT_STRUCT_MEMBER_SIZE(0);
	}
	for(UINT i=0;i<m_TransparencyAnimations.GetCount();i++)
	{
		Size+=GetAnimationBlockSmartStructSize<FLOAT>(m_TransparencyAnimations[i].AlphaAnimations);
		Size+=SMART_STRUCT_STRUCT_MEMBER_SIZE(0);		
	}

	for(UINT i=0;i<m_TextureUVAnimations.GetCount();i++)
	{
		Size+=GetAnimationBlockSmartStructSize<CD3DVector3>(m_TextureUVAnimations[i].Translations);
		Size+=GetAnimationBlockSmartStructSize<CD3DQuaternion>(m_TextureUVAnimations[i].Rotations);
		Size+=GetAnimationBlockSmartStructSize<CD3DVector3>(m_TextureUVAnimations[i].Scalings);
		Size+=SMART_STRUCT_STRUCT_MEMBER_SIZE(0);
	}

	for(UINT i=0;i<m_Bones.GetCount();i++)
	{
		Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(m_Bones[i].AnimationSeq));
		Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(m_Bones[i].Flags));
		Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(m_Bones[i].ParentBone));
		Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(m_Bones[i].GeosetID));
		Size+=SMART_STRUCT_STRING_MEMBER_SIZE(sizeof(m_Bones[i].PivotPoint));
		Size+=GetAnimationBlockSmartStructSize<CD3DVector3>(m_Bones[i].Translations);
		Size+=GetAnimationBlockSmartStructSize<CD3DQuaternion>(m_Bones[i].Rotations);
		Size+=GetAnimationBlockSmartStructSize<CD3DVector3>(m_Bones[i].Scalings);
		Size+=SMART_STRUCT_STRUCT_MEMBER_SIZE(0);
	}

	Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(m_SkinMeshBoneCount));
	Size+=SMART_STRUCT_STRING_MEMBER_SIZE(m_KeyBoneIndex.GetCount()*sizeof(short));
	Size+=SMART_STRUCT_STRING_MEMBER_SIZE(m_GlobalSequences.GetCount()*sizeof(UINT));

	for(UINT i=0;i<m_Attachments.GetCount();i++)
	{
		Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(m_Attachments[i].ID));
		Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(m_Attachments[i].Bone));
		Size+=SMART_STRUCT_STRING_MEMBER_SIZE(m_Attachments[i].Name.GetLength());
		Size+=SMART_STRUCT_STRING_MEMBER_SIZE(sizeof(m_Attachments[i].Position));
		Size+=SMART_STRUCT_STRUCT_MEMBER_SIZE(0);
	}

	for(UINT i=0;i<m_ParticleEmitterInfos.GetCount();i++)
	{
		Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(m_ParticleEmitterInfos[i].Flags));
		Size+=SMART_STRUCT_STRING_MEMBER_SIZE(sizeof(m_ParticleEmitterInfos[i].Position));
		Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(m_ParticleEmitterInfos[i].Bone));
		Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(int));
		Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(m_ParticleEmitterInfos[i].BlendingType));
		Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(m_ParticleEmitterInfos[i].EmitterType));
		Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(m_ParticleEmitterInfos[i].ParticleColorRef));
		Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(m_ParticleEmitterInfos[i].ParticleType));
		Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(m_ParticleEmitterInfos[i].HeadOrTail));
		Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(m_ParticleEmitterInfos[i].TextureTileRotation));
		Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(m_ParticleEmitterInfos[i].TextureRows));
		Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(m_ParticleEmitterInfos[i].TextureCols));
		Size+=SMART_STRUCT_STRING_MEMBER_SIZE(sizeof(m_ParticleEmitterInfos[i].Scale));
		Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(m_ParticleEmitterInfos[i].Slowdown));
		Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(m_ParticleEmitterInfos[i].Rotation));
		Size+=SMART_STRUCT_STRING_MEMBER_SIZE(sizeof(m_ParticleEmitterInfos[i].Trans));
		Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(int));
		Size+=GetAnimationBlockSmartStructSize<FLOAT>(m_ParticleEmitterInfos[i].Speed);
		Size+=GetAnimationBlockSmartStructSize<FLOAT>(m_ParticleEmitterInfos[i].SpeedVariation);
		Size+=GetAnimationBlockSmartStructSize<FLOAT>(m_ParticleEmitterInfos[i].VerticalRange);
		Size+=GetAnimationBlockSmartStructSize<FLOAT>(m_ParticleEmitterInfos[i].HorizontalRange);
		Size+=GetAnimationBlockSmartStructSize<FLOAT>(m_ParticleEmitterInfos[i].Gravity);
		Size+=GetAnimationBlockSmartStructSize<FLOAT>(m_ParticleEmitterInfos[i].LifeSpan);
		Size+=GetAnimationBlockSmartStructSize<FLOAT>(m_ParticleEmitterInfos[i].EmissionRate);
		Size+=GetAnimationBlockSmartStructSize<FLOAT>(m_ParticleEmitterInfos[i].EmissionAreaLength);
		Size+=GetAnimationBlockSmartStructSize<FLOAT>(m_ParticleEmitterInfos[i].EmissionAreaWidth);
		Size+=GetAnimationBlockSmartStructSize<FLOAT>(m_ParticleEmitterInfos[i].Decelerate);

		Size+=GetFakeAnimationBlockSmartStructSize<CD3DVector3>(m_ParticleEmitterInfos[i].ParticleColor);
		Size+=GetFakeAnimationBlockSmartStructSize<FLOAT>(m_ParticleEmitterInfos[i].ParticleOpacity);
		Size+=GetFakeAnimationBlockSmartStructSize<CD3DVector2>(m_ParticleEmitterInfos[i].ParticleSizes);
		Size+=GetFakeAnimationBlockSmartStructSize<short>(m_ParticleEmitterInfos[i].Intensity);
		
		Size+=SMART_STRUCT_STRUCT_MEMBER_SIZE(0);
	}
	for(UINT i=0;i<m_RibbonEmitterInfos.GetCount();i++)
	{
		Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(m_RibbonEmitterInfos[i].BoneID));
		Size+=SMART_STRUCT_STRING_MEMBER_SIZE(sizeof(m_RibbonEmitterInfos[i].Position));

		for(UINT j=0;j<m_RibbonEmitterInfos[i].Textures.GetCount();j++)
		{
			Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(int));
		}
		Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(int));
		Size+=SMART_STRUCT_STRING_MEMBER_SIZE(sizeof(m_RibbonEmitterInfos[i].IsTransparent));
		Size+=SMART_STRUCT_STRING_MEMBER_SIZE(sizeof(m_RibbonEmitterInfos[i].Resolution));
		Size+=SMART_STRUCT_STRING_MEMBER_SIZE(sizeof(m_RibbonEmitterInfos[i].Length));
		Size+=SMART_STRUCT_STRING_MEMBER_SIZE(sizeof(m_RibbonEmitterInfos[i].Emissionangle));

		Size+=GetAnimationBlockSmartStructSize<CD3DVector3>(m_RibbonEmitterInfos[i].Color);
		Size+=GetAnimationBlockSmartStructSize<FLOAT>(m_RibbonEmitterInfos[i].Opacity);
		Size+=GetAnimationBlockSmartStructSize<FLOAT>(m_RibbonEmitterInfos[i].Above);
		Size+=GetAnimationBlockSmartStructSize<FLOAT>(m_RibbonEmitterInfos[i].Below);
		Size+=SMART_STRUCT_STRUCT_MEMBER_SIZE(0);
	}
	return Size;
}	



void CD3DWOWM2ModelResource::MakeSubmeshMaterial(BYTE * pModelData,BYTE * pSkinData,UINT SubMeshIndex,CD3DSubMesh * pD3DSubMesh)
{	
	if(pModelData==NULL||pSkinData==NULL)
		return;

	BLZ_M2_HEADER * pHeader=(BLZ_M2_HEADER *)pModelData;


	

	WORD * pTexturesLookup=(WORD *)(pModelData+pHeader->TexLookupOffset);
	short * pTextureAnimLookup=(short *)(pModelData+pHeader->TexAnimLookupOffset);
	short * pTransparencylookup=(short *)(pModelData+pHeader->TransLookupOffset);
	short * pTextureUnitsLookup=(short *)(pModelData+pHeader->TexUnitsOffset);	


	M2_MODEL_TEXTURE * pTextures=(M2_MODEL_TEXTURE *)(pModelData+pHeader->TexturesOffset);
	M2_MODEL_ANIMATION_BLOCK * pTransparency=(M2_MODEL_ANIMATION_BLOCK *)(pModelData+pHeader->TransparencyOffset);
	M2_MODEL_RENDER_FLAG * pRenderFlags=(M2_MODEL_RENDER_FLAG *)(pModelData+pHeader->RenderFlagsOffset);


	SKIN_HEADER * pSkinHeader=(SKIN_HEADER *)pSkinData;


	SKIN_SUB_MESH * pSubMesh=(SKIN_SUB_MESH *)(pSkinData+pSkinHeader->SubmeshesOffset);

	SKIN_TEXTURE_UNIT * pTextureUnits=(SKIN_TEXTURE_UNIT *)(pSkinData+pSkinHeader->TextureUnitsOffset);

	
	D3DCOLORVALUE WhiteColor={1.0f,1.0f,1.0f,1.0f};
	D3DCOLORVALUE GrayColor={0.8f,0.8f,0.8f,1.0f};
	D3DCOLORVALUE BlackColor={0.0f,0.0f,0.0f,1.0f};

	pD3DSubMesh->GetMaterial().GetMaterial().Ambient=WhiteColor;
	pD3DSubMesh->GetMaterial().GetMaterial().Diffuse=WhiteColor;
	pD3DSubMesh->GetMaterial().GetMaterial().Specular=WhiteColor;
	pD3DSubMesh->GetMaterial().GetMaterial().Emissive=BlackColor;
	pD3DSubMesh->GetMaterial().GetMaterial().Power=40.0f;

	UINT TextureType=0;
	UINT TextureFlag=0;
	UINT RenderFlag=0;
	UINT BlendingMode=0;

	//for(UINT i=0;i<pHeader->TexLookupCount;i++)
	//{
	//	PrintImportantLog(0,"Index=%u,TexLookup=%u",i,pTexturesLookup[i]);
	//}

	//for(UINT i=0;i<pHeader->TexUnitsCount;i++)
	//{
	//	PrintImportantLog(0,"Index=%u,TextureUnitsLookup=%d",i,pTextureUnitsLookup[i]);
	//}

	//for(UINT i=0;i<pHeader->RenderFlagsCount;i++)
	//{
	//	PrintImportantLog(0,"Index=%u,Flags=%u,BlendingMode=%u"
	//		,i,pRenderFlags[i].Flags,pRenderFlags[i].BlendingMode);
	//}

	//for(UINT i=0;i<pHeader->TexturesCount;i++)
	//{
	//	PrintImportantLog(0,"Index=%u,Type=%u,Unknown=%u,Flags=%u,FileName=%s"
	//		,i,
	//		pTextures[i].Type,
	//		pTextures[i].Unknown,
	//		pTextures[i].Flags,			
	//		(char *)pModelData+pTextures[i].FileNameOffset);
	//}

	

	for(UINT i=0;i<pSkinHeader->TextureUnitsCount;i++)
	{
		//PrintImportantLog(0,"Index=%u,Flags=%u,Shading=%u,SubmeshIndex=%u,SubmeshIndex2=%u,ColorIndex=%d,RenderFlags=%u,"
		//	"TexUnitNumber=%u,Mode=%u,Texture=%u,TexUnitNumber2=%u,Transparency=%u,TextureAnim=%u",i,
		//	pTextureUnits[i].Flags,
		//	pTextureUnits[i].Shading,
		//	pTextureUnits[i].SubmeshIndex,
		//	pTextureUnits[i].SubmeshIndex2,
		//	pTextureUnits[i].ColorIndex,
		//	pTextureUnits[i].RenderFlags,
		//	pTextureUnits[i].TexUnitNumber,
		//	pTextureUnits[i].Mode,
		//	pTextureUnits[i].Texture,
		//	pTextureUnits[i].TexUnitNumber2,
		//	pTextureUnits[i].Transparency,
		//	pTextureUnits[i].TextureAnim);

		if(pTextureUnits[i].SubmeshIndex==SubMeshIndex)
		{			
			M2_MODEL_TEXTURE * pTexture=pTextures+pTexturesLookup[pTextureUnits[i].Texture];

			TextureType=(pTexture->Type<<16)&SMP_TEXTURE_TYPE;
			TextureFlag=(pTexture->Flags<<20)&SMP_TEXTURE_FLAG;

			int RenderFlagsIndex=pTextureUnits[i].RenderFlags;

			RenderFlag=(pRenderFlags[RenderFlagsIndex].Flags<<4)&SMP_RENDER_FLAG;
			BlendingMode=(pRenderFlags[RenderFlagsIndex].BlendingMode<<9)&SMP_BLENDING_MODE;

			if(pTextureUnitsLookup[pTextureUnits[i].TexUnitNumber2]<0&&(RenderFlag&SMP_DISABLE_ZBUFFER))
			{
				TextureFlag|=SMP_TEXTURE_ENV_MAP;
			}
			
			CD3DTexture * pTex=NULL;

			if(TextureType==SMP_TEXTURE_TYPE_DIRECT)
			{
				char * pTextureFileName=(char *)pModelData+pTexture->FileNameOffset;
				pTex=m_pManager->GetDevice()->GetTextureManager()->LoadTexture(pTextureFileName);
			}
			short ColorAniIndex=pTextureUnits[i].ColorIndex;
			short TransparencyAniIndex=-1;
			short UVAniIndex=-1;
			if(pTextureUnits[i].Transparency<pHeader->TransLookupCount)
				TransparencyAniIndex=pTransparencylookup[pTextureUnits[i].Transparency];
			if(pTextureUnits[i].TextureAnim<pHeader->TexAnimLookupCount)
				UVAniIndex=pTextureAnimLookup[pTextureUnits[i].TextureAnim];
			UINT64 TextureProperty=TextureType|TextureFlag|RenderFlag|BlendingMode;
			TextureProperty|=((((INT64)ColorAniIndex)<<24)&SMP_COLOR_ANI_INDEX)|
				((((INT64)TransparencyAniIndex)<<32)&SMP_TRANSPARENCY_ANI_INDEX)|
				((((INT64)UVAniIndex)<<40)&SMP_UV_ANI_INDEX);
			pD3DSubMesh->GetMaterial().AddTexture(pTex,TextureProperty);
			
		}
	}

	
	BuildFX(&(pD3DSubMesh->GetMaterial()));

	BlendingMode=pD3DSubMesh->GetMaterial().GetTextureProperty(0)&SMP_BLENDING_MODE;
	if(BlendingMode>=SMP_BLENDING_ALPHA_BLENDING)
	{
		pD3DSubMesh->SetTransparent(true);
	}		
}


void CD3DWOWM2ModelResource::CreateBounding()
{
	for(UINT i=0;i<m_SubMeshList.GetCount();i++)
	{		
		m_SubMeshList[i]->CreateBounding();
		if(i==0)
		{
			m_BoundingBox=m_SubMeshList[i]->GetBoundingBox();
			m_BoundingSphere=m_SubMeshList[i]->GetBoundingSphere();
		}
		else
		{
			m_BoundingBox+=m_SubMeshList[i]->GetBoundingBox();
			m_BoundingSphere+=m_SubMeshList[i]->GetBoundingSphere();
		}		
	}

}

bool CD3DWOWM2ModelResource::LoadBones(BYTE * pModelData,LPCTSTR szModelPath)
{
	BLZ_M2_HEADER * pHeader=(BLZ_M2_HEADER *)pModelData;
	M2_MODEL_BONE * pBones=(M2_MODEL_BONE *)(pModelData+pHeader->BonesOffset);	


	short * pBoneLookup=(short *)(pModelData+pHeader->BoneLookupOffset);
	short * pKeyBoneLookup=(short *)(pModelData+pHeader->KeyBoneLookupOffset);

	M2_MODEL_ANIMATION_SEQUENCE * pAnimationSequence=(M2_MODEL_ANIMATION_SEQUENCE *)(pModelData+pHeader->AnimationsOffset);
	
	DWORD * pGlobalSequences=(DWORD *)(pModelData+pHeader->GlobalSequencesOffset);

	m_GlobalSequences.Resize(pHeader->GlobalSequencesCount);
	for(UINT i=0;i<pHeader->GlobalSequencesCount;i++)
	{
		m_GlobalSequences[i]=pGlobalSequences[i];
	}
	
	m_AnimationSequence.Resize(pHeader->AnimationsCount);
	for(UINT i=0;i<pHeader->AnimationsCount;i++)
	{
		
		m_AnimationSequence[i].AnimationID=pAnimationSequence[i].AnimationID;
		m_AnimationSequence[i].SubAnimationID=pAnimationSequence[i].SubAnimationID;
		m_AnimationSequence[i].Index=i;
		m_AnimationSequence[i].Length=pAnimationSequence[i].Length;
		m_AnimationSequence[i].MovingSpeed=pAnimationSequence[i].MovingSpeed;
		m_AnimationSequence[i].Flags=pAnimationSequence[i].Flags;
		m_AnimationSequence[i].Flags2=pAnimationSequence[i].Flags2;
		m_AnimationSequence[i].PlaybackSpeed=pAnimationSequence[i].PlaybackSpeed;
		m_AnimationSequence[i].BoundingBox.m_Min=BLZTranslationToD3D(pAnimationSequence[i].BoundingBox[0]);
		m_AnimationSequence[i].BoundingBox.m_Max=BLZTranslationToD3D(pAnimationSequence[i].BoundingBox[1]);
		m_AnimationSequence[i].BoundingBox.Rebuild();
		m_AnimationSequence[i].BoundingSphere.m_Center=0.0f;
		m_AnimationSequence[i].BoundingSphere.m_Radius=pAnimationSequence[i].Radius;
		CBLZWOWDatabase::BLZ_DB_ANIMATION_DATA_RECORD * pData=CBLZWOWDatabase::GetInstance()->
			FindAnimationData(m_AnimationSequence[i].AnimationID);
		if(pData)
		{
			m_AnimationSequence[i].AnimationName=pData->Name;
		}		
	}
	
	m_Bones.Resize(pHeader->BonesCount);
	for(UINT i=0;i<pHeader->BonesCount;i++)
	{			
		
		m_Bones[i].AnimationSeq=pBones[i].AnimationSeq;
		m_Bones[i].Flags=pBones[i].Flags;
		m_Bones[i].ChildCount=0;
		m_Bones[i].ParentBone=pBones[i].ParentBone;
		if(m_Bones[i].ParentBone>=0)
		{
			m_Bones[m_Bones[i].ParentBone].ChildCount++;
		}
		m_Bones[i].GeosetID=pBones[i].GeosetID;
		m_Bones[i].PivotPoint=BLZTranslationToD3D(pBones[i].PivotPoint);

		m_Bones[i].Translations.InterpolationType=pBones[i].Translation.InterpolationType;
		m_Bones[i].Translations.GlobalSequenceID=pBones[i].Translation.GlobalSequenceID;
		m_Bones[i].Translations.Animations.Resize(pBones[i].Translation.TimestampsCount);

		m_Bones[i].Rotations.InterpolationType=pBones[i].Rotation.InterpolationType;
		m_Bones[i].Rotations.GlobalSequenceID=pBones[i].Rotation.GlobalSequenceID;
		m_Bones[i].Rotations.Animations.Resize(pBones[i].Rotation.TimestampsCount);

		m_Bones[i].Scalings.InterpolationType=pBones[i].Scaling.InterpolationType;
		m_Bones[i].Scalings.GlobalSequenceID=pBones[i].Scaling.GlobalSequenceID;
		m_Bones[i].Scalings.Animations.Resize(pBones[i].Scaling.TimestampsCount);

		//if(m_Bones[i].Translations.GlobalSequenceID>=0)
		//{
		//	PrintSystemLog(0,"BoneInfo[%u].Translations.GlobalSequenceID=%u",i,m_Bones[i].Translations.GlobalSequenceID);
		//}
		//if(m_Bones[i].Rotations.GlobalSequenceID>=0)
		//{
		//	PrintSystemLog(0,"BoneInfo[%u].Rotations.GlobalSequenceID=%u",i,m_Bones[i].Rotations.GlobalSequenceID);
		//}
		//if(m_Bones[i].Scalings.GlobalSequenceID>=0)
		//{
		//	PrintSystemLog(0,"BoneInfo[%u].Scalings.GlobalSequenceID=%u",i,m_Bones[i].Scalings.GlobalSequenceID);
		//}

		for(UINT AniIndex=0;AniIndex<m_AnimationSequence.GetCount();AniIndex++)
		{
			if(m_AnimationSequence[AniIndex].Flags&BLZ_M2_ANIMATION_SEQUENCE_FLAG_HAVE_KEY_DATA)
			{	
				LoadBoneAnimation(pModelData,pModelData,m_Bones[i],pBones[i],AniIndex);
			}
		}

		
	}

	for(UINT i=0;i<m_AnimationSequence.GetCount();i++)
	{
		if((m_AnimationSequence[i].Flags&BLZ_M2_ANIMATION_SEQUENCE_FLAG_HAVE_KEY_DATA)==0)
		{
			LoadAnimationFromFile(pModelData,m_AnimationSequence[i].AnimationID,m_AnimationSequence[i].SubAnimationID,
				i,szModelPath,pBones);
		}
	}
	m_KeyBoneIndex.Resize(pHeader->KeyBoneLookupCount);
	for(UINT i=0;i<pHeader->KeyBoneLookupCount;i++)
	{
		m_KeyBoneIndex[i]=pKeyBoneLookup[i];
	}
		
	return true;
}

int CD3DWOWM2ModelResource::GetAnimationID(LPCTSTR AnimationName)
{
	for(UINT i=0;i<m_AnimationSequence.GetCount();i++)
	{
		if(_stricmp(m_AnimationSequence[i].AnimationName,AnimationName)==0)
		{
			return m_AnimationSequence[i].AnimationID;
		}
	}
	return -1;
}

CD3DWOWM2ModelResource::ANIMATION_SEQUENCE * CD3DWOWM2ModelResource::FindAnimationInfo(UINT AnimationID,UINT SubAnimationID)
{
	for(UINT i=0;i<m_AnimationSequence.GetCount();i++)
	{
		if(m_AnimationSequence[i].AnimationID==AnimationID&&m_AnimationSequence[i].SubAnimationID==SubAnimationID)
		{
			return &(m_AnimationSequence[i]);
		}
	}
	return NULL;
}

int CD3DWOWM2ModelResource::GetAnimationIndex(UINT AnimationID,UINT SubAnimationID)
{
	for(UINT i=0;i<m_AnimationSequence.GetCount();i++)
	{
		if(m_AnimationSequence[i].AnimationID==AnimationID&&m_AnimationSequence[i].SubAnimationID==SubAnimationID)
		{
			return i;
		}
	}
	return -1;
}

CD3DWOWM2ModelResource::ANIMATION_SEQUENCE * CD3DWOWM2ModelResource::GetAnimationInfo(UINT AnimationIndex)
{
	return m_AnimationSequence.GetObject(AnimationIndex);
}


UINT CD3DWOWM2ModelResource::GetAnimationCount()
{
	return m_AnimationSequence.GetCount();
}

CD3DWOWM2ModelResource::ANIMATION_SEQUENCE * CD3DWOWM2ModelResource::GetAnimationInfoByIndex(UINT AnimationIndex)
{
	if(AnimationIndex<m_AnimationSequence.GetCount())
	{
		return &(m_AnimationSequence[AnimationIndex]);
	}
	return NULL;
}


UINT CD3DWOWM2ModelResource::GetBoneCount()
{
	return m_Bones.GetCount();
}

UINT CD3DWOWM2ModelResource::GetSkinMeshBoneCount()
{
	return m_SkinMeshBoneCount;
}

CD3DWOWM2ModelResource::MODEL_BONE * CD3DWOWM2ModelResource::GetBoneInfo(UINT Index)
{
	if(Index<m_Bones.GetCount())
	{
		return &(m_Bones[Index]);
	}
	return NULL;
}

int CD3DWOWM2ModelResource::GetKeyBone(UINT Key)
{
	if(Key<m_KeyBoneIndex.GetCount())
	{
		return m_KeyBoneIndex[Key];
	}
	return -1;
}

UINT CD3DWOWM2ModelResource::GetKeyBoneCount()
{
	return m_Bones.GetCount();
}


bool CD3DWOWM2ModelResource::MakeAnimationBoneFrame(UINT AnimationIndex,UINT Time,BONE_MATRIX * pMatrixBuffer,UINT MatrixBufferSize,bool ResetMatrix)
{	
	FUNCTION_BEGIN;
	if(MatrixBufferSize<m_Bones.GetCount())
		return false;


	ANIMATION_SEQUENCE * pAniInfo=m_AnimationSequence.GetObject(AnimationIndex);

	if(pAniInfo==NULL)
		return false;

	for(UINT i=0;i<m_Bones.GetCount();i++)
	{

		bool IsAnimated=MakeOneAnimationBoneFrame(pAniInfo,Time,i,pMatrixBuffer+i,true);

		
		if(ResetMatrix&&(!IsAnimated))
		{
			pMatrixBuffer[i].Matrix.SetIdentity();
		}
		
		
	}
	
	FUNCTION_END;
	return true;
}

bool CD3DWOWM2ModelResource::MakeAnimationBoneFrameByTree(UINT AnimationIndex,UINT Time,UINT StartBone,BONE_MATRIX * pMatrixBuffer,UINT MatrixBufferSize,bool ResetMatrix)
{

	ANIMATION_SEQUENCE * pAniInfo=m_AnimationSequence.GetObject(AnimationIndex);

	if(pAniInfo==NULL)
		return false;

	return MakeAnimationBoneFrameByTree(pAniInfo,Time,StartBone,pMatrixBuffer,MatrixBufferSize,ResetMatrix);
}


bool CD3DWOWM2ModelResource::MakeAnimationBoneFrameByTree(ANIMATION_SEQUENCE * pAniInfo,UINT Time,UINT StartBone,BONE_MATRIX * pMatrixBuffer,UINT MatrixBufferSize,bool ResetMatrix)
{
	FUNCTION_BEGIN;
	if(StartBone>=m_Bones.GetCount()||MatrixBufferSize<GetBoneCount())
		return false;

	bool IsAnimated=MakeOneAnimationBoneFrame(pAniInfo,Time,StartBone,pMatrixBuffer+StartBone,false);


	if(ResetMatrix&&(!IsAnimated))
	{
		pMatrixBuffer[StartBone].Matrix.SetIdentity();
	}	

	for(UINT i=0;i<m_Bones.GetCount();i++)
	{
		if(m_Bones[i].ParentBone==StartBone)
		{
			MakeAnimationBoneFrameByTree(pAniInfo,Time,i,pMatrixBuffer,MatrixBufferSize,ResetMatrix);
		}
	}
	FUNCTION_END;
	return true;
}

bool CD3DWOWM2ModelResource::MakeColorAnimationFrame(UINT AniIndex,UINT Time,D3DCOLORVALUE& Color)
{
	FUNCTION_BEGIN;
	if(AniIndex<m_ColorAnimations.GetCount())
	{
		CD3DVector3 ColorRGB(1.0f,1.0f,1.0f);
		FLOAT Alpha=1.0f;

		if(m_ColorAnimations[AniIndex].ColorAnimations.Animations.GetCount())
		{
			UINT AniLength=0;
			if(m_ColorAnimations[AniIndex].ColorAnimations.GlobalSequenceID>=0)
			{
				AniLength=m_GlobalSequences[m_ColorAnimations[AniIndex].ColorAnimations.GlobalSequenceID];
			}
			GetInterpolationValue(Time,
				m_ColorAnimations[AniIndex].ColorAnimations.InterpolationType,
				AniLength,
				m_ColorAnimations[AniIndex].ColorAnimations.Animations[0],
				ColorRGB);
		}

		if(m_ColorAnimations[AniIndex].AlphaAnimations.Animations.GetCount())
		{
			UINT AniLength=0;
			if(m_ColorAnimations[AniIndex].AlphaAnimations.GlobalSequenceID>=0)
			{
				AniLength=m_GlobalSequences[m_ColorAnimations[AniIndex].AlphaAnimations.GlobalSequenceID];
			}
			GetInterpolationValue(Time,
				m_ColorAnimations[AniIndex].AlphaAnimations.InterpolationType,
				AniLength,
				m_ColorAnimations[AniIndex].AlphaAnimations.Animations[0],
				Alpha);
		}
		
		Color.r=ColorRGB.x;
		Color.g=ColorRGB.y;
		Color.b=ColorRGB.z;
		Color.a=Alpha;
		
		return true;
		
	}
	FUNCTION_END;
	return false;
}

bool CD3DWOWM2ModelResource::MakeTransparencyAnimationFrame(UINT AniIndex,UINT Time,FLOAT& Alpha)
{
	FUNCTION_BEGIN;
	if(AniIndex<m_TransparencyAnimations.GetCount())
	{
		if(m_TransparencyAnimations[AniIndex].AlphaAnimations.Animations.GetCount())
		{
			UINT AniLength=0;
			if(m_TransparencyAnimations[AniIndex].AlphaAnimations.GlobalSequenceID>=0)
			{
				AniLength=m_GlobalSequences[m_TransparencyAnimations[AniIndex].AlphaAnimations.GlobalSequenceID];
			}
			GetInterpolationValue(Time,
				m_TransparencyAnimations[AniIndex].AlphaAnimations.InterpolationType,
				AniLength,
				m_TransparencyAnimations[AniIndex].AlphaAnimations.Animations[0],
				Alpha);
			return true;
		}
	}
	FUNCTION_END;
	return false;
}


CD3DWOWM2ModelResource::MODEL_ATTACHMENT * CD3DWOWM2ModelResource::GetAttachment(UINT ID)
{
	for(UINT i=0;i<m_Attachments.GetCount();i++)
	{
		if(m_Attachments[i].ID==ID)
		{
			return &(m_Attachments[i]);
		}
	}
	return NULL;
}

UINT CD3DWOWM2ModelResource::GetAttachmentCount()
{
	return m_Attachments.GetCount();
}
CD3DWOWM2ModelResource::MODEL_ATTACHMENT * CD3DWOWM2ModelResource::GetAttachmentByIndex(UINT Index)
{
	if(Index<m_Attachments.GetCount())
	{
		return &(m_Attachments[Index]);
	}
	return NULL;
}

UINT CD3DWOWM2ModelResource::GetParticleEmitterCount()
{
	return m_ParticleEmitterInfos.GetCount();
}
CD3DWOWM2ModelResource::PARTICLE_EMITTER_INFO * CD3DWOWM2ModelResource::GetParticleEmitterInfo(UINT Index)
{	
	return m_ParticleEmitterInfos.GetObject(Index);	
}
bool CD3DWOWM2ModelResource::MakeParticleParam(UINT Index,UINT Time,PARTICLE_PARAM& Param)
{
	if(Index<m_ParticleEmitterInfos.GetCount())
	{
		UINT AniLength=0;
		if(m_ParticleEmitterInfos[Index].Speed.GlobalSequenceID>=0)
		{
			AniLength=m_GlobalSequences[m_ParticleEmitterInfos[Index].Speed.GlobalSequenceID];
		}
		GetInterpolationValue(Time,
			m_ParticleEmitterInfos[Index].Speed.InterpolationType,
			AniLength,
			m_ParticleEmitterInfos[Index].Speed.Animations[0],
			Param.Speed);

		AniLength=0;
		if(m_ParticleEmitterInfos[Index].SpeedVariation.GlobalSequenceID>=0)
		{
			AniLength=m_GlobalSequences[m_ParticleEmitterInfos[Index].SpeedVariation.GlobalSequenceID];
		}
		GetInterpolationValue(Time,
			m_ParticleEmitterInfos[Index].SpeedVariation.InterpolationType,
			AniLength,
			m_ParticleEmitterInfos[Index].SpeedVariation.Animations[0],
			Param.SpeedVariation);

		AniLength=0;
		if(m_ParticleEmitterInfos[Index].VerticalRange.GlobalSequenceID>=0)
		{
			AniLength=m_GlobalSequences[m_ParticleEmitterInfos[Index].VerticalRange.GlobalSequenceID];
		}
		GetInterpolationValue(Time,
			m_ParticleEmitterInfos[Index].VerticalRange.InterpolationType,
			AniLength,
			m_ParticleEmitterInfos[Index].VerticalRange.Animations[0],
			Param.VerticalRange);

		AniLength=0;
		if(m_ParticleEmitterInfos[Index].HorizontalRange.GlobalSequenceID>=0)
		{
			AniLength=m_GlobalSequences[m_ParticleEmitterInfos[Index].HorizontalRange.GlobalSequenceID];
		}
		GetInterpolationValue(Time,
			m_ParticleEmitterInfos[Index].HorizontalRange.InterpolationType,
			AniLength,
			m_ParticleEmitterInfos[Index].HorizontalRange.Animations[0],
			Param.HorizontalRange);

		AniLength=0;
		if(m_ParticleEmitterInfos[Index].Gravity.GlobalSequenceID>=0)
		{
			AniLength=m_GlobalSequences[m_ParticleEmitterInfos[Index].Gravity.GlobalSequenceID];
		}
		GetInterpolationValue(Time,
			m_ParticleEmitterInfos[Index].Gravity.InterpolationType,
			AniLength,
			m_ParticleEmitterInfos[Index].Gravity.Animations[0],
			Param.Gravity);

		AniLength=0;
		if(m_ParticleEmitterInfos[Index].LifeSpan.GlobalSequenceID>=0)
		{
			AniLength=m_GlobalSequences[m_ParticleEmitterInfos[Index].LifeSpan.GlobalSequenceID];
		}
		GetInterpolationValue(Time,
			m_ParticleEmitterInfos[Index].LifeSpan.InterpolationType,
			AniLength,
			m_ParticleEmitterInfos[Index].LifeSpan.Animations[0],
			Param.LifeSpan);

		AniLength=0;
		if(m_ParticleEmitterInfos[Index].EmissionRate.GlobalSequenceID>=0)
		{
			AniLength=m_GlobalSequences[m_ParticleEmitterInfos[Index].EmissionRate.GlobalSequenceID];
		}
		GetInterpolationValue(Time,
			m_ParticleEmitterInfos[Index].EmissionRate.InterpolationType,
			AniLength,
			m_ParticleEmitterInfos[Index].EmissionRate.Animations[0],
			Param.EmissionRate);

		AniLength=0;
		if(m_ParticleEmitterInfos[Index].EmissionAreaLength.GlobalSequenceID>=0)
		{
			AniLength=m_GlobalSequences[m_ParticleEmitterInfos[Index].EmissionAreaLength.GlobalSequenceID];
		}
		GetInterpolationValue(Time,
			m_ParticleEmitterInfos[Index].EmissionAreaLength.InterpolationType,
			AniLength,
			m_ParticleEmitterInfos[Index].EmissionAreaLength.Animations[0],
			Param.EmissionAreaLength);

		AniLength=0;
		if(m_ParticleEmitterInfos[Index].EmissionAreaWidth.GlobalSequenceID>=0)
		{
			AniLength=m_GlobalSequences[m_ParticleEmitterInfos[Index].EmissionAreaWidth.GlobalSequenceID];
		}
		GetInterpolationValue(Time,
			m_ParticleEmitterInfos[Index].EmissionAreaWidth.InterpolationType,
			AniLength,
			m_ParticleEmitterInfos[Index].EmissionAreaWidth.Animations[0],
			Param.EmissionAreaWidth);

		AniLength=0;
		if(m_ParticleEmitterInfos[Index].Decelerate.GlobalSequenceID>=0)
		{
			AniLength=m_GlobalSequences[m_ParticleEmitterInfos[Index].Decelerate.GlobalSequenceID];
		}
		GetInterpolationValue(Time,
			m_ParticleEmitterInfos[Index].Decelerate.InterpolationType,
			AniLength,
			m_ParticleEmitterInfos[Index].Decelerate.Animations[0],
			Param.Decelerate);
		return true;
	}
	return false;
}

UINT CD3DWOWM2ModelResource::GetRibbonEmitterCount()
{
	return m_RibbonEmitterInfos.GetCount();
}

CD3DWOWM2ModelResource::RIBBON_EMITTER_INFO * CD3DWOWM2ModelResource::GetRibbonEmitterInfo(UINT Index)
{
	return m_RibbonEmitterInfos.GetObject(Index);
}

bool CD3DWOWM2ModelResource::MakeRibbonParam(UINT Index,UINT Time,RIBBON_PARAM& Param)
{
	if(Index<m_RibbonEmitterInfos.GetCount())
	{
		UINT AniLength=0;
		if(m_RibbonEmitterInfos[Index].Color.GlobalSequenceID>=0)
		{
			AniLength=m_GlobalSequences[m_RibbonEmitterInfos[Index].Color.GlobalSequenceID];
		}
		CD3DVector3 Color=1.0f;
		GetInterpolationValue(Time,
			m_RibbonEmitterInfos[Index].Color.InterpolationType,
			AniLength,
			m_RibbonEmitterInfos[Index].Color.Animations[0],
			Color);

		AniLength=0;
		if(m_RibbonEmitterInfos[Index].Opacity.GlobalSequenceID>=0)
		{
			AniLength=m_GlobalSequences[m_RibbonEmitterInfos[Index].Opacity.GlobalSequenceID];
		}
		FLOAT Alpha=1.0f;
		GetInterpolationValue(Time,
			m_RibbonEmitterInfos[Index].Opacity.InterpolationType,
			AniLength,
			m_RibbonEmitterInfos[Index].Opacity.Animations[0],
			Alpha);
		Param.Color=D3DCOLOR_COLORVALUE(Color.x,Color.y,Color.z,Alpha);

		AniLength=0;
		if(m_RibbonEmitterInfos[Index].Above.GlobalSequenceID>=0)
		{
			AniLength=m_GlobalSequences[m_RibbonEmitterInfos[Index].Above.GlobalSequenceID];
		}
		GetInterpolationValue(Time,
			m_RibbonEmitterInfos[Index].Above.InterpolationType,
			AniLength,
			m_RibbonEmitterInfos[Index].Above.Animations[0],
			Param.Above);

		AniLength=0;
		if(m_RibbonEmitterInfos[Index].Below.GlobalSequenceID>=0)
		{
			AniLength=m_GlobalSequences[m_RibbonEmitterInfos[Index].Below.GlobalSequenceID];
		}
		GetInterpolationValue(Time,
			m_RibbonEmitterInfos[Index].Below.InterpolationType,
			AniLength,
			m_RibbonEmitterInfos[Index].Below.Animations[0],
			Param.Below);
		return true;
	}
	return false;
}

bool CD3DWOWM2ModelResource::MakeTextureUVAniFrame(UINT Index,UINT Time,CD3DMatrix& Frame)
{
	FUNCTION_BEGIN;
	if(Index<m_TextureUVAnimations.GetCount())
	{
		Frame.SetIdentity();

		GetInterpolationTransformGlobal(Time,Frame,
			m_TextureUVAnimations[Index].Translations,
			m_TextureUVAnimations[Index].Rotations,
			m_TextureUVAnimations[Index].Scalings,
			true);		
		return true;
	}
	FUNCTION_END;
	return false;
}

void CD3DWOWM2ModelResource::SetBoneAniCache(UINT CacheSize,UINT CacheFreq)
{
	m_BoneAniCache.Clear();	
	m_BoneAniCacheSize=CacheSize;
	m_BoneAniCacheFreq=CacheFreq;
	BuildGlobalBoneAniCache();	
}

UINT CD3DWOWM2ModelResource::CaculateDataSize()
{
	UINT DataSize=0;
	for(UINT i=0;i<m_SubMeshList.GetCount();i++)
	{
		DataSize+=m_SubMeshList[i]->GetVertexFormat().VertexSize*m_SubMeshList[i]->GetVertexCount();
		DataSize+=m_SubMeshList[i]->GetVertexFormat().IndexSize*m_SubMeshList[i]->GetIndexCount();
	}
	for(UINT i=0;i<m_AnimationSequence.GetCount();i++)
	{
		DataSize+=sizeof(ANIMATION_SEQUENCE)+m_AnimationSequence[i].AnimationName.GetBufferSize();
	}
	for(UINT i=0;i<m_ColorAnimations.GetCount();i++)
	{
		DataSize+=CaculateAnimationBlockSize<FLOAT>(m_ColorAnimations[i].AlphaAnimations);
		DataSize+=CaculateAnimationBlockSize<CD3DVector3>(m_ColorAnimations[i].ColorAnimations);		
	}
	for(UINT i=0;i<m_TransparencyAnimations.GetCount();i++)
	{
		DataSize+=CaculateAnimationBlockSize<FLOAT>(m_TransparencyAnimations[i].AlphaAnimations);
	}
	for(UINT i=0;i<m_TextureUVAnimations.GetCount();i++)
	{
		DataSize+=CaculateAnimationBlockSize<CD3DVector3>(m_TextureUVAnimations[i].Scalings);
		DataSize+=CaculateAnimationBlockSize<CD3DQuaternion>(m_TextureUVAnimations[i].Rotations);
		DataSize+=CaculateAnimationBlockSize<CD3DVector3>(m_TextureUVAnimations[i].Translations);
	}

	for(UINT i=0;i<m_Bones.GetCount();i++)
	{
		DataSize+=sizeof(MODEL_BONE);
		DataSize+=CaculateAnimationBlockSize<CD3DVector3>(m_Bones[i].Scalings);
		DataSize+=CaculateAnimationBlockSize<CD3DQuaternion>(m_Bones[i].Rotations);
		DataSize+=CaculateAnimationBlockSize<CD3DVector3>(m_Bones[i].Translations);
	}
	DataSize+=m_KeyBoneIndex.GetCount()*sizeof(short);
	DataSize+=m_GlobalSequences.GetCount()*sizeof(UINT);
	for(UINT i=0;i<m_Attachments.GetCount();i++)
	{
		DataSize+=sizeof(MODEL_ATTACHMENT)+m_Attachments[i].Name.GetBufferSize();
	}
	return DataSize;
}

bool CD3DWOWM2ModelResource::LoadAnimationFromFile(BYTE * pData,UINT AnimationID,UINT SubAnimationID,UINT AniIndex,LPCTSTR szModelPath,M2_MODEL_BONE * pOrgBoneInfo)
{
	CEasyString AniFileName;
	AniFileName.Format("%s%04d-%02d.anim",szModelPath,AnimationID,SubAnimationID);

	IFileAccessor * pFile;


	CEasyBuffer AniData;

	pFile=CD3DWOWM2Model::CreateFileAccessor();
	if(pFile==NULL)
		return false;
	if(!pFile->Open(AniFileName,IFileAccessor::modeRead))
	{
		PrintSystemLog(0,"打开外部动画文件%s失败",(LPCTSTR)AniFileName);
		pFile->Release();
		return false;	
	}
	UINT FileSize=(UINT)pFile->GetSize();	
	AniData.Create(FileSize);	
	pFile->Read(AniData.GetBuffer(),FileSize);
	AniData.SetUsedSize(FileSize);
	pFile->Release();

	for(UINT i=0;i<m_Bones.GetCount();i++)
	{
		LoadBoneAnimation(pData,(BYTE *)AniData.GetBuffer(),m_Bones[i],pOrgBoneInfo[i],AniIndex);
	}

	PrintSystemLog(0,"装载了外部动画文件%s",(LPCTSTR)AniFileName);
	return true;
}

bool CD3DWOWM2ModelResource::LoadColorAnimation(BYTE * pModelData)
{
	BLZ_M2_HEADER * pHeader=(BLZ_M2_HEADER *)pModelData;

	M2_COLOR_ANIMATION * pColorAni=(M2_COLOR_ANIMATION *)(pModelData+pHeader->ColorsOffset);

	m_ColorAnimations.Resize(pHeader->ColorsCount);
	for(UINT i=0;i<pHeader->ColorsCount;i++)
	{
		LoadAniBlockVector3(pModelData,pColorAni[i].ColorAni,m_ColorAnimations[i].ColorAnimations);
		LoadAniBlockAlpha(pModelData,pColorAni[i].AlphaAni,m_ColorAnimations[i].AlphaAnimations);
		//{
		//	m_ColorAnimations[i].ColorAnimations.InterpolationType=pColorAni[i].ColorAni.InterpolationType;
		//	m_ColorAnimations[i].ColorAnimations.GlobalSequenceID=pColorAni[i].ColorAni.GlobalSequenceID;

		//	M2_MODEL_ANIMATION_PAIR * pTimeStampPair=(M2_MODEL_ANIMATION_PAIR *)(pModelData+pColorAni[i].ColorAni.TimestampsOffset);
		//	M2_MODEL_ANIMATION_PAIR * pKeyPair=(M2_MODEL_ANIMATION_PAIR *)(pModelData+pColorAni[i].ColorAni.KeysOffset);

		//	m_ColorAnimations[i].ColorAnimations.Animations.Resize(pColorAni[i].ColorAni.TimestampsCount);

		//	for(UINT j=0;j<pColorAni[i].ColorAni.TimestampsCount;j++)
		//	{
		//		if(pTimeStampPair[j].Count)
		//		{					
		//			LoadColorFrames(pModelData,pTimeStampPair[j].Count,pTimeStampPair[j].Offset,
		//				pKeyPair[j].Count,pKeyPair[j].Offset,m_ColorAnimations[i].ColorAnimations.Animations[j]);						
		//		}
		//		if(pTimeStampPair[j].Count>1)
		//		{
		//			PrintSystemLog(0,"有Color动画");
		//		}
		//	}
		//}

	

		

		//{
		//	m_ColorAnimations[i].AlphaAnimations.InterpolationType=pColorAni[i].AlphaAni.InterpolationType;
		//	m_ColorAnimations[i].AlphaAnimations.GlobalSequenceID=pColorAni[i].AlphaAni.GlobalSequenceID;

		//	M2_MODEL_ANIMATION_PAIR * pTimeStampPair=(M2_MODEL_ANIMATION_PAIR *)(pModelData+pColorAni[i].AlphaAni.TimestampsOffset);
		//	M2_MODEL_ANIMATION_PAIR * pKeyPair=(M2_MODEL_ANIMATION_PAIR *)(pModelData+pColorAni[i].AlphaAni.KeysOffset);

		//	m_ColorAnimations[i].AlphaAnimations.Animations.Resize(pColorAni[i].AlphaAni.TimestampsCount);

		//	for(UINT j=0;j<pColorAni[i].AlphaAni.TimestampsCount;j++)
		//	{
		//		if(pTimeStampPair[j].Count)
		//		{					
		//			LoadAlphaFrames(pModelData,pTimeStampPair[j].Count,pTimeStampPair[j].Offset,
		//				pKeyPair[j].Count,pKeyPair[j].Offset,m_ColorAnimations[i].AlphaAnimations.Animations[j]);						
		//		}
		//		if(pTimeStampPair[j].Count>1)
		//		{
		//			PrintSystemLog(0,"有Color动画");
		//		}
		//	}
		//}
	}

	return true;

}

bool CD3DWOWM2ModelResource::LoadTransparencyAnimation(BYTE * pModelData)
{
	BLZ_M2_HEADER * pHeader=(BLZ_M2_HEADER *)pModelData;

	M2_TRANSPARENCY_ANIMATION * pTransparencyAni=(M2_TRANSPARENCY_ANIMATION *)(pModelData+pHeader->TransparencyOffset);

	m_TransparencyAnimations.Resize(pHeader->TransparencyCount);
	for(UINT i=0;i<pHeader->TransparencyCount;i++)
	{
		LoadAniBlockAlpha(pModelData,pTransparencyAni[i].AlphaAni,m_TransparencyAnimations[i].AlphaAnimations);
		
		//{
		//	m_TransparencyAnimations[i].AlphaAnimations.InterpolationType=pTransparencyAni[i].AlphaAni.InterpolationType;
		//	m_TransparencyAnimations[i].AlphaAnimations.GlobalSequenceID=pTransparencyAni[i].AlphaAni.GlobalSequenceID;

		//	M2_MODEL_ANIMATION_PAIR * pTimeStampPair=(M2_MODEL_ANIMATION_PAIR *)(pModelData+pTransparencyAni[i].AlphaAni.TimestampsOffset);
		//	M2_MODEL_ANIMATION_PAIR * pKeyPair=(M2_MODEL_ANIMATION_PAIR *)(pModelData+pTransparencyAni[i].AlphaAni.KeysOffset);

		//	m_TransparencyAnimations[i].AlphaAnimations.Animations.Resize(pTransparencyAni[i].AlphaAni.TimestampsCount);

		//	for(UINT j=0;j<pTransparencyAni[i].AlphaAni.TimestampsCount;j++)
		//	{
		//		if(pTimeStampPair[j].Count)
		//		{					
		//			LoadAlphaFrames(pModelData,pTimeStampPair[j].Count,pTimeStampPair[j].Offset,
		//				pKeyPair[j].Count,pKeyPair[j].Offset,m_TransparencyAnimations[i].AlphaAnimations.Animations[j]);						
		//		}
		//		if(pTimeStampPair[j].Count>1)
		//		{
		//			PrintSystemLog(0,"有Transparency动画");
		//		}
		//	}
		//}
	}

	return true;
}

bool CD3DWOWM2ModelResource::LoadRibbonEmitters(BYTE * pModelData)
{
	BLZ_M2_HEADER * pHeader=(BLZ_M2_HEADER *)pModelData;

	//PrintSystemLog(0,"一共有%u个条带生成器",
	//	pHeader->RibbonEmittersCount);

	M2_RIBBON_EMITTER * pRibbonEmitters=(M2_RIBBON_EMITTER *)(pModelData+pHeader->RibbonEmittersOffset);

	WORD * pTexturesLookup=(WORD *)(pModelData+pHeader->TexLookupOffset);

	M2_MODEL_TEXTURE * pTextures=(M2_MODEL_TEXTURE *)(pModelData+pHeader->TexturesOffset);

	M2_MODEL_RENDER_FLAG * pRenderFlags=(M2_MODEL_RENDER_FLAG *)(pModelData+pHeader->RenderFlagsOffset);

	m_RibbonEmitterInfos.Resize(pHeader->RibbonEmittersCount);
	for(UINT i=0;i<pHeader->RibbonEmittersCount;i++)
	{
		m_RibbonEmitterInfos[i].BoneID=pRibbonEmitters[i].BoneID;
		m_RibbonEmitterInfos[i].Position=BLZTranslationToD3D(pRibbonEmitters[i].Position);
		m_RibbonEmitterInfos[i].Resolution=pRibbonEmitters[i].Resolution;
		m_RibbonEmitterInfos[i].Length=pRibbonEmitters[i].Length;
		m_RibbonEmitterInfos[i].Emissionangle=pRibbonEmitters[i].Emissionangle;

		UINT * pRibbonTextures=(UINT *)(pModelData+pRibbonEmitters[i].TextureOffset);
		m_RibbonEmitterInfos[i].Textures.Resize(pRibbonEmitters[i].TextureCount);
		for(UINT j=0;j<pRibbonEmitters[i].TextureCount;j++)
		{
			LPCTSTR szTextureName=(char *)pModelData+pTextures[pRibbonTextures[j]].FileNameOffset;
			CD3DTexture * pTexture=m_pManager->GetDevice()->GetTextureManager()->LoadTexture(szTextureName);
			m_RibbonEmitterInfos[i].Textures[j]=pTexture;
		}
		UINT * pRibbonBlends=(UINT *)(pModelData+pRibbonEmitters[i].BlendRefOffset);
		m_RibbonEmitterInfos[i].pFX=BuildRibbonFX(pRenderFlags[pRibbonBlends[0]].Flags,
			pRenderFlags[pRibbonBlends[0]].BlendingMode);
		m_RibbonEmitterInfos[i].IsTransparent=(pRenderFlags[pRibbonBlends[0]].BlendingMode!=BLZ_M2_BLENDING_MODE_OPAQUE&&
			pRenderFlags[pRibbonBlends[0]].BlendingMode!=BLZ_M2_BLENDING_MODE_ALPHA_TEST);		

		LoadAniBlockVector3(pModelData,pRibbonEmitters[i].Color,m_RibbonEmitterInfos[i].Color);
		LoadAniBlockAlpha(pModelData,pRibbonEmitters[i].Opacity,m_RibbonEmitterInfos[i].Opacity);
		LoadAniBlockFloat(pModelData,pRibbonEmitters[i].Above,m_RibbonEmitterInfos[i].Above);
		LoadAniBlockFloat(pModelData,pRibbonEmitters[i].Below,m_RibbonEmitterInfos[i].Below);


	}

	return true;
}
bool CD3DWOWM2ModelResource::LoadParticleEmitters(BYTE * pModelData)
{
	BLZ_M2_HEADER * pHeader=(BLZ_M2_HEADER *)pModelData;

	//PrintSystemLog(0,"一共有%u个粒子生成器",
	//	pHeader->ParticleEmittersCount);

	M2_PARTICLE_EMITTER * pParticleEmitters=(M2_PARTICLE_EMITTER *)(pModelData+pHeader->ParticleEmittersOffset);

	WORD * pTexturesLookup=(WORD *)(pModelData+pHeader->TexLookupOffset);

	M2_MODEL_TEXTURE * pTextures=(M2_MODEL_TEXTURE *)(pModelData+pHeader->TexturesOffset);

	m_ParticleEmitterInfos.Resize(pHeader->ParticleEmittersCount);
	for(UINT i=0;i<pHeader->ParticleEmittersCount;i++)
	{
		//PrintSystemLog(0,"Particle%u,Flag=0x%X",i,pParticleEmitters[i].Flags);
		m_ParticleEmitterInfos[i].Flags=pParticleEmitters[i].Flags;
		m_ParticleEmitterInfos[i].Position=BLZTranslationToD3D(pParticleEmitters[i].Position);
		m_ParticleEmitterInfos[i].Bone=pParticleEmitters[i].Bone;
		m_ParticleEmitterInfos[i].BlendingType=pParticleEmitters[i].BlendingType;
		m_ParticleEmitterInfos[i].EmitterType=pParticleEmitters[i].EmitterType;		
		m_ParticleEmitterInfos[i].ParticleColorRef=pParticleEmitters[i].ParticleColorRef;
		m_ParticleEmitterInfos[i].ParticleType=pParticleEmitters[i].ParticleType;
		m_ParticleEmitterInfos[i].HeadOrTail=pParticleEmitters[i].HeadOrTail;
		m_ParticleEmitterInfos[i].TextureTileRotation=pParticleEmitters[i].TextureTileRotation;
		m_ParticleEmitterInfos[i].TextureRows=pParticleEmitters[i].TextureRows;
		m_ParticleEmitterInfos[i].TextureCols=pParticleEmitters[i].TextureCols;
		m_ParticleEmitterInfos[i].Scale=pParticleEmitters[i].Scale;
		m_ParticleEmitterInfos[i].Slowdown=pParticleEmitters[i].Slowdown;
		m_ParticleEmitterInfos[i].Rotation=pParticleEmitters[i].Rotation;
		m_ParticleEmitterInfos[i].Trans=BLZTranslationToD3D(pParticleEmitters[i].Trans);

		if(pTextures[pParticleEmitters[i].Texture].Type==0)
		{
			LPCTSTR szTextureName=(char *)pModelData+pTextures[pParticleEmitters[i].Texture].FileNameOffset;
			m_ParticleEmitterInfos[i].pTexture=m_pManager->GetDevice()->GetTextureManager()->LoadTexture(szTextureName);
		}		
		m_ParticleEmitterInfos[i].pFX=BuildParticleFX(m_ParticleEmitterInfos[i].BlendingType);
		
		LoadAniBlockFloat(pModelData,pParticleEmitters[i].EmissionSpeed,m_ParticleEmitterInfos[i].Speed);
		LoadAniBlockFloat(pModelData,pParticleEmitters[i].SpeedVariation,m_ParticleEmitterInfos[i].SpeedVariation);
		LoadAniBlockFloat(pModelData,pParticleEmitters[i].VerticalRange,m_ParticleEmitterInfos[i].VerticalRange);
		LoadAniBlockFloat(pModelData,pParticleEmitters[i].HorizontalRange,m_ParticleEmitterInfos[i].HorizontalRange);
		LoadAniBlockFloat(pModelData,pParticleEmitters[i].Gravity,m_ParticleEmitterInfos[i].Gravity);
		LoadAniBlockFloat(pModelData,pParticleEmitters[i].LifeSpan,m_ParticleEmitterInfos[i].LifeSpan);
		LoadAniBlockFloat(pModelData,pParticleEmitters[i].EmissionRate,m_ParticleEmitterInfos[i].EmissionRate);
		LoadAniBlockFloat(pModelData,pParticleEmitters[i].EmissionAreaLength,m_ParticleEmitterInfos[i].EmissionAreaLength);
		LoadAniBlockFloat(pModelData,pParticleEmitters[i].EmissionAreaWidth,m_ParticleEmitterInfos[i].EmissionAreaWidth);
		LoadAniBlockFloat(pModelData,pParticleEmitters[i].Decelerate,m_ParticleEmitterInfos[i].Decelerate);
		//LoadAniBlockBool(pModelData,pParticleEmitters[i].EnabledIn,m_ParticleEmitterInfos[i].EnabledIn);

		LoadFakeAniBlockColor(pModelData,pParticleEmitters[i].ParticleColor,m_ParticleEmitterInfos[i].ParticleColor);
		LoadFakeAniBlockAlpha(pModelData,pParticleEmitters[i].ParticleOpacity,m_ParticleEmitterInfos[i].ParticleOpacity);
		LoadFakeAniBlockSize(pModelData,pParticleEmitters[i].ParticleSizes,m_ParticleEmitterInfos[i].ParticleSizes);
		LoadFakeAniBlockShort(pModelData,pParticleEmitters[i].Intensity,m_ParticleEmitterInfos[i].Intensity);
		//LoadFakeAniBlockShort(pModelData,pParticleEmitters[i].UnknownReferences1,m_ParticleEmitterInfos[i].UnknowBlock);

		//m_ParticleEmitterInfos[i].ParticleColor;		
		//m_ParticleEmitterInfos[i].ParticleOpacity;	
		//m_ParticleEmitterInfos[i].ParticleSizes;		
		//m_ParticleEmitterInfos[i].Intensity;		
		

	}

	return true;
}

bool CD3DWOWM2ModelResource::LoadTextureUVAnimation(BYTE * pModelData)
{
	BLZ_M2_HEADER * pHeader=(BLZ_M2_HEADER *)pModelData;

	//PrintSystemLog(0,"一共有%u个纹理动画",
	//	pHeader->TexAnimsCount);

	M2_TEXTURE_UV_ANIMATION * pTexAnims=(M2_TEXTURE_UV_ANIMATION *)(pModelData+pHeader->TexAnimsOffset);

	m_TextureUVAnimations.Resize(pHeader->TexAnimsCount);
	for(UINT i=0;i<pHeader->TexAnimsCount;i++)
	{
		LoadAniBlockVector3(pModelData,pTexAnims[i].Translation,m_TextureUVAnimations[i].Translations);
		LoadAniBlockQuaternion(pModelData,pTexAnims[i].Rotation,m_TextureUVAnimations[i].Rotations);
		LoadAniBlockVector3(pModelData,pTexAnims[i].Scaling,m_TextureUVAnimations[i].Scalings);
	}

	return true;
}

void CD3DWOWM2ModelResource::LoadBoneAnimation(BYTE * pData,BYTE * pAniData,MODEL_BONE& BoneInfo,M2_MODEL_BONE& OrgBoneInfo,UINT AniIndex)
{
	{
		M2_MODEL_ANIMATION_PAIR * pTimeStampPair=(M2_MODEL_ANIMATION_PAIR *)(pData+OrgBoneInfo.Translation.TimestampsOffset);
		M2_MODEL_ANIMATION_PAIR * pKeyPair=(M2_MODEL_ANIMATION_PAIR *)(pData+OrgBoneInfo.Translation.KeysOffset);
	

		if(AniIndex<OrgBoneInfo.Translation.TimestampsCount)
		{		

			if(pTimeStampPair[AniIndex].Count)
			{					
				LoadTranslationFrames(pAniData,pTimeStampPair[AniIndex].Count,pTimeStampPair[AniIndex].Offset,
					pKeyPair[AniIndex].Count,pKeyPair[AniIndex].Offset,BoneInfo.Translations.Animations[AniIndex]);						
			}				
		}
	}	


	{
		M2_MODEL_ANIMATION_PAIR * pTimeStampPair=(M2_MODEL_ANIMATION_PAIR *)(pData+OrgBoneInfo.Rotation.TimestampsOffset);
		M2_MODEL_ANIMATION_PAIR * pKeyPair=(M2_MODEL_ANIMATION_PAIR *)(pData+OrgBoneInfo.Rotation.KeysOffset);
	

		if(AniIndex<OrgBoneInfo.Rotation.TimestampsCount)
		{				

			if(pTimeStampPair[AniIndex].Count)
			{
				LoadRotationFrames(pAniData,pTimeStampPair[AniIndex].Count,pTimeStampPair[AniIndex].Offset,
					pKeyPair[AniIndex].Count,pKeyPair[AniIndex].Offset,BoneInfo.Rotations.Animations[AniIndex]);
			}				
		}
	}	

	{
		M2_MODEL_ANIMATION_PAIR * pTimeStampPair=(M2_MODEL_ANIMATION_PAIR *)(pData+OrgBoneInfo.Scaling.TimestampsOffset);
		M2_MODEL_ANIMATION_PAIR * pKeyPair=(M2_MODEL_ANIMATION_PAIR *)(pData+OrgBoneInfo.Scaling.KeysOffset);


		if(AniIndex<OrgBoneInfo.Scaling.TimestampsCount)
		{			

			if(pTimeStampPair[AniIndex].Count)
			{
				LoadScalingFrames(pAniData,pTimeStampPair[AniIndex].Count,pTimeStampPair[AniIndex].Offset,
					pKeyPair[AniIndex].Count,pKeyPair[AniIndex].Offset,BoneInfo.Scalings.Animations[AniIndex]);
			}				
		}
	}
}

void CD3DWOWM2ModelResource::LoadTranslationFrames(BYTE * pData,UINT TimeStampCount,UINT TimeStampOffset,UINT KeyCount,UINT KeyOffset,ANIMATION_FRAME<CD3DVector3>& AniFrame)
{
	UINT32 * pTimeStamps=(UINT *)(pData+TimeStampOffset);
	AniFrame.TimeStamps.Resize(TimeStampCount);
	for(UINT k=0;k<TimeStampCount;k++)
	{				
		AniFrame.TimeStamps[k]=pTimeStamps[k];
	}
	CD3DVector3 * pKeys=(CD3DVector3 *)(pData+KeyOffset);
	AniFrame.Keys.Resize(KeyCount);
	for(UINT k=0;k<KeyCount;k++)
	{
		AniFrame.Keys[k]=BLZTranslationToD3D(pKeys[k]);
	}
}
void CD3DWOWM2ModelResource::LoadRotationFrames(BYTE * pData,UINT TimeStampCount,UINT TimeStampOffset,UINT KeyCount,UINT KeyOffset,ANIMATION_FRAME<CD3DQuaternion>& AniFrame)
{
	UINT32 * pTimeStamps=(UINT *)(pData+TimeStampOffset);	
	AniFrame.TimeStamps.Resize(TimeStampCount);
	for(UINT k=0;k<TimeStampCount;k++)
	{				
		AniFrame.TimeStamps[k]=pTimeStamps[k];
	}
	BLZ_SHORT_QUATERNION * pRotations=(BLZ_SHORT_QUATERNION *)(pData+KeyOffset);
	AniFrame.Keys.Resize(KeyCount);
	for(UINT k=0;k<KeyCount;k++)
	{
		CD3DQuaternion Rotation=BLZRotationToD3D(ShortQuaternionToLongQuaternion(pRotations[k]));
		Rotation.Normalize();
		AniFrame.Keys[k]=Rotation;
	}
}
void CD3DWOWM2ModelResource::LoadScalingFrames(BYTE * pData,UINT TimeStampCount,UINT TimeStampOffset,UINT KeyCount,UINT KeyOffset,ANIMATION_FRAME<CD3DVector3>& AniFrame)
{
	UINT32 * pTimeStamps=(UINT *)(pData+TimeStampOffset);
	AniFrame.TimeStamps.Resize(TimeStampCount);
	for(UINT k=0;k<TimeStampCount;k++)
	{				
		AniFrame.TimeStamps[k]=pTimeStamps[k];								
	}
	CD3DVector3 * pKeys=(CD3DVector3 *)(pData+KeyOffset);
	AniFrame.Keys.Resize(KeyCount);
	for(UINT k=0;k<KeyCount;k++)
	{
		AniFrame.Keys[k]=BLZScalingToD3D(pKeys[k]);
	}
}




void CD3DWOWM2ModelResource::LoadAniBlockFloat(BYTE * pData,M2_MODEL_ANIMATION_BLOCK& AniBlockInfo,ANIMATION_BLOCK<FLOAT>& AniBlock)
{
	AniBlock.InterpolationType=AniBlockInfo.InterpolationType;
	AniBlock.GlobalSequenceID=AniBlockInfo.GlobalSequenceID;

	M2_MODEL_ANIMATION_PAIR * pTimeStampPair=(M2_MODEL_ANIMATION_PAIR *)(pData+AniBlockInfo.TimestampsOffset);
	M2_MODEL_ANIMATION_PAIR * pKeyPair=(M2_MODEL_ANIMATION_PAIR *)(pData+AniBlockInfo.KeysOffset);

	AniBlock.Animations.Resize(AniBlockInfo.TimestampsCount);

	for(UINT j=0;j<AniBlockInfo.TimestampsCount;j++)
	{
		if(pTimeStampPair[j].Count)
		{			
			UINT32 * pTimeStamps=(UINT *)(pData+pTimeStampPair[j].Offset);
			AniBlock.Animations[j].TimeStamps.Resize(pTimeStampPair[j].Count);
			for(UINT k=0;k<pTimeStampPair[j].Count;k++)
			{				
				AniBlock.Animations[j].TimeStamps[k]=pTimeStamps[k];								
			}
			float * pKeys=(float *)(pData+pKeyPair[j].Offset);
			AniBlock.Animations[j].Keys.Resize(pKeyPair[j].Count);
			for(UINT k=0;k<pKeyPair[j].Count;k++)
			{
				AniBlock.Animations[j].Keys[k]=pKeys[k];
			}
		}
		
	}
}

void CD3DWOWM2ModelResource::LoadAniBlockVector3(BYTE * pData,M2_MODEL_ANIMATION_BLOCK& AniBlockInfo,ANIMATION_BLOCK<CD3DVector3>& AniBlock)
{
	AniBlock.InterpolationType=AniBlockInfo.InterpolationType;
	AniBlock.GlobalSequenceID=AniBlockInfo.GlobalSequenceID;

	M2_MODEL_ANIMATION_PAIR * pTimeStampPair=(M2_MODEL_ANIMATION_PAIR *)(pData+AniBlockInfo.TimestampsOffset);
	M2_MODEL_ANIMATION_PAIR * pKeyPair=(M2_MODEL_ANIMATION_PAIR *)(pData+AniBlockInfo.KeysOffset);

	AniBlock.Animations.Resize(AniBlockInfo.TimestampsCount);

	for(UINT j=0;j<AniBlockInfo.TimestampsCount;j++)
	{
		if(pTimeStampPair[j].Count)
		{					
			UINT32 * pTimeStamps=(UINT *)(pData+pTimeStampPair[j].Offset);		
			AniBlock.Animations[j].TimeStamps.Resize(pTimeStampPair[j].Count);
			for(UINT k=0;k<pTimeStampPair[j].Count;k++)
			{				
				AniBlock.Animations[j].TimeStamps[k]=(pTimeStamps[k]);								
			}
			CD3DVector3 * pKeys=(CD3DVector3 *)(pData+pKeyPair[j].Offset);
			AniBlock.Animations[j].Keys.Resize(pKeyPair[j].Count);
			for(UINT k=0;k<pKeyPair[j].Count;k++)
			{
				AniBlock.Animations[j].Keys[k]=(pKeys[k]);
			}
		}		
	}
}

void CD3DWOWM2ModelResource::LoadAniBlockQuaternion(BYTE * pData,M2_MODEL_ANIMATION_BLOCK& AniBlockInfo,ANIMATION_BLOCK<CD3DQuaternion>& AniBlock)
{
	AniBlock.InterpolationType=AniBlockInfo.InterpolationType;
	AniBlock.GlobalSequenceID=AniBlockInfo.GlobalSequenceID;

	M2_MODEL_ANIMATION_PAIR * pTimeStampPair=(M2_MODEL_ANIMATION_PAIR *)(pData+AniBlockInfo.TimestampsOffset);
	M2_MODEL_ANIMATION_PAIR * pKeyPair=(M2_MODEL_ANIMATION_PAIR *)(pData+AniBlockInfo.KeysOffset);

	AniBlock.Animations.Resize(AniBlockInfo.TimestampsCount);
	for(UINT j=0;j<AniBlockInfo.TimestampsCount;j++)
	{		

		if(pTimeStampPair[j].Count)
		{	
			UINT32 * pTimeStamps=(UINT *)(pData+pTimeStampPair[j].Offset);		
			AniBlock.Animations[j].TimeStamps.Resize(pTimeStampPair[j].Count);
			for(UINT k=0;k<pTimeStampPair[j].Count;k++)
			{				
				AniBlock.Animations[j].TimeStamps[k]=(pTimeStamps[k]);								
			}
			BLZ_SHORT_QUATERNION * pKeys=(BLZ_SHORT_QUATERNION *)(pData+pKeyPair[j].Offset);
			AniBlock.Animations[j].Keys.Resize(pKeyPair[j].Count);
			for(UINT k=0;k<pKeyPair[j].Count;k++)
			{
				CD3DQuaternion Rotation=ShortQuaternionToLongQuaternion(pKeys[k]);
				AniBlock.Animations[j].Keys[k]=(Rotation);
			}
		}				
	}	
}

void CD3DWOWM2ModelResource::LoadAniBlockAlpha(BYTE * pData,M2_MODEL_ANIMATION_BLOCK& AniBlockInfo,ANIMATION_BLOCK<FLOAT>& AniBlock)
{
	AniBlock.InterpolationType=AniBlockInfo.InterpolationType;
	AniBlock.GlobalSequenceID=AniBlockInfo.GlobalSequenceID;

	M2_MODEL_ANIMATION_PAIR * pTimeStampPair=(M2_MODEL_ANIMATION_PAIR *)(pData+AniBlockInfo.TimestampsOffset);
	M2_MODEL_ANIMATION_PAIR * pKeyPair=(M2_MODEL_ANIMATION_PAIR *)(pData+AniBlockInfo.KeysOffset);

	AniBlock.Animations.Resize(AniBlockInfo.TimestampsCount);

	for(UINT j=0;j<AniBlockInfo.TimestampsCount;j++)
	{
		if(pTimeStampPair[j].Count)
		{					
			
			UINT32 * pTimeStamps=(UINT *)(pData+pTimeStampPair[j].Offset);							
			AniBlock.Animations[j].TimeStamps.Resize(pTimeStampPair[j].Count);
			for(UINT k=0;k<pTimeStampPair[j].Count;k++)
			{				
				AniBlock.Animations[j].TimeStamps[k]=(pTimeStamps[k]);								
			}
			short * pKeys=(short *)(pData+pKeyPair[j].Offset);
			AniBlock.Animations[j].Keys.Resize(pKeyPair[j].Count);
			for(UINT k=0;k<pKeyPair[j].Count;k++)
			{
				FLOAT Alpha=(FLOAT)pKeys[k]/0x7FFF;
				AniBlock.Animations[j].Keys[k]=(Alpha);
			}
			
		}		
	}
}

void CD3DWOWM2ModelResource::LoadAniBlockTranslation(BYTE * pData,M2_MODEL_ANIMATION_BLOCK& AniBlockInfo,ANIMATION_BLOCK<CD3DVector3>& AniBlock)
{
	AniBlock.InterpolationType=AniBlockInfo.InterpolationType;
	AniBlock.GlobalSequenceID=AniBlockInfo.GlobalSequenceID;

	M2_MODEL_ANIMATION_PAIR * pTimeStampPair=(M2_MODEL_ANIMATION_PAIR *)(pData+AniBlockInfo.TimestampsOffset);
	M2_MODEL_ANIMATION_PAIR * pKeyPair=(M2_MODEL_ANIMATION_PAIR *)(pData+AniBlockInfo.KeysOffset);

	AniBlock.Animations.Resize(AniBlockInfo.TimestampsCount);
	for(UINT j=0;j<AniBlockInfo.TimestampsCount;j++)
	{		

		if(pTimeStampPair[j].Count)
		{			
			LoadTranslationFrames(pData,pTimeStampPair[j].Count,pTimeStampPair[j].Offset,
				pKeyPair[j].Count,pKeyPair[j].Offset,AniBlock.Animations[j]);			
		}				
	}
	
}

void CD3DWOWM2ModelResource::LoadAniBlockRotation(BYTE * pData,M2_MODEL_ANIMATION_BLOCK& AniBlockInfo,ANIMATION_BLOCK<CD3DQuaternion>& AniBlock)
{
	AniBlock.InterpolationType=AniBlockInfo.InterpolationType;
	AniBlock.GlobalSequenceID=AniBlockInfo.GlobalSequenceID;

	M2_MODEL_ANIMATION_PAIR * pTimeStampPair=(M2_MODEL_ANIMATION_PAIR *)(pData+AniBlockInfo.TimestampsOffset);
	M2_MODEL_ANIMATION_PAIR * pKeyPair=(M2_MODEL_ANIMATION_PAIR *)(pData+AniBlockInfo.KeysOffset);

	AniBlock.Animations.Resize(AniBlockInfo.TimestampsCount);
	for(UINT j=0;j<AniBlockInfo.TimestampsCount;j++)
	{		

		if(pTimeStampPair[j].Count)
		{	
			LoadRotationFrames(pData,pTimeStampPair[j].Count,pTimeStampPair[j].Offset,
				pKeyPair[j].Count,pKeyPair[j].Offset,AniBlock.Animations[j]);
		}				
	}	
}

void CD3DWOWM2ModelResource::LoadAniBlockScaling(BYTE * pData,M2_MODEL_ANIMATION_BLOCK& AniBlockInfo,ANIMATION_BLOCK<CD3DVector3>& AniBlock)
{
	AniBlock.InterpolationType=AniBlockInfo.InterpolationType;
	AniBlock.GlobalSequenceID=AniBlockInfo.GlobalSequenceID;

	M2_MODEL_ANIMATION_PAIR * pTimeStampPair=(M2_MODEL_ANIMATION_PAIR *)(pData+AniBlockInfo.TimestampsOffset);
	M2_MODEL_ANIMATION_PAIR * pKeyPair=(M2_MODEL_ANIMATION_PAIR *)(pData+AniBlockInfo.KeysOffset);

	AniBlock.Animations.Resize(AniBlockInfo.TimestampsCount);
	for(UINT j=0;j<AniBlockInfo.TimestampsCount;j++)
	{		

		if(pTimeStampPair[j].Count)
		{			
			LoadScalingFrames(pData,pTimeStampPair[j].Count,pTimeStampPair[j].Offset,
				pKeyPair[j].Count,pKeyPair[j].Offset,AniBlock.Animations[j]);
		}				
	}	
}

void CD3DWOWM2ModelResource::LoadAniBlockBool(BYTE * pData,M2_MODEL_ANIMATION_BLOCK& AniBlockInfo,ANIMATION_BLOCK<bool>& AniBlock)
{
	AniBlock.InterpolationType=AniBlockInfo.InterpolationType;
	AniBlock.GlobalSequenceID=AniBlockInfo.GlobalSequenceID;

	M2_MODEL_ANIMATION_PAIR * pTimeStampPair=(M2_MODEL_ANIMATION_PAIR *)(pData+AniBlockInfo.TimestampsOffset);
	M2_MODEL_ANIMATION_PAIR * pKeyPair=(M2_MODEL_ANIMATION_PAIR *)(pData+AniBlockInfo.KeysOffset);

	AniBlock.Animations.Resize(AniBlockInfo.TimestampsCount);

	for(UINT j=0;j<AniBlockInfo.TimestampsCount;j++)
	{
		if(pTimeStampPair[j].Count)
		{					

			UINT32 * pTimeStamps=(UINT *)(pData+pTimeStampPair[j].Offset);							
			AniBlock.Animations[j].TimeStamps.Resize(pTimeStampPair[j].Count);
			for(UINT k=0;k<pTimeStampPair[j].Count;k++)
			{				
				AniBlock.Animations[j].TimeStamps[k]=(pTimeStamps[k]);								
			}
			bool * pKeys=(bool *)(pData+pKeyPair[j].Offset);
			AniBlock.Animations[j].Keys.Resize(pKeyPair[j].Count);
			for(UINT k=0;k<pKeyPair[j].Count;k++)
			{
				AniBlock.Animations[j].Keys[k]=pKeys[k];
			}

		}		
	}
}

void CD3DWOWM2ModelResource::LoadFakeAniBlockColor(BYTE * pData,M2_MODEL_FAKE_ANIMATION_BLOCK& AniBlockInfo,FAKE_ANIMATION_FRAME<CD3DVector3>& AniBlock)
{
	short * pTimeStamps=(short *)(pData+AniBlockInfo.TimestampsOffset);
	AniBlock.TimeStamps.Resize(AniBlockInfo.TimestampsCount);
	for(UINT k=0;k<AniBlockInfo.TimestampsCount;k++)
	{				
		AniBlock.TimeStamps[k]=pTimeStamps[k]/32767.0f;
	}
	CD3DVector3 * pKeys=(CD3DVector3 *)(pData+AniBlockInfo.KeysOffset);
	AniBlock.Keys.Resize(AniBlockInfo.KeysCount);
	for(UINT k=0;k<AniBlockInfo.KeysCount;k++)
	{
		AniBlock.Keys[k]=pKeys[k];
	}
	
}

void CD3DWOWM2ModelResource::LoadFakeAniBlockAlpha(BYTE * pData,M2_MODEL_FAKE_ANIMATION_BLOCK& AniBlockInfo,FAKE_ANIMATION_FRAME<FLOAT>& AniBlock)
{
	short * pTimeStamps=(short *)(pData+AniBlockInfo.TimestampsOffset);	
	AniBlock.TimeStamps.Resize(AniBlockInfo.TimestampsCount);
	for(UINT k=0;k<AniBlockInfo.TimestampsCount;k++)
	{				
		AniBlock.TimeStamps[k]=pTimeStamps[k]/32767.0f;
	}
	WORD * pKeys=(WORD *)(pData+AniBlockInfo.KeysOffset);
	AniBlock.Keys.Resize(AniBlockInfo.KeysCount);
	for(UINT k=0;k<AniBlockInfo.KeysCount;k++)
	{
		FLOAT Alpha=(FLOAT)pKeys[k]/0x7FFF;
		AniBlock.Keys[k]=Alpha;
	}
}
void CD3DWOWM2ModelResource::LoadFakeAniBlockSize(BYTE * pData,M2_MODEL_FAKE_ANIMATION_BLOCK& AniBlockInfo,FAKE_ANIMATION_FRAME<CD3DVector2>& AniBlock)
{
	short * pTimeStamps=(short *)(pData+AniBlockInfo.TimestampsOffset);
	AniBlock.TimeStamps.Resize(AniBlockInfo.TimestampsCount);
	for(UINT k=0;k<AniBlockInfo.TimestampsCount;k++)
	{				
		AniBlock.TimeStamps[k]=pTimeStamps[k]/32767.0f;
	}
	CD3DVector2 * pKeys=(CD3DVector2 *)(pData+AniBlockInfo.KeysOffset);
	AniBlock.Keys.Resize(AniBlockInfo.KeysCount);
	for(UINT k=0;k<AniBlockInfo.KeysCount;k++)
	{
		AniBlock.Keys[k]=pKeys[k];
	}
}
void CD3DWOWM2ModelResource::LoadFakeAniBlockShort(BYTE * pData,M2_MODEL_FAKE_ANIMATION_BLOCK& AniBlockInfo,FAKE_ANIMATION_FRAME<short>& AniBlock)
{
	short * pTimeStamps=(short *)(pData+AniBlockInfo.TimestampsOffset);
	AniBlock.TimeStamps.Resize(AniBlockInfo.TimestampsCount);
	for(UINT k=0;k<AniBlockInfo.TimestampsCount;k++)
	{				
		AniBlock.TimeStamps[k]=pTimeStamps[k]/32767.0f;
	}
	short * pKeys=(short *)(pData+AniBlockInfo.KeysOffset);
	AniBlock.Keys.Resize(AniBlockInfo.KeysCount);
	for(UINT k=0;k<AniBlockInfo.KeysCount;k++)
	{
		AniBlock.Keys[k]=pKeys[k];
	}
}

bool CD3DWOWM2ModelResource::GetInterpolationValue(UINT Time,int InterpolationType,UINT AniLength,ANIMATION_FRAME<FLOAT>& AniFrame,FLOAT& Value)
{
	UINT FrameCount=AniFrame.TimeStamps.GetCount();	
	bool ValueChanged=false;

	if(FrameCount==1)
	{			
		Value=AniFrame.Keys[0];
		ValueChanged=true;
	}
	else if(FrameCount)
	{				
		UINT FrameTime=Time;
		if(AniLength>0)
			FrameTime=FrameTime%AniLength;	
		else if(FrameCount>1)
			FrameTime=FrameTime%AniFrame.TimeStamps[FrameCount-1];
		UINT Frame=0;
		for(;Frame<AniFrame.TimeStamps.GetCount()-1;Frame++)
		{
			if(FrameTime>=AniFrame.TimeStamps[Frame]&&
				FrameTime<AniFrame.TimeStamps[Frame+1])
				break;
		}
		if(Frame<AniFrame.TimeStamps.GetCount()-1)
		{
			FLOAT S=(FLOAT)(FrameTime-AniFrame.TimeStamps[Frame])/
				(FLOAT)(AniFrame.TimeStamps[Frame+1]-AniFrame.TimeStamps[Frame]);
			switch(InterpolationType)
			{
			case BLZ_M2_INTERPOLATE_NONE:
				Value=AniFrame.Keys[Frame];							
				break;
			case BLZ_M2_INTERPOLATE_LINEAR:
				Value=AniFrame.Keys[Frame]*(1.0f-S)+AniFrame.Keys[Frame+1]*S;
				break;
			case BLZ_M2_INTERPOLATE_HERMITE:
				PrintSystemLog(0,"Unsupport Hermite Interpolate in GetInterpolationValue");
				break;
			}				
		}
		else
		{
			Value=AniFrame.Keys[Frame];
		}
		ValueChanged=true;
	}
	return ValueChanged;
}

bool CD3DWOWM2ModelResource::GetInterpolationValue(UINT Time,int InterpolationType,UINT AniLength,ANIMATION_FRAME<CD3DVector3>& AniFrame,CD3DVector3& Value)
{
	UINT FrameCount=AniFrame.TimeStamps.GetCount();
	bool ValueChanged=false;

	if(FrameCount==1)
	{			
		Value=AniFrame.Keys[0];
		ValueChanged=true;
	}
	else if(FrameCount)
	{				
		UINT FrameTime=Time;
		if(AniLength>0)
			FrameTime=FrameTime%AniLength;	
		else if(FrameCount>1)
			FrameTime=FrameTime%AniFrame.TimeStamps[FrameCount-1];
		UINT Frame=0;
		for(;Frame<AniFrame.TimeStamps.GetCount()-1;Frame++)
		{
			if(FrameTime>=AniFrame.TimeStamps[Frame]&&
				FrameTime<AniFrame.TimeStamps[Frame+1])
				break;
		}
		if(Frame<AniFrame.TimeStamps.GetCount()-1)
		{
			FLOAT S=(FLOAT)(FrameTime-AniFrame.TimeStamps[Frame])/
				(FLOAT)(AniFrame.TimeStamps[Frame+1]-AniFrame.TimeStamps[Frame]);
			switch(InterpolationType)
			{
			case BLZ_M2_INTERPOLATE_NONE:
				Value=AniFrame.Keys[Frame];							
				break;
			case BLZ_M2_INTERPOLATE_LINEAR:
				Value=CD3DVector3::Lerp(AniFrame.Keys[Frame],AniFrame.Keys[Frame+1],S);
				break;
			case BLZ_M2_INTERPOLATE_HERMITE:
				PrintSystemLog(0,"Unsupport Hermite Interpolate in GetInterpolationValue");
				break;
			}				
		}
		else
		{
			Value=AniFrame.Keys[Frame];
		}	
		ValueChanged=true;
	}
	return ValueChanged;
}

bool CD3DWOWM2ModelResource::GetInterpolationValue(UINT Time,int InterpolationType,UINT AniLength,ANIMATION_FRAME<CD3DQuaternion>& AniFrame,CD3DQuaternion& Value)
{
	UINT FrameCount=AniFrame.TimeStamps.GetCount();
	bool ValueChanged=false;

	Value.SetIdentity();

	if(FrameCount==1)
	{			
		Value=AniFrame.Keys[0];
		ValueChanged=true;
	}
	else if(FrameCount)
	{				
		UINT FrameTime=Time;
		if(AniLength>0)
			FrameTime=FrameTime%AniLength;	
		else if(FrameCount>1)
			FrameTime=FrameTime%AniFrame.TimeStamps[FrameCount-1];
		UINT Frame=0;
		for(;Frame<AniFrame.TimeStamps.GetCount()-1;Frame++)
		{
			if(FrameTime>=AniFrame.TimeStamps[Frame]&&
				FrameTime<AniFrame.TimeStamps[Frame+1])
				break;
		}
		if(Frame<AniFrame.TimeStamps.GetCount()-1)
		{
			FLOAT S=(FLOAT)(FrameTime-AniFrame.TimeStamps[Frame])/
				(FLOAT)(AniFrame.TimeStamps[Frame+1]-AniFrame.TimeStamps[Frame]);
			switch(InterpolationType)
			{
			case BLZ_M2_INTERPOLATE_NONE:
				Value=AniFrame.Keys[Frame];							
				break;
			case BLZ_M2_INTERPOLATE_LINEAR:
				Value=CD3DQuaternion::SLerp(AniFrame.Keys[Frame],AniFrame.Keys[Frame+1],S);
				break;
			case BLZ_M2_INTERPOLATE_HERMITE:
				PrintSystemLog(0,"Unsupport Hermite Interpolate in GetInterpolationValue");
				break;
			}				
		}
		else
		{
			Value=AniFrame.Keys[Frame];
		}	
		ValueChanged=true;
	}
	return ValueChanged;
}

WORD CD3DWOWM2ModelResource::RebuildVertexIndex(CEasyArray<WORD>& VertexIndexList,WORD VertexIndex)
{
	for(UINT i=0;i<VertexIndexList.GetCount();i++)
	{
		if(VertexIndexList[i]==VertexIndex)
		{
			return i;
		}
	}
	VertexIndexList.Add(VertexIndex);
	return VertexIndexList.GetCount()-1;
}

bool CD3DWOWM2ModelResource::LoadAttachments(BYTE * pModelData)
{
	if(pModelData==NULL)
		return false;

	BLZ_M2_HEADER * pHeader=(BLZ_M2_HEADER *)pModelData;

	M2_MODEL_ATTACHMENT * pAttachments=(M2_MODEL_ATTACHMENT *)(pModelData+pHeader->AttachmentsOffset);	
	short * pAttachmentLookup=(short *)(pModelData+pHeader->AttachLookupOffset);

	m_Attachments.Resize(pHeader->AttachmentsCount);
	for(UINT i=0;i<pHeader->AttachmentsCount;i++)
	{
	

		m_Attachments[i].ID=pAttachments[i].ID;
		m_Attachments[i].Bone=pAttachments[i].Bone;
		m_Attachments[i].Position=BLZTranslationToD3D(pAttachments[i].Position);
		if(m_Attachments[i].ID<CAI_MAX)
		{
			m_Attachments[i].Name=CHAR_ATTACHMENT_NAME[m_Attachments[i].ID];
		}
	}
	return true;
}


void CD3DWOWM2ModelResource::BuildFX(CD3DSubMeshMaterial * pSubMeshMaterial)
{
	CEasyString FXName;
	CEasyString PSDiffuseFunction;
	CEasyString VSDiffuseFunction;
	CEasyString EnableZWrite;
	CEasyString EnableFog;
	CEasyString CullMode;
	CEasyString EnableAlphaBlend;
	CEasyString BlendOp;
	CEasyString SrcBlend;
	CEasyString DestBlend;
	CEasyString EnableAlphaTest;
	CEasyString Texture2PSOp;
	CEasyString VShader;
	CEasyString PShader;
	UINT64 RenderFlag=pSubMeshMaterial->GetTextureProperty(0)&SMP_RENDER_FLAG;
	UINT64 BlendingMode=pSubMeshMaterial->GetTextureProperty(0)&SMP_BLENDING_MODE;
	
	FXName.Format("M2Model_0x%llX_0x%llX",
		pSubMeshMaterial->GetTextureProperty(0),
		pSubMeshMaterial->GetTextureProperty(1));
	
	if(m_SkinMeshBoneCount>MAX_BONE_NUM)
	{
		FXName+="_SoftBone";
		VShader="VShaderNormal";
	}
	else
	{
		VShader="VShaderWithSkinMesh";
	}
	if(RenderFlag&SMP_NO_LIGHT)
	{		
		VSDiffuseFunction="1.0f";
	}
	else
	{
		PSDiffuseFunction=" * Diffuse";
		VSDiffuseFunction="CaculateDiffuse(Normal)";
	}	
	
	if(RenderFlag&SMP_NO_FOG)
	{
		EnableFog="False";
	}
	else
	{
		EnableFog="True";
	}
	if(RenderFlag&SMP_NO_CULL)
	{
		CullMode="None";
	}
	else
	{
		CullMode="CCW";
	}
	
	if(RenderFlag&SMP_DISABLE_ZBUFFER)
	{		
		EnableZWrite="False";
	}
	else
	{		
		EnableZWrite="True";
	}

	EnableAlphaBlend="False";	
	EnableAlphaTest="False";
	BlendOp="Add";
	SrcBlend="SrcAlpha";
	DestBlend="InvSrcAlpha";
	switch(BlendingMode)
	{	
	case SMP_BLENDING_ALPHA_TEST:
		EnableAlphaTest="True";
		break;
	case SMP_BLENDING_ALPHA_BLENDING:
		EnableAlphaBlend="True";
		break;
	case SMP_BLENDING_ADDITIVE:
		EnableAlphaBlend="True";
		SrcBlend="One";
		DestBlend="One";
		break;
	case SMP_BLENDING_ADDITIVE_ALPHA:
		EnableAlphaBlend="True";
		SrcBlend="SrcAlpha";
		DestBlend="One";
		break;
	case SMP_BLENDING_MODULATE:
		break;
	case SMP_BLENDING_MUL:
		break;
	}		
	if(pSubMeshMaterial->GetTextureLayerCount()>1)
	{
		if(pSubMeshMaterial->GetTextureProperty(1)&SMP_TEXTURE_ENV_MAP)
		{
			UINT64 Texture2BlendingMode=pSubMeshMaterial->GetTextureProperty(1)&SMP_BLENDING_MODE;
			if(Texture2BlendingMode==SMP_BLENDING_MUL)
			{
				Texture2PSOp="* (texCUBE( Sampler1, EnvTex ) + 1.0f)";
			}
			else
			{
				Texture2PSOp="+ texCUBE( Sampler1, EnvTex ) ";
			}
		}
		else
		{
			Texture2PSOp="+ tex2D( Sampler1, Tex0 ) ";
		}
	}
	
	PShader="PShaderWithNormal";

	IFileAccessor * pFile;

	CEasyString FxContent;


	pFile=CD3DWOWM2Model::CreateFileAccessor();
	if(pFile==NULL)
		return ;
	if(!pFile->Open(M2_MODEL_FX_FILE_NAME,IFileAccessor::modeRead))
	{
		pFile->Release();
		return ;	
	}
	int FileSize=(int)pFile->GetSize();	
	FxContent.Resize(FileSize);
	pFile->Read((LPVOID)FxContent.GetBuffer(),FileSize);	
	FxContent.SetLength(FileSize);
	pFile->Release();
	FxContent.Replace("<PSDiffuseFunction>",PSDiffuseFunction);
	FxContent.Replace("<VSDiffuseFunction>",VSDiffuseFunction);
	FxContent.Replace("<EnableZWrite>",EnableZWrite);
	FxContent.Replace("<EnableFog>",EnableFog);
	FxContent.Replace("<CullMode>",CullMode);
	FxContent.Replace("<EnableAlphaBlend>",EnableAlphaBlend);
	FxContent.Replace("<BlendOp>",BlendOp);
	FxContent.Replace("<SrcBlend>",SrcBlend);
	FxContent.Replace("<DestBlend>",DestBlend);
	FxContent.Replace("<EnableAlphaTest>",EnableAlphaTest);
	FxContent.Replace("<Texture2PSOp>",Texture2PSOp);
	FxContent.Replace("<VShader>",VShader);
	FxContent.Replace("<PShader>",PShader);

	CD3DFX * pFX=m_pManager->GetDevice()->GetFXManager()->LoadFXFromMemory(FXName,FxContent.GetBuffer(),FxContent.GetLength());	
	pSubMeshMaterial->SetFX(pFX);
}

CD3DFX * CD3DWOWM2ModelResource::BuildParticleFX(UINT BlendingType)
{
	CEasyString FXName;	
	CEasyString EnableZWrite;	
	CEasyString EnableAlphaBlend;
	CEasyString BlendOp;
	CEasyString SrcBlend;
	CEasyString DestBlend;
	CEasyString EnableAlphaTest;
	

	FXName.Format("M2Particle_0x%X",BlendingType);

	switch(BlendingType)
	{
	case EBT_OPACITY:			//glDisable(GL_BLEND); glDisable(GL_ALPHA_TEST); 
		EnableZWrite="True";
		EnableAlphaBlend="False";
		BlendOp="Add";
		SrcBlend="SrcAlpha";
		DestBlend="InvSrcAlpha";
		EnableAlphaTest="False";
		break;
	case EBT_ADD:				//glBlendFunc(GL_SRC_COLOR, GL_ONE);
		EnableZWrite="False";
		EnableAlphaBlend="True";
		BlendOp="Add";
		SrcBlend="SrcColor";
		DestBlend="One";
		EnableAlphaTest="False";
		break;
	case EBT_ALPHA_BLEND:		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  
		EnableZWrite="False";
		EnableAlphaBlend="True";
		BlendOp="Add";
		SrcBlend="SrcAlpha";
		DestBlend="InvSrcAlpha";
		EnableAlphaTest="False";
		break;
	case EBT_ALPHA_TEST:		//glDisable(GL_BLEND); glEnable(GL_ALPHA_TEST);  
		EnableZWrite="True";
		EnableAlphaBlend="False";
		BlendOp="Add";
		SrcBlend="SrcAlpha";
		DestBlend="InvSrcAlpha";
		EnableAlphaTest="True";
		break;
	case EBT_ALPHA_ADD:			//glBlendFunc(GL_SRC_ALPHA, GL_ONE); 
		EnableZWrite="False";
		EnableAlphaBlend="True";
		BlendOp="Add";
		SrcBlend="SrcAlpha";
		DestBlend="One";
		EnableAlphaTest="False";
		break;
	}	

	IFileAccessor * pFile;

	CEasyString FxContent;


	pFile=CD3DWOWM2Model::CreateFileAccessor();
	if(pFile==NULL)
		return NULL;
	if(!pFile->Open(M2_PARTICLE_FX_FILE_NAME,IFileAccessor::modeRead))
	{
		pFile->Release();
		return NULL;	
	}
	int FileSize=(int)pFile->GetSize();	
	FxContent.Resize(FileSize);
	pFile->Read((LPVOID)FxContent.GetBuffer(),FileSize);	
	FxContent.SetLength(FileSize);
	pFile->Release();
	FxContent.Replace("<EnableZWrite>",EnableZWrite);
	FxContent.Replace("<EnableAlphaBlend>",EnableAlphaBlend);
	FxContent.Replace("<BlendOp>",BlendOp);
	FxContent.Replace("<SrcBlend>",SrcBlend);
	FxContent.Replace("<DestBlend>",DestBlend);
	FxContent.Replace("<EnableAlphaTest>",EnableAlphaTest);

	CD3DFX * pFX=m_pManager->GetDevice()->GetFXManager()->LoadFXFromMemory(FXName,FxContent.GetBuffer(),FxContent.GetLength());
	return pFX;
}

CD3DFX * CD3DWOWM2ModelResource::BuildRibbonFX(UINT RenderFlag,UINT BlendingMode)
{
	CEasyString FXName;	
	CEasyString EnableZWrite;
	CEasyString EnableFog;
	CEasyString CullMode;
	CEasyString EnableAlphaBlend;
	CEasyString BlendOp;
	CEasyString SrcBlend;
	CEasyString DestBlend;
	CEasyString EnableAlphaTest;	

	FXName.Format("M2Ribbon_0x%X_0x%X",
		RenderFlag,BlendingMode);
	
	
	if(RenderFlag&BLZ_M2_RENDER_FLAG_NO_CULL)
	{
		CullMode="None";
	}
	else
	{
		CullMode="CCW";
	}

	if(RenderFlag&BLZ_M2_RENDER_FLAG_DISABLE_ZBUFFER)
	{		
		EnableZWrite="False";
	}
	else
	{		
		EnableZWrite="True";
	}

	EnableAlphaBlend="False";	
	EnableAlphaTest="False";
	BlendOp="Add";
	SrcBlend="SrcAlpha";
	DestBlend="InvSrcAlpha";
	switch(BlendingMode)
	{	
	case BLZ_M2_BLENDING_MODE_ALPHA_TEST:
		EnableAlphaTest="True";
		break;
	case BLZ_M2_BLENDING_MODE_ALPHA_BLENDING:
		EnableAlphaBlend="True";
		break;
	case BLZ_M2_BLENDING_MODE_ADDITIVE:
		EnableAlphaBlend="True";
		SrcBlend="One";
		DestBlend="One";
		break;
	case BLZ_M2_BLENDING_MODE_ADDITIVE_ALPHA:
		EnableAlphaBlend="True";
		SrcBlend="DestColor";
		DestBlend="One";
		break;
	case BLZ_M2_BLENDING_MODE_MODULATE:
		break;
	case BLZ_M2_BLENDING_MODE_MUL:
		break;
	}		

	IFileAccessor * pFile;

	CEasyString FxContent;


	pFile=CD3DWOWM2Model::CreateFileAccessor();
	if(pFile==NULL)
		return NULL;
	if(!pFile->Open(M2_RIBBON_FX_FILE_NAME,IFileAccessor::modeRead))
	{
		pFile->Release();
		return NULL;	
	}
	int FileSize=(int)pFile->GetSize();	
	FxContent.Resize(FileSize);
	pFile->Read((LPVOID)FxContent.GetBuffer(),FileSize);	
	FxContent.SetLength(FileSize);
	pFile->Release();
	FxContent.Replace("<EnableZWrite>",EnableZWrite);
	FxContent.Replace("<EnableFog>",EnableFog);
	FxContent.Replace("<CullMode>",CullMode);
	FxContent.Replace("<EnableAlphaBlend>",EnableAlphaBlend);
	FxContent.Replace("<BlendOp>",BlendOp);
	FxContent.Replace("<SrcBlend>",SrcBlend);
	FxContent.Replace("<DestBlend>",DestBlend);
	FxContent.Replace("<EnableAlphaTest>",EnableAlphaTest);

	CD3DFX * pFX=m_pManager->GetDevice()->GetFXManager()->LoadFXFromMemory(FXName,FxContent.GetBuffer(),FxContent.GetLength());	
	return pFX;
}


bool CD3DWOWM2ModelResource::MakeOneAnimationBoneFrame(ANIMATION_SEQUENCE * pAniInfo,UINT Time,UINT Bone,BONE_MATRIX * pMatrix,bool UseCache)
{
	FUNCTION_BEGIN;

	bool IsAnimated=false;


	if(Bone>=m_Bones.GetCount()||pMatrix==NULL)
		return IsAnimated;

	pMatrix->BoneID=Bone;
	pMatrix->ParentID=m_Bones[Bone].ParentBone;
	pMatrix->Flags=m_Bones[Bone].Flags;
	pMatrix->PivotPoint=m_Bones[Bone].PivotPoint;

	if(pAniInfo)
	{			
		BONE_ANI_CACHE * pAniCache=NULL;
		if(m_BoneAniCacheSize&&UseCache)
		{
			pAniCache=FindBoneAniCache(pAniInfo->Index);
			if(pAniCache==NULL)
			{
				pAniCache=BuildBoneAniCache(pAniInfo);
			}
		}

		if(pAniCache)
		{
			IsAnimated=GetInterpolationTransformGlobal(Time,pMatrix->Matrix,
				m_Bones[Bone].Translations,m_Bones[Bone].Rotations,m_Bones[Bone].Scalings,false);
			if(IsAnimated)
			{
				pMatrix->Matrix=CD3DMatrix::FromTranslation(m_Bones[Bone].PivotPoint*(-1.0f))*
					pMatrix->Matrix*
					CD3DMatrix::FromTranslation(m_Bones[Bone].PivotPoint);
			}
			else
			{
				IsAnimated=MakeOneAnimationBoneFrameFromCache(pAniCache,Time,Bone,pMatrix->Matrix);
			}
			
		}
		else
		{
			IsAnimated=GetInterpolationTransformWithGlobal(Time,pAniInfo->Index,pAniInfo->Length,pMatrix->Matrix,
				m_Bones[Bone].Translations,m_Bones[Bone].Rotations,m_Bones[Bone].Scalings);
			if(IsAnimated)
			{
				pMatrix->Matrix=CD3DMatrix::FromTranslation(m_Bones[Bone].PivotPoint*(-1.0f))*
					pMatrix->Matrix*
					CD3DMatrix::FromTranslation(m_Bones[Bone].PivotPoint);
			}
			
		}	
	}
	
	return IsAnimated;
	FUNCTION_END;
	return false;
}


bool CD3DWOWM2ModelResource::GetInterpolationTransform(UINT Time,UINT AniIndex,UINT AniLength,CD3DMatrix& TransformMatrix,ANIMATION_BLOCK<CD3DVector3>& Translations,ANIMATION_BLOCK<CD3DQuaternion>& Rotations,ANIMATION_BLOCK<CD3DVector3>& Scalings)
{

	bool IsAnimated=false;

	CD3DVector3 Translation(0,0,0);
	CD3DQuaternion Rotation;
	CD3DVector3 Scaling(1.0f,1.0f,1.0f);
	Rotation.SetIdentity();
		
	
	if(AniIndex<Translations.Animations.GetCount())
	{
		GetInterpolationValue(Time,
			Translations.InterpolationType,
			AniLength,
			Translations.Animations[AniIndex],
			Translation);
		IsAnimated=true;
	}

	if(AniIndex<Rotations.Animations.GetCount())
	{	
		GetInterpolationValue(Time,
			Rotations.InterpolationType,
			AniLength,
			Rotations.Animations[AniIndex],
			Rotation);
		IsAnimated=true;
	}

	if(AniIndex<Scalings.Animations.GetCount())
	{
		GetInterpolationValue(Time,
			Scalings.InterpolationType,
			AniLength,
			Scalings.Animations[AniIndex],
			Scaling);
		IsAnimated=true;
	}
	
	if(IsAnimated)
		//TransformMatrix=CD3DMatrix::FromScale(Scaling)*CD3DMatrix::FromRotationQuaternion(Rotation)*CD3DMatrix::FromTranslation(Translation);
		TransformMatrix.SetTransformation(Scaling,Rotation,Translation);


	return IsAnimated;

}

bool CD3DWOWM2ModelResource::GetInterpolationTransformGlobal(UINT Time,CD3DMatrix& TransformMatrix,ANIMATION_BLOCK<CD3DVector3>& Translations,ANIMATION_BLOCK<CD3DQuaternion>& Rotations,ANIMATION_BLOCK<CD3DVector3>& Scalings,bool ForceGlobalSequence)
{

	bool IsAnimated=false;

	CD3DVector3 Translation(0,0,0);
	CD3DQuaternion Rotation;
	CD3DVector3 Scaling(1.0f,1.0f,1.0f);
	Rotation.SetIdentity();

	if(Translations.GlobalSequenceID>=0)
	{
		if(Translations.Animations.GetCount())
		{
			GetInterpolationValue(Time,
				Translations.InterpolationType,
				m_GlobalSequences[Translations.GlobalSequenceID],
				Translations.Animations[0],
				Translation);
			IsAnimated=true;
		}			
	}
	else if(ForceGlobalSequence)
	{
		if(Translations.Animations.GetCount())
		{
			
			GetInterpolationValue(Time,
				Translations.InterpolationType,
				0,
				Translations.Animations[0],
				Translation);
			IsAnimated=true;
		}
	}
	
	if(Rotations.GlobalSequenceID>=0)
	{		
		if(Rotations.Animations.GetCount())
		{	
			GetInterpolationValue(Time,
				Rotations.InterpolationType,
				m_GlobalSequences[Rotations.GlobalSequenceID],
				Rotations.Animations[0],
				Rotation);
			IsAnimated=true;
		}
	}
	else if(ForceGlobalSequence)
	{		
		if(Rotations.Animations.GetCount())
		{	
			GetInterpolationValue(Time,
				Rotations.InterpolationType,
				0,
				Rotations.Animations[0],
				Rotation);
			IsAnimated=true;
		}
	}
	

	if(Scalings.GlobalSequenceID>=0)
	{
		if(Scalings.Animations.GetCount())
		{
			GetInterpolationValue(Time,
				Scalings.InterpolationType,
				m_GlobalSequences[Scalings.GlobalSequenceID],
				Scalings.Animations[0],
				Scaling);
			IsAnimated=true;
		}
	}
	else if(ForceGlobalSequence)
	{
		if(Scalings.Animations.GetCount())
		{
			GetInterpolationValue(Time,
				Scalings.InterpolationType,
				0,
				Scalings.Animations[0],
				Scaling);
			IsAnimated=true;
		}
	}
	
	if(IsAnimated)
		//TransformMatrix=CD3DMatrix::FromScale(Scaling)*CD3DMatrix::FromRotationQuaternion(Rotation)*CD3DMatrix::FromTranslation(Translation);
		TransformMatrix.SetTransformation(Scaling,Rotation,Translation);

	return IsAnimated;

}

bool CD3DWOWM2ModelResource::GetInterpolationTransformWithGlobal(UINT Time,UINT AniIndex,UINT AniLength,CD3DMatrix& TransformMatrix,ANIMATION_BLOCK<CD3DVector3>& Translations,ANIMATION_BLOCK<CD3DQuaternion>& Rotations,ANIMATION_BLOCK<CD3DVector3>& Scalings)
{

	bool IsAnimated=false;

	CD3DVector3 Translation(0,0,0);
	CD3DQuaternion Rotation;
	CD3DVector3 Scaling(1.0f,1.0f,1.0f);
	Rotation.SetIdentity();


	if(Translations.GlobalSequenceID>=0)
	{
		if(Translations.Animations.GetCount())
		{
			GetInterpolationValue(Time,
				Translations.InterpolationType,
				m_GlobalSequences[Translations.GlobalSequenceID],
				Translations.Animations[0],
				Translation);
			IsAnimated=true;
		}			
	}
	else if(AniIndex<Translations.Animations.GetCount())
	{
		GetInterpolationValue(Time,
			Translations.InterpolationType,
			AniLength,
			Translations.Animations[AniIndex],
			Translation);
		IsAnimated=true;
	}
	

	if(Rotations.GlobalSequenceID>=0)
	{		
		if(Rotations.Animations.GetCount())
		{	
			GetInterpolationValue(Time,
				Rotations.InterpolationType,
				m_GlobalSequences[Rotations.GlobalSequenceID],
				Rotations.Animations[0],
				Rotation);
			IsAnimated=true;
		}
	}
	else if(AniIndex<Rotations.Animations.GetCount())
	{	
		GetInterpolationValue(Time,
			Rotations.InterpolationType,
			AniLength,
			Rotations.Animations[AniIndex],
			Rotation);
		IsAnimated=true;
	}

	if(Scalings.GlobalSequenceID>=0)
	{
		if(Scalings.Animations.GetCount())
		{
			GetInterpolationValue(Time,
				Scalings.InterpolationType,
				m_GlobalSequences[Scalings.GlobalSequenceID],
				Scalings.Animations[0],
				Scaling);
			IsAnimated=true;
		}
	}
	else if(AniIndex<Scalings.Animations.GetCount())
	{
		GetInterpolationValue(Time,
			Scalings.InterpolationType,
			AniLength,
			Scalings.Animations[AniIndex],
			Scaling);
		IsAnimated=true;
	}

	if(IsAnimated)
		//TransformMatrix=CD3DMatrix::FromScale(Scaling)*CD3DMatrix::FromRotationQuaternion(Rotation)*CD3DMatrix::FromTranslation(Translation);
		TransformMatrix.SetTransformation(Scaling,Rotation,Translation);


	return IsAnimated;

}

bool CD3DWOWM2ModelResource::BuildGlobalBoneAniCache()
{	
	m_GlobalBoneAniCache.Clear();	
	UINT FrameTimeSpan=1000/m_BoneAniCacheFreq;
	if(m_GlobalSequences.GetCount())
	{
		m_GlobalBoneAniCache.Resize(m_GlobalSequences.GetCount());
		for(UINT i=0;i<m_Bones.GetCount();i++)
		{
			int GlobalSequenceID=-1;
			if(m_Bones[i].Translations.GlobalSequenceID>=0)
				GlobalSequenceID=m_Bones[i].Translations.GlobalSequenceID;
			if(m_Bones[i].Rotations.GlobalSequenceID>=0)
				GlobalSequenceID=m_Bones[i].Rotations.GlobalSequenceID;
			if(m_Bones[i].Scalings.GlobalSequenceID>=0)
				GlobalSequenceID=m_Bones[i].Scalings.GlobalSequenceID;

			if(GlobalSequenceID>=0)
			{
				BONE_ANI_CACHE& AniCache=m_GlobalBoneAniCache[GlobalSequenceID];
				AniCache.AniIndex=GlobalSequenceID;
				AniCache.Bones.Resize(AniCache.Bones.GetCount()+1);
				BONE_ANI_CACHE_FRAME& BoneFrames=AniCache.Bones[AniCache.Bones.GetCount()-1];
				BoneFrames.BoneID=i;
				UINT FrameCount=m_GlobalSequences[GlobalSequenceID]/FrameTimeSpan;
				BoneFrames.Matrix.Resize(FrameCount);
				for(UINT j=0;j<FrameCount;j++)
				{
					GetInterpolationTransformGlobal(FrameTimeSpan*j,BoneFrames.Matrix[j],
						m_Bones[i].Translations,
						m_Bones[i].Rotations,
						m_Bones[i].Scalings,
						false);
				}
			}
		}
	}	
	return true;
}

CD3DWOWM2ModelResource::BONE_ANI_CACHE * CD3DWOWM2ModelResource::BuildBoneAniCache(ANIMATION_SEQUENCE * pAniInfo)
{
	if(m_BoneAniCache.GetCount()>m_BoneAniCacheSize)
	{
		m_BoneAniCache.Delete(m_BoneAniCache.GetHead());
	}
	UINT FrameTimeSpan=1000/m_BoneAniCacheFreq;
	UINT FrameCount=pAniInfo->Length/FrameTimeSpan;
	BONE_ANI_CACHE& AniCache=m_BoneAniCache.InsertAfter();
	AniCache.AniIndex=pAniInfo->Index;
	AniCache.Bones.Resize(m_Bones.GetCount());
	for(UINT i=0;i<m_Bones.GetCount();i++)
	{		
		BONE_ANI_CACHE_FRAME& BoneFrames=AniCache.Bones[i];
		BoneFrames.BoneID=i;			
		BoneFrames.Matrix.Resize(FrameCount);		
		for(UINT j=0;j<FrameCount;j++)
		{
			BoneFrames.Matrix[j].SetIdentity();
			GetInterpolationTransform(FrameTimeSpan*j,pAniInfo->Index,pAniInfo->Length,BoneFrames.Matrix[j],
				m_Bones[i].Translations,
				m_Bones[i].Rotations,
				m_Bones[i].Scalings);
			//if((m_Bones[i].Flags&CD3DWOWM2ModelResource::BONE_FLAG_BILLBOARD)==0)
			{
				BoneFrames.Matrix[j]=CD3DMatrix::FromTranslation(m_Bones[i].PivotPoint*(-1.0f))*
					BoneFrames.Matrix[j]*
					CD3DMatrix::FromTranslation(m_Bones[i].PivotPoint);
			}
		}		
	}	
	return &AniCache;
}

CD3DWOWM2ModelResource::BONE_ANI_CACHE * CD3DWOWM2ModelResource::FindBoneAniCache(UINT AniIndex)
{
	void * Pos=m_BoneAniCache.GetHead();
	while(Pos)
	{
		BONE_ANI_CACHE& AniCache=m_BoneAniCache.GetNextObject(Pos);
		if(AniCache.AniIndex==AniIndex)
		{
			return &AniCache;
		}
	}
	return NULL;
}

bool CD3DWOWM2ModelResource::MakeOneAnimationBoneFrameFromCache(BONE_ANI_CACHE * pAniCache,UINT Time,UINT Bone,CD3DMatrix& TransformMatrix)
{
	if(Bone<pAniCache->Bones.GetCount())
	{
		BONE_ANI_CACHE_FRAME& BoneFrames=pAniCache->Bones[Bone];
		if(BoneFrames.Matrix.GetCount())
		{
			UINT FrameTimeSpan=1000/m_BoneAniCacheFreq;
			UINT Frame=Time/FrameTimeSpan;
			Frame=Frame%BoneFrames.Matrix.GetCount();
			TransformMatrix=BoneFrames.Matrix[Frame];			
			return true;
		}
	}
	return false;
}


//CNameObject::STORAGE_STRUCT * CD3DWOWM2ModelResource::USOCreateHead(UINT Param)
//{
//	STORAGE_STRUCT * pHead=new STORAGE_STRUCT;
//	ZeroMemory(pHead,sizeof(STORAGE_STRUCT));
//	pHead->Size=sizeof(STORAGE_STRUCT);
//	return pHead;
//}
//
//int CD3DWOWM2ModelResource::USOWriteHead(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,UINT Param)
//{
//	UINT HeadSize=CD3DObjectResource::USOWriteHead(pHead,pUSOFile,Param);
//	if(HeadSize<0)
//		return -1;
//
//	STORAGE_STRUCT * pLocalHead=(STORAGE_STRUCT *)pHead;
//
//	
//	pLocalHead->BoundingBox=m_BoundingBox;
//	pLocalHead->BoundingSphere=m_BoundingSphere;
//	pLocalHead->SubMeshCount=(int)m_SubMeshList.GetCount();
//	pLocalHead->SkinMeshBoneCount=m_SkinMeshBoneCount;
//	pLocalHead->BoneCount=(int)m_Bones.GetCount();
//	pLocalHead->KeyBoneCount=(int)m_KeyBoneIndex.GetCount();
//	pLocalHead->AnimationCount=(int)m_AnimationSequence.GetCount();
//	pLocalHead->GlobalSequenceCount=(int)m_GlobalSequences.GetCount();
//	pLocalHead->AttachmentCount=(int)m_Attachments.GetCount();
//
//	pLocalHead->Size+=sizeof(STORAGE_STRUCT_BONE)*pLocalHead->BoneCount;
//	pLocalHead->Size+=sizeof(short)*pLocalHead->KeyBoneCount;
//	pLocalHead->Size+=sizeof(STORAGE_STRUCT_ANIMATION_INFO)*pLocalHead->AnimationCount;
//	pLocalHead->Size+=sizeof(UINT)*pLocalHead->GlobalSequenceCount;
//	pLocalHead->Size+=sizeof(STORAGE_STRUCT_ATTACHMENT)*pLocalHead->AttachmentCount;
//
//	for(UINT i=0;i<m_Bones.GetCount();i++)
//	{
//		pLocalHead->Size+=sizeof(STORAGE_STRUCT_BONE_ANI_FRAMES)*m_Bones[i].Translations.Animations.GetCount();
//		for(UINT j=0;j<m_Bones[i].Translations.Animations.GetCount();j++)
//		{
//			pLocalHead->Size+=sizeof(UINT)*m_Bones[i].Translations.Animations[j].TimeStamps.GetCount();
//			pLocalHead->Size+=sizeof(CD3DVector3)*m_Bones[i].Translations.Animations[j].Keys.GetCount();
//		}
//		pLocalHead->Size+=sizeof(STORAGE_STRUCT_BONE_ANI_FRAMES)*m_Bones[i].Rotations.Animations.GetCount();
//		for(UINT j=0;j<m_Bones[i].Rotations.Animations.GetCount();j++)
//		{
//			pLocalHead->Size+=sizeof(UINT)*m_Bones[i].Rotations.Animations[j].TimeStamps.GetCount();
//			pLocalHead->Size+=sizeof(CD3DQuaternion)*m_Bones[i].Rotations.Animations[j].Keys.GetCount();
//		}
//		pLocalHead->Size+=sizeof(STORAGE_STRUCT_BONE_ANI_FRAMES)*m_Bones[i].Scalings.Animations.GetCount();
//		for(UINT j=0;j<m_Bones[i].Scalings.Animations.GetCount();j++)
//		{
//			pLocalHead->Size+=sizeof(UINT)*m_Bones[i].Scalings.Animations[j].TimeStamps.GetCount();
//			pLocalHead->Size+=sizeof(CD3DVector3)*m_Bones[i].Scalings.Animations[j].Keys.GetCount();
//		}
//	}	
//
//	return sizeof(STORAGE_STRUCT);
//}
//
//bool CD3DWOWM2ModelResource::USOWriteData(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,UINT Param)
//{
//	if(!CD3DObjectResource::USOWriteData(pHead,pUSOFile,Param))
//		return false;
//
//	if(pUSOFile==NULL)
//		return false;	
//
//	IFileAccessor * pFile=pUSOFile->GetFile();
//	if(pFile==NULL)
//		return false;
//
//	for(UINT i=0;i<m_Bones.GetCount();i++)
//	{
//		STORAGE_STRUCT_BONE BoneInfo;
//		BoneInfo.AnimationSeq=m_Bones[i].AnimationSeq;
//		BoneInfo.Flags=m_Bones[i].Flags;
//		BoneInfo.ParentBone=m_Bones[i].ParentBone;
//		BoneInfo.GeosetID=m_Bones[i].GeosetID;
//		BoneInfo.PivotPoint=m_Bones[i].PivotPoint;
//
//		BoneInfo.Translations.InterpolationType=m_Bones[i].Translations.InterpolationType;
//		BoneInfo.Translations.GlobalSequenceID=m_Bones[i].Translations.GlobalSequenceID;
//		BoneInfo.Translations.AniCount=m_Bones[i].Translations.Animations.GetCount();
//
//		BoneInfo.Rotations.InterpolationType=m_Bones[i].Rotations.InterpolationType;
//		BoneInfo.Rotations.GlobalSequenceID=m_Bones[i].Rotations.GlobalSequenceID;
//		BoneInfo.Rotations.AniCount=m_Bones[i].Rotations.Animations.GetCount();
//
//		BoneInfo.Scalings.InterpolationType=m_Bones[i].Scalings.InterpolationType;
//		BoneInfo.Scalings.GlobalSequenceID=m_Bones[i].Scalings.GlobalSequenceID;
//		BoneInfo.Scalings.AniCount=m_Bones[i].Scalings.Animations.GetCount();
//
//		pFile->Write(&BoneInfo,sizeof(BoneInfo));
//
//		for(UINT j=0;j<m_Bones[i].Translations.Animations.GetCount();j++)
//		{
//			STORAGE_STRUCT_BONE_ANI_FRAMES AniInfo;
//			AniInfo.TimeStampCount=m_Bones[i].Translations.Animations[j].TimeStamps.GetCount();
//			AniInfo.KeyCount=m_Bones[i].Translations.Animations[j].Keys.GetCount();
//			pFile->Write(&AniInfo,sizeof(AniInfo));
//
//			if(AniInfo.KeyCount)
//				pFile->Write(&(m_Bones[i].Translations.Animations[j].TimeStamps[0]),sizeof(UINT)*AniInfo.TimeStampCount);
//
//			if(AniInfo.TimeStampCount)
//				pFile->Write(&(m_Bones[i].Translations.Animations[j].Keys[0]),sizeof(CD3DVector3)*AniInfo.KeyCount);			
//		}
//
//		for(UINT j=0;j<m_Bones[i].Rotations.Animations.GetCount();j++)
//		{
//			STORAGE_STRUCT_BONE_ANI_FRAMES AniInfo;
//			AniInfo.TimeStampCount=m_Bones[i].Rotations.Animations[j].TimeStamps.GetCount();
//			AniInfo.KeyCount=m_Bones[i].Rotations.Animations[j].Keys.GetCount();
//			pFile->Write(&AniInfo,sizeof(AniInfo));
//
//			if(AniInfo.KeyCount)
//				pFile->Write(&(m_Bones[i].Rotations.Animations[j].TimeStamps[0]),sizeof(UINT)*AniInfo.TimeStampCount);
//
//			if(AniInfo.TimeStampCount)
//				pFile->Write(&(m_Bones[i].Rotations.Animations[j].Keys[0]),sizeof(CD3DQuaternion)*AniInfo.KeyCount);
//		}
//
//		for(UINT j=0;j<m_Bones[i].Scalings.Animations.GetCount();j++)
//		{
//			STORAGE_STRUCT_BONE_ANI_FRAMES AniInfo;
//			AniInfo.TimeStampCount=m_Bones[i].Scalings.Animations[j].TimeStamps.GetCount();
//			AniInfo.KeyCount=m_Bones[i].Scalings.Animations[j].Keys.GetCount();
//			pFile->Write(&AniInfo,sizeof(AniInfo));
//
//			if(AniInfo.KeyCount)
//				pFile->Write(&(m_Bones[i].Scalings.Animations[j].TimeStamps[0]),sizeof(UINT)*AniInfo.TimeStampCount);
//
//			if(AniInfo.TimeStampCount)
//				pFile->Write(&(m_Bones[i].Scalings.Animations[j].Keys[0]),sizeof(CD3DVector3)*AniInfo.KeyCount);
//		}
//	}	
//
//	if(m_KeyBoneIndex.GetCount())
//		pFile->Write(&(m_KeyBoneIndex[0]),sizeof(short)*m_KeyBoneIndex.GetCount());
//
//	for(UINT i=0;i<m_AnimationSequence.GetCount();i++)
//	{
//		STORAGE_STRUCT_ANIMATION_INFO AniInfo;
//		AniInfo.AnimationID=m_AnimationSequence[i].AnimationID;
//		strncpy_0(AniInfo.AnimationName,MAX_M2_ANI_NAME_LEN,m_AnimationSequence[i].AnimationName,MAX_M2_ANI_NAME_LEN);
//		AniInfo.SubAnimationID=m_AnimationSequence[i].SubAnimationID;
//		AniInfo.Index=m_AnimationSequence[i].Index;
//		AniInfo.Length=m_AnimationSequence[i].Length;
//		AniInfo.MovingSpeed=m_AnimationSequence[i].MovingSpeed;
//		AniInfo.Flags=m_AnimationSequence[i].Flags;
//		AniInfo.Flags2=m_AnimationSequence[i].Flags2;
//		AniInfo.PlaybackSpeed=m_AnimationSequence[i].PlaybackSpeed;
//		AniInfo.BoundingBox=m_AnimationSequence[i].BoundingBox;
//		AniInfo.Radius=m_AnimationSequence[i].BoundingSphere.m_Radius;
//
//		pFile->Write(&AniInfo,sizeof(AniInfo));
//	}
//
//	if(m_GlobalSequences.GetCount())
//		pFile->Write(&(m_GlobalSequences[0]),sizeof(UINT)*m_GlobalSequences.GetCount());
//
//	for(UINT i=0;i<m_Attachments.GetCount();i++)
//	{
//		STORAGE_STRUCT_ATTACHMENT AttachmentInfo;
//		AttachmentInfo.ID=m_Attachments[i].ID;
//		AttachmentInfo.Bone=m_Attachments[i].Bone;
//		strncpy_0(AttachmentInfo.Name,MAX_M2_ATTACHMENT_NAME_LEN,m_Attachments[i].Name,MAX_M2_ATTACHMENT_NAME_LEN);
//		AttachmentInfo.Position=m_Attachments[i].Position;
//
//		pFile->Write(&AttachmentInfo,sizeof(AttachmentInfo));
//	}
//	return true;
//}
//
//
//int CD3DWOWM2ModelResource::USOReadHead(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,UINT Param)
//{	
//	int ReadSize=CD3DObjectResource::USOReadHead(pHead,pUSOFile,Param);
//	if(ReadSize<0)
//		return -1;
//
//	if(pHead->Size<sizeof(STORAGE_STRUCT))
//		return false;
//
//	STORAGE_STRUCT * pLocalHead=(STORAGE_STRUCT *)pHead;
//
//	m_SkinMeshBoneCount=pLocalHead->SkinMeshBoneCount;
//
//	return sizeof(STORAGE_STRUCT);
//}
//
//int CD3DWOWM2ModelResource::USOReadData(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,BYTE * pData,int DataSize,UINT Param)
//{
//	int ReadSize=CD3DObjectResource::USOReadData(pHead,pUSOFile,pData,DataSize,Param);
//
//	pData+=ReadSize;
//	DataSize-=ReadSize;
//
//	STORAGE_STRUCT * pLocalHead=(STORAGE_STRUCT *)pHead;
//
//	m_Bones.Resize(pLocalHead->BoneCount);
//
//	for(int i=0;i<pLocalHead->BoneCount;i++)
//	{
//		STORAGE_STRUCT_BONE * pBoneInfo=(STORAGE_STRUCT_BONE *)pData;
//		pData+=sizeof(STORAGE_STRUCT_BONE);
//		ReadSize+=sizeof(STORAGE_STRUCT_BONE);
//
//		m_Bones[i].AnimationSeq=pBoneInfo->AnimationSeq;
//		m_Bones[i].Flags=pBoneInfo->Flags;
//		m_Bones[i].ParentBone=pBoneInfo->ParentBone;
//		m_Bones[i].GeosetID=pBoneInfo->GeosetID;
//		m_Bones[i].PivotPoint=pBoneInfo->PivotPoint;
//
//		m_Bones[i].Translations.InterpolationType=pBoneInfo->Translations.InterpolationType;
//		m_Bones[i].Translations.GlobalSequenceID=pBoneInfo->Translations.GlobalSequenceID;
//		m_Bones[i].Translations.Animations.Resize(pBoneInfo->Translations.AniCount);
//
//		m_Bones[i].Rotations.InterpolationType=pBoneInfo->Rotations.InterpolationType;
//		m_Bones[i].Rotations.GlobalSequenceID=pBoneInfo->Rotations.GlobalSequenceID;
//		m_Bones[i].Rotations.Animations.Resize(pBoneInfo->Rotations.AniCount);
//
//		m_Bones[i].Scalings.InterpolationType=pBoneInfo->Scalings.InterpolationType;
//		m_Bones[i].Scalings.GlobalSequenceID=pBoneInfo->Scalings.GlobalSequenceID;
//		m_Bones[i].Scalings.Animations.Resize(pBoneInfo->Scalings.AniCount);
//		for(int j=0;j<pBoneInfo->Translations.AniCount;j++)
//		{
//			STORAGE_STRUCT_BONE_ANI_FRAMES * pAniInfo=(STORAGE_STRUCT_BONE_ANI_FRAMES *)pData;
//			pData+=sizeof(STORAGE_STRUCT_BONE_ANI_FRAMES);
//			ReadSize+=sizeof(STORAGE_STRUCT_BONE_ANI_FRAMES);			
//			if(pAniInfo->TimeStampCount)
//			{
//				m_Bones[i].Translations.Animations[j].TimeStamps.Resize(pAniInfo->TimeStampCount);			
//				memcpy(&(m_Bones[i].Translations.Animations[j].TimeStamps[0]),pData,sizeof(UINT)*pAniInfo->TimeStampCount);
//				pData+=sizeof(UINT)*pAniInfo->TimeStampCount;
//				ReadSize+=sizeof(UINT)*pAniInfo->TimeStampCount;
//			}
//			if(pAniInfo->KeyCount)
//			{
//				m_Bones[i].Translations.Animations[j].Keys.Resize(pAniInfo->KeyCount);
//				memcpy(&(m_Bones[i].Translations.Animations[j].Keys[0]),pData,sizeof(CD3DVector3)*pAniInfo->KeyCount);
//				pData+=sizeof(CD3DVector3)*pAniInfo->KeyCount;
//				ReadSize+=sizeof(CD3DVector3)*pAniInfo->KeyCount;
//			}
//		}
//		for(int j=0;j<pBoneInfo->Rotations.AniCount;j++)
//		{
//			STORAGE_STRUCT_BONE_ANI_FRAMES * pAniInfo=(STORAGE_STRUCT_BONE_ANI_FRAMES *)pData;
//			pData+=sizeof(STORAGE_STRUCT_BONE_ANI_FRAMES);
//			ReadSize+=sizeof(STORAGE_STRUCT_BONE_ANI_FRAMES);
//			if(pAniInfo->TimeStampCount)
//			{
//				m_Bones[i].Rotations.Animations[j].TimeStamps.Resize(pAniInfo->TimeStampCount);			
//				memcpy(&(m_Bones[i].Rotations.Animations[j].TimeStamps[0]),pData,sizeof(UINT)*pAniInfo->TimeStampCount);
//				pData+=sizeof(UINT)*pAniInfo->TimeStampCount;
//				ReadSize+=sizeof(UINT)*pAniInfo->TimeStampCount;
//			}
//			if(pAniInfo->KeyCount)
//			{
//				m_Bones[i].Rotations.Animations[j].Keys.Resize(pAniInfo->KeyCount);
//				memcpy(&(m_Bones[i].Rotations.Animations[j].Keys[0]),pData,sizeof(CD3DQuaternion)*pAniInfo->KeyCount);
//				pData+=sizeof(CD3DQuaternion)*pAniInfo->KeyCount;
//				ReadSize+=sizeof(CD3DQuaternion)*pAniInfo->KeyCount;
//			}
//		}
//		for(int j=0;j<pBoneInfo->Scalings.AniCount;j++)
//		{
//			STORAGE_STRUCT_BONE_ANI_FRAMES * pAniInfo=(STORAGE_STRUCT_BONE_ANI_FRAMES *)pData;
//			pData+=sizeof(STORAGE_STRUCT_BONE_ANI_FRAMES);
//			ReadSize+=sizeof(STORAGE_STRUCT_BONE_ANI_FRAMES);
//			if(pAniInfo->TimeStampCount)
//			{
//				m_Bones[i].Scalings.Animations[j].TimeStamps.Resize(pAniInfo->TimeStampCount);			
//				memcpy(&(m_Bones[i].Scalings.Animations[j].TimeStamps[0]),pData,sizeof(UINT)*pAniInfo->TimeStampCount);
//				pData+=sizeof(UINT)*pAniInfo->TimeStampCount;
//				ReadSize+=sizeof(UINT)*pAniInfo->TimeStampCount;
//			}
//			if(pAniInfo->KeyCount)
//			{
//				m_Bones[i].Scalings.Animations[j].Keys.Resize(pAniInfo->KeyCount);
//				memcpy(&(m_Bones[i].Scalings.Animations[j].Keys[0]),pData,sizeof(CD3DVector3)*pAniInfo->KeyCount);
//				pData+=sizeof(CD3DVector3)*pAniInfo->KeyCount;
//				ReadSize+=sizeof(CD3DVector3)*pAniInfo->KeyCount;
//			}
//		}
//	}
//
//	if(pLocalHead->KeyBoneCount)
//	{
//		m_KeyBoneIndex.Resize(pLocalHead->KeyBoneCount);
//		memcpy(&(m_KeyBoneIndex[0]),pData,sizeof(short)*pLocalHead->KeyBoneCount);
//		pData+=sizeof(short)*pLocalHead->KeyBoneCount;
//		ReadSize+=sizeof(short)*pLocalHead->KeyBoneCount;
//	}
//
//	m_AnimationSequence.Resize(pLocalHead->AnimationCount);
//	for(int i=0;i<pLocalHead->AnimationCount;i++)
//	{
//		STORAGE_STRUCT_ANIMATION_INFO * pAniInfo=(STORAGE_STRUCT_ANIMATION_INFO *)pData;
//		pData+=sizeof(STORAGE_STRUCT_ANIMATION_INFO);
//		ReadSize+=sizeof(STORAGE_STRUCT_ANIMATION_INFO);
//
//		m_AnimationSequence[i].AnimationID=pAniInfo->AnimationID;
//		m_AnimationSequence[i].AnimationName=pAniInfo->AnimationName;
//		m_AnimationSequence[i].SubAnimationID=pAniInfo->SubAnimationID;
//		m_AnimationSequence[i].Index=pAniInfo->Index;
//		m_AnimationSequence[i].Length=pAniInfo->Length;
//		m_AnimationSequence[i].MovingSpeed=pAniInfo->MovingSpeed;
//		m_AnimationSequence[i].Flags=pAniInfo->Flags;
//		m_AnimationSequence[i].Flags2=pAniInfo->Flags2;
//		m_AnimationSequence[i].PlaybackSpeed=pAniInfo->PlaybackSpeed;
//		m_AnimationSequence[i].BoundingBox=pAniInfo->BoundingBox;
//		m_AnimationSequence[i].BoundingSphere.m_Radius=pAniInfo->Radius;
//		m_AnimationSequence[i].BoundingSphere.m_Center=0.0f;
//	}
//
//	if(pLocalHead->GlobalSequenceCount)
//	{
//		m_GlobalSequences.Resize(pLocalHead->GlobalSequenceCount);
//		memcpy(&(m_GlobalSequences[0]),pData,sizeof(UINT)*pLocalHead->GlobalSequenceCount);
//		pData+=sizeof(UINT)*pLocalHead->GlobalSequenceCount;
//		ReadSize+=sizeof(UINT)*pLocalHead->GlobalSequenceCount;
//	}
//
//	m_Attachments.Resize(pLocalHead->AttachmentCount);
//	for(int i=0;i<pLocalHead->AttachmentCount;i++)
//	{
//		STORAGE_STRUCT_ATTACHMENT * pAttachmentInfo=(STORAGE_STRUCT_ATTACHMENT *)pData;
//		pData+=sizeof(STORAGE_STRUCT_ATTACHMENT);
//		ReadSize+=sizeof(STORAGE_STRUCT_ATTACHMENT);
//		m_Attachments[i].ID=pAttachmentInfo->ID;
//		m_Attachments[i].Bone=pAttachmentInfo->Bone;
//		m_Attachments[i].Name=pAttachmentInfo->Name;
//		m_Attachments[i].Position=pAttachmentInfo->Position;
//	}
//	return ReadSize;
//}

template<class KEY_TYPE>
bool CD3DWOWM2ModelResource::PackAnimationBlock(ANIMATION_BLOCK<KEY_TYPE>& AniBlock,CSmartStruct& Packet,WORD MemberID)
{
	UINT BufferSize;
	void * pBuffer=Packet.PrepareMember(BufferSize);
	CSmartStruct AniPacket(pBuffer,BufferSize,true);	
	
	CHECK_SMART_STRUCT_ADD_AND_RETURN(AniPacket.AddMember(SST_AB_INTERPOLATION_TYPE,AniBlock.InterpolationType));
	CHECK_SMART_STRUCT_ADD_AND_RETURN(AniPacket.AddMember(SST_AB_GLOBALSEQUENCE_ID,AniBlock.GlobalSequenceID));
	for(UINT i=0;i<AniBlock.Animations.GetCount();i++)
	{
		UINT BufferSize;
		void * pBuffer=AniPacket.PrepareMember(BufferSize);
		CSmartStruct SubPacket(pBuffer,BufferSize,true);
		if(AniBlock.Animations[i].TimeStamps.GetCount())
		{
			SubPacket.AddMember(SST_AF_TIMESTAMPS,
				(char *)AniBlock.Animations[i].TimeStamps.GetBuffer(),
				AniBlock.Animations[i].TimeStamps.GetCount()*sizeof(UINT));
		}
		if(AniBlock.Animations[i].Keys.GetCount())
		{
			SubPacket.AddMember(SST_AF_KEYS,
				(char *)AniBlock.Animations[i].Keys.GetBuffer(),
				AniBlock.Animations[i].Keys.GetCount()*sizeof(KEY_TYPE));
		}
		if(!AniPacket.FinishMember(SST_AB_ANIMATION_FRAME,SubPacket.GetDataLen()))
			return false;
	}
	if(!Packet.FinishMember(MemberID,AniPacket.GetDataLen()))
		return false;
	return true;
}

template<class KEY_TYPE>
bool CD3DWOWM2ModelResource::PackFakeAnimationBlock(FAKE_ANIMATION_FRAME<KEY_TYPE>& AniBlock,CSmartStruct& Packet,WORD MemberID)
{
	UINT BufferSize;
	void * pBuffer=Packet.PrepareMember(BufferSize);
	CSmartStruct AniPacket(pBuffer,BufferSize,true);	
	
	if(AniBlock.TimeStamps.GetCount())
	{
		AniPacket.AddMember(SST_AF_TIMESTAMPS,
			(char *)AniBlock.TimeStamps.GetBuffer(),
			AniBlock.TimeStamps.GetCount()*sizeof(FLOAT));
	}
	if(AniBlock.Keys.GetCount())
	{
		AniPacket.AddMember(SST_AF_KEYS,
			(char *)AniBlock.Keys.GetBuffer(),
			AniBlock.Keys.GetCount()*sizeof(KEY_TYPE));
	}
		
	if(!Packet.FinishMember(MemberID,AniPacket.GetDataLen()))
		return false;
	return true;
}

UINT CD3DWOWM2ModelResource::GetPacketAniBlockCount(CSmartStruct& Packet)
{	
	void * Pos=Packet.GetFirstMemberPosition();
	UINT AniCount=0;
	while(Pos)
	{
		WORD MemberID;
		CSmartValue SubValue=Packet.GetNextMember(Pos,MemberID);
		if(MemberID==SST_AB_ANIMATION_FRAME)
			AniCount++;
	}
	return AniCount;
}

template<class KEY_TYPE>
bool CD3DWOWM2ModelResource::UnpackAnimationBlock(ANIMATION_BLOCK<KEY_TYPE>& AniBlock,CSmartStruct& Packet,WORD MemberID)
{
	CSmartStruct AniPacket=Packet.GetMember(MemberID);
	void * Pos=AniPacket.GetFirstMemberPosition();
	UINT AniIndex=0;
	while(Pos)
	{
		WORD MemberID;
		CSmartValue Value=AniPacket.GetNextMember(Pos,MemberID);
		switch(MemberID)
		{
		case SST_AB_INTERPOLATION_TYPE:
			AniBlock.InterpolationType=Value;
			break;
		case SST_AB_GLOBALSEQUENCE_ID:
			AniBlock.GlobalSequenceID=Value;
			break;
		case SST_AB_ANIMATION_FRAME:
			{
				CSmartStruct Frames=Value;
				CSmartValue TimeStamps=Frames.GetMember(SST_AF_TIMESTAMPS);
				CSmartValue Keys=Frames.GetMember(SST_AF_KEYS);
				UINT Count=TimeStamps.GetLength()/sizeof(UINT);
				if(Count)
				{
					AniBlock.Animations[AniIndex].TimeStamps.Resize(Count);
					memcpy(AniBlock.Animations[AniIndex].TimeStamps.GetBuffer(),
						(LPCTSTR)TimeStamps,sizeof(UINT)*Count);
				}
				Count=Keys.GetLength()/sizeof(KEY_TYPE);
				if(Count)
				{
					AniBlock.Animations[AniIndex].Keys.Resize(Count);
					memcpy(AniBlock.Animations[AniIndex].Keys.GetBuffer(),
						(LPCTSTR)Keys,sizeof(KEY_TYPE)*Count);
				}
			}
			AniIndex++;
			break;
		}
	}
	return true;
}

template<class KEY_TYPE>
bool CD3DWOWM2ModelResource::UnpackFakeAnimationBlock(FAKE_ANIMATION_FRAME<KEY_TYPE>& AniBlock,CSmartStruct& Packet,WORD MemberID)
{
	CSmartStruct AniPacket=Packet.GetMember(MemberID);
	CSmartValue TimeStamps=AniPacket.GetMember(SST_AF_TIMESTAMPS);
	CSmartValue Keys=AniPacket.GetMember(SST_AF_KEYS);
	UINT Count=TimeStamps.GetLength()/sizeof(FLOAT);
	if(Count)
	{
		AniBlock.TimeStamps.Resize(Count);
		memcpy(AniBlock.TimeStamps.GetBuffer(),
			(LPCTSTR)TimeStamps,sizeof(FLOAT)*Count);
	}
	Count=Keys.GetLength()/sizeof(KEY_TYPE);
	if(Count)
	{
		AniBlock.Keys.Resize(Count);
		memcpy(AniBlock.Keys.GetBuffer(),
			(LPCTSTR)Keys,sizeof(KEY_TYPE)*Count);
	}
	return true;
}

template<class KEY_TYPE>
UINT CD3DWOWM2ModelResource::GetAnimationBlockSmartStructSize(ANIMATION_BLOCK<KEY_TYPE>& AniBlock)
{
	UINT Size=0;

	Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(AniBlock.InterpolationType));
	Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(AniBlock.GlobalSequenceID));

	for(UINT i=0;i<AniBlock.Animations.GetCount();i++)
	{
		if(AniBlock.Animations[i].TimeStamps.GetCount())
		{
			Size+=SMART_STRUCT_STRING_MEMBER_SIZE(AniBlock.Animations[i].TimeStamps.GetCount()*sizeof(UINT));
		}
		if(AniBlock.Animations[i].Keys.GetCount())
		{
			Size+=SMART_STRUCT_STRING_MEMBER_SIZE(AniBlock.Animations[i].Keys.GetCount()*sizeof(KEY_TYPE));
		}
		Size+=SMART_STRUCT_STRUCT_MEMBER_SIZE(0);
	}
	Size+=SMART_STRUCT_STRUCT_MEMBER_SIZE(0);
	return Size;
}

template<class KEY_TYPE>
UINT CD3DWOWM2ModelResource::GetFakeAnimationBlockSmartStructSize(FAKE_ANIMATION_FRAME<KEY_TYPE>& AniBlock)
{
	UINT Size=0;
	if(AniBlock.TimeStamps.GetCount())
	{
		Size+=SMART_STRUCT_STRING_MEMBER_SIZE(AniBlock.TimeStamps.GetCount()*sizeof(FLOAT));
	}
	if(AniBlock.Keys.GetCount())
	{
		Size+=SMART_STRUCT_STRING_MEMBER_SIZE(AniBlock.Keys.GetCount()*sizeof(KEY_TYPE));
	}
	Size+=SMART_STRUCT_STRUCT_MEMBER_SIZE(0);

	return Size;
}

void CD3DWOWM2ModelResource::CheckAni()
{
	m_HasBoneAni=false;
	for(UINT i=0;i<m_Bones.GetCount();i++)
	{		
		for(UINT j=0;j<m_Bones[i].Translations.Animations.GetCount();j++)
		{
			if(m_Bones[i].Translations.Animations[j].TimeStamps.GetCount())
			{
				m_HasBoneAni=true;
				break;
			}			
		}
		for(UINT j=0;j<m_Bones[i].Rotations.Animations.GetCount();j++)
		{
			if(m_Bones[i].Rotations.Animations[j].TimeStamps.GetCount())
			{
				m_HasBoneAni=true;
				break;
			}				
		}
		for(UINT j=0;j<m_Bones[i].Scalings.Animations.GetCount();j++)
		{
			if(m_Bones[i].Scalings.Animations[j].TimeStamps.GetCount())
			{
				m_HasBoneAni=true;
				break;
			}				
		}
		if(m_HasBoneAni)
			break;
	}
	m_HasMaterialAni=false;
	for(UINT i=0;i<m_SubMeshList.GetCount();i++)
	{
		CD3DSubMeshMaterial& Material=m_SubMeshList[i]->GetMaterial();
		
		if(Material.GetTextureLayerCount()>0)
		{
			UINT64 TextureProperty=Material.GetTextureProperty(0);
			signed char ColorAniIndex=(signed char)((TextureProperty&CD3DWOWM2ModelResource::SMP_COLOR_ANI_INDEX)>>24);
			signed char TransparencyAniIndex=(signed char)((TextureProperty&CD3DWOWM2ModelResource::SMP_TRANSPARENCY_ANI_INDEX)>>32);
			if(ColorAniIndex>=0||TransparencyAniIndex>=0)
			{
				m_HasMaterialAni=true;
				return;
			}
			for(UINT j=0;j<Material.GetTextureLayerCount();j++)
			{
				UINT64 TextureProperty=Material.GetTextureProperty(j);
				signed char UVAniIndex=(signed char)((TextureProperty&CD3DWOWM2ModelResource::SMP_UV_ANI_INDEX)>>40);
				if(UVAniIndex>=0)
				{
					m_HasMaterialAni=true;
					return;
				}
			}
		}
	}
}

template<class KEY_TYPE>
UINT CD3DWOWM2ModelResource::CaculateAnimationBlockSize(ANIMATION_BLOCK<KEY_TYPE>& AniBlock)
{
	UINT DataSize=sizeof(ANIMATION_BLOCK<KEY_TYPE>);
	for(UINT i=0;i<AniBlock.Animations.GetCount();i++)
	{
		DataSize+=sizeof(KEY_TYPE)*AniBlock.Animations[i].Keys.GetCount();
		DataSize+=sizeof(UINT)*AniBlock.Animations[i].TimeStamps.GetCount();
	}
	return DataSize;
}


//void CD3DWOWM2ModelResource::PrintBoneInfo()
//{
//	PrintImportantLog(0,"BoneCount=%u",m_Bones.GetCount());
//	for(UINT i=0;i<m_Bones.GetCount();i++)
//	{
//		PrintImportantLog(0,"  Bone[%u]TransCount=%u",i,m_Bones[i].Translations.Animations.GetCount());
//		for(UINT j=0;j<m_Bones[i].Translations.Animations.GetCount();j++)
//		{
//			PrintImportantLog(0,"  Bone[%u]Trans[%u]TimeStamp=%u,Key=%u",i,j,
//				m_Bones[i].Translations.Animations[j].TimeStamps.GetCount(),
//				m_Bones[i].Translations.Animations[j].Keys.GetCount());
//		}
//		PrintImportantLog(0,"  Bone[%u]RotationCount=%u",i,m_Bones[i].Rotations.Animations.GetCount());
//		for(UINT j=0;j<m_Bones[i].Rotations.Animations.GetCount();j++)
//		{
//			PrintImportantLog(0,"  Bone[%u]Rotation[%u]TimeStamp=%u,Key=%u",i,j,
//				m_Bones[i].Rotations.Animations[j].TimeStamps.GetCount(),
//				m_Bones[i].Rotations.Animations[j].Keys.GetCount());
//		}
//		PrintImportantLog(0,"  Bone[%u]ScalingCount=%u",i,m_Bones[i].Scalings.Animations.GetCount());
//		for(UINT j=0;j<m_Bones[i].Scalings.Animations.GetCount();j++)
//		{
//			PrintImportantLog(0,"  Bone[%u]Scaling[%u]TimeStamp=%u,Key=%u",i,j,
//				m_Bones[i].Scalings.Animations[j].TimeStamps.GetCount(),
//				m_Bones[i].Scalings.Animations[j].Keys.GetCount());
//		}
//	}
//}

}

