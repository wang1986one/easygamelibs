/****************************************************************************/
/*                                                                          */
/*      文件名:    D3DWOWWMOModelResource.h                                 */
/*      创建日期:  2010年02月09日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once

namespace D3DLib{

class CD3DWOWWMOModelResource :
	public CD3DObjectResource
{
public:
	enum SUB_MESH_PROPERTY:UINT64
	{
		SMP_INSIDE=(1<<4),
		SMP_OUTSIDE=(1<<5),
	};
	struct DOODAD_INFO
	{
		CD3DWOWM2ModelResource *	pDoodadModel;
		CD3DVector3					Translation;
		CD3DQuaternion				Rotation;
		FLOAT						Scaling;
		D3DCOLOR					Color;
		WORD						GroupIndex;
	};

	struct DOODAD_SET_INFO
	{
		CEasyString		Name;
		UINT			StartDoodad;
		UINT			DoodadCount;
	};

	struct PORTAL_INFO
	{
		WORD						GroupIndex;
		WORD						Filler;
		CD3DVector3					Normal;
		CD3DVector3					Center;
		FLOAT						Factor;
		CEasyArray<CD3DVector3>		Vertices;		
	};

	struct GROUP_INFO
	{
		UINT						Index;
		CEasyString					Name;
		UINT						Flags;
		CD3DBoundingBox				BoundingBox;
		CEasyArray<CD3DSubMesh *>	GroupSubMeshList;
		CEasyArray<PORTAL_INFO>		PortalList;
	};
	
protected:
	enum SST_MEMBER_ID
	{
		SST_D3DWWMR_DOODAD_INFO_LIST=SST_D3DOR_MAX,
		SST_D3DWWMR_DOODAD_SET_LIST,
		SST_D3DWWMR_GROUP_LIST,		
		SST_D3DWWMR_MAX=SST_D3DOR_MAX+50,
	};
	enum SST_DOODAD_INFO_LIST
	{
		SST_DIL_DOODAD_INFO=1,
	};
	enum SST_DOODAD_INFO
	{
		SST_DI_MODEL_RESOURCE=1,
		SST_DI_TRANSLATION,
		SST_DI_ROTATION,
		SST_DI_SCALING,
		SST_DI_COLOR,
		SST_DI_GROUP_INDEX,
	};
	enum SST_DOODAD_SET_LIST
	{
		SST_DSL_DOODAD_SET=1,
	};
	enum SST_DOODAD_SET
	{
		SST_DS_NAME=1,
		SST_DS_START_DOODAD,
		SST_DS_DOODAD_COUNT,
	};
	enum SST_GROUP_LIST
	{
		SST_GL_GROUP_INFO=1,
	};
	enum SST_PORTAL_LIST
	{
		SST_PL_GROUP_INDEX=1,
		SST_PL_FILLER,
		SST_PL_NORMAL,
		SST_PL_CENTER,
		SST_PL_FACTOR,
		SST_PL_VERTICES,
	};
	enum SST_GROUP_INFO
	{
		SST_GI_INDEX=1,
		SST_GI_NAME,
		SST_GI_FLAG,
		SST_GI_BOUNDING_BOX,
		SST_GI_SUB_MESH,
		SST_GI_PORTAL_INFO,
	};

	struct MODEL_VERTEXT
	{
		CD3DVector3			Pos;
		CD3DVector3			Normal;		
		D3DCOLOR			Diffuse;
		CD3DVector2			TextureCoord;	
	};

	struct SUBMESH_INFO
	{
		UINT	StartIndex;
		UINT	IndexCount;
		UINT	MaterialIndex;
	};


	CEasyArray<DOODAD_INFO>			m_DoodadInfos;
	CEasyArray<DOODAD_SET_INFO>		m_DoodadSets;
	CEasyArray<GROUP_INFO>			m_Groups;

	DECLARE_CLASS_INFO(CD3DWOWWMOModelResource)
public:
	CD3DWOWWMOModelResource(void);
	CD3DWOWWMOModelResource(CD3DObjectResourceManager* pManager);
	~CD3DWOWWMOModelResource(void);

	virtual void Destory();

	virtual bool Reset();
	virtual bool Restore();

	bool LoadFromFile(LPCTSTR ModelFileName);

	UINT GetDoodadSetCount();
	DOODAD_SET_INFO * GetDoodadSetInfo(UINT Index);

	UINT GetDoodadCount();
	DOODAD_INFO * GetDoodadInfo(UINT Index);

	UINT GetGroupCount();
	GROUP_INFO * GetGroupInfo(UINT Index);



public:	

	virtual void PickResource(CNameObjectSet * pObjectSet,UINT Param=0);
	virtual bool ToSmartStruct(CSmartStruct& Packet,CUSOFile * pUSOFile,UINT Param=0);
	virtual bool FromSmartStruct(CSmartStruct& Packet,CUSOFile * pUSOFile,UINT Param=0);
	virtual UINT GetSmartStructSize(UINT Param=0);

protected:
	bool LoadGroup(GROUP_INFO& GroupInfo,LPCTSTR ModelFileName,BLZ_CHUNK_MOMT * pMaterials,UINT MaterialCount,BLZ_CHUNK_MOTX * pTextureNames,BLZ_CHUNK_MOGN * pGroupNames);

	bool LoadDoodads(UINT DoodadCount,UINT DoodadSetCount,BLZ_CHUNK_MODS * pDoodadSets,BLZ_CHUNK_MODN * pDoodadFileNames,BLZ_CHUNK_MODD * pDoodads);
	bool LoadGroups(LPCTSTR ModelFileName,UINT GroupCount,BLZ_CHUNK_MOGI * pGroups,BLZ_CHUNK_MOGN * pGroupNames,BLZ_CHUNK_MOMT * pMaterials,UINT MaterialCount,BLZ_CHUNK_MOTX * pTextureNames);

	WORD RebuildVertexIndex(CEasyArray<WORD>& VertexIndexList,WORD VertexIndex);
	bool MakeMaterial(CD3DSubMeshMaterial& SubMeshMaterial,WMOMaterial& MaterialInfo,BLZ_CHUNK_MOTX * pTextureNames);

	CD3DFX * BuildFX(UINT BlendMode,UINT TextureFlag);

	void MakeSubMesh(WMOTriangleMaterialInfo * pTriangles,UINT TriangleCount,CEasyArray<SUBMESH_INFO>& SubMeshInfo);
	
};

}