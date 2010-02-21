#include "StdAfx.h"

namespace D3DLib{

IMPLEMENT_CLASS_INFO(CD3DWOWWMOModelResource,CD3DObjectResource);

CD3DWOWWMOModelResource::CD3DWOWWMOModelResource(void)
{
}

CD3DWOWWMOModelResource::CD3DWOWWMOModelResource(CD3DObjectResourceManager* pManager)
	:CD3DObjectResource(pManager)
{
	
}

CD3DWOWWMOModelResource::~CD3DWOWWMOModelResource(void)
{	
	Destory();
}


void CD3DWOWWMOModelResource::Destory()
{
	for(UINT i=0;i<m_DoodadInfos.GetCount();i++)
	{
		SAFE_RELEASE(m_DoodadInfos[i].pDoodadModel);
	}
	m_DoodadInfos.Clear();
	m_DoodadSets.Clear();
	m_Groups.Clear();
	CD3DObjectResource::Destory();
}

bool CD3DWOWWMOModelResource::Reset()
{
	return CD3DObjectResource::Reset();
}

bool CD3DWOWWMOModelResource::Restore()
{
	return CD3DObjectResource::Restore();
}

bool CD3DWOWWMOModelResource::LoadFromFile(LPCTSTR ModelFileName)
{
	IFileAccessor * pFile;

	pFile=CD3DWOWWMOModel::CreateFileAccessor();
	if(pFile==NULL)
		return false;
	if(!pFile->Open(ModelFileName,IFileAccessor::modeRead))
	{
		pFile->Release();
		return false;	
	}

	CBLZChunkFile WMOChunk;

	if(!WMOChunk.Load(pFile))
	{
		pFile->Release();
		return false;
	}
	pFile->Release();	

	Destory();

	BLZ_CHUNK_VERSION * pVersion=(BLZ_CHUNK_VERSION *)WMOChunk.GetFirstChunk(CHUNK_ID_VERSION);
	if(pVersion==NULL)
		return false;

	BLZ_CHUNK_MOHD * pHeader=(BLZ_CHUNK_MOHD *)WMOChunk.GetFirstChunk(CHUNK_ID_WMO_MOHD);
	if(pHeader==NULL)
		return false;

	BLZ_CHUNK_MOTX * pTextureNames=(BLZ_CHUNK_MOTX *)WMOChunk.GetFirstChunk(CHUNK_ID_WMO_MOTX);
	if(pHeader->MaterialCount&&pTextureNames==NULL)
		return false;

	BLZ_CHUNK_MOMT * pMaterials=(BLZ_CHUNK_MOMT *)WMOChunk.GetFirstChunk(CHUNK_ID_WMO_MOMT);
	if(pHeader->MaterialCount&&pMaterials==NULL)
		return false;

	BLZ_CHUNK_MOGN * pGroupNames=(BLZ_CHUNK_MOGN *)WMOChunk.GetFirstChunk(CHUNK_ID_WMO_MOGN);
	if(pHeader->GroupCount&&pGroupNames==NULL)
		return false;

	BLZ_CHUNK_MOGI * pGroups=(BLZ_CHUNK_MOGI *)WMOChunk.GetFirstChunk(CHUNK_ID_WMO_MOGI);
	if(pHeader->GroupCount&&pGroups==NULL)
		return false;

	BLZ_CHUNK_MOPV * pPortalVertices=(BLZ_CHUNK_MOPV *)WMOChunk.GetFirstChunk(CHUNK_ID_WMO_MOPV);
	if(pHeader->PortalCount&&pPortalVertices==NULL)
		return false;

	BLZ_CHUNK_MOPT * pPortalInfos=(BLZ_CHUNK_MOPT *)WMOChunk.GetFirstChunk(CHUNK_ID_WMO_MOPT);
	if(pHeader->PortalCount&&pPortalInfos==NULL)
		return false;

	BLZ_CHUNK_MOPR * pPortalRelationship=(BLZ_CHUNK_MOPR *)WMOChunk.GetFirstChunk(CHUNK_ID_WMO_MOPR);
	if(pHeader->PortalCount&&pPortalRelationship==NULL)
		return false;

	BLZ_CHUNK_MOVV * pVisibleBlockVertices=(BLZ_CHUNK_MOVV *)WMOChunk.GetFirstChunk(CHUNK_ID_WMO_MOVV);
	BLZ_CHUNK_MOVB * pVisibleBlockList=(BLZ_CHUNK_MOVB *)WMOChunk.GetFirstChunk(CHUNK_ID_WMO_MOVB);

	BLZ_CHUNK_MOLT * pLights=(BLZ_CHUNK_MOLT *)WMOChunk.GetFirstChunk(CHUNK_ID_WMO_MOLT);
	if(pHeader->LightCount&&pLights==NULL)
		return false;

	BLZ_CHUNK_MODS * pDoodadSets=(BLZ_CHUNK_MODS *)WMOChunk.GetFirstChunk(CHUNK_ID_WMO_MODS);
	if(pHeader->DoodadSetCount&&pDoodadSets==NULL)
		return false;

	BLZ_CHUNK_MODN * pDoodadFileNames=(BLZ_CHUNK_MODN *)WMOChunk.GetFirstChunk(CHUNK_ID_WMO_MODN);
	if(pHeader->DoodadCount&&pDoodadFileNames==NULL)
		return false;

	BLZ_CHUNK_MODD * pDoodads=(BLZ_CHUNK_MODD *)WMOChunk.GetFirstChunk(CHUNK_ID_WMO_MODD);
	if(pHeader->DoodadCount&&pDoodads==NULL)
		return false;
	
	BLZ_CHUNK_MFOG * pFog=(BLZ_CHUNK_MFOG *)WMOChunk.GetFirstChunk(CHUNK_ID_WMO_MFOG);
	if(pFog==NULL)
		return false;

	m_BoundingBox.m_Min=BLZTranslationToD3D(pHeader->BoundingBox[0]);
	m_BoundingBox.m_Max=BLZTranslationToD3D(pHeader->BoundingBox[1]);
	m_BoundingBox.Rebuild();

	BLZ_CHUNK_MCVP * pConvexVolumePlanes=(BLZ_CHUNK_MCVP *)WMOChunk.GetFirstChunk(CHUNK_ID_WMO_MCVP);

	if(!LoadDoodads(pHeader->DoodadCount,pHeader->DoodadSetCount,pDoodadSets,pDoodadFileNames,pDoodads))
		return false;

	if(!LoadGroups(ModelFileName,pHeader->GroupCount,pGroups,pGroupNames,pMaterials,pHeader->MaterialCount,pTextureNames))
		return false;

	return true;
}


UINT CD3DWOWWMOModelResource::GetDoodadSetCount()
{
	return m_DoodadSets.GetCount();
}
CD3DWOWWMOModelResource::DOODAD_SET_INFO * CD3DWOWWMOModelResource::GetDoodadSetInfo(UINT Index)
{
	return m_DoodadSets.GetObject(Index);
}

UINT CD3DWOWWMOModelResource::GetDoodadCount()
{
	return m_DoodadInfos.GetCount();
}
CD3DWOWWMOModelResource::DOODAD_INFO * CD3DWOWWMOModelResource::GetDoodadInfo(UINT Index)
{
	return m_DoodadInfos.GetObject(Index);
}

UINT CD3DWOWWMOModelResource::GetGroupCount()
{
	return m_Groups.GetCount();
}
CD3DWOWWMOModelResource::GROUP_INFO * CD3DWOWWMOModelResource::GetGroupInfo(UINT Index)
{
	return m_Groups.GetObject(Index);
}

void CD3DWOWWMOModelResource::PickResource(CNameObjectSet * pObjectSet,UINT Param)
{
	CD3DObjectResource::PickResource(pObjectSet,Param);	
	for(UINT i=0;i<m_DoodadInfos.GetCount();i++)
	{
		if(m_DoodadInfos[i].pDoodadModel)
		{
			m_DoodadInfos[i].pDoodadModel->PickResource(pObjectSet,Param);
			pObjectSet->Add(m_DoodadInfos[i].pDoodadModel);
		}
	}
}

bool CD3DWOWWMOModelResource::ToSmartStruct(CSmartStruct& Packet,CUSOFile * pUSOFile,UINT Param)
{
	if(!CD3DObjectResource::ToSmartStruct(Packet,pUSOFile,Param))
		return false;

	{
		UINT BufferSize;
		void * pBuffer=Packet.PrepareMember(BufferSize);
		CSmartStruct DoodadInfoList(pBuffer,BufferSize,true);
		for(UINT i=0;i<m_DoodadInfos.GetCount();i++)
		{
			pBuffer=DoodadInfoList.PrepareMember(BufferSize);
			CSmartStruct DoodadInfo(pBuffer,BufferSize,true);
			int ResourceID=pUSOFile->ResourceObjectToIndex(m_DoodadInfos[i].pDoodadModel);
			CHECK_SMART_STRUCT_ADD_AND_RETURN(DoodadInfo.AddMember(SST_DI_MODEL_RESOURCE,ResourceID));
			CHECK_SMART_STRUCT_ADD_AND_RETURN(DoodadInfo.AddMember(SST_DI_TRANSLATION,
				(LPCTSTR)(&m_DoodadInfos[i].Translation),sizeof(m_DoodadInfos[i].Translation)));
			CHECK_SMART_STRUCT_ADD_AND_RETURN(DoodadInfo.AddMember(SST_DI_ROTATION,
				(LPCTSTR)(&m_DoodadInfos[i].Rotation),sizeof(m_DoodadInfos[i].Rotation)));
			CHECK_SMART_STRUCT_ADD_AND_RETURN(DoodadInfo.AddMember(SST_DI_SCALING,
				(LPCTSTR)(&m_DoodadInfos[i].Scaling),sizeof(m_DoodadInfos[i].Scaling)));
			CHECK_SMART_STRUCT_ADD_AND_RETURN(DoodadInfo.AddMember(SST_DI_COLOR,m_DoodadInfos[i].Color));
			if(!DoodadInfoList.FinishMember(SST_DIL_DOODAD_INFO,DoodadInfo.GetDataLen()))
				return false;
		}
		if(!Packet.FinishMember(SST_D3DWWMR_DOODAD_INFO_LIST,DoodadInfoList.GetDataLen()))
			return false;
	}

	{
		UINT BufferSize;
		void * pBuffer=Packet.PrepareMember(BufferSize);
		CSmartStruct DoodadSetList(pBuffer,BufferSize,true);
		for(UINT i=0;i<m_DoodadSets.GetCount();i++)
		{
			pBuffer=DoodadSetList.PrepareMember(BufferSize);
			CSmartStruct DoodadSet(pBuffer,BufferSize,true);			
			CHECK_SMART_STRUCT_ADD_AND_RETURN(DoodadSet.AddMember(SST_DS_NAME,(LPCTSTR)m_DoodadSets[i].Name));			
			CHECK_SMART_STRUCT_ADD_AND_RETURN(DoodadSet.AddMember(SST_DS_START_DOODAD,m_DoodadSets[i].StartDoodad));
			CHECK_SMART_STRUCT_ADD_AND_RETURN(DoodadSet.AddMember(SST_DS_DOODAD_COUNT,m_DoodadSets[i].DoodadCount));
			if(!DoodadSetList.FinishMember(SST_DSL_DOODAD_SET,DoodadSet.GetDataLen()))
				return false;
		}
		if(!Packet.FinishMember(SST_D3DWWMR_DOODAD_SET_LIST,DoodadSetList.GetDataLen()))
			return false;
	}

	{
		UINT BufferSize;
		void * pBuffer=Packet.PrepareMember(BufferSize);
		CSmartStruct GroupList(pBuffer,BufferSize,true);
		for(UINT i=0;i<m_Groups.GetCount();i++)
		{
			pBuffer=GroupList.PrepareMember(BufferSize);
			CSmartStruct GroupInfo(pBuffer,BufferSize,true);			
			CHECK_SMART_STRUCT_ADD_AND_RETURN(GroupInfo.AddMember(SST_GI_INDEX,m_Groups[i].Index));
			CHECK_SMART_STRUCT_ADD_AND_RETURN(GroupInfo.AddMember(SST_GI_NAME,(LPCTSTR)m_Groups[i].Name));			
			CHECK_SMART_STRUCT_ADD_AND_RETURN(GroupInfo.AddMember(SST_GI_FLAG,m_Groups[i].Flags));
			CHECK_SMART_STRUCT_ADD_AND_RETURN(GroupInfo.AddMember(SST_GI_BOUNDING_BOX,
				(LPCTSTR)(&m_Groups[i].BoundingBox),sizeof(m_Groups[i].BoundingBox)));
			for(UINT j=0;j<m_Groups[i].GroupSubMeshList.GetCount();j++)
			{
				int ResourceID=pUSOFile->ResourceObjectToIndex(m_Groups[i].GroupSubMeshList[j]);
				CHECK_SMART_STRUCT_ADD_AND_RETURN(GroupInfo.AddMember(SST_GI_SUB_MESH,ResourceID));
			}
			
			if(!GroupList.FinishMember(SST_GL_GROUP_INFO,GroupInfo.GetDataLen()))
				return false;
		}
		if(!Packet.FinishMember(SST_D3DWWMR_GROUP_LIST,GroupList.GetDataLen()))
			return false;
	}

	return true;
}
bool CD3DWOWWMOModelResource::FromSmartStruct(CSmartStruct& Packet,CUSOFile * pUSOFile,UINT Param)
{
	Destory();

	if(!CD3DObjectResource::FromSmartStruct(Packet,pUSOFile,Param))
		return false;


	UINT DoodadInfoCount=0;
	UINT DoodadSetCount=0;
	UINT GroupCount=0;

	void * Pos=Packet.GetFirstMemberPosition();
	while(Pos)
	{
		WORD MemberID;
		CSmartStruct SubPacket=Packet.GetNextMember(Pos,MemberID);
		switch(MemberID)
		{	
		case SST_D3DWWMR_DOODAD_INFO_LIST:
			DoodadInfoCount=SubPacket.GetMemberCount();
			break;
		case SST_D3DWWMR_DOODAD_SET_LIST:
			DoodadSetCount=SubPacket.GetMemberCount();
			break;
		case SST_D3DWWMR_GROUP_LIST:
			GroupCount=SubPacket.GetMemberCount();
			break;
		}
	}

	m_DoodadInfos.Resize(DoodadInfoCount);
	m_DoodadSets.Resize(DoodadSetCount);
	m_Groups.Resize(GroupCount);

	DoodadInfoCount=0;
	DoodadSetCount=0;
	GroupCount=0;


	Pos=Packet.GetFirstMemberPosition();
	while(Pos)
	{
		WORD MemberID;
		CSmartStruct SubPacket=Packet.GetNextMember(Pos,MemberID);
		switch(MemberID)
		{	
		case SST_D3DWWMR_DOODAD_INFO_LIST:
			{
				void * SubPos=SubPacket.GetFirstMemberPosition();
				while(SubPos)
				{
					WORD SubMemberID;
					CSmartStruct DoodadInfo=Packet.GetNextMember(SubPos,SubMemberID);
					if(SubMemberID==SST_DIL_DOODAD_INFO)
					{
						int ResourceID=DoodadInfo.GetMember(SST_DI_MODEL_RESOURCE);
						m_DoodadInfos[DoodadInfoCount].pDoodadModel=(CD3DWOWM2ModelResource *)pUSOFile->ResourceIndexToObject(ResourceID,GET_CLASS_INFO(CD3DWOWM2ModelResource));
						if(m_DoodadInfos[DoodadInfoCount].pDoodadModel)
							m_DoodadInfos[DoodadInfoCount].pDoodadModel->AddUseRef();
						memcpy(&(m_DoodadInfos[DoodadInfoCount].Translation),(LPCTSTR)DoodadInfo.GetMember(SST_DI_TRANSLATION),
							sizeof(m_DoodadInfos[DoodadInfoCount].Translation));
						memcpy(&(m_DoodadInfos[DoodadInfoCount].Rotation),(LPCTSTR)DoodadInfo.GetMember(SST_DI_ROTATION),
							sizeof(m_DoodadInfos[DoodadInfoCount].Rotation));
						memcpy(&(m_DoodadInfos[DoodadInfoCount].Scaling),(LPCTSTR)DoodadInfo.GetMember(SST_DI_SCALING),
							sizeof(m_DoodadInfos[DoodadInfoCount].Scaling));
						m_DoodadInfos[DoodadInfoCount].Color=DoodadInfo.GetMember(SST_DI_COLOR);
						DoodadInfoCount++;
					}
				}
			}
			break;
		case SST_D3DWWMR_DOODAD_SET_LIST:
			{
				void * SubPos=SubPacket.GetFirstMemberPosition();
				while(SubPos)
				{
					WORD SubMemberID;
					CSmartStruct DoodadSet=Packet.GetNextMember(SubPos,SubMemberID);
					if(SubMemberID==SST_DSL_DOODAD_SET)
					{
						m_DoodadSets[DoodadSetCount].Name=(LPCTSTR)DoodadSet.GetMember(SST_DS_NAME);
						m_DoodadSets[DoodadSetCount].StartDoodad=DoodadSet.GetMember(SST_DS_START_DOODAD);
						m_DoodadSets[DoodadSetCount].DoodadCount=DoodadSet.GetMember(SST_DS_DOODAD_COUNT);
						DoodadSetCount++;
					}
				}
			}
			break;
		case SST_D3DWWMR_GROUP_LIST:
			{
				void * SubPos=SubPacket.GetFirstMemberPosition();
				while(SubPos)
				{
					WORD SubMemberID;
					CSmartStruct GroupInfo=Packet.GetNextMember(SubPos,SubMemberID);
					if(SubMemberID==SST_GL_GROUP_INFO)
					{
						void * GroupPos=GroupInfo.GetFirstMemberPosition();
						while(GroupPos)
						{
							WORD GroupMemberID;
							CSmartValue Value=GroupInfo.GetNextMember(GroupPos,GroupMemberID);
							switch(GroupMemberID)
							{
							case SST_GI_INDEX:
								m_Groups[GroupCount].Index=Value;
								break;
							case SST_GI_NAME:
								m_Groups[GroupCount].Name=(LPCTSTR)Value;
								break;
							case SST_GI_FLAG:
								m_Groups[GroupCount].Flags=Value;
								break;
							case SST_GI_BOUNDING_BOX:
								memcpy(&(m_Groups[GroupCount].BoundingBox),(LPCTSTR)Value,sizeof(m_Groups[GroupCount].BoundingBox));
								break;
							case SST_GI_SUB_MESH:
								{
									int ResourceID=Value;
									CD3DSubMesh * pSubMesh=(CD3DSubMesh *)pUSOFile->ResourceIndexToObject(ResourceID,GET_CLASS_INFO(CD3DSubMesh));
									if(pSubMesh)
									{
										pSubMesh->AddUseRef();
										m_Groups[GroupCount].GroupSubMeshList.Add(pSubMesh);
									}
									
								}
								break;
							}
						}
						GroupCount++;
					}
				}
			}
			break;
		}
	}

	return true;
}
UINT CD3DWOWWMOModelResource::GetSmartStructSize(UINT Param)
{
	UINT Size=CD3DObjectResource::GetSmartStructSize(Param);
	Size+=SMART_STRUCT_STRUCT_MEMBER_SIZE(0);
	for(UINT i=0;i<m_DoodadInfos.GetCount();i++)
	{
		Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(int));
		Size+=SMART_STRUCT_STRING_MEMBER_SIZE(sizeof(m_DoodadInfos[i].Translation));
		Size+=SMART_STRUCT_STRING_MEMBER_SIZE(sizeof(m_DoodadInfos[i].Rotation));
		Size+=SMART_STRUCT_STRING_MEMBER_SIZE(sizeof(m_DoodadInfos[i].Scaling));
		Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(m_DoodadInfos[i].Color));
		Size+=SMART_STRUCT_STRUCT_MEMBER_SIZE(0);
	}
	Size+=SMART_STRUCT_STRUCT_MEMBER_SIZE(0);
	for(UINT i=0;i<m_DoodadSets.GetCount();i++)
	{		
		Size+=SMART_STRUCT_STRING_MEMBER_SIZE(m_DoodadSets[i].Name.GetLength());
		Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(m_DoodadSets[i].StartDoodad));
		Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(m_DoodadSets[i].DoodadCount));
		Size+=SMART_STRUCT_STRUCT_MEMBER_SIZE(0);
	}
	Size+=SMART_STRUCT_STRUCT_MEMBER_SIZE(0);
	for(UINT i=0;i<m_Groups.GetCount();i++)
	{	
		Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(m_Groups[i].Index));
		Size+=SMART_STRUCT_STRING_MEMBER_SIZE(m_Groups[i].Name.GetLength());
		Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(m_Groups[i].Flags));
		Size+=SMART_STRUCT_STRING_MEMBER_SIZE(sizeof(m_Groups[i].BoundingBox));
		for(UINT j=0;j<m_Groups[i].GroupSubMeshList.GetCount();j++)
		{
			Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(int));			
		}
		Size+=SMART_STRUCT_STRUCT_MEMBER_SIZE(0);
	}
	return Size;
}

bool CD3DWOWWMOModelResource::LoadGroup(GROUP_INFO& GroupInfo,LPCTSTR ModelFileName,BLZ_CHUNK_MOMT * pMaterials,UINT MaterialCount,BLZ_CHUNK_MOTX * pTextureNames)
{
	IFileAccessor * pFile;

	pFile=CD3DWOWWMOModel::CreateFileAccessor();
	if(pFile==NULL)
		return false;
	if(!pFile->Open(ModelFileName,IFileAccessor::modeRead))
	{
		pFile->Release();
		return false;	
	}

	CBLZChunkFile WMOChunk;

	if(!WMOChunk.Load(pFile))
	{
		pFile->Release();
		return false;
	}
	pFile->Release();

	BLZ_CHUNK_MOGP * pGroup=(BLZ_CHUNK_MOGP *)WMOChunk.GetFirstChunk(CHUNK_ID_WMO_MOGP);
	if(pGroup==NULL)
		return false;

	CBLZChunkFile SubChunk;

	if(!SubChunk.Load((BYTE *)pGroup+sizeof(BLZ_CHUNK_MOGP),pGroup->ChunkSize-sizeof(BLZ_CHUNK_MOGP)+sizeof(BLZ_CHUNK_HEADER)))
		return false;

	BLZ_CHUNK_MOPY * pTriangleMaterialInfo=(BLZ_CHUNK_MOPY *)SubChunk.GetFirstChunk(CHUNK_ID_WMO_MOPY);
	if(pTriangleMaterialInfo==NULL)
		return false;
	UINT TriangleMaterialInfoCount=pTriangleMaterialInfo->ChunkSize/sizeof(WMOTriangleMaterialInfo);

	BLZ_CHUNK_MOVI * pIndices=(BLZ_CHUNK_MOVI *)SubChunk.GetFirstChunk(CHUNK_ID_WMO_MOVI);
	if(pIndices==NULL)
		return false;
	UINT TotalIndexCount=pIndices->ChunkSize/sizeof(UINT16);
	UINT TotalTriangleCount=TotalIndexCount/3;

	BLZ_CHUNK_MOVT * pVertices=(BLZ_CHUNK_MOVT *)SubChunk.GetFirstChunk(CHUNK_ID_WMO_MOVT);
	if(pVertices==NULL)
		return false;
	UINT TotalVertexCount=pVertices->ChunkSize/sizeof(CD3DVector3);

	BLZ_CHUNK_MONR * pNormals=(BLZ_CHUNK_MONR *)SubChunk.GetFirstChunk(CHUNK_ID_WMO_MONR);
	if(pNormals==NULL)
		return false;
	UINT TotalNormalCount=pNormals->ChunkSize/sizeof(CD3DVector3);

	BLZ_CHUNK_MOTV * pTextureUVs=(BLZ_CHUNK_MOTV *)SubChunk.GetFirstChunk(CHUNK_ID_WMO_MOTV);
	if(pTextureUVs==NULL)
		return false;
	UINT TotalUVCount=pTextureUVs->ChunkSize/sizeof(CD3DVector2);

	BLZ_CHUNK_MOBA * pRenderBatchs=(BLZ_CHUNK_MOBA *)SubChunk.GetFirstChunk(CHUNK_ID_WMO_MOBA);
	if(pRenderBatchs==NULL)
		return false;
	UINT RenderBatchCount=pRenderBatchs->ChunkSize/sizeof(WMORenderBatch);

	BLZ_CHUNK_MOLR * pLightRefrences=(BLZ_CHUNK_MOLR *)SubChunk.GetFirstChunk(CHUNK_ID_WMO_MOLR);

	BLZ_CHUNK_MODR * pDoodadRefrences=(BLZ_CHUNK_MODR *)SubChunk.GetFirstChunk(CHUNK_ID_WMO_MODR);



	BLZ_CHUNK_MOBN * pBSPNodes=(BLZ_CHUNK_MOBN *)SubChunk.GetFirstChunk(CHUNK_ID_WMO_MOBN);
	if(pBSPNodes==NULL)
		return false;
	UINT BSPNodeCount=pBSPNodes->ChunkSize/(sizeof(BLZ_CHUNK_MOBN)-sizeof(BLZ_CHUNK_HEADER));
	

	BLZ_CHUNK_MOBR * pTriangleIndices=(BLZ_CHUNK_MOBR *)SubChunk.GetFirstChunk(CHUNK_ID_WMO_MOBR);
	if(pTriangleIndices==NULL)
		return false;
	UINT TriangleIndexCount=pTriangleIndices->ChunkSize/sizeof(UINT16);

	BLZ_CHUNK_MOCV * pVertexColors=(BLZ_CHUNK_MOCV *)SubChunk.GetFirstChunk(CHUNK_ID_WMO_MOCV);

	BLZ_CHUNK_MLIQ * pLiquidInfos=(BLZ_CHUNK_MLIQ *)SubChunk.GetFirstChunk(CHUNK_ID_WMO_MLIQ);

	UINT StartBatch=0;
	UINT TotalBatchCount=pGroup->Batches[0]+pGroup->Batches[1]+pGroup->Batches[2];
	if(RenderBatchCount<TotalBatchCount)
		return false;

	for(UINT i=StartBatch;i<TotalBatchCount;i++)
	{
		CD3DSubMesh * pD3DSubMesh=new CD3DSubMesh;

		UINT StartIndex=pRenderBatchs->RenderBatchs[i].StartIndex;
		UINT IndexCount=pRenderBatchs->RenderBatchs[i].IndexCount;
		UINT StartVertex=pRenderBatchs->RenderBatchs[i].StartVertex;
		UINT VertexCount=pRenderBatchs->RenderBatchs[i].EndVertex-pRenderBatchs->RenderBatchs[i].StartVertex+1;


		pD3DSubMesh->GetVertexFormat().FVF=D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1;
		pD3DSubMesh->GetVertexFormat().VertexSize=sizeof(MODEL_VERTEXT);
		pD3DSubMesh->GetVertexFormat().IndexSize=sizeof(WORD);
		pD3DSubMesh->SetPrimitiveType(D3DPT_TRIANGLELIST);
		pD3DSubMesh->SetPrimitiveCount(IndexCount/3);		
		pD3DSubMesh->SetVertexCount(VertexCount);
		pD3DSubMesh->SetIndexCount(IndexCount);
		
		int MaterialIndex=pRenderBatchs->RenderBatchs[i].Material;
		

		pD3DSubMesh->AllocDXIndexBuffer(m_pManager->GetDevice());

		CEasyArray<WORD> VertexIndexList;

		VertexIndexList.Reserve(VertexCount);

		WORD * pModelIndices=NULL;
		pD3DSubMesh->GetDXIndexBuffer()->Lock(0,0,(LPVOID *)&pModelIndices,0);
		for(UINT j=0;j<IndexCount;j++)
		{
			UINT Index=((StartIndex+j)/3)*3+(2-(StartIndex+j)%3);			
			if(Index>=TotalIndexCount)
			{
				PrintSystemLog(0,"Error Index");
			}			
			WORD VertexIndex=pIndices->VertexIndices[Index];
			if(VertexIndex>=TotalVertexCount)
			{
				PrintSystemLog(0,"Error Vertex");
			}	
			pModelIndices[j]=RebuildVertexIndex(VertexIndexList,VertexIndex);
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
				pModelVertices[j].Pos=BLZTranslationToD3D(pVertices->Vertices[VertexIndex]);
						
				pModelVertices[j].Normal=BLZTranslationToD3D(pNormals->Normals[VertexIndex]);
				pModelVertices[j].TextureCoord=pTextureUVs->TextureCoordinates[VertexIndex];	
				
				pModelVertices[j].Diffuse=pMaterials->Materials[MaterialIndex].TextureColor1;
			}

			pD3DSubMesh->GetDXVertexBuffer()->Unlock();
		}
		else
		{
			PrintSystemLog(0,"Error VertexCount");
		}

		if(!MakeMaterial(pD3DSubMesh->GetMaterial(),pMaterials->Materials[MaterialIndex],pTextureNames))
			return false;

		
		pD3DSubMesh->CreateBounding();

		pD3DSubMesh->SetID(GroupInfo.Index*1000+i);
		CEasyString SubMeshName;
		SubMeshName.Format("%03d-%03d",GroupInfo.Index,i);
		pD3DSubMesh->SetName(SubMeshName);

		GroupInfo.GroupSubMeshList.Add(pD3DSubMesh);
		m_SubMeshList.Add(pD3DSubMesh);
	}

	return true;
}

bool CD3DWOWWMOModelResource::LoadDoodads(UINT DoodadCount,UINT DoodadSetCount,BLZ_CHUNK_MODS * pDoodadSets,BLZ_CHUNK_MODN * pDoodadFileNames,BLZ_CHUNK_MODD * pDoodads)
{
	if(DoodadCount)
	{
		m_DoodadInfos.Resize(DoodadCount);
		for(UINT i=0;i<DoodadCount;i++)
		{
			m_DoodadInfos[i].pDoodadModel=NULL;
			if(pDoodads->Doodads[i].NameIndex<pDoodadFileNames->ChunkSize)
			{
				CEasyString ModelFileName=pDoodadFileNames->DoodadFileNames+pDoodads->Doodads[i].NameIndex;
				CEasyString SkinFileName;
				int Pos=ModelFileName.ReverseFind('.');
				if(Pos>=0)
				{
					ModelFileName=ModelFileName.Left(Pos);
					SkinFileName=ModelFileName+"00.skin";
					ModelFileName+=".m2";

					CEasyString ObjectName=ModelFileName+"_"+GetPathFileName(SkinFileName);

					CD3DWOWM2ModelResource* pResource=
						dynamic_cast<CD3DWOWM2ModelResource*>(m_pManager->GetResource(ObjectName));
					if(!pResource)
					{
						pResource=new CD3DWOWM2ModelResource(m_pManager);
						if(pResource->LoadFromFile(ModelFileName,SkinFileName))
						{
							PrintSystemLog(0,"¼ÓÔØÁË[%s]",(LPCTSTR)ModelFileName);
							if(!m_pManager->AddResource(pResource,ObjectName))
							{
								pResource->Release();
								pResource=NULL;
							}
						}
						else
						{
							pResource->Release();
							pResource=NULL;
						}						
					}
					else
					{
						pResource->AddUseRef();
					}	
					m_DoodadInfos[i].pDoodadModel=pResource;
				}	
				
				m_DoodadInfos[i].Translation=BLZTranslationToD3D(pDoodads->Doodads[i].Translation);
				m_DoodadInfos[i].Rotation=BLZRotationToD3D(pDoodads->Doodads[i].Rotation);
				m_DoodadInfos[i].Scaling=pDoodads->Doodads[i].Scaling;
				m_DoodadInfos[i].Color=pDoodads->Doodads[i].Color;
			}			
		}
		if(DoodadSetCount)
		{
			m_DoodadSets.Resize(DoodadSetCount);
			for(UINT i=0;i<DoodadSetCount;i++)
			{
				m_DoodadSets[i].Name=pDoodadSets->DoodadSets[i].Name;
				m_DoodadSets[i].StartDoodad=pDoodadSets->DoodadSets[i].FirstInstanceIndex;
				m_DoodadSets[i].DoodadCount=pDoodadSets->DoodadSets[i].DoodadCount;
			}
		}
	}
	return true;
}

bool CD3DWOWWMOModelResource::LoadGroups(LPCTSTR ModelFileName,UINT GroupCount,BLZ_CHUNK_MOGI * pGroups,BLZ_CHUNK_MOGN * pGroupNames,BLZ_CHUNK_MOMT * pMaterials,UINT MaterialCount,BLZ_CHUNK_MOTX * pTextureNames)
{
	if(GroupCount)
	{
		m_Groups.Resize(GroupCount);
		CEasyString GroupFileName=ModelFileName;
		CEasyString GroupFileExt;
		int Pos=GroupFileName.ReverseFind('.');
		if(Pos>=0)
		{
			GroupFileExt=GroupFileName.Right(GroupFileName.GetLength()-Pos);
			GroupFileName=GroupFileName.Left(Pos);
		}

		for(UINT i=0;i<GroupCount;i++)
		{
			if(pGroups->Group[i].NameOffset>=0&&pGroups->Group[i].NameOffset<(int)pGroupNames->ChunkSize)
			{
				m_Groups[i].Name=pGroupNames->GroupFileNames+pGroups->Group[i].NameOffset;
			}
			m_Groups[i].Index=i;
			m_Groups[i].Flags=pGroups->Group[i].Flags;
			m_Groups[i].BoundingBox.m_Min=BLZTranslationToD3D(pGroups->Group[i].BoundingBox[0]);
			m_Groups[i].BoundingBox.m_Max=BLZTranslationToD3D(pGroups->Group[i].BoundingBox[1]);
			m_Groups[i].BoundingBox.Rebuild();
			CEasyString FileName;
			FileName.Format("%s_%03d%s",
				(LPCTSTR)GroupFileName,i,(LPCTSTR)GroupFileExt);
			if(!LoadGroup(m_Groups[i],FileName,pMaterials,MaterialCount,pTextureNames))
				return false;
		}
	}
	return true;
}



WORD CD3DWOWWMOModelResource::RebuildVertexIndex(CEasyArray<WORD>& VertexIndexList,WORD VertexIndex)
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


bool CD3DWOWWMOModelResource::MakeMaterial(CD3DSubMeshMaterial& SubMeshMaterial,WMOMaterial& MaterialInfo,BLZ_CHUNK_MOTX * pTextureNames)
{
	
	LPCTSTR TextureFileName1=pTextureNames->TextureFileNames+MaterialInfo.Texture1;
	LPCTSTR TextureFileName2=pTextureNames->TextureFileNames+MaterialInfo.Texture2;

	SubMeshMaterial.AddTexture(
		m_pManager->GetDevice()->GetTextureManager()->LoadTexture(TextureFileName1),0);
	if(TextureFileName2[0])
	{
		SubMeshMaterial.AddTexture(
			m_pManager->GetDevice()->GetTextureManager()->LoadTexture(TextureFileName2),0);
	}
	

	D3DCOLORVALUE WhiteColor={1.0f,1.0f,1.0f,1.0f};
	D3DCOLORVALUE GrayColor={0.8f,0.8f,0.8f,1.0f};
	D3DCOLORVALUE BlackColor={0.0f,0.0f,0.0f,1.0f};

	SubMeshMaterial.GetMaterial().Ambient=WhiteColor;
	SubMeshMaterial.GetMaterial().Diffuse=WhiteColor;
	SubMeshMaterial.GetMaterial().Specular=WhiteColor;
	SubMeshMaterial.GetMaterial().Emissive=BlackColor;
	SubMeshMaterial.GetMaterial().Power=40.0f;


	SubMeshMaterial.SetFX(BuildFX(MaterialInfo.BlendMode,MaterialInfo.Flags));

	return true;
}


CD3DFX * CD3DWOWWMOModelResource::BuildFX(UINT BlendMode,UINT TextureFlag)
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
	CEasyString PShader;
	
	FXName.Format("WMOModel_0x%X_0x%X",
		BlendMode,
		TextureFlag);
	
	PSDiffuseFunction=" * Diffuse";	
	VSDiffuseFunction="CaculateDiffuse(Normal)";

	EnableFog="False";
	
	if(TextureFlag&WMOMF_NO_CULL)
	{
		CullMode="None";
	}
	else
	{
		CullMode="CCW";
	}

		
	EnableZWrite="True";

	EnableAlphaBlend="False";	
	EnableAlphaTest="False";
	BlendOp="Add";
	SrcBlend="SrcAlpha";
	DestBlend="InvSrcAlpha";
	switch(BlendMode)
	{	
	case WMOBM_OPAQUE:		
		break;
	case WMOBM_ALPHA_TEST:
		EnableAlphaTest="True";
		break;	
	}	

	PShader="PShaderWithNormal";

	IFileAccessor * pFile;

	CEasyString FxContent;


	pFile=CD3DWOWM2Model::CreateFileAccessor();
	if(pFile==NULL)
		return NULL;
	if(!pFile->Open(WMO_MODEL_FX_FILE_NAME,IFileAccessor::modeRead))
	{
		pFile->Release();
		return NULL;	
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
	FxContent.Replace("<PShader>",PShader);

	CD3DFX * pFX=m_pManager->GetDevice()->GetFXManager()->LoadFXFromMemory(FXName,FxContent.GetBuffer(),FxContent.GetLength());	
	return pFX;
}

void CD3DWOWWMOModelResource::MakeSubMesh(WMOTriangleMaterialInfo * pTriangles,UINT TriangleCount,CEasyArray<SUBMESH_INFO>& SubMeshInfo)
{
	int OldMaterialIndex=pTriangles[0].MaterialIndex;
	UINT StartTriangle=0;
	for(UINT i=0;i<TriangleCount;i++)
	{
		if(pTriangles[i].MaterialIndex!=OldMaterialIndex)
		{
			if(OldMaterialIndex>=0)
			{
				SUBMESH_INFO Info;
				Info.StartIndex=StartTriangle*3;
				Info.IndexCount=(i-StartTriangle)*3;
				Info.MaterialIndex=OldMaterialIndex;
				SubMeshInfo.Add(Info);
			}
			OldMaterialIndex=pTriangles[i].MaterialIndex;
			StartTriangle=i;
		}
	}
	if(OldMaterialIndex>=0)
	{
		SUBMESH_INFO Info;
		Info.StartIndex=StartTriangle*3;
		Info.IndexCount=(TriangleCount-StartTriangle)*3;
		Info.MaterialIndex=pTriangles[StartTriangle].MaterialIndex;
		SubMeshInfo.Add(Info);
	}
	
}

}