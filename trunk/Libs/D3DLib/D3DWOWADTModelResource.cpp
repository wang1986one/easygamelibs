/****************************************************************************/
/*                                                                          */
/*      文件名:    D3DWOWADTModelResource.cpp                               */
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


const WORD	ADT_HOLE_MASK[4][4]={	{0x1,	0x2,	0x4,	0x8},
									{0x10,	0x20,	0x40,	0x80},
									{0x100,	0x200,	0x400,	0x800},
									{0x1000,0x2000,	0x4000,	0x8000}};


IMPLEMENT_CLASS_INFO(CD3DWOWADTModelResource,CD3DObjectResource);

CD3DWOWADTModelResource::CD3DWOWADTModelResource(void)
{
}

CD3DWOWADTModelResource::CD3DWOWADTModelResource(CD3DObjectResourceManager* pManager)
	:CD3DObjectResource(pManager)
{

}

CD3DWOWADTModelResource::~CD3DWOWADTModelResource(void)
{	
	for(UINT i=0;i<m_M2ObjectList.GetCount();i++)
	{
		SAFE_RELEASE(m_M2ObjectList[i].pModelResource);
	}
	for(UINT i=0;i<m_WMOObjectList.GetCount();i++)
	{
		SAFE_RELEASE(m_WMOObjectList[i].pModelResource);
	}	
	m_M2ObjectList.Clear();
	m_WMOObjectList.Clear();
	Destory();
}

void CD3DWOWADTModelResource::Destory()
{
	CD3DObjectResource::Destory();
}

bool CD3DWOWADTModelResource::Reset()
{
	return CD3DObjectResource::Reset();
}

bool CD3DWOWADTModelResource::Restore()
{
	return CD3DObjectResource::Restore();
}

bool CD3DWOWADTModelResource::LoadFromFile(LPCTSTR ModelFileName)
{
	IFileAccessor * pFile;


	Destory();

	pFile=CD3DWOWWMOModel::CreateFileAccessor();
	if(pFile==NULL)
		return false;
	if(!pFile->Open(ModelFileName,IFileAccessor::modeRead))
	{
		pFile->Release();
		return false;	
	}

	CBLZChunkFile ADTChunk;

	if(!ADTChunk.Load(pFile))
	{
		pFile->Release();
		return false;
	}
	pFile->Release();	

	BLZ_CHUNK_MHDR * pMHDR=(BLZ_CHUNK_MHDR *)ADTChunk.GetFirstChunk(CHUNK_ID_ADT_MHDR);

	UINT OffsetStart=sizeof(BLZ_CHUNK_VERSION)+sizeof(BLZ_CHUNK_HEADER);

	BLZ_CHUNK_MCIN * pMCIN=(BLZ_CHUNK_MCIN *)ADTChunk.GetChunkByOffset(CHUNK_ID_ADT_MCIN,OffsetStart+pMHDR->MCINChunkOffset);

	BLZ_CHUNK_MTEX * pMTEX=(BLZ_CHUNK_MTEX *)ADTChunk.GetChunkByOffset(CHUNK_ID_ADT_MTEX,OffsetStart+pMHDR->MTEXChunkOffset);

	BLZ_CHUNK_MMDX * pMMDX=(BLZ_CHUNK_MMDX *)ADTChunk.GetChunkByOffset(CHUNK_ID_ADT_MMDX,OffsetStart+pMHDR->MMDXChunkOffset);

	BLZ_CHUNK_MMID * pMMID=(BLZ_CHUNK_MMID *)ADTChunk.GetChunkByOffset(CHUNK_ID_ADT_MMID,OffsetStart+pMHDR->MMIDChunkOffset);

	BLZ_CHUNK_MWMO * pMWMO=(BLZ_CHUNK_MWMO *)ADTChunk.GetChunkByOffset(CHUNK_ID_ADT_MWMO,OffsetStart+pMHDR->MWMOChunkOffset);

	BLZ_CHUNK_MWID * pMWID=(BLZ_CHUNK_MWID *)ADTChunk.GetChunkByOffset(CHUNK_ID_ADT_MWID,OffsetStart+pMHDR->MWIDChunkOffset);

	BLZ_CHUNK_MDDF * pMDDF=(BLZ_CHUNK_MDDF *)ADTChunk.GetChunkByOffset(CHUNK_ID_ADT_MDDF,OffsetStart+pMHDR->MDDFChunkOffset);

	BLZ_CHUNK_MODF * pMODF=(BLZ_CHUNK_MODF *)ADTChunk.GetChunkByOffset(CHUNK_ID_ADT_MODF,OffsetStart+pMHDR->MODFChunkOffset);

	BLZ_CHUNK_MH2O * pMH2O=(BLZ_CHUNK_MH2O *)ADTChunk.GetChunkByOffset(CHUNK_ID_ADT_MH2O,OffsetStart+pMHDR->MH2OChunkOffset);

	BLZ_CHUNK_MFBO * pMFBO=(BLZ_CHUNK_MFBO *)ADTChunk.GetChunkByOffset(CHUNK_ID_ADT_MFBO,OffsetStart+pMHDR->MFBOChunkOffset);

	BLZ_CHUNK_MTFX * pMTFX=(BLZ_CHUNK_MTFX *)ADTChunk.GetChunkByOffset(CHUNK_ID_ADT_MTFX,OffsetStart+pMHDR->MTFXChunkOffset);

	CEasyArray<CSmartPtr<CD3DTexture> >		TextureList;
	CEasyArray<TEXTURE_LAYER_INFO>			TextureLayerInfos;

	
	UINT Ptr=0;
	UINT TextureCount=0;
	while(Ptr<pMTEX->ChunkSize)
	{
		TextureCount++;
		Ptr+=strlen(pMTEX->TextureFileNames+Ptr)+1;
	}
	TextureList.Resize(TextureCount);
	Ptr=0;
	TextureCount=0;
	while(Ptr<pMTEX->ChunkSize)
	{
		TextureList[TextureCount]=m_pManager->GetDevice()->GetTextureManager()->LoadTexture(pMTEX->TextureFileNames+Ptr);
		TextureCount++;
		Ptr+=strlen(pMTEX->TextureFileNames+Ptr)+1;
	}

	

	CD3DVector3 MapAreaPos;


	CBLZChunkFile::CChunkList * pChunkList=ADTChunk.GetChunks(CHUNK_ID_ADT_MCNK);
	m_SubMeshList.Create(pChunkList->GetCount());
	TextureLayerInfos.Resize(pChunkList->GetCount());
	for(UINT i=0;i<pChunkList->GetCount();i++)
	{
		CBLZChunkFile MCNKChunk;

		MCNKChunk.Load((BYTE *)((*pChunkList)[i]),((*pChunkList)[i])->ChunkSize+sizeof(BLZ_CHUNK_HEADER));

		BLZ_CHUNK_MCNK * pMCNK=(BLZ_CHUNK_MCNK *)MCNKChunk.GetFirstChunk(CHUNK_ID_ADT_MCNK);
		BLZ_CHUNK_MCVT * pMCVT=(BLZ_CHUNK_MCVT *)MCNKChunk.GetChunkByOffset(CHUNK_ID_ADT_MCVT,pMCNK->MCVTOffset);
		BLZ_CHUNK_MCCV * pMCCV=(BLZ_CHUNK_MCCV *)MCNKChunk.GetChunkByOffset(CHUNK_ID_ADT_MCCV,pMCNK->MCCVOffset);
		BLZ_CHUNK_MCNR * pMCNR=(BLZ_CHUNK_MCNR *)MCNKChunk.GetChunkByOffset(CHUNK_ID_ADT_MCNR,pMCNK->MCNROffset);
		BLZ_CHUNK_MCLY * pMCLY=(BLZ_CHUNK_MCLY *)MCNKChunk.GetChunkByOffset(CHUNK_ID_ADT_MCLY,pMCNK->MCLYOffset);
		BLZ_CHUNK_MCAL * pMCAL=(BLZ_CHUNK_MCAL *)MCNKChunk.GetChunkByOffset(CHUNK_ID_ADT_MCAL,pMCNK->MCALOffset);
		BLZ_CHUNK_MCSH * pMCSH=(BLZ_CHUNK_MCSH *)MCNKChunk.GetChunkByOffset(CHUNK_ID_ADT_MCSH,pMCNK->MCSHOffset);
		

		//if(pMCCV)
		//{
		//	PrintSystemLog(0,"%d有顶点色",
		//		i);
		//}
		

		if(!LoadAlphaLayer(TextureLayerInfos[i],pMCNK->LayerCount,pMCLY,pMCAL))
			return false;

		if((pMCNK->Flags&MCNK_FLAG_MCSH_AVAILABLE)&&pMCSH)
		{
			LoadShadowMap(TextureLayerInfos[i],pMCSH);
		}


		CD3DSubMesh * pD3DSubMesh=new CD3DSubMesh;


		UINT IndexCount=3*4*64;

		UINT VertexCount=145;

		pD3DSubMesh->GetVertexFormat().FVF=D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1;
		pD3DSubMesh->GetVertexFormat().VertexSize=sizeof(MODEL_VERTEXT);
		pD3DSubMesh->GetVertexFormat().IndexSize=sizeof(WORD);
		pD3DSubMesh->SetPrimitiveType(D3DPT_TRIANGLELIST);
		pD3DSubMesh->SetPrimitiveCount(IndexCount/3);		
		pD3DSubMesh->SetVertexCount(VertexCount);
		pD3DSubMesh->SetIndexCount(IndexCount);

		pD3DSubMesh->AllocDXIndexBuffer(m_pManager->GetDevice());
		pD3DSubMesh->AllocDXVertexBuffer(m_pManager->GetDevice());

		D3DCOLORVALUE WhiteColor={1.0f,1.0f,1.0f,1.0f};
		D3DCOLORVALUE GrayColor={0.8f,0.8f,0.8f,1.0f};
		D3DCOLORVALUE BlackColor={0.0f,0.0f,0.0f,1.0f};

		pD3DSubMesh->GetMaterial().GetMaterial().Ambient=WhiteColor;
		pD3DSubMesh->GetMaterial().GetMaterial().Diffuse=WhiteColor;
		pD3DSubMesh->GetMaterial().GetMaterial().Specular=WhiteColor;
		pD3DSubMesh->GetMaterial().GetMaterial().Emissive=BlackColor;
		pD3DSubMesh->GetMaterial().GetMaterial().Power=40.0f;

		CD3DFX * pFX=BuildFX(i,TextureLayerInfos[i].LayerCount,TextureLayerInfos[i].pShadowMap!=NULL);
		pD3DSubMesh->GetMaterial().SetFX(pFX);

		
		for(UINT j=0;j<TextureLayerInfos[i].LayerCount;j++)
		{			
			CD3DTexture * pTexture=TextureList[TextureLayerInfos[i].Layers[j].TextureIndex];
			pD3DSubMesh->GetMaterial().AddTexture(pTexture,0);
			if(pTexture)
				pTexture->AddUseRef();
			if(TextureLayerInfos[i].Layers[j].Flag&MLIF_ALPHA_MAP)
			{
				pD3DSubMesh->GetMaterial().AddTexture(TextureLayerInfos[i].Layers[j].pAlphaMap,0);
				if(TextureLayerInfos[i].Layers[j].pAlphaMap)
					TextureLayerInfos[i].Layers[j].pAlphaMap->AddUseRef();
			}			
		}

		if(TextureLayerInfos[i].pShadowMap)
		{
			pD3DSubMesh->GetMaterial().AddTexture(TextureLayerInfos[i].pShadowMap,0);
			TextureLayerInfos[i].pShadowMap->AddUseRef();
		}
		

		WORD * pModelIndices=NULL;
		pD3DSubMesh->GetDXIndexBuffer()->Lock(0,0,(LPVOID *)&pModelIndices,0);

		MODEL_VERTEXT * pModelVertices=NULL;
		pD3DSubMesh->GetDXVertexBuffer()->Lock(0,0,(LPVOID *)&pModelVertices,0);
		

		CD3DVector3 Pos=pMCNK->Position;
		Pos.x=BLZ_ADT_MAP_TRANS_VALUE-Pos.x;
		Pos.y=BLZ_ADT_MAP_TRANS_VALUE-Pos.y;

		Pos=BLZTranslationToD3D(Pos);
	

		float Pitch=BLZ_ADT_MAP_TILE_SIZE/16;

		UINT VertexIndex=0;
		int Line=0;
		while(VertexIndex<145)
		{
			if(Line&1)
			{
				//8 vertices/line
				for(int x=0;x<8;x++)
				{
					pModelVertices[VertexIndex].Pos.x=Pitch+Pitch*x*2;
					pModelVertices[VertexIndex].Pos.z=-Pitch*Line;					
					pModelVertices[VertexIndex].Pos.y=pMCVT->Heights[VertexIndex];
					pModelVertices[VertexIndex].Pos+=Pos;

					CD3DVector3 Normal;
					Normal.x=pMCNR->Normals[VertexIndex][0]/127.0f;
					Normal.y=pMCNR->Normals[VertexIndex][1]/127.0f;
					Normal.z=pMCNR->Normals[VertexIndex][2]/127.0f;
					pModelVertices[VertexIndex].Normal=BLZTranslationToD3D(Normal);

					pModelVertices[VertexIndex].Color=0xffffffff;

					pModelVertices[VertexIndex].Tex.x=(x*2.0f+1.0f)/16.0f;
					pModelVertices[VertexIndex].Tex.y=Line/16.0f;
					

					VertexIndex++;
				}

			}
			else
			{
				//9 vertices/line
				for(int x=0;x<9;x++)
				{
					pModelVertices[VertexIndex].Pos.x=Pitch*x*2;
					pModelVertices[VertexIndex].Pos.z=-Pitch*Line;
					pModelVertices[VertexIndex].Pos.y=pMCVT->Heights[VertexIndex];					
					pModelVertices[VertexIndex].Pos+=Pos;

					CD3DVector3 Normal;
					Normal.x=pMCNR->Normals[VertexIndex][0]/127.0f;
					Normal.y=pMCNR->Normals[VertexIndex][1]/127.0f;
					Normal.z=pMCNR->Normals[VertexIndex][2]/127.0f;
					pModelVertices[VertexIndex].Normal=BLZTranslationToD3D(Normal);		

					pModelVertices[VertexIndex].Color=0xffffffff;

					pModelVertices[VertexIndex].Tex.x=(x*2.0f)/16.0f;
					pModelVertices[VertexIndex].Tex.y=Line/16.0f;
	

					VertexIndex++;
				}
			}
			Line++;
		}	

		
		UINT IndexIndex=0;
		for(int y=0;y<8;y++)
		{
			for(int x=0;x<8;x++)
			{
				if((ADT_HOLE_MASK[y/2][x/2]&((WORD)pMCNK->Holes))==0)
				{
					pModelIndices[IndexIndex]	=(y*8+(y+1)*9)+x;
					pModelIndices[IndexIndex+1]	=(y*8+(y+1)*9)+x+8;
					pModelIndices[IndexIndex+2]	=(y*8+(y+1)*9)+x-9;


					pModelIndices[IndexIndex+3]	=(y*8+(y+1)*9)+x;
					pModelIndices[IndexIndex+4]	=(y*8+(y+1)*9)+x+9;
					pModelIndices[IndexIndex+5]	=(y*8+(y+1)*9)+x+8;


					pModelIndices[IndexIndex+6]	=(y*8+(y+1)*9)+x;
					pModelIndices[IndexIndex+7]	=(y*8+(y+1)*9)+x-8;
					pModelIndices[IndexIndex+8]	=(y*8+(y+1)*9)+x+9;


					pModelIndices[IndexIndex+9]	=(y*8+(y+1)*9)+x;
					pModelIndices[IndexIndex+10]=(y*8+(y+1)*9)+x-9;
					pModelIndices[IndexIndex+11]=(y*8+(y+1)*9)+x-8;				

					IndexIndex+=3*4;
				}
			}
		}

		pD3DSubMesh->SetIndexCount(IndexIndex);

		pD3DSubMesh->GetDXVertexBuffer()->Unlock();

		pD3DSubMesh->GetDXIndexBuffer()->Unlock();

		pD3DSubMesh->SetID(i);
		CEasyString SubMeshName;
		SubMeshName.Format("%d",i);
		pD3DSubMesh->SetName(SubMeshName);

		m_SubMeshList.Add(pD3DSubMesh);
		
	}

	UINT Count=pMDDF->ChunkSize/sizeof(ADT_M2_OBJECT_INFO);
	m_M2ObjectList.Resize(Count);
	for(UINT i=0;i<Count;i++)
	{
		m_M2ObjectList[i].ID=pMDDF->M2Objects[i].ID;
		m_M2ObjectList[i].pModelResource=NULL;
		CEasyString ModelFileName=pMMDX->M2FileNames+pMMID->M2FileNameIndices[pMDDF->M2Objects[i].Index];
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
					PrintSystemLog(0,"加载了[%s]",(LPCTSTR)ModelFileName);
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
			m_M2ObjectList[i].pModelResource=pResource;
		}			
		
		m_M2ObjectList[i].Position.x=pMDDF->M2Objects[i].Position.x;
		m_M2ObjectList[i].Position.y=pMDDF->M2Objects[i].Position.y;
		m_M2ObjectList[i].Position.z=-pMDDF->M2Objects[i].Position.z;					
		m_M2ObjectList[i].Orientation=CD3DQuaternion::FromRotationYawPitchRoll(
			-pMDDF->M2Objects[i].Orientation.y*PI/180.0f,
			-pMDDF->M2Objects[i].Orientation.x*PI/180.0f,
			pMDDF->M2Objects[i].Orientation.z*PI/180.0f);
		m_M2ObjectList[i].Orientation.Normalize();
		m_M2ObjectList[i].Scale=pMDDF->M2Objects[i].Scale/1024.0f;
	}


	Count=pMODF->ChunkSize/sizeof(ADT_WMO_OBJECT_INFO);
	m_WMOObjectList.Resize(Count);
	for(UINT i=0;i<Count;i++)
	{
		m_WMOObjectList[i].ID=pMODF->WMOObjects[i].ID;
		CEasyString FileName=pMWMO->WMOFileNames+pMWID->WMOFileNameIndices[pMODF->WMOObjects[i].Index];
		FileName.MakeUpper();
		FileName.Replace(".MDX",".M2");		

		CD3DWOWWMOModelResource* pResource=
			dynamic_cast<CD3DWOWWMOModelResource*>(m_pManager->GetResource(FileName));
		if(!pResource)
		{
			pResource=new CD3DWOWWMOModelResource(m_pManager);
			if(pResource->LoadFromFile(FileName))
			{
				PrintSystemLog(0,"加载了[%s]",(LPCTSTR)FileName);
				if(!m_pManager->AddResource(pResource,FileName))
				{
					pResource->Release();
					pResource=NULL;
				}
			}
			else
			{
				PrintImportantLog(0,"加载WMO文件%s失败",(LPCTSTR)FileName);
				pResource->Release();
				pResource=NULL;
			}						
		}
		else
		{
			pResource->AddUseRef();
		}	

		m_WMOObjectList[i].pModelResource=pResource;
		
		m_WMOObjectList[i].Position.x=pMODF->WMOObjects[i].Position.x;
		m_WMOObjectList[i].Position.y=pMODF->WMOObjects[i].Position.y;
		m_WMOObjectList[i].Position.z=-pMODF->WMOObjects[i].Position.z;		
			
		m_WMOObjectList[i].Orientation=CD3DQuaternion::FromRotationYawPitchRoll(
			-pMODF->WMOObjects[i].Orientation.y*PI/180.0f,
			-pMODF->WMOObjects[i].Orientation.x*PI/180.0f,
			pMODF->WMOObjects[i].Orientation.z*PI/180.0f);
		m_WMOObjectList[i].Orientation.Normalize();
		m_WMOObjectList[i].DoodadSet=pMODF->WMOObjects[i].DoodadSet;
	}

	CreateBounding();
	return true;
}

UINT CD3DWOWADTModelResource::GetM2ObjectCount()
{
	return m_M2ObjectList.GetCount();
}
CD3DWOWADTModelResource::M2_OBJECT_INFO * CD3DWOWADTModelResource::GetM2ObjectInfo(UINT Index)
{
	return m_M2ObjectList.GetObject(Index);
}

UINT CD3DWOWADTModelResource::GetWMOObjectCount()
{
	return m_WMOObjectList.GetCount();
}
CD3DWOWADTModelResource::WMO_OBJECT_INFO * CD3DWOWADTModelResource::GetWMOObjectInfo(UINT Index)
{
	return m_WMOObjectList.GetObject(Index);
}	


void CD3DWOWADTModelResource::PickResource(CNameObjectSet * pObjectSet,UINT Param)
{
	CD3DObjectResource::PickResource(pObjectSet,Param);	
	for(UINT i=0;i<m_M2ObjectList.GetCount();i++)
	{
		if(m_M2ObjectList[i].pModelResource)
		{
			m_M2ObjectList[i].pModelResource->PickResource(pObjectSet,Param);
			pObjectSet->Add(m_M2ObjectList[i].pModelResource);
		}
	}
	for(UINT i=0;i<m_WMOObjectList.GetCount();i++)
	{
		if(m_WMOObjectList[i].pModelResource)
		{
			m_WMOObjectList[i].pModelResource->PickResource(pObjectSet,Param);
			pObjectSet->Add(m_WMOObjectList[i].pModelResource);
		}
	}
}

bool CD3DWOWADTModelResource::ToSmartStruct(CSmartStruct& Packet,CUSOFile * pUSOFile,UINT Param)
{
	if(!CD3DObjectResource::ToSmartStruct(Packet,pUSOFile,Param))
		return false;

	{
		UINT BufferSize;
		void * pBuffer=Packet.PrepareMember(BufferSize);
		CSmartStruct M2ObjectInfoList(pBuffer,BufferSize,true);
		for(UINT i=0;i<m_M2ObjectList.GetCount();i++)
		{
			pBuffer=M2ObjectInfoList.PrepareMember(BufferSize);
			CSmartStruct M2ObjectInfo(pBuffer,BufferSize,true);

			CHECK_SMART_STRUCT_ADD_AND_RETURN(M2ObjectInfo.AddMember(SST_MOI_ID,m_M2ObjectList[i].ID));
			int ResourceID=pUSOFile->ResourceObjectToIndex(m_M2ObjectList[i].pModelResource);
			CHECK_SMART_STRUCT_ADD_AND_RETURN(M2ObjectInfo.AddMember(SST_MOI_MODEL_RESOURCE,ResourceID));
			CHECK_SMART_STRUCT_ADD_AND_RETURN(M2ObjectInfo.AddMember(SST_MOI_POSITION,
				(LPCTSTR)(&m_M2ObjectList[i].Position),sizeof(m_M2ObjectList[i].Position)));
			CHECK_SMART_STRUCT_ADD_AND_RETURN(M2ObjectInfo.AddMember(SST_MOI_ORIENTATION,
				(LPCTSTR)(&m_M2ObjectList[i].Orientation),sizeof(m_M2ObjectList[i].Orientation)));
			CHECK_SMART_STRUCT_ADD_AND_RETURN(M2ObjectInfo.AddMember(SST_MOI_SCALE,
				(LPCTSTR)(&m_M2ObjectList[i].Scale),sizeof(m_M2ObjectList[i].Scale)));
			
			if(!M2ObjectInfoList.FinishMember(SST_MOL_M2_OBJECT_INFO,M2ObjectInfo.GetDataLen()))
				return false;
		}
		if(!Packet.FinishMember(SST_D3DWAMR_M2_OBJECT_LIST,M2ObjectInfoList.GetDataLen()))
			return false;
	}

	{
		UINT BufferSize;
		void * pBuffer=Packet.PrepareMember(BufferSize);
		CSmartStruct WMOObjectInfoList(pBuffer,BufferSize,true);
		for(UINT i=0;i<m_WMOObjectList.GetCount();i++)
		{
			pBuffer=WMOObjectInfoList.PrepareMember(BufferSize);
			CSmartStruct WMOObjectInfo(pBuffer,BufferSize,true);

			CHECK_SMART_STRUCT_ADD_AND_RETURN(WMOObjectInfo.AddMember(SST_WOI_ID,m_WMOObjectList[i].ID));
			int ResourceID=pUSOFile->ResourceObjectToIndex(m_WMOObjectList[i].pModelResource);
			CHECK_SMART_STRUCT_ADD_AND_RETURN(WMOObjectInfo.AddMember(SST_WOI_MODEL_RESOURCE,ResourceID));
			CHECK_SMART_STRUCT_ADD_AND_RETURN(WMOObjectInfo.AddMember(SST_WOI_POSITION,
				(LPCTSTR)(&m_WMOObjectList[i].Position),sizeof(m_WMOObjectList[i].Position)));
			CHECK_SMART_STRUCT_ADD_AND_RETURN(WMOObjectInfo.AddMember(SST_WOI_ORIENTATION,
				(LPCTSTR)(&m_WMOObjectList[i].Orientation),sizeof(m_WMOObjectList[i].Orientation)));
			CHECK_SMART_STRUCT_ADD_AND_RETURN(WMOObjectInfo.AddMember(SST_WOI_DOODAD_SET,m_WMOObjectList[i].DoodadSet));

			if(!WMOObjectInfoList.FinishMember(SST_WOL_WMO_OBJECT_INFO,WMOObjectInfo.GetDataLen()))
				return false;
		}
		if(!Packet.FinishMember(SST_D3DWAMR_WMO_OBJECT_LIST,WMOObjectInfoList.GetDataLen()))
			return false;
	}

	return true;
}

bool CD3DWOWADTModelResource::FromSmartStruct(CSmartStruct& Packet,CUSOFile * pUSOFile,UINT Param)
{
	Destory();

	if(!CD3DObjectResource::FromSmartStruct(Packet,pUSOFile,Param))
		return false;

	UINT M2ObjectCount=0;
	UINT WMOObjectCount=0;

	void * Pos=Packet.GetFirstMemberPosition();
	while(Pos)
	{
		WORD MemberID;
		CSmartStruct SubPacket=Packet.GetNextMember(Pos,MemberID);
		switch(MemberID)
		{	
		case SST_D3DWAMR_M2_OBJECT_LIST:
			M2ObjectCount=SubPacket.GetMemberCount();
			break;
		case SST_D3DWAMR_WMO_OBJECT_LIST:
			WMOObjectCount=SubPacket.GetMemberCount();
			break;
		}
	}

	m_M2ObjectList.Resize(M2ObjectCount);
	m_WMOObjectList.Resize(WMOObjectCount);

	M2ObjectCount=0;
	WMOObjectCount=0;

	Pos=Packet.GetFirstMemberPosition();
	while(Pos)
	{
		WORD MemberID;
		CSmartStruct SubPacket=Packet.GetNextMember(Pos,MemberID);
		switch(MemberID)
		{	
		case SST_D3DWAMR_M2_OBJECT_LIST:
			{
				void * SubPos=SubPacket.GetFirstMemberPosition();
				while(SubPos)
				{
					WORD SubMemberID;
					CSmartStruct M2ObjectInfo=Packet.GetNextMember(SubPos,SubMemberID);
					if(SubMemberID==SST_MOL_M2_OBJECT_INFO)
					{
						m_M2ObjectList[M2ObjectCount].ID=M2ObjectInfo.GetMember(SST_MOI_ID);
						int ResourceID=M2ObjectInfo.GetMember(SST_MOI_MODEL_RESOURCE);
						m_M2ObjectList[M2ObjectCount].pModelResource=(CD3DWOWM2ModelResource *)pUSOFile->ResourceIndexToObject(ResourceID,GET_CLASS_INFO(CD3DWOWM2ModelResource));
						if(m_M2ObjectList[M2ObjectCount].pModelResource)
							m_M2ObjectList[M2ObjectCount].pModelResource->AddUseRef();
						memcpy(&(m_M2ObjectList[M2ObjectCount].Position),(LPCTSTR)M2ObjectInfo.GetMember(SST_MOI_POSITION),
							sizeof(m_M2ObjectList[M2ObjectCount].Position));
						memcpy(&(m_M2ObjectList[M2ObjectCount].Orientation),(LPCTSTR)M2ObjectInfo.GetMember(SST_MOI_ORIENTATION),
							sizeof(m_M2ObjectList[M2ObjectCount].Orientation));
						memcpy(&(m_M2ObjectList[M2ObjectCount].Scale),(LPCTSTR)M2ObjectInfo.GetMember(SST_MOI_SCALE),
							sizeof(m_M2ObjectList[M2ObjectCount].Scale));						
						M2ObjectCount++;
					}
				}
			}
			break;
		case SST_D3DWAMR_WMO_OBJECT_LIST:
			{
				void * SubPos=SubPacket.GetFirstMemberPosition();
				while(SubPos)
				{
					WORD SubMemberID;
					CSmartStruct WMOObjectInfo=Packet.GetNextMember(SubPos,SubMemberID);
					if(SubMemberID==SST_WOL_WMO_OBJECT_INFO)
					{
						m_WMOObjectList[WMOObjectCount].ID=WMOObjectInfo.GetMember(SST_WOI_ID);
						int ResourceID=WMOObjectInfo.GetMember(SST_WOI_MODEL_RESOURCE);
						m_WMOObjectList[WMOObjectCount].pModelResource=(CD3DWOWWMOModelResource *)pUSOFile->ResourceIndexToObject(ResourceID,GET_CLASS_INFO(CD3DWOWWMOModelResource));
						if(m_WMOObjectList[WMOObjectCount].pModelResource)
							m_WMOObjectList[WMOObjectCount].pModelResource->AddUseRef();
						memcpy(&(m_WMOObjectList[WMOObjectCount].Position),(LPCTSTR)WMOObjectInfo.GetMember(SST_WOI_POSITION),
							sizeof(m_WMOObjectList[WMOObjectCount].Position));
						memcpy(&(m_WMOObjectList[WMOObjectCount].Orientation),(LPCTSTR)WMOObjectInfo.GetMember(SST_WOI_ORIENTATION),
							sizeof(m_WMOObjectList[WMOObjectCount].Orientation));
						m_WMOObjectList[WMOObjectCount].ID=WMOObjectInfo.GetMember(SST_WOI_DOODAD_SET);
						WMOObjectCount++;
					}
				}
			}
			break;		
		}
	}

	return true;
}

UINT CD3DWOWADTModelResource::GetSmartStructSize(UINT Param)
{
	UINT Size=CD3DObjectResource::GetSmartStructSize(Param);
	Size+=SMART_STRUCT_STRUCT_MEMBER_SIZE(0);
	for(UINT i=0;i<m_M2ObjectList.GetCount();i++)
	{
		Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(m_M2ObjectList[i].ID));
		Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(int));		
		Size+=SMART_STRUCT_STRING_MEMBER_SIZE(sizeof(m_M2ObjectList[i].Position));
		Size+=SMART_STRUCT_STRING_MEMBER_SIZE(sizeof(m_M2ObjectList[i].Orientation));
		Size+=SMART_STRUCT_STRING_MEMBER_SIZE(sizeof(m_M2ObjectList[i].Scale));		
		Size+=SMART_STRUCT_STRUCT_MEMBER_SIZE(0);
	}
	Size+=SMART_STRUCT_STRUCT_MEMBER_SIZE(0);
	for(UINT i=0;i<m_WMOObjectList.GetCount();i++)
	{
		Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(m_WMOObjectList[i].ID));
		Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(int));		
		Size+=SMART_STRUCT_STRING_MEMBER_SIZE(sizeof(m_WMOObjectList[i].Position));
		Size+=SMART_STRUCT_STRING_MEMBER_SIZE(sizeof(m_WMOObjectList[i].Orientation));
		Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(m_WMOObjectList[i].DoodadSet));		
		Size+=SMART_STRUCT_STRUCT_MEMBER_SIZE(0);
	}

	return Size;
}


CD3DFX * CD3DWOWADTModelResource::BuildFX(UINT Index,UINT LayCount,bool HaveShadowMap)
{
	CEasyString FXName;
	CEasyString PSFunction;
	

	FXName.Format("ADTModel_0x%X%s",
		LayCount,HaveShadowMap?"S":"");

	if(HaveShadowMap)
	{
		switch(LayCount)
		{
		case 4:
			PSFunction="PShaderFourLayerWithShadow";
			break;
		case 3:
			PSFunction="PShaderThreeLayerWithShadow";
			break;
		case 2:
			PSFunction="PShaderTwoLayerWithShadow";
			break;
		default:
			PSFunction="PShaderOneLayerWithShadow";
		}
	}
	else
	{
		switch(LayCount)
		{
		case 4:
			PSFunction="PShaderFourLayer";
			break;
		case 3:
			PSFunction="PShaderThreeLayer";
			break;
		case 2:
			PSFunction="PShaderTwoLayer";
			break;
		default:
			PSFunction="PShaderOneLayer";
		}
	}

	

	IFileAccessor * pFile;

	CEasyString FxContent;


	pFile=CD3DWOWM2Model::CreateFileAccessor();
	if(pFile==NULL)
		return NULL;
	if(!pFile->Open(ADT_MODEL_FX_FILE_NAME,IFileAccessor::modeRead))
	{
		pFile->Release();
		return NULL;	
	}
	int FileSize=(int)pFile->GetSize();	
	FxContent.Resize(FileSize);
	pFile->Read((LPVOID)FxContent.GetBuffer(),FileSize);	
	FxContent.SetLength(FileSize);
	pFile->Release();
	FxContent.Replace("<PSFunction>",PSFunction);
	

	CD3DFX * pFX=m_pManager->GetDevice()->GetFXManager()->LoadFXFromMemory(FXName,FxContent.GetBuffer(),FxContent.GetLength());	
	return pFX;
}

bool CD3DWOWADTModelResource::LoadAlphaLayer(TEXTURE_LAYER_INFO& LayInfo,UINT LayerCount,BLZ_CHUNK_MCLY * pMCLY,BLZ_CHUNK_MCAL * pMCAL)
{
	if(LayerCount>BLZ_ADT_TEXTURE_LAYER_COUNT)
		return false;
	LayInfo.LayerCount=LayerCount;	
	for(UINT i=0;i<LayerCount;i++)
	{
		LayInfo.Layers[i].TextureIndex=pMCLY->Layers[i].TextureID;
		LayInfo.Layers[i].Flag=pMCLY->Layers[i].Flags;
		LayInfo.Layers[i].pAlphaMap=NULL;
		if(LayInfo.Layers[i].Flag&MLIF_ALPHA_MAP)
		{
			int AlphaDataType;
			if(LayInfo.Layers[i].Flag&MLIF_COMPRESSED_ALPHA_MAP)
			{
				AlphaDataType=ADT_COMPRESS;
			}
			else
			{
				if(pMCAL->ChunkSize/(LayerCount-1)>2048)
				{
					AlphaDataType=ADT_UNCOMPRESS_4096;
				}
				else
				{
					AlphaDataType=ADT_UNCOMPRESS_2048;
				}
			}
			
			UINT ProcessSize=0;
			LPBYTE pAlphaData=pMCAL->AlphaMapData+pMCLY->Layers[i].MCALOffset;
			UINT AlphaDataSize=pMCAL->ChunkSize-pMCLY->Layers[i].MCALOffset;
			LayInfo.Layers[i].pAlphaMap=CreateAlphaMap(AlphaDataType,pAlphaData,AlphaDataSize,ProcessSize);
			if(LayInfo.Layers[i].pAlphaMap==NULL)
				return false;
			
		}
	}
	return true;
}

CD3DTexture * CD3DWOWADTModelResource::CreateAlphaMap(int DataType,LPBYTE pData,UINT DataSize,UINT& ProcessSize)
{
	ProcessSize=0;	

	CSmartPtr<CD3DTexture>  pAlphaMap=new CD3DTexture(m_pManager->GetDevice()->GetTextureManager());
	if(!pAlphaMap->CreateTexture(64,64,D3DFMT_A8))
		return NULL;
	
	switch(DataType)
	{
	case ADT_UNCOMPRESS_2048:
		{	
			//PrintSystemLog(0,"4位AlphaMap");
			if(DataSize>=2048)
			{
				D3DLOCKED_RECT LockedRect;
				if(pAlphaMap->LockBits(0,&LockedRect,NULL,0))
				{
					CEasyBuffer Buffer(LockedRect.pBits,4096);
					for(UINT i=0;i<2048;i++)
					{
						Buffer.PushConstBack(((pData[i]&0xF)<<4),1);
						Buffer.PushConstBack((pData[i]&0xF0),1);
					}
					pAlphaMap->UnlockBits(0);
					ProcessSize=2048;
					CD3DTexture * pRet=pAlphaMap;
					pAlphaMap.Detach();
					return pRet;
				}
				else
				{
					return NULL;
				}
			}
		}
		break;
	case ADT_UNCOMPRESS_4096:
		{
			//PrintSystemLog(0,"8位AlphaMap");
			if(DataSize>=4096)
			{
				D3DLOCKED_RECT LockedRect;
				if(pAlphaMap->LockBits(0,&LockedRect,NULL,0))
				{
					CEasyBuffer Buffer(LockedRect.pBits,4096);
					Buffer.PushBack(pData,4096);
					pAlphaMap->UnlockBits(0);
					ProcessSize=4096;
					CD3DTexture * pRet=pAlphaMap;
					pAlphaMap.Detach();
					return pRet;
				}
				else
				{
					return NULL;
				}
			}
		}
		break;
	case ADT_COMPRESS:
		{
			//PrintSystemLog(0,"压缩格式AlphaMap");
			D3DLOCKED_RECT LockedRect;
			if(pAlphaMap->LockBits(0,&LockedRect,NULL,0))
			{
				CEasyBuffer Buffer(LockedRect.pBits,4096);
				UINT AlphaCount=0;
				UINT ReadPtr=0;
				while(AlphaCount<4096)
				{
					if(pData[ReadPtr]&0x80)
					{
						BYTE Count=pData[ReadPtr]&0x7F;
						ReadPtr++;
						for(BYTE i=0;i<Count;i++)
						{
							Buffer.PushConstBack(pData[ReadPtr],1);
						}
						Buffer.PushBack(pData+ReadPtr,Count);
						ReadPtr++;
						AlphaCount+=Count;
					}
					else
					{
						BYTE Count=pData[ReadPtr]&0x7F;
						ReadPtr++;
						Buffer.PushBack(pData+ReadPtr,Count);
						ReadPtr+=Count;
						AlphaCount+=Count;
					}
				}
				pAlphaMap->UnlockBits(0);
				ProcessSize=4096;
				CD3DTexture * pRet=pAlphaMap;
				pAlphaMap.Detach();
				return pRet;
			}
			else
			{
				return NULL;
			}
		}
		break;
	}
	return NULL;
}


bool CD3DWOWADTModelResource::LoadShadowMap(TEXTURE_LAYER_INFO& LayInfo,BLZ_CHUNK_MCSH * pMCSH)
{	
	CSmartPtr<CD3DTexture>  pAlphaMap=new CD3DTexture(m_pManager->GetDevice()->GetTextureManager());
	if(!pAlphaMap->CreateTexture(64,64,D3DFMT_A8))
		return false;

	D3DLOCKED_RECT LockedRect;
	if(pAlphaMap->LockBits(0,&LockedRect,NULL,0))
	{
		CEasyBuffer Buffer(LockedRect.pBits,4096);
		for(int y=0;y<64;y++)
		{
			UINT64 ShadowData=pMCSH->ShadowMap[y];
			for(int x=0;x<64;x++)
			{
				if(ShadowData&1)
					Buffer.PushConstBack(0xB0,1);
				else
					Buffer.PushConstBack(0xff,1);
				ShadowData=ShadowData>>1;
			}
		}		
		pAlphaMap->UnlockBits(0);	
		LayInfo.pShadowMap=pAlphaMap;
		pAlphaMap.Detach();
		return true;
	}
	else
	{
		return false;
	}
}

}