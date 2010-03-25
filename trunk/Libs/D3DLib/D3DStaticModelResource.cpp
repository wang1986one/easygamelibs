/****************************************************************************/
/*                                                                          */
/*      文件名:    D3DStaticModelResource.cpp                               */
/*      创建日期:  2009年09月25日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#include "StdAfx.h"

namespace D3DLib{



#define	SYS_FLAG			(*(int*)"ORB ")			// SYS 系统文件标志
#define SYS_VERSION			0x00010000
#define	SKELETON_FLAG		(*(int*)"BONE")			// 骨骼文件标志
#define SKELETON_VERSION	0x00010002
#define	ACTION_FLAG			(*(int*)"ACTN")			// 动化文件标志
#define ACTION_VERSION		0x00010002
#define	SKIN_FLAG			(*(int*)"SKIN")			// 皮肤文件标志
#define SKIN_VERSION		0x00010002
#define	MODEL_FLAG			(*(int*)"MDL ")			// 刚体文件标志
#define MODEL_VERSION		0x00010002


typedef std::map<int,int> CIntMap;

class CSubMesh
{
protected:
	struct INDEXDATA
	{
		WORD NewIndex;
		WORD OldIndex;
	};
	CEasyArray<BYTE> m_VertextData;
	CEasyArray<INDEXDATA> m_IndexData;
	int m_VertextSize;
	int m_MatterialID;
	CIntMap m_IndexMap;
public:
	CSubMesh(){m_VertextSize=0;m_MatterialID=0;}
	~CSubMesh(){}

	void SetMaterialID(int ID)
	{
		m_MatterialID=ID;
	}
	int GetMaterialID()
	{
		return m_MatterialID;
	}

	bool AddVertext(WORD Index,BYTE * pVertextData,int VertextSize)
	{		
		INDEXDATA IndexData;
		bool HaveFind=false;
		IndexData.OldIndex=Index;
		if(m_VertextSize)
		{
			if(m_VertextSize!=VertextSize)
				return false;
		}
		else
			m_VertextSize=VertextSize;		
		int NewIndex=m_IndexMap[Index]-1;
		if(NewIndex>=0)
		{		
			IndexData.NewIndex=NewIndex;
			m_IndexData.Add(IndexData);
		}
		else
		{
			IndexData.NewIndex=(WORD)((int)m_VertextData.GetCount()/VertextSize);
			m_IndexMap[Index]=IndexData.NewIndex+1;
			m_IndexData.Add(IndexData);
			for(int i=0;i<VertextSize;i++)
			{
				m_VertextData.Add(pVertextData[i]);
			}
		}
		return true;

	}
	int GetIndexCount()
	{
		return (int)m_IndexData.GetCount();
	}
	int GetVertexCount()
	{
		if(m_VertextSize)
			return (int)(int)m_VertextData.GetCount()/m_VertextSize;
		else
			return 0;
	}
	int GetVertexSize()
	{
		return m_VertextSize;
	}
	void GetIndex(WORD * pBuff,int Size)
	{
		if(Size>GetIndexCount())
			Size=GetIndexCount();
		for(int i=0;i<Size;i++)
			pBuff[i]=m_IndexData[i].NewIndex;
	}
	void GetVertex(LPVOID pBuff,int Size)
	{
		if(Size<(int)m_VertextData.GetCount())
			Size=(int)(int)m_VertextData.GetCount();
		memcpy(pBuff,&(m_VertextData[0]),Size);
	}

};

struct MDL_HEADER
{
	DWORD	MainFlag;
	DWORD	MainVersion;
	DWORD	SubFlag;
	DWORD	SubVersion;
};

struct MDL_DATA_BLOCK_HEADER
{
	int Offset;
	int Length;
};

enum MDL_BLOCK_TYPE
{
	BLOCK_NAMETBL,
	BLOCK_MATERIAL,
	BLOCK_MESHPART,
	BLOCK_MESHVERTEX,
	BLOCK_MESHINDEX,
	BLOCK_DUMMY,
	BLOCK_MAX,
};

struct MDL_MATERIAL
{
	int	  nMainTexOffset;
	int	  nAddtionOffset;
	int	  nFxOffset;
	int	  nUnknown;
};

struct MDA_MATERIAL
{
	int	  nMainTexOffset;
	int	  nAddtionOffset;
	int	  nFxOffset;	
};

struct MDL_MESHPART
{
	int		nMaterial;			// 材质编号
	int		nVertexCount;
	int		nFirstVertex;
	int		nFaceCount;
	int		nFirstIndex;
};
struct MDL_MESHVERTEX
{
	D3DXVECTOR3	position;
	D3DXVECTOR3	normal;
	D3DXVECTOR2	uv;			//贴图坐标	
};



enum SMDL_BLOCK_TYPE
{
	SMDLB_GEOSET,
	SMDLB_GEOSETINFO,
	SMDLB_GEOSETPOS,
	SMDLB_GEOSETNORMAL,
	SMDLB_GEOSETUV,
	SMDLB_GEOSETINDICES,
	SMDLB_GEOSETVISTRACK,
	SMDLB_GEOSETBONEWEIGHTGROUUP,
	SMDLB_GEOSETBONEWEIGHTS,
	SMDLB_GEOSETVERTEXID,
	SMDLB_GEOSETMATERIAL,
	SMDLB_NAMETABLE,
	SMDLB_MODELINFO,
	SMDLB_ANIMATION,
	SMDLB_COLLISIONGRID,
	SMDLB_COLLISIONGRID2,
	SMDLB_COLLISIONGRID3,
};

struct SMDL_BLOCK_HEADER
{
	DWORD Type;
	DWORD Size;
};

struct SMDL_SUB_MESH
{
	CD3DVector3 *	pPos;
	CD3DVector3 *	pNormals;
	CD3DVector2 *	pUVs;
	WORD *			pIndexs;

	WORD			Property;
	WORD			UVSuit;

	int				VertexCount;
	int				IndexCount;
	int				MaterialID;

	CD3DBoundingBox		BoundingBox;
	CD3DBoundingSphere	BoundingSphere;
	SMDL_SUB_MESH()
	{
		ZeroMemory(this,sizeof(*this));
	}
	SMDL_SUB_MESH(const SMDL_SUB_MESH &SubMesh)
	{
		pPos=SubMesh.pPos;
		pNormals=SubMesh.pNormals;
		pUVs=SubMesh.pUVs;
		pIndexs=SubMesh.pIndexs;
		Property=SubMesh.Property;
		UVSuit=SubMesh.UVSuit;
		VertexCount=SubMesh.VertexCount;
		IndexCount=SubMesh.IndexCount;
		MaterialID=SubMesh.MaterialID;
	}
};

struct SMDL_MATERIAL
{
	int			MainTexID;		// 主贴图文件名索引
	int			AddtionID;		// 附加贴图文件名索引
	int			FxID;				// Fx文件名索引
	SMDL_MATERIAL()
	{
		MainTexID =0;
		AddtionID = 0;
		FxID = 0;
	}

};

enum	SMDL_SUB_MESH_PROPERTY
{
	SMLP_TIME				= 0x03,	// 2bit,白昼的隐藏属性。
	SMLP_RENDER				= 0x0C,	// 2bit,渲染类型
	SMLP_NOCASTSHADOW		= 0x10,	// 1bit,是否投射阴影
	SMLP_NORCVSHADOW		= 0x20,	// 1bit,是否接受阴影
	SMLP_BRIGE				= 0X40,
	SMLP_ALWAYSANIM			= 0X80,

	SMLP_TIMESHIFT			= 0,
	SMLP_RENDERSHIFT		= 2,
	SMLP_CASTSHIFT			= 4,
	SMLP_RCVSHIFT			= 5,
	SMLP_BRIGESHIFT			= 6,
	SMLP_ALWAYSANIMSHIFT	= 7,
};

IMPLEMENT_FILE_PATH_MANAGER(CD3DStaticModelResource)

IMPLEMENT_FILE_CHANNEL_MANAGER(CD3DStaticModelResource)

IMPLEMENT_CLASS_INFO(CD3DStaticModelResource,CD3DObjectResource);

CD3DStaticModelResource::CD3DStaticModelResource()
:CD3DObjectResource()
{	
	m_pDummys=NULL;
	m_DummyCount=0;	
}

CD3DStaticModelResource::CD3DStaticModelResource(CD3DObjectResourceManager* pManager)
	:CD3DObjectResource(pManager)
{	
	m_pDummys=NULL;
	m_DummyCount=0;	
}

CD3DStaticModelResource::~CD3DStaticModelResource(void)
{
	Destory();
}

void CD3DStaticModelResource::Destory()
{	
	
	SAFE_DELETE_ARRAY(m_pDummys);	
	m_DummyCount=0;
	CD3DObjectResource::Destory();
}



bool CD3DStaticModelResource::LoadFromXFile(LPCTSTR FileName,bool UseSystemMemoryCopy)
{
	HRESULT hr;
	ID3DXMesh	*pMesh;	
	ID3DXBuffer *pMaterialsBuffer;	
	D3DXMATERIAL *pMaterials;	
	DWORD NumMaterials;		
	DWORD * pAttributes;
	CEasyArray<CSubMesh>  OrgSubMeshs;
	BYTE * pVertexBuff=NULL;
	WORD * pIndexBuff=NULL;

	Destory();	

	CEasyString ModelFileName=FindFileOne(FileName);

	IFileAccessor * pFile;

	pFile=CFileSystemManager::GetInstance()->CreateFileAccessor(m_FileChannel);
	if(pFile==NULL)
		return false;
	if(!pFile->Open(ModelFileName,IFileAccessor::modeRead))
	{
		pFile->Release();
		return false;	
	}
	int FileSize=(int)pFile->GetSize();	
	BYTE * pData=new BYTE[FileSize];
	pFile->Read(pData,FileSize);
	pFile->Release();

	hr=D3DXLoadMeshFromXInMemory( pData,FileSize, D3DXMESH_SYSTEMMEM, 
		m_pManager->GetDevice()->GetD3DDevice(),
		NULL, &pMaterialsBuffer, NULL,
		&NumMaterials, &pMesh);

	delete[] pData;

	if(FAILED(hr))
		return false;


	//获取顶点数据
	
	LPDIRECT3DVERTEXBUFFER9 pVertexsBuffer;
	D3DVERTEXBUFFER_DESC	VertexsBufferDesc;
	pMesh->GetVertexBuffer(&pVertexsBuffer);
	pVertexsBuffer->GetDesc(&VertexsBufferDesc);	
	pVertexsBuffer->Lock(0,0,(LPVOID *)&pVertexBuff,D3DLOCK_READONLY);	
	
	int VertexSize=pMesh->GetNumBytesPerVertex();
	int VertexCount=VertexsBufferDesc.Size/VertexSize;

	//获取索引数据
	LPDIRECT3DINDEXBUFFER9	pIndexsBuffer;
	D3DINDEXBUFFER_DESC		IndexsBufferDesc;

	pMesh->GetIndexBuffer(&pIndexsBuffer);
	pIndexsBuffer->GetDesc(&IndexsBufferDesc);			
	pIndexsBuffer->Lock(0,0,(LPVOID *)&pIndexBuff,D3DLOCK_READONLY);		
	
	int IndexCount=IndexsBufferDesc.Size/sizeof(WORD);



	//获取材质和面的对应信息
	pMaterials=(D3DXMATERIAL *)pMaterialsBuffer->GetBufferPointer();

	hr=pMesh->LockAttributeBuffer(D3DLOCK_READONLY,&pAttributes);
	if(FAILED(hr))
	{
		SAFE_RELEASE(pMesh);	
		SAFE_RELEASE(pMaterialsBuffer);
		return false;
	}	
	//按材质建立SubMesh
	OrgSubMeshs.Resize(NumMaterials);
	for(int i=0;i<(int)OrgSubMeshs.GetCount();i++)
	{
		OrgSubMeshs[i].SetMaterialID(i);
	}

	//按材质划分SubMesh
	for(int i=0;i<(int)pMesh->GetNumFaces();i++)
	{
		OrgSubMeshs[pAttributes[i]].AddVertext(pIndexBuff[i*3],pVertexBuff+pIndexBuff[i*3]*VertexSize,VertexSize);
		OrgSubMeshs[pAttributes[i]].AddVertext(pIndexBuff[i*3+1],pVertexBuff+pIndexBuff[i*3+1]*VertexSize,VertexSize);
		OrgSubMeshs[pAttributes[i]].AddVertext(pIndexBuff[i*3+2],pVertexBuff+pIndexBuff[i*3+2]*VertexSize,VertexSize);

	}

	pMesh->UnlockAttributeBuffer();


	//除去空的SubMesh
	for(int i=NumMaterials-1;i>=0;i--)
	{
		if(OrgSubMeshs[i].GetIndexCount()<=0)
			OrgSubMeshs.Delete(i);
	}

	

	


	for(UINT i=0;i<OrgSubMeshs.GetCount();i++)
	{
		CD3DSubMesh * pD3DSubMesh=new CD3DSubMesh;

		pD3DSubMesh->GetVertexFormat().FVF=pMesh->GetFVF();
		pD3DSubMesh->GetVertexFormat().VertexSize=VertexSize;
		pD3DSubMesh->GetVertexFormat().IndexSize=sizeof(WORD);
		pD3DSubMesh->SetPrimitiveType(D3DPT_TRIANGLELIST);
		pD3DSubMesh->SetPrimitiveCount(OrgSubMeshs[i].GetIndexCount()/3);		
		pD3DSubMesh->SetVertexCount(OrgSubMeshs[i].GetVertexCount());		
		pD3DSubMesh->SetIndexCount(OrgSubMeshs[i].GetIndexCount());

		int Size=OrgSubMeshs[i].GetVertexCount()*VertexSize;		

		pD3DSubMesh->AllocDXVertexBuffer(m_pManager->GetDevice());

		
		BYTE * pBuff;
		pD3DSubMesh->GetDXVertexBuffer()->Lock(0,0,(LPVOID *)&pBuff,0);
		OrgSubMeshs[i].GetVertex(pBuff,Size);
		pD3DSubMesh->GetDXVertexBuffer()->Unlock();

		Size=OrgSubMeshs[i].GetIndexCount();

		pD3DSubMesh->AllocDXIndexBuffer(m_pManager->GetDevice());	


		pD3DSubMesh->GetDXIndexBuffer()->Lock(0,0,(LPVOID *)&pBuff,0);
		OrgSubMeshs[i].GetIndex((WORD *)pBuff,Size);
		pD3DSubMesh->GetDXIndexBuffer()->Unlock();




		int MatID=OrgSubMeshs[i].GetMaterialID();



		pD3DSubMesh->GetMaterial().SetMaterial(pMaterials[MatID].MatD3D);
		if(pMaterials[MatID].pTextureFilename)
		{
			
			CD3DTexture * pTexture=m_pManager->GetDevice()->GetTextureManager()->LoadTexture(pMaterials[MatID].pTextureFilename,0);
			if(pTexture)
				pD3DSubMesh->GetMaterial().AddTexture(pTexture,0);
			CD3DFX * pFX=m_pManager->GetDevice()->GetFXManager()->LoadFXFromMemory("DEFAULT_MODEL_FX",(void *)DEFAULT_MODEL_FX,(int)strlen(DEFAULT_MODEL_FX));
			pD3DSubMesh->GetMaterial().SetFX(pFX);
				
		}		
		else
		{		
			CD3DFX * pFX=m_pManager->GetDevice()->GetFXManager()->LoadFXFromMemory("DEFAULT_MODEL_FX_NT",(void *)DEFAULT_MODEL_FX_NT,(int)strlen(DEFAULT_MODEL_FX_NT));
			pD3DSubMesh->GetMaterial().SetFX(pFX);
		}

		m_SubMeshList.Add(pD3DSubMesh);

	}	

	pVertexsBuffer->Unlock();
	pIndexsBuffer->Unlock();
	SAFE_RELEASE(pVertexsBuffer);
	SAFE_RELEASE(pIndexsBuffer);
	SAFE_RELEASE(pMesh);	
	SAFE_RELEASE(pMaterialsBuffer);
	CreateBounding();
	return true;
}



bool CD3DStaticModelResource::LoadFromMDL(LPCTSTR FileName,bool UseSystemMemoryCopy)
{
	IFileAccessor * pFile;

	Destory();

	CEasyString ModelFileName=FindFileOne(FileName);

	pFile=CFileSystemManager::GetInstance()->CreateFileAccessor(m_FileChannel);
	if(pFile==NULL)
		return false;
	if(!pFile->Open(ModelFileName,IFileAccessor::modeRead))
	{
		pFile->Release();
		return false;	
	}
	int FileSize=(int)pFile->GetSize();	
	BYTE * pData=new BYTE[FileSize];
	pFile->Read(pData,FileSize);
	pFile->Release();

	BYTE * DataReadPtr=pData;
	MDL_HEADER * pHeader=(MDL_HEADER *)DataReadPtr;
	DataReadPtr+=sizeof(MDL_HEADER);

	//校验文件类型和版本号
	if( (pHeader->MainFlag != SYS_FLAG) || (pHeader->MainVersion != SYS_VERSION) ||
		(pHeader->SubFlag != MODEL_FLAG) || (pHeader->SubVersion != MODEL_VERSION))
	{
		delete[] pData;
		return false;
	}
	
	MDL_DATA_BLOCK_HEADER * pBlockHeaders=(MDL_DATA_BLOCK_HEADER *)DataReadPtr;

	char*				pNameTables			= NULL;
	MDL_MATERIAL*		pMaterials			= NULL;
	MDL_MESHPART*		pMeshParts			= NULL;	
	MDL_MESHVERTEX *	pMeshVertices		= NULL;
	WORD*				pMeshIndex			= NULL;
	STATIC_MODEL_DUMMY*			pDummys				= NULL;

	int MaterialCount;
	int MeshPartCount;
	int VertexCount;
	int IndexCount;	
	int DummyCount;

	pNameTables=(char *)(DataReadPtr+pBlockHeaders[BLOCK_NAMETBL].Offset);

	pMaterials=(MDL_MATERIAL *)(DataReadPtr+pBlockHeaders[BLOCK_MATERIAL].Offset);
	MaterialCount=pBlockHeaders[BLOCK_MATERIAL].Length/sizeof(MDL_MATERIAL);

	pMeshParts=(MDL_MESHPART *)(DataReadPtr+pBlockHeaders[BLOCK_MESHPART].Offset);
	MeshPartCount=pBlockHeaders[BLOCK_MESHPART].Length/sizeof(MDL_MESHPART);

	pMeshVertices=(MDL_MESHVERTEX *)(DataReadPtr+pBlockHeaders[BLOCK_MESHVERTEX].Offset);
	VertexCount=pBlockHeaders[BLOCK_MESHVERTEX].Length/sizeof(MDL_MESHVERTEX);

	pMeshIndex=(WORD *)(DataReadPtr+pBlockHeaders[BLOCK_MESHINDEX].Offset);
	IndexCount=pBlockHeaders[BLOCK_MESHINDEX].Length/sizeof(WORD);

	pDummys=(STATIC_MODEL_DUMMY *)(DataReadPtr+pBlockHeaders[BLOCK_DUMMY].Offset);
	DummyCount=pBlockHeaders[BLOCK_DUMMY].Length/sizeof(STATIC_MODEL_DUMMY);

	m_pDummys=new STATIC_MODEL_DUMMY[DummyCount];
	memcpy(m_pDummys,pDummys,sizeof(STATIC_MODEL_DUMMY)*DummyCount);
	m_DummyCount=DummyCount;

	LPDIRECT3DVERTEXBUFFER9 pVertexBuffer;	
	m_pManager->GetDevice()->GetD3DDevice()->CreateVertexBuffer( 
		VertexCount*sizeof(MDL_MESHVERTEX),
		D3DUSAGE_WRITEONLY, 0,
		D3DPOOL_SYSTEMMEM, &pVertexBuffer, NULL );

	BYTE * pBuff;
	pVertexBuffer->Lock(0,0,(LPVOID *)&pBuff,0);
	memcpy(pBuff,pMeshVertices,VertexCount*sizeof(MDL_MESHVERTEX));
	pVertexBuffer->Unlock();

	LPDIRECT3DINDEXBUFFER9 pIndexBuffer;
	m_pManager->GetDevice()->GetD3DDevice()->CreateIndexBuffer(IndexCount*sizeof(WORD),
		D3DUSAGE_WRITEONLY,			
		D3DFMT_INDEX16,
		D3DPOOL_SYSTEMMEM,
		&pIndexBuffer,
		NULL);
	pIndexBuffer->Lock(0,0,(LPVOID *)&pBuff,0);
	memcpy(pBuff,pMeshIndex,IndexCount*sizeof(WORD));
	pIndexBuffer->Unlock();




	for(int i=0;i<MeshPartCount;i++)
	{

		CD3DSubMesh * pD3DSubMesh=new CD3DSubMesh;
		
		pD3DSubMesh->GetVertexFormat().FVF=D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1;
		pD3DSubMesh->GetVertexFormat().VertexSize=sizeof(MDL_MESHVERTEX);
		pD3DSubMesh->GetVertexFormat().IndexSize=sizeof(WORD);
		pD3DSubMesh->SetPrimitiveType(D3DPT_TRIANGLELIST);
		pD3DSubMesh->SetPrimitiveCount(pMeshParts[i].nFaceCount);		
		pD3DSubMesh->SetVertexCount(pMeshParts[i].nVertexCount);
		pD3DSubMesh->SetVertexStart(pMeshParts[i].nFirstVertex);
		pD3DSubMesh->SetIndexCount(pMeshParts[i].nFaceCount*3);
		pD3DSubMesh->SetIndexStart(pMeshParts[i].nFirstIndex);
		pD3DSubMesh->SetDXVertexBuffer(pVertexBuffer,true);
		pVertexBuffer->AddRef();
		pD3DSubMesh->SetDXIndexBuffer(pIndexBuffer,true);
		pIndexBuffer->AddRef();


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
				if(pTexture)
					pD3DSubMesh->GetMaterial().AddTexture(pTexture,0);

			}
			else
			{			
				CD3DTexture * pTexture=m_pManager->GetDevice()->GetTextureManager()->LoadTexture(FileName,0);
				if(pTexture)
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
				if(pTexture)
					pD3DSubMesh->GetMaterial().AddTexture(pTexture,0);

			}
			else
			{			
				CD3DTexture * pTexture=m_pManager->GetDevice()->GetTextureManager()->LoadTexture(FileName,0);
				if(pTexture)
					pD3DSubMesh->GetMaterial().AddTexture(pTexture,0);
			}				
		}		

		if(pMaterials[MatID].nFxOffset>=0)
		{
			FileName.Format("%s.fx",pNameTables+pMaterials[MatID].nFxOffset);

			CD3DFX * pFX=m_pManager->GetDevice()->GetFXManager()->LoadFX(FileName);
			pD3DSubMesh->GetMaterial().SetFX(pFX);

			if(pFX)
			{
				int TransparentFactor;

				if(pFX->GetInt("TransparentFactor",TransparentFactor))
				{
					if(TransparentFactor>=4)
						pD3DSubMesh->SetTransparent(true);
				}
			}
		}	
		m_SubMeshList.Add(pD3DSubMesh);

	}	

	SAFE_RELEASE(pVertexBuffer);
	SAFE_RELEASE(pIndexBuffer);
	delete[] pData;
	CreateBounding();
	return true;
}

bool CD3DStaticModelResource::LoadFromSMDL(LPCTSTR FileName,bool UseSystemMemoryCopy)
{
	IFileAccessor * pFile;

	CEasyArray<SMDL_SUB_MESH>	m_SMDLSubMeshs;
	SMDL_SUB_MESH *				pCurSMDLSubMesh=NULL;
	char *						pNameTables=NULL;
	SMDL_MATERIAL *				pMaterials=NULL;

	Destory();

	CEasyString ModelFileName=FindFileOne(FileName);

	pFile=CFileSystemManager::GetInstance()->CreateFileAccessor(m_FileChannel);

	if(pFile==NULL)
		return false;

	if(!pFile->Open(ModelFileName,IFileAccessor::modeRead))
	{
		pFile->Release();
		return false;
	}

	char SMDLFlag[4];
	pFile->Read(SMDLFlag,4);
	if(strncmp(SMDLFlag,"SMDL",4)!=0)
	{
		pFile->Release();
		return false;
	}
	while(!pFile->IsEOF())
	{
		SMDL_BLOCK_HEADER BlockHeader;
		int ReadSize=(int)pFile->Read(&BlockHeader,sizeof(BlockHeader));
		if(ReadSize!=sizeof(BlockHeader))
			break;
		switch(BlockHeader.Type)
		{
		case SMDLB_GEOSET:
			m_SMDLSubMeshs.Resize(m_SMDLSubMeshs.GetCount()+1);
			pCurSMDLSubMesh=&(m_SMDLSubMeshs[m_SMDLSubMeshs.GetCount()-1]);
			break;
		case SMDLB_GEOSETINFO:
			pFile->Read(&pCurSMDLSubMesh->MaterialID,sizeof(int));
			pFile->Read(&pCurSMDLSubMesh->Property,sizeof(WORD));			
			pFile->Read(&pCurSMDLSubMesh->UVSuit,sizeof(WORD));
			pFile->Read(&pCurSMDLSubMesh->BoundingBox.m_Max,sizeof(CD3DVector3));
			pFile->Read(&pCurSMDLSubMesh->BoundingBox.m_Min,sizeof(CD3DVector3));
			pFile->Read(&pCurSMDLSubMesh->BoundingSphere.m_Radius,sizeof(FLOAT));		
			pCurSMDLSubMesh->BoundingSphere.m_Center=
				(pCurSMDLSubMesh->BoundingBox.m_Min+pCurSMDLSubMesh->BoundingBox.m_Max)/2;
			break;
		case SMDLB_GEOSETPOS:
			{
				DWORD Size;
				pFile->Read(&Size,sizeof(DWORD));
				pCurSMDLSubMesh->VertexCount=Size;

				pCurSMDLSubMesh->pPos=new CD3DVector3[Size];
				pFile->Read(pCurSMDLSubMesh->pPos,Size*sizeof(CD3DVector3));
			}
			break;
		case SMDLB_GEOSETNORMAL:
			{
				DWORD Size;
				pFile->Read(&Size,sizeof(DWORD));				

				pCurSMDLSubMesh->pNormals=new CD3DVector3[Size];
				pFile->Read(pCurSMDLSubMesh->pNormals,Size*sizeof(CD3DVector3));
			}
			break;
		case SMDLB_GEOSETUV:
			{
				DWORD Size;
				pFile->Read(&Size,sizeof(DWORD));

				pCurSMDLSubMesh->pUVs=new CD3DVector2[Size];
				pFile->Read(pCurSMDLSubMesh->pUVs,Size*sizeof(CD3DVector2));
			}
			break;
		case SMDLB_GEOSETINDICES:
			{
				DWORD Size;
				pFile->Read(&Size,sizeof(DWORD));
				pCurSMDLSubMesh->IndexCount=Size;

				pCurSMDLSubMesh->pIndexs=new WORD[Size];
				pFile->Read(pCurSMDLSubMesh->pIndexs,Size*sizeof(WORD));
			}
			break;
		case SMDLB_NAMETABLE:
			{
				DWORD Size;
				pFile->Read(&Size,sizeof(DWORD));

				pNameTables=new char[Size];

				pFile->Read(pNameTables,Size*sizeof(char));
			}
			break;
		case SMDLB_GEOSETMATERIAL:
			{
				DWORD Size;
				pFile->Read(&Size,sizeof(DWORD));

				pMaterials=new SMDL_MATERIAL[Size];
				pFile->Read(pMaterials,Size*sizeof(SMDL_MATERIAL));
			}
			break;
		case SMDLB_GEOSETVISTRACK:
		case SMDLB_GEOSETBONEWEIGHTGROUUP:
		case SMDLB_GEOSETBONEWEIGHTS:
		case SMDLB_GEOSETVERTEXID:
				
		case SMDLB_MODELINFO:
		case SMDLB_ANIMATION:
		case SMDLB_COLLISIONGRID:
		case SMDLB_COLLISIONGRID2:
		case SMDLB_COLLISIONGRID3:
		default:
			pFile->Seek(BlockHeader.Size,IFileAccessor::seekCurrent);
			break;
		}
	}
	pFile->Release();

	

	

	for(int i=0;i<(int)m_SMDLSubMeshs.GetCount();i++)
	{
		CD3DSubMesh * pD3DSubMesh=new CD3DSubMesh;

		int VertexSize=0;
		
		if(m_SMDLSubMeshs[i].UVSuit==2)
		{
			pD3DSubMesh->GetVertexFormat().FVF=D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX2;
			VertexSize=sizeof(STATIC_MODEL_VERTEXT_TEX2);
			
		}
		else
		{		
			pD3DSubMesh->GetVertexFormat().FVF=D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1;
			VertexSize=sizeof(STATIC_MODEL_VERTEXT_TEX1);
		}
		pD3DSubMesh->GetVertexFormat().VertexSize=VertexSize;
		pD3DSubMesh->GetVertexFormat().IndexSize=sizeof(WORD);
		pD3DSubMesh->SetPrimitiveType(D3DPT_TRIANGLELIST);
		pD3DSubMesh->SetPrimitiveCount(m_SMDLSubMeshs[i].IndexCount/3);		
		pD3DSubMesh->SetVertexCount(m_SMDLSubMeshs[i].VertexCount);
		pD3DSubMesh->SetIndexCount(m_SMDLSubMeshs[i].IndexCount);
		if(m_SMDLSubMeshs[i].Property&SMLP_BRIGE)
		{
			pD3DSubMesh->SetProperty(1);
		}

		if(pD3DSubMesh->GetVertexCount()==0)
			continue;

		

		pD3DSubMesh->AllocDXVertexBuffer(m_pManager->GetDevice());
		
		if(m_SMDLSubMeshs[i].UVSuit==2)
		{
			STATIC_MODEL_VERTEXT_TEX2 * pBuff;
			pD3DSubMesh->GetDXVertexBuffer()->Lock(0,0,(LPVOID *)&pBuff,0);
			for(UINT j=0;j<pD3DSubMesh->GetVertexCount();j++)
			{
				pBuff[j].Pos=m_SMDLSubMeshs[i].pPos[j];
				pBuff[j].Normal=m_SMDLSubMeshs[i].pNormals[j];
				pBuff[j].Tex1=m_SMDLSubMeshs[i].pUVs[j*2];
				pBuff[j].Tex2=m_SMDLSubMeshs[i].pUVs[j*2+1];
			}
			pD3DSubMesh->GetDXVertexBuffer()->Unlock();
		}
		else
		{
			STATIC_MODEL_VERTEXT_TEX1 * pBuff;
			pD3DSubMesh->GetDXVertexBuffer()->Lock(0,0,(LPVOID *)&pBuff,0);
			for(UINT j=0;j<pD3DSubMesh->GetVertexCount();j++)
			{
				pBuff[j].Pos=m_SMDLSubMeshs[i].pPos[j];
				pBuff[j].Normal=m_SMDLSubMeshs[i].pNormals[j];
				pBuff[j].Tex1=m_SMDLSubMeshs[i].pUVs[j];				
			}
			pD3DSubMesh->GetDXVertexBuffer()->Unlock();
		}

		

		pD3DSubMesh->AllocDXIndexBuffer(m_pManager->GetDevice());

		

		WORD * pBuff;
		pD3DSubMesh->GetDXIndexBuffer()->Lock(0,0,(LPVOID *)&pBuff,0);
		memcpy(pBuff,m_SMDLSubMeshs[i].pIndexs,pD3DSubMesh->GetIndexCount()*sizeof(WORD));
		pD3DSubMesh->GetDXIndexBuffer()->Unlock();

		
		

		int MatID=m_SMDLSubMeshs[i].MaterialID;



		D3DCOLORVALUE WhiteColor={1.0f,1.0f,1.0f,1.0f};
		D3DCOLORVALUE BlackColor={0.0f,0.0f,0.0f,1.0f};

		pD3DSubMesh->GetMaterial().GetMaterial().Ambient=WhiteColor;
		pD3DSubMesh->GetMaterial().GetMaterial().Diffuse=WhiteColor;
		pD3DSubMesh->GetMaterial().GetMaterial().Specular=WhiteColor;
		pD3DSubMesh->GetMaterial().GetMaterial().Emissive=BlackColor;
		pD3DSubMesh->GetMaterial().GetMaterial().Power=40.0f;

		CEasyString FileName;

		if(pMaterials[MatID].MainTexID>=0)
		{		
			FileName.Format("%s.*",pNameTables+pMaterials[MatID].MainTexID);
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

		if(pMaterials[MatID].AddtionID>=0)
		{		
			FileName.Format("%s.*",pNameTables+pMaterials[MatID].AddtionID);
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
		
		if(pMaterials[MatID].FxID>=0)
		{
			FileName.Format("%s.fx",pNameTables+pMaterials[MatID].FxID);

			CD3DFX * pFX=m_pManager->GetDevice()->GetFXManager()->LoadFX(FileName);
			pD3DSubMesh->GetMaterial().SetFX(pFX);

			if(pFX)
			{			
				int TransparentFactor;

				if(pFX->GetInt("TransparentFactor",TransparentFactor))
				{
					if(TransparentFactor>=4)
						pD3DSubMesh->SetTransparent(true);
				}
			}
		}		
		m_BoundingBox=m_SMDLSubMeshs[i].BoundingBox;
		m_BoundingSphere=m_SMDLSubMeshs[i].BoundingSphere;

		m_SubMeshList.Add(pD3DSubMesh);

		
		SAFE_DELETE_ARRAY(m_SMDLSubMeshs[i].pPos);
		SAFE_DELETE_ARRAY(m_SMDLSubMeshs[i].pNormals);
		SAFE_DELETE_ARRAY(m_SMDLSubMeshs[i].pUVs);
		SAFE_DELETE_ARRAY(m_SMDLSubMeshs[i].pIndexs);
	}
	SAFE_DELETE_ARRAY(pMaterials);
	SAFE_DELETE_ARRAY(pNameTables);
	CreateBounding();
	return true;
}

bool CD3DStaticModelResource::LoadFromMDA(IFileAccessor * pFile,bool UseSystemMemoryCopy)
{
	char*				pNameTables			= NULL;
	MDA_MATERIAL*		pMaterials			= NULL;
	MDL_MESHPART*		pMeshParts			= NULL;	
	MDL_MESHVERTEX *	pMeshVertices		= NULL;
	D3DCOLOR*			pMeshVertexColor	= NULL;
	WORD*				pMeshIndex			= NULL;	

	int MaterialCount;
	int MeshPartCount;
	int VertexCount;
	int IndexCount;	
	

	int FrameIndex;
	pFile->Read(&FrameIndex,sizeof(int));

	int DataLen;

	pFile->Read(&DataLen,sizeof(int));
	pNameTables=new char[DataLen];
	pFile->Read(pNameTables,DataLen);

	pFile->Read(&DataLen,sizeof(int));
	pMaterials=(MDA_MATERIAL*)new char[DataLen];
	pFile->Read(pMaterials,DataLen);
	MaterialCount=DataLen/sizeof(MDA_MATERIAL);

	pFile->Read(&DataLen,sizeof(int));
	pMeshParts=(MDL_MESHPART*)new char[DataLen];
	pFile->Read(pMeshParts,DataLen);
	MeshPartCount=DataLen/sizeof(MDL_MESHPART);

	pFile->Read(&DataLen,sizeof(int));
	pMeshVertices=(MDL_MESHVERTEX*)new char[DataLen];
	pFile->Read(pMeshVertices,DataLen);
	VertexCount=DataLen/sizeof(MDL_MESHVERTEX);

	pFile->Read(&DataLen,sizeof(int));
	pMeshVertexColor=(D3DCOLOR*)new char[DataLen];
	pFile->Read(pMeshVertexColor,DataLen);

	pFile->Read(&DataLen,sizeof(int));
	pMeshIndex=(WORD*)new char[DataLen];
	pFile->Read(pMeshIndex,DataLen);
	IndexCount=DataLen/sizeof(WORD);

	LPDIRECT3DVERTEXBUFFER9 pVertexBuffer;	
	m_pManager->GetDevice()->GetD3DDevice()->CreateVertexBuffer( 
		VertexCount*sizeof(STATIC_MODEL_VERTEXT_DIFFUSE_TEX1),
		D3DUSAGE_WRITEONLY, 0,
		D3DPOOL_SYSTEMMEM, &pVertexBuffer, NULL );

	STATIC_MODEL_VERTEXT_DIFFUSE_TEX1* pVertex;
	pVertexBuffer->Lock(0,0,(LPVOID *)&pVertex,0);
	for(int i=0;i<VertexCount;i++)
	{
		pVertex[i].Pos=pMeshVertices[i].position;
		pVertex[i].Diffuse=pMeshVertexColor[i];
		pVertex[i].Tex1=pMeshVertices[i].uv;
	}	
	pVertexBuffer->Unlock();

	BYTE * pBuff;
	LPDIRECT3DINDEXBUFFER9 pIndexBuffer;
	m_pManager->GetDevice()->GetD3DDevice()->CreateIndexBuffer(IndexCount*sizeof(WORD),
		D3DUSAGE_WRITEONLY,			
		D3DFMT_INDEX16,
		D3DPOOL_SYSTEMMEM,
		&pIndexBuffer,
		NULL);
	pIndexBuffer->Lock(0,0,(LPVOID *)&pBuff,0);
	memcpy(pBuff,pMeshIndex,IndexCount*sizeof(WORD));
	pIndexBuffer->Unlock();



	for(int i=0;i<MeshPartCount;i++)
	{

		CD3DSubMesh * pD3DSubMesh=new CD3DSubMesh;
		
		pD3DSubMesh->GetVertexFormat().FVF=D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1;
		pD3DSubMesh->GetVertexFormat().VertexSize=sizeof(STATIC_MODEL_VERTEXT_DIFFUSE_TEX1);
		pD3DSubMesh->GetVertexFormat().IndexSize=sizeof(WORD);
		pD3DSubMesh->SetPrimitiveType(D3DPT_TRIANGLELIST);
		pD3DSubMesh->SetPrimitiveCount(pMeshParts[i].nFaceCount);
		pD3DSubMesh->SetVertexCount(pMeshParts[i].nVertexCount);
		pD3DSubMesh->SetVertexStart(pMeshParts[i].nFirstVertex);
		pD3DSubMesh->SetIndexCount(pMeshParts[i].nFaceCount*3);
		pD3DSubMesh->SetIndexStart(pMeshParts[i].nFirstIndex);
		pD3DSubMesh->SetDXVertexBuffer(pVertexBuffer,true);
		pVertexBuffer->AddRef();
		pD3DSubMesh->SetDXIndexBuffer(pIndexBuffer);
		pIndexBuffer->AddRef();


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
			if(pFX==NULL)
			{
				pFX=m_pManager->GetDevice()->GetFXManager()->GetFX("DEFAULT_MDA_FX");
				if(pFX)
					pFX->AddUseRef();
			}
			pD3DSubMesh->GetMaterial().SetFX(pFX);

			if(pFX)
			{
				int TransparentFactor;

				if(pFX->GetInt("TransparentFactor",TransparentFactor))
				{
					if(TransparentFactor>=4)
						pD3DSubMesh->SetTransparent(true);
				}
			}
		}	
		m_SubMeshList.Add(pD3DSubMesh);

	}	

	SAFE_RELEASE(pVertexBuffer);
	SAFE_RELEASE(pIndexBuffer);

	SAFE_DELETE(pNameTables);
	SAFE_DELETE(pMaterials);
	SAFE_DELETE(pMeshParts);
	SAFE_DELETE(pMeshVertices);
	SAFE_DELETE(pMeshVertexColor);
	SAFE_DELETE(pMeshIndex);

	CreateBounding();
	return true;
}

	

int CD3DStaticModelResource::GetDummyCount()
{
	return m_DummyCount;
}

char * CD3DStaticModelResource::GetDummyName(int Index)
{
	return m_pDummys[Index].szName;
}

CD3DMatrix * CD3DStaticModelResource::GetDummyMatrix(int Index)
{
	return &(m_pDummys[Index].TransformMatrix);
}






//bool CD3DStaticModelResource::ToUSOFile(CUSOFile * pUSOFile,UINT Param)
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
//	Data.ObjectHead.Size=sizeof(STORAGE_STRUCT)+m_DummyCount*sizeof(STATIC_MODEL_DUMMY);
//
//	Data.BoundingBox=m_BoundingBox;
//	Data.BoundingSphere=m_BoundingSphere;
//	Data.SubMeshCount=(int)m_SubMeshList.GetCount();
//	Data.DummyCount=m_DummyCount;
//
//	pFile->Write(&Data,sizeof(STORAGE_STRUCT));
//
//	if(m_DummyCount)
//		pFile->Write(m_pDummys,m_DummyCount*sizeof(STATIC_MODEL_DUMMY));
//
//	for(size_t i=0;i<m_SubMeshList.GetCount();i++)
//	{
//		if(!m_SubMeshList[i]->ToUSOFile(pUSOFile,(UINT)(m_pManager->GetDevice())))
//			return false;		
//	}
//
//	
//
//	return true;
//}


//bool CD3DStaticModelResource::FromUSOFile(CUSOFile * pUSOFile,UINT Param)
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
//	
//	m_DummyCount=pData->DummyCount;
//
//	
//	BYTE * pCurData=pBuff+sizeof(STORAGE_STRUCT);
//
//	if(m_DummyCount)
//	{
//		m_pDummys=new STATIC_MODEL_DUMMY[m_DummyCount];
//		memcpy(m_pDummys,pCurData,m_DummyCount*sizeof(STATIC_MODEL_DUMMY));
//	}
//
//	for(int i=0;i<pData->SubMeshCount;i++)
//	{
//		CD3DSubMesh * pD3DSubMesh=new CD3DSubMesh;
//		m_SubMeshList.Add(pD3DSubMesh);
//		if(!pD3DSubMesh->FromUSOFile(pUSOFile,(UINT)(m_pManager->GetDevice())))
//		{
//			delete[] pBuff;
//			return false;
//		}
//
//		
//	}	
//
//	delete[] pBuff;
//	return true;
//
//}


bool CD3DStaticModelResource::ToSmartStruct(CSmartStruct& Packet,CUSOFile * pUSOFile,UINT Param)
{
	if(!CD3DObjectResource::ToSmartStruct(Packet,pUSOFile,Param))
		return false;	

	for(int i=0;i<m_DummyCount;i++)
	{
		UINT BufferSize;
		void * pBuffer=Packet.PrepareMember(BufferSize);
		CSmartStruct SubPacket(pBuffer,BufferSize,true);
		CHECK_SMART_STRUCT_ADD_AND_RETURN(SubPacket.AddMember(SST_DI_NAME,m_pDummys[i].szName));
		CHECK_SMART_STRUCT_ADD_AND_RETURN(SubPacket.AddMember(SST_DI_TRANSFROM_MATRIX,
			(char *)&(m_pDummys[i].TransformMatrix),sizeof(m_pDummys[i].TransformMatrix)));
		if(!Packet.FinishMember(SST_D3DSMR_DUMMY,SubPacket.GetDataLen()))
			return false;
	}
	return true;
}

bool CD3DStaticModelResource::FromSmartStruct(CSmartStruct& Packet,CUSOFile * pUSOFile,UINT Param)
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
		case SST_D3DSMR_DUMMY:
			m_DummyCount++;
			break;
		}
	}
	if(m_DummyCount)
	{
		m_pDummys=new STATIC_MODEL_DUMMY[m_DummyCount];
		Pos=Packet.GetFirstMemberPosition();
		int Index=0;
		while(Pos)
		{
			WORD MemberID;
			CSmartStruct Value=Packet.GetNextMember(Pos,MemberID);
			switch(MemberID)
			{	
			case SST_D3DSMR_DUMMY:
				if(Index<m_DummyCount)
				{
					strncpy_0(m_pDummys[Index].szName,256,Value.GetMember(SST_DI_NAME),256);
					memcpy(&(m_pDummys[Index].TransformMatrix),(LPCTSTR)Value.GetMember(SST_DI_TRANSFROM_MATRIX),
						sizeof(m_pDummys[Index].TransformMatrix));
					Index++;
				}

				break;
			}
		}
	}
	return true;
}

UINT CD3DStaticModelResource::GetSmartStructSize(UINT Param)
{
	UINT Size=CD3DObjectResource::GetSmartStructSize(Param);
	for(int i=0;i<m_DummyCount;i++)
	{
		Size+=SMART_STRUCT_STRING_MEMBER_SIZE(strlen(m_pDummys[i].szName));
		Size+=SMART_STRUCT_STRING_MEMBER_SIZE(sizeof(m_pDummys[i].TransformMatrix));
		Size+=SMART_STRUCT_STRUCT_MEMBER_SIZE(0);
	}
	return Size;
}	

//CNameObject::STORAGE_STRUCT * CD3DStaticModelResource::USOCreateHead(UINT Param)
//{
//	STORAGE_STRUCT * pHead=new STORAGE_STRUCT;
//	ZeroMemory(pHead,sizeof(STORAGE_STRUCT));
//	pHead->Size=sizeof(STORAGE_STRUCT);
//	return pHead;
//}
//
//int CD3DStaticModelResource::USOWriteHead(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,UINT Param)
//{
//	UINT HeadSize=CD3DObjectResource::USOWriteHead(pHead,pUSOFile,Param);
//	if(HeadSize<0)
//		return -1;
//
//	STORAGE_STRUCT * pLocalHead=(STORAGE_STRUCT *)pHead;
//
//	pLocalHead->Size+=m_DummyCount*sizeof(STATIC_MODEL_DUMMY);
//	
//	pLocalHead->DummyCount=m_DummyCount;
//
//	return sizeof(STORAGE_STRUCT);
//}
//
//bool CD3DStaticModelResource::USOWriteData(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,UINT Param)
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
//	if(m_DummyCount)
//	{
//		if(pFile->Write(m_pDummys,m_DummyCount*sizeof(STATIC_MODEL_DUMMY))<m_DummyCount*sizeof(STATIC_MODEL_DUMMY))
//			return false;
//	}
//	return true;
//}
//
//
//int CD3DStaticModelResource::USOReadHead(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,UINT Param)
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
//	m_DummyCount=pLocalHead->DummyCount;
//
//	return sizeof(STORAGE_STRUCT);
//}
//
//int CD3DStaticModelResource::USOReadData(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,BYTE * pData,int DataSize,UINT Param)
//{
//	int ReadSize=CD3DObjectResource::USOReadData(pHead,pUSOFile,pData,DataSize,Param);
//
//	pData+=ReadSize;
//	DataSize-=ReadSize;
//
//	if(m_DummyCount)
//	{
//		if(DataSize<m_DummyCount*(int)sizeof(STATIC_MODEL_DUMMY))
//			return -1;
//		m_pDummys=new STATIC_MODEL_DUMMY[m_DummyCount];
//		memcpy(m_pDummys,pData,m_DummyCount*sizeof(STATIC_MODEL_DUMMY));
//		ReadSize+=m_DummyCount*sizeof(STATIC_MODEL_DUMMY);
//	}
//	return ReadSize;
//}




}