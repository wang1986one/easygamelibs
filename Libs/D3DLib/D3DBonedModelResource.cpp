#include "StdAfx.h"

namespace D3DLib{

#define	SYS_FLAG			(*(int*)"ORB ")			// SYS 系统文件标志
#define SYS_VERSION			0x00010000
#define	SKELETON_FLAG		(*(int*)"BONE")			// 骨骼文件标志
#define SKELETON_VERSION	0x00010002
#define	ACTION_FLAG			(*(int*)"ACTN")			// 动化文件标志
#define ACTION_VERSION		0x00010002
#define	SKIN_FLAG			(*(int*)"SKIN")			// 皮肤文件标志
#define SKIN_VERSION		0x00010003
#define	MODEL_FLAG			(*(int*)"MDL ")			// 刚体文件标志
#define MODEL_VERSION		0x00010002

struct FILE_HEADER
{
	DWORD	MainFlag;
	DWORD	MainVersion;
	DWORD	SubFlag;
	DWORD	SubVersion;
};

struct DATA_BLOCK_HEADER
{
	int Offset;
	int Length;
};

enum SKN_BLOCK_TYPE
{
	SBT_SKELETON,
	SBT_NAMETBL,
	SBT_MATERIAL,
	SBT_SKINVERTEX,
	SBT_BONEWEIGHT,
	SBT_MESHPART,
	SBT_MESHVERTEX,
	SBT_MESHINDEX,
	SBT_MAX,
};

struct SKN_MATERIAL
{
	int	  nMainTexOffset;
	int	  nAddtionOffset;
	int	  nFxOffset;
	int	  nMaterialNameOffset;
};
struct SKN_BONEWEIGHT
{
	int		nBoneID;
	float 	Weight;				//权值
};

struct SKN_MESHPART
{
	int		nMaterial;			// 材质编号
	int		nVertexCount;
	int		nFirstVertex;
	int		nFaceCount;
	int		nFirstIndex;
};

struct SKIN_VERTEX
{
	CD3DVector3		Position;		//
	CD3DVector3		Normal;
	int				nWeightCount;
	int				nWeightOffset;
};

struct SKIN_MESHVERTEX
{
	int					nSkinVertexID;	
	CD3DVector2			uv;			//贴图坐标	
};

enum BONE_BLOCK_TYPE
{
	BBT_BONES,
	BBT_NAME,
	BBT_CHILDREN,
	BBT_MAX,
};

struct BONE_BONE
{
	short nNameLen;
	short nNameOffset;
	short nChildCount;
	short nFirstChild;
	short nParentID;
	short nIsDummy;	
	CD3DMatrix tmInv;
};	


IMPLEMENT_FILE_CHANNEL_MANAGER(CD3DBonedModelResource)

IMPLEMENT_CLASS_INFO(CD3DBonedModelResource,CD3DObjectResource);

CD3DBonedModelResource::CD3DBonedModelResource()
:CD3DObjectResource()
{	
	m_pBones=NULL;
	m_BoneCount=0;	
}

CD3DBonedModelResource::CD3DBonedModelResource(CD3DObjectResourceManager* pManager)
	:CD3DObjectResource(pManager)
{
	m_pBones=NULL;
	m_BoneCount=0;	
}

CD3DBonedModelResource::~CD3DBonedModelResource(void)
{
	Destory();
}

void CD3DBonedModelResource::Destory()
{	
	SAFE_DELETE_ARRAY(m_pBones);
	m_BoneCount=0;
	CD3DObjectResource::Destory();
}


bool CD3DBonedModelResource::LoadFromSKN(LPCTSTR FileName)
{
	IFileAccessor * pFile;

	Destory();

	pFile=CFileSystemManager::GetInstance()->CreateFileAccessor(m_FileChannel);
	if(pFile==NULL)
		return false;
	if(!pFile->Open(FileName,IFileAccessor::modeRead))
	{
		pFile->Release();
		return false;	
	}
	int FileSize=(int)pFile->GetSize();	
	BYTE * pData=new BYTE[FileSize];
	pFile->Read(pData,FileSize);
	pFile->Release();

	BYTE * DataReadPtr=pData;
	FILE_HEADER * pHeader=(FILE_HEADER *)DataReadPtr;
	DataReadPtr+=sizeof(FILE_HEADER);

	//校验文件类型和版本号
	if( (pHeader->MainFlag != SYS_FLAG) || (pHeader->MainVersion != SYS_VERSION) ||
		(pHeader->SubFlag != SKIN_FLAG) || (pHeader->SubVersion != SKIN_VERSION))
	{
		delete[] pData;
		return false;
	}

	DATA_BLOCK_HEADER * pBlockHeaders=(DATA_BLOCK_HEADER *)DataReadPtr;

	char*				pBoneFileName		= NULL;
	char*				pNameTables			= NULL;	
	SKN_MATERIAL*		pMaterials			= NULL;
	SKIN_VERTEX *		pSkinVertices		= NULL;
	SKN_BONEWEIGHT*		pBoneWeights		= NULL;
	SKN_MESHPART*		pMeshParts			= NULL;	
	
	SKIN_MESHVERTEX*	pMeshVertices		= NULL;
	WORD*				pMeshIndex			= NULL;
	
	

	int MaterialCount;
	int SkinVertexCount;
	int BoneWeightsCount;
	int MeshPartCount;	
	int VertexCount;
	int IndexCount;		
	
	pBoneFileName=(char *)(DataReadPtr+pBlockHeaders[SBT_SKELETON].Offset);

	pNameTables=(char *)(DataReadPtr+pBlockHeaders[SBT_NAMETBL].Offset);

	pMaterials=(SKN_MATERIAL *)(DataReadPtr+pBlockHeaders[SBT_MATERIAL].Offset);
	MaterialCount=pBlockHeaders[SBT_MATERIAL].Length/sizeof(SKN_MATERIAL);

	pSkinVertices=(SKIN_VERTEX *)(DataReadPtr+pBlockHeaders[SBT_SKINVERTEX].Offset);
	SkinVertexCount=pBlockHeaders[SBT_SKINVERTEX].Length/sizeof(SKIN_VERTEX);

	pBoneWeights=(SKN_BONEWEIGHT *)(DataReadPtr+pBlockHeaders[SBT_BONEWEIGHT].Offset);
	BoneWeightsCount=pBlockHeaders[SBT_BONEWEIGHT].Length/sizeof(SKN_BONEWEIGHT);

	pMeshParts=(SKN_MESHPART *)(DataReadPtr+pBlockHeaders[SBT_MESHPART].Offset);
	MeshPartCount=pBlockHeaders[SBT_MESHPART].Length/sizeof(SKN_MESHPART);

	pMeshVertices=(SKIN_MESHVERTEX *)(DataReadPtr+pBlockHeaders[SBT_MESHVERTEX].Offset);
	VertexCount=pBlockHeaders[SBT_MESHVERTEX].Length/sizeof(SKIN_MESHVERTEX);

	pMeshIndex=(WORD *)(DataReadPtr+pBlockHeaders[SBT_MESHINDEX].Offset);
	IndexCount=pBlockHeaders[SBT_MESHINDEX].Length/sizeof(WORD);	


	char drv[20],dir[MAX_PATH],BoneFileName[MAX_PATH];

	_splitpath_s(FileName,drv,20,dir,MAX_PATH,NULL,0,NULL,0);

	_makepath_s(BoneFileName,MAX_PATH,drv,dir,pBoneFileName,"");	

	if(!LoadSkeleton(BoneFileName))
	{
		delete[] pData;
		return false;
	}


	for(int i=0;i<MeshPartCount;i++)
	{
		CD3DSubMesh * pD3DSubMesh=new CD3DSubMesh;

		pD3DSubMesh->GetVertexFormat().FVF=D3DFVF_XYZB5|D3DFVF_NORMAL|D3DFVF_TEX1|D3DFVF_LASTBETA_UBYTE4;
		pD3DSubMesh->GetVertexFormat().VertexSize=sizeof(BONED_MODEL_VERTEXT_TEX1);
		pD3DSubMesh->GetVertexFormat().IndexSize=sizeof(WORD);
		pD3DSubMesh->SetPrimitiveType(D3DPT_TRIANGLELIST);
		pD3DSubMesh->SetPrimitiveCount(pMeshParts[i].nFaceCount);		
		pD3DSubMesh->SetVertexCount(pMeshParts[i].nVertexCount);
		pD3DSubMesh->SetIndexCount(pMeshParts[i].nFaceCount*3);


		pD3DSubMesh->AllocDXVertexBuffer(m_pManager->GetDevice());		

		BONED_MODEL_VERTEXT_TEX1 * pBuff;
		pD3DSubMesh->GetDXVertexBuffer()->Lock(0,0,(LPVOID *)&pBuff,0);
		for(int j=0;j<pMeshParts[i].nVertexCount;j++)
		{
			int MeshVertexID=j+pMeshParts[i].nFirstVertex;
			int SkinVertexID=pMeshVertices[MeshVertexID].nSkinVertexID;
			pBuff[j].Pos=pSkinVertices[SkinVertexID].Position;
			pBuff[j].Normal=pSkinVertices[SkinVertexID].Normal;	
			pBuff[j].Tex1=pMeshVertices[MeshVertexID].uv;
			for(int b=0;b<pSkinVertices[SkinVertexID].nWeightCount;b++)
			{
				int BoneWeightID=pSkinVertices[SkinVertexID].nWeightOffset+b;
				pBuff[j].BoneID[b]=pBoneWeights[BoneWeightID].nBoneID;
				pBuff[j].BoneWeight[b]=pBoneWeights[BoneWeightID].Weight;

			}
			for(int b=pSkinVertices[SkinVertexID].nWeightCount;b<4;b++)
			{
				pBuff[j].BoneID[b]=0;
				pBuff[j].BoneWeight[b]=0;
			}
			
		}
		pD3DSubMesh->GetDXVertexBuffer()->Unlock();

		pD3DSubMesh->AllocDXIndexBuffer(m_pManager->GetDevice());
		
		pD3DSubMesh->GetDXIndexBuffer()->Lock(0,0,(LPVOID *)&pBuff,0);
		memcpy(pBuff,pMeshIndex+pMeshParts[i].nFirstIndex,pMeshParts[i].nFaceCount*3*sizeof(WORD));
		pD3DSubMesh->GetDXIndexBuffer()->Unlock();

		
		


		int MatID=pMeshParts[i].nMaterial;

		D3DCOLORVALUE WhiteColor={1.0f,1.0f,1.0f,1.0f};
		D3DCOLORVALUE BlackColor={0.0f,0.0f,0.0f,1.0f};

		pD3DSubMesh->GetMaterial().GetMaterial().Ambient=WhiteColor;
		pD3DSubMesh->GetMaterial().GetMaterial().Diffuse=WhiteColor;
		pD3DSubMesh->GetMaterial().GetMaterial().Specular=WhiteColor;
		pD3DSubMesh->GetMaterial().GetMaterial().Emissive=BlackColor;
		pD3DSubMesh->GetMaterial().GetMaterial().Power=40.0f;

		CEasyString FileName;

		if(pMaterials[MatID].nMainTexOffset>=0)
		{		
			FileName.Format("%s.*",pNameTables+pMaterials[MatID].nMainTexOffset);
			CEasyString FileType=FileName;
			FileType.MakeUpper();
			if(FileType.Find(".IFL")>=0)
			{
				CD3DTexture * pTexture=m_pManager->GetDevice()->GetTextureManager()->LoadIFLTexture(FileName,0);
				pD3DSubMesh->GetMaterial().AddTexture(pTexture,0);

			}
			else
			{			
				CD3DTexture * pTexture=m_pManager->GetDevice()->GetTextureManager()->LoadTexture(FileName,0);
				pD3DSubMesh->GetMaterial().AddTexture(pTexture,0);
			}		
		}

		if(pMaterials[MatID].nAddtionOffset>=0)
		{		
			FileName.Format("%s.*",pNameTables+pMaterials[MatID].nAddtionOffset);
			CEasyString FileType=FileName;
			FileType.MakeUpper();
			if(FileType.Find(".IFL")>=0)
			{
				CD3DTexture * pTexture=m_pManager->GetDevice()->GetTextureManager()->LoadIFLTexture(FileName,0);
				pD3DSubMesh->GetMaterial().AddTexture(pTexture,0);
			}
			else
			{			
				CD3DTexture * pTexture=m_pManager->GetDevice()->GetTextureManager()->LoadTexture(FileName,0);
				pD3DSubMesh->GetMaterial().AddTexture(pTexture,0);
			}				
		}		

		if(pMaterials[MatID].nFxOffset>=0)
		{
			FileName.Format("%s.fx",pNameTables+pMaterials[MatID].nFxOffset);
			
			CD3DFX * pFX=m_pManager->GetDevice()->GetFXManager()->LoadFX(FileName);
			pD3DSubMesh->GetMaterial().SetFX(pFX);
			int TransparentFactor;

			if(pFX->GetInt("TransparentFactor",TransparentFactor))
			{
				if(TransparentFactor>=4)
					pD3DSubMesh->SetTransparent(true);
			}
		}	

		m_SubMeshList.Add(pD3DSubMesh);

	}	

	//RebuildBoneIndex();
	
	CreateBounding();

	

	delete[] pData;

	return true;
}

int CD3DBonedModelResource::GetBoneCount()
{
	return m_BoneCount;
}

BONED_MODEL_BONE * CD3DBonedModelResource::GetBone(int Index)
{
	return m_pBones+Index;
}

bool CD3DBonedModelResource::ToSmartStruct(CSmartStruct& Packet,CUSOFile * pUSOFile,UINT Param)
{
	if(!CD3DObjectResource::ToSmartStruct(Packet,pUSOFile,Param))
		return false;	

	for(int i=0;i<m_BoneCount;i++)
	{
		UINT BufferSize;
		void * pBuffer=Packet.PrepareMember(BufferSize);
		CSmartStruct SubPacket(pBuffer,BufferSize,true);
		CHECK_SMART_STRUCT_ADD_AND_RETURN(SubPacket.AddMember(SST_BI_NAME,m_pBones[i].Name));
		CHECK_SMART_STRUCT_ADD_AND_RETURN(SubPacket.AddMember(SST_BI_PARENT_ID,m_pBones[i].ParentID));
		CHECK_SMART_STRUCT_ADD_AND_RETURN(SubPacket.AddMember(SST_BI_INIT_MATRIX,
			(char *)&(m_pBones[i].BoneInitMatrix),sizeof(m_pBones[i].BoneInitMatrix)));
		if(!Packet.FinishMember(SST_D3DBMR_BONE,SubPacket.GetDataLen()))
			return false;
	}
	return true;
}

bool CD3DBonedModelResource::FromSmartStruct(CSmartStruct& Packet,CUSOFile * pUSOFile,UINT Param)
{
	if(!CD3DObjectResource::FromSmartStruct(Packet,pUSOFile,Param))
		return false;
	void * Pos=Packet.GetFirstMemberPosition();
	while(Pos)
	{
		WORD MemberID;
		CSmartStruct Value=Packet.GetNextMember(Pos,MemberID);
		switch(MemberID)
		{	
		case SST_D3DBMR_BONE:
			m_BoneCount++;
			break;
		}
	}
	m_pBones=new BONED_MODEL_BONE[m_BoneCount];
	Pos=Packet.GetFirstMemberPosition();
	int Index=0;
	while(Pos)
	{
		WORD MemberID;
		CSmartStruct Value=Packet.GetNextMember(Pos,MemberID);
		switch(MemberID)
		{	
		case SST_D3DBMR_BONE:
			if(Index<m_BoneCount)
			{
				strncpy_0(m_pBones[Index].Name,MAX_BONE_NAME_LEN,Value.GetMember(SST_BI_NAME),MAX_BONE_NAME_LEN);
				m_pBones[Index].ParentID=Value.GetMember(SST_BI_PARENT_ID);
				memcpy(&(m_pBones[Index].BoneInitMatrix),(LPCTSTR)Value.GetMember(SST_BI_INIT_MATRIX),
					sizeof(m_pBones[Index].BoneInitMatrix));
				Index++;
			}

			break;
		}
	}
	return true;
}

UINT CD3DBonedModelResource::GetSmartStructSize(UINT Param)
{
	UINT Size=CD3DObjectResource::GetSmartStructSize(Param);
	for(int i=0;i<m_BoneCount;i++)
	{
		Size+=SMART_STRUCT_STRING_MEMBER_SIZE(strlen(m_pBones[i].Name));
		Size+=SMART_STRUCT_FIX_MEMBER_SIZE(sizeof(m_pBones[i].ParentID));
		Size+=SMART_STRUCT_STRING_MEMBER_SIZE(sizeof(m_pBones[i].BoneInitMatrix));
		Size+=SMART_STRUCT_STRUCT_MEMBER_SIZE(0);
	}
	
	return Size;
}

bool CD3DBonedModelResource::LoadSkeleton(LPCTSTR FileName)
{
	IFileAccessor * pFile;

	pFile=CFileSystemManager::GetInstance()->CreateFileAccessor(m_FileChannel);
	if(pFile==NULL)
		return false;
	if(!pFile->Open(FileName,IFileAccessor::modeRead))
	{
		pFile->Release();
		return false;	
	}
	int FileSize=(int)pFile->GetSize();	
	BYTE * pData=new BYTE[FileSize];
	pFile->Read(pData,FileSize);
	pFile->Release();

	BYTE * DataReadPtr=pData;
	FILE_HEADER * pHeader=(FILE_HEADER *)DataReadPtr;
	DataReadPtr+=sizeof(FILE_HEADER);

	//校验文件类型和版本号
	if( (pHeader->MainFlag != SYS_FLAG) || (pHeader->MainVersion != SYS_VERSION) ||
		(pHeader->SubFlag != SKELETON_FLAG) || (pHeader->SubVersion != SKELETON_VERSION))
	{
		delete[] pData;
		return false;
	}

	DATA_BLOCK_HEADER * pBlockHeaders=(DATA_BLOCK_HEADER *)DataReadPtr;


	BONE_BONE *		pBones			=NULL;
	char *			pNames			=NULL;
	BYTE *			pChilds			=NULL;

	int BoneCount;
	int ChildCount;


	pBones=(BONE_BONE *)(DataReadPtr+pBlockHeaders[BBT_BONES].Offset);
	BoneCount=pBlockHeaders[BBT_BONES].Length/sizeof(BONE_BONE);

	pNames=(char *)(DataReadPtr+pBlockHeaders[BBT_NAME].Offset);

	pChilds=(BYTE *)(DataReadPtr+pBlockHeaders[BBT_CHILDREN].Offset);
	ChildCount=pBlockHeaders[BBT_CHILDREN].Length/sizeof(BYTE);

	m_BoneCount=BoneCount;
	m_pBones=new BONED_MODEL_BONE[m_BoneCount];

	for(int i=0;i<m_BoneCount;i++)
	{
		strncpy_0(m_pBones[i].Name,MAX_BONE_NAME_LEN,pNames+pBones[i].nNameOffset,MAX_BONE_NAME_LEN);		
		m_pBones[i].ParentID=pBones[i].nParentID;
		m_pBones[i].BoneInitMatrix=pBones[i].tmInv;
	}

	delete[] pData;
	return true;
}

//void CD3DBonedModelResource::RebuildBoneIndex()
//{
//	for(int i=0;i<m_SubMeshCount;i++)
//	{
//		BONED_MODEL_VERTEXT_TEX1 * pVertexs=NULL;
//		if(pD3DSubMesh->pVertexBuffer)
//		{
//			pD3DSubMesh->pVertexBuffer->Lock(0,0,(LPVOID *)&pVertexs,0);
//		}
//		else
//		{
//			pVertexs=(BONED_MODEL_VERTEXT_TEX1 *)pD3DSubMesh->pVertexs;
//		}
//
//		pVertexs+=pD3DSubMesh->StartVertex;
//		pD3DSubMesh->MinBoneIndex=255;
//		pD3DSubMesh->MaxBoneIndex=0;
//		for(int j=0;j<pD3DSubMesh->VertexCount;j++)
//		{
//			for(int b=0;b<4;b++)
//			{
//				if(pVertexs[j].BoneWeight[b]!=0)
//				{
//					if((BYTE)(pVertexs[j].BoneID[b])<pD3DSubMesh->MinBoneIndex)
//						pD3DSubMesh->MinBoneIndex=(BYTE)(pVertexs[j].BoneID[b]);
//					if((BYTE)(pVertexs[j].BoneID[b])>pD3DSubMesh->MaxBoneIndex)
//						pD3DSubMesh->MaxBoneIndex=(BYTE)(pVertexs[j].BoneID[b]);
//				}
//			}
//		}	
//
//		if(pD3DSubMesh->pVertexBuffer)
//		{
//			pD3DSubMesh->pVertexBuffer->Unlock();
//		}
//	}
//}




//bool CD3DBonedModelResource::ToUSOFile(CUSOFile * pUSOFile,UINT Param)
//{	
//
//	if(pUSOFile==NULL)
//		return false;	
//
//	IFileAccessor * pFile=pUSOFile->GetFile();
//	if(pFile==NULL)
//		return false;
//
//
//	STORAGE_STRUCT Data;
//	
//	strncpy_0(Data.ObjectHead.Type,USO_FILE_MAX_TYPE_LEN,GetClassInfo().ClassName,USO_FILE_MAX_TYPE_LEN);			
//	strncpy_0(Data.ObjectHead.Name,USO_FILE_MAX_OBJECT_NAME,GetName(),USO_FILE_MAX_OBJECT_NAME);
//	Data.ObjectHead.Size=sizeof(STORAGE_STRUCT)+m_BoneCount*sizeof(BONED_MODEL_BONE);
//
//	Data.BoundingBox=m_BoundingBox;
//	Data.BoundingSphere=m_BoundingSphere;
//	Data.SubMeshCount=(int)m_SubMeshList.size();
//	Data.BoneCount=m_BoneCount;
//
//	pFile->Write(&Data,sizeof(STORAGE_STRUCT));
//
//	if(m_BoneCount)
//		pFile->Write(m_pBones,m_BoneCount*sizeof(BONED_MODEL_BONE));
//
//	for(size_t i=0;i<m_SubMeshList.size();i++)
//	{
//		if(!m_SubMeshList[i]->ToUSOFile(pUSOFile,(UINT)(m_pManager->GetDevice())))
//			return false;		
//	}
//
//	
//
//	return true;
//}
//
//
//bool CD3DBonedModelResource::FromUSOFile(CUSOFile * pUSOFile,UINT Param)
//{
//	if(pUSOFile==NULL)
//		return false;	
//
//	IFileAccessor * pFile=pUSOFile->GetFile();
//	if(pFile==NULL)
//		return false;
//
//	STORAGE_STRUCT * pData;
//	BYTE * pBuff;
//	UINT Size;
//
//	pFile->Read(&Size,sizeof(UINT));
//	pBuff=new BYTE[Size];
//	pFile->Read(pBuff+sizeof(UINT),Size-sizeof(UINT));
//	pData=(STORAGE_STRUCT *)pBuff;
//	pData->ObjectHead.Size=Size;
//
//
//	if((!GetClassInfo().IsKindOf(pData->ObjectHead.Type))||
//		pData->ObjectHead.Size<sizeof(STORAGE_STRUCT))
//	{	
//		delete[] pBuff;
//		return false;
//	}
//
//	Destory();
//
//
//	pData->ObjectHead.Name[USO_FILE_MAX_OBJECT_NAME-1]=0;
//	SetName(pData->ObjectHead.Name);
//	if(m_pManager)
//		m_pManager->AddResource(this,GetName());
//
//	m_BoundingBox=pData->BoundingBox;
//	m_BoundingSphere=pData->BoundingSphere;	
//	m_BoneCount=pData->BoneCount;
//	
//	BYTE * pCurData=pBuff+sizeof(STORAGE_STRUCT);
//
//	if(m_BoneCount)
//	{
//		m_pBones=new BONED_MODEL_BONE[m_BoneCount];
//		memcpy(m_pBones,pCurData,m_BoneCount*sizeof(BONED_MODEL_BONE));
//	}
//
//	for(int i=0;i<pData->SubMeshCount;i++)
//	{
//
//		CD3DSubMesh * pD3DSubMesh=new CD3DSubMesh;
//		m_SubMeshList.push_back(pD3DSubMesh);
//		if(!pD3DSubMesh->FromUSOFile(pUSOFile,(UINT)(m_pManager->GetDevice())))
//		{
//			delete[] pBuff;
//			return false;
//		}		
//	}
//
//	
//
//	//RebuildBoneIndex();
//
//	delete[] pBuff;
//	return true;
//
//}

//CNameObject::STORAGE_STRUCT * CD3DBonedModelResource::USOCreateHead(UINT Param)
//{
//	STORAGE_STRUCT * pHead=new STORAGE_STRUCT;
//	ZeroMemory(pHead,sizeof(STORAGE_STRUCT));
//	pHead->Size=sizeof(STORAGE_STRUCT);
//	return pHead;
//}
//
//int CD3DBonedModelResource::USOWriteHead(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,UINT Param)
//{
//	UINT HeadSize=CD3DObjectResource::USOWriteHead(pHead,pUSOFile,Param);
//	if(HeadSize<0)
//		return -1;
//
//	STORAGE_STRUCT * pLocalHead=(STORAGE_STRUCT *)pHead;
//
//	pLocalHead->Size+=m_BoneCount*sizeof(BONED_MODEL_BONE);
//	pLocalHead->BoneCount=m_BoneCount;
//
//	return sizeof(STORAGE_STRUCT);
//}
//
//bool CD3DBonedModelResource::USOWriteData(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,UINT Param)
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
//	if(m_BoneCount)
//	{
//		if(pFile->Write(m_pBones,m_BoneCount*sizeof(BONED_MODEL_BONE))<m_BoneCount*sizeof(BONED_MODEL_BONE))
//			return false;
//	}
//	return true;
//}
//
//
//int CD3DBonedModelResource::USOReadHead(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,UINT Param)
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
//	m_BoneCount=pLocalHead->BoneCount;
//
//	return sizeof(STORAGE_STRUCT);
//}
//
//int CD3DBonedModelResource::USOReadData(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,BYTE * pData,int DataSize,UINT Param)
//{
//	int ReadSize=CD3DObjectResource::USOReadData(pHead,pUSOFile,pData,DataSize,Param);
//
//	pData+=ReadSize;
//	DataSize-=ReadSize;
//
//	if(m_BoneCount)
//	{
//		if(DataSize<m_BoneCount*(int)sizeof(BONED_MODEL_BONE))
//			return -1;
//		m_pBones=new BONED_MODEL_BONE[m_BoneCount];
//		memcpy(m_pBones,pData,m_BoneCount*sizeof(BONED_MODEL_BONE));		
//		ReadSize+=m_BoneCount*sizeof(BONED_MODEL_BONE);
//	}
//	return ReadSize;
//}

}