/****************************************************************************/
/*                                                                          */
/*      文件名:    D3DSubMesh.h                                             */
/*      创建日期:  2009年09月25日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once
namespace D3DLib{

	
class CD3DDevice;
	
class CD3DSubMesh:public CNameObject
{
	DECLARE_CLASS_INFO(CD3DSubMesh)
public:
	struct VERTEX_FORMAT
	{
		DWORD			FVF;
		WORD			VertexSize;
		WORD			IndexSize;
		LPDIRECT3DVERTEXDECLARATION9	pVertexDeclaration;
	};	
	enum SUB_MESH_PROPERTYS
	{
		SMF_IS_VISIBLE=1,
		SMF_IS_TRANSPARENT=(1<<1),		
	};
	enum SUB_MESH_FLAGS
	{		
		SMF_IS_CULLED=1,
		SMF_IS_SELECTED=(1<<1),
	};
	enum BUFFER_USED
	{
		BUFFER_USE_DX,
		BUFFER_USE_CUSTOM,
		BUFFER_USE_BACKUP,
	};
	//struct STORAGE_VERTEX_FORMAT
	//{
	//	DWORD			FVF;
	//	WORD			VertexSize;
	//	WORD			IndexSize;
	//};
	//struct STORAGE_STRUCT:public CNameObject::STORAGE_STRUCT
	//{		
	//	STORAGE_VERTEX_FORMAT	VertexFormat;
	//	int						PrimitiveType;
	//	int						PrimitiveCount;
	//	int						VertexCount;	
	//	int						IndexCount;			
	//	CD3DBoundingBox			BoundingBox;
	//	CD3DBoundingSphere		BoundingSphere;
	//	UINT64					Property;		
	//};
	enum SST_MEMBER_ID
	{
		SST_D3DSM_FVF=SST_NO_MAX,
		SST_D3DSM_VERTEX_DECLARATION,
		SST_D3DSM_VERTEX_SIZE,
		SST_D3DSM_INDEX_SIZE,
		SST_D3DSM_PRIMITIVE_TYPE,
		SST_D3DSM_PRIMITIVE_COUNT,
		SST_D3DSM_VERTEX_COUNT,	
		SST_D3DSM_INDEX_COUNT,	
		SST_D3DSM_BOUNDING_BOX,
		SST_D3DSM_BOUNDING_SPHERE,
		SST_D3DSM_PROPERTY,
		SST_D3DSM_VERTEX,
		SST_D3DSM_INDEX,
		SST_D3DSM_MATERIAL,
		SST_D3DSM_MAX=SST_NO_MAX+50,
	};
	
protected:
	VERTEX_FORMAT			m_VertexFormat;
	UINT					m_PrimitiveType;
	UINT					m_PrimitiveCount;
	BYTE*					m_pVertexBuffer;	
	LPDIRECT3DVERTEXBUFFER9 m_pDXVertexBuffer;
	BYTE*					m_pBackupVertexBuffer;
	bool					m_IsVertexsSelfDelete;
	bool					m_IsDXVertexBufferSelfRelease;
	UINT					m_VertexCount;
	UINT					m_StartVertex;
	BYTE*					m_pIndexBuffer;
	LPDIRECT3DINDEXBUFFER9	m_pDXIndexBuffer;
	BYTE*					m_pBackupIndexBuffer;
	bool					m_IsIndexsSelfDelete;
	bool					m_IsDXIndexBufferSelfRelease;
	UINT					m_IndexCount;	
	UINT					m_StartIndex;
	CD3DSubMeshMaterial		m_Material;
	CD3DBoundingBox			m_BoundingBox;
	CD3DBoundingSphere		m_BoundingSphere;
	UINT64					m_Property;
	UINT64					m_Flag;
	UINT					m_RenderBufferUsed;
	UINT					m_OrginDataBufferUsed;

	////以下给渲染器使用
	//bool					m_IsTransparent;
	//bool					m_IsCulled;
	//bool					m_IsSelected;
public:

	CD3DSubMesh(void);
	virtual ~CD3DSubMesh(void);

	virtual void Destory();
	

	bool Reset();
	bool Restore();

	void CreateBounding();

	bool RayIntersect(const CD3DMatrix& WorldMatrix,const CD3DVector3& Point,const CD3DVector3& Dir,CD3DVector3& IntersectPoint,FLOAT& Distance,bool TestOnly=true);

	VERTEX_FORMAT& GetVertexFormat();
	bool DeclareVertexFormat(CD3DDevice * pDevice,D3DVERTEXELEMENT9* pVertexElements,WORD VertexSize,WORD IndexSize);

	UINT GetPrimitiveType();
	void SetPrimitiveType(UINT Type);
	UINT GetPrimitiveCount();
	void SetPrimitiveCount(UINT Count);

	BYTE * GetVertexBuffer();
	void SetVertices(BYTE * pVertexBuffer,bool IsSelfDelete=false);
	void AllocVertexBuffer();
	LPDIRECT3DVERTEXBUFFER9 GetDXVertexBuffer();
	void SetDXVertexBuffer(LPDIRECT3DVERTEXBUFFER9 pDXVertexBuffer,bool IsSelfRelease=false);
	bool AllocDXVertexBuffer(CD3DDevice * pDevice,DWORD Usage=0,D3DPOOL Pool=D3DPOOL_MANAGED);	
	UINT GetVertexCount();
	void SetVertexCount(UINT Count);
	UINT GetVertexStart();
	void SetVertexStart(UINT Start);
		void AllocBackupVertexBuffer();
	BYTE * GetBackupVertexBuffer();

	BYTE * GetIndexBuffer();
	void SetIndices(BYTE * pIndexBuffer,bool IsSelfDelete=false);
	void AllocIndexBuffer();
	LPDIRECT3DINDEXBUFFER9 GetDXIndexBuffer();
	void SetDXIndexBuffer(LPDIRECT3DINDEXBUFFER9 pDXIndexBuffer,bool IsSelfRelease=false);
	bool AllocDXIndexBuffer(CD3DDevice * pDevice,DWORD Usage=0,D3DPOOL Pool=D3DPOOL_MANAGED);
	UINT GetIndexCount();
	void SetIndexCount(UINT Count);
	UINT GetIndexStart();
	void SetIndexStart(UINT Start);
	void AllocBackupIndexBuffer();
	BYTE * GetBackupIndexBuffer();

	CD3DSubMeshMaterial& GetMaterial();
	CD3DBoundingBox& GetBoundingBox();
	CD3DBoundingSphere& GetBoundingSphere();

	UINT64 GetProperty();
	void SetProperty(UINT64 Property);
	void AddProperty(UINT64 Property);
	void RemoveProperty(UINT64 Property);
	bool CheckProperty(UINT64 Property);
	UINT64 GetFlag();
	void SetFlag(UINT64 Flag);
	bool IsTransparent();
	void SetTransparent(bool Enable);
	bool IsCulled();
	void SetCulled(bool Enable);
	bool IsSelected();
	void SetSelected(bool Enable);
	bool IsVisible();
	void SetVisible(bool Enable);

	UINT GetRenderBufferUsed();
	void SetRenderBufferUsed(UINT Which);
	UINT GetOrginDataBufferUsed();
	void SetOrginDataBufferUsed(UINT Which);


	static bool SortByName(CD3DSubMesh * pSubMesh1,CD3DSubMesh * pSubMesh2);
	static bool SortByRender(CD3DSubMesh * pSubMesh1,CD3DSubMesh * pSubMesh2);

	virtual void PickResource(CNameObjectSet * pObjectSet,UINT Param=0);

	virtual bool ToSmartStruct(CSmartStruct& Packet,CUSOFile * pUSOFile,UINT Param=0);
	virtual bool FromSmartStruct(CSmartStruct& Packet,CUSOFile * pUSOFile,UINT Param=0);
	virtual UINT GetSmartStructSize(UINT Param=0);
protected:
	//virtual CNameObject::STORAGE_STRUCT * USOCreateHead(UINT Param=0);
	//virtual int USOWriteHead(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,UINT Param=0);
	//virtual bool USOWriteData(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,UINT Param=0);
	//bool USOWriteChild(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,UINT Param=0);

	//virtual int USOReadHead(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,UINT Param=0);
	//virtual int USOReadData(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,BYTE * pData,int DataSize,UINT Param=0);
	//virtual bool USOReadChild(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,UINT Param=0);
	//virtual bool USOReadFinish(CNameObject::STORAGE_STRUCT * pHead,UINT Param=0);
};


inline CD3DSubMesh::VERTEX_FORMAT& CD3DSubMesh::GetVertexFormat()
{
	return m_VertexFormat;
}

inline UINT CD3DSubMesh::GetPrimitiveType()
{
	return m_PrimitiveType;
}
inline void CD3DSubMesh::SetPrimitiveType(UINT Type)
{
	m_PrimitiveType=Type;
}
inline UINT CD3DSubMesh::GetPrimitiveCount()
{
	return m_PrimitiveCount;
}
inline void CD3DSubMesh::SetPrimitiveCount(UINT Count)
{
	m_PrimitiveCount=Count;
}

inline BYTE * CD3DSubMesh::GetVertexBuffer()
{
	return m_pVertexBuffer;
}
inline void CD3DSubMesh::SetVertices(BYTE * pVertexBuffer,bool IsSelfDelete)
{
	m_pVertexBuffer=pVertexBuffer;
	m_IsVertexsSelfDelete=IsSelfDelete;
}
inline LPDIRECT3DVERTEXBUFFER9 CD3DSubMesh::GetDXVertexBuffer()
{
	return m_pDXVertexBuffer;
}
inline void CD3DSubMesh::SetDXVertexBuffer(LPDIRECT3DVERTEXBUFFER9 pDXVertexBuffer,bool IsSelfRelease)
{
	m_pDXVertexBuffer=pDXVertexBuffer;
	m_IsDXVertexBufferSelfRelease=IsSelfRelease;
}
inline UINT CD3DSubMesh::GetVertexCount()
{
	return m_VertexCount;
}
inline void CD3DSubMesh::SetVertexCount(UINT Count)
{
	m_VertexCount=Count;
}
inline UINT CD3DSubMesh::GetVertexStart()
{
	return m_StartVertex;
}
inline void CD3DSubMesh::SetVertexStart(UINT Start)
{
	m_StartVertex=Start;
}

inline BYTE * CD3DSubMesh::GetBackupVertexBuffer()
{
	return m_pBackupVertexBuffer;
}

inline BYTE * CD3DSubMesh::GetIndexBuffer()
{
	return m_pIndexBuffer;
}
inline void CD3DSubMesh::SetIndices(BYTE * pIndexBuffer,bool IsSelfDelete)
{
	m_pIndexBuffer=pIndexBuffer;
	m_IsIndexsSelfDelete=IsSelfDelete;
}
inline LPDIRECT3DINDEXBUFFER9 CD3DSubMesh::GetDXIndexBuffer()
{
	return m_pDXIndexBuffer;
}
inline void CD3DSubMesh::SetDXIndexBuffer(LPDIRECT3DINDEXBUFFER9 pDXIndexBuffer,bool IsSelfRelease)
{
	m_pDXIndexBuffer=pDXIndexBuffer;
	m_IsDXIndexBufferSelfRelease=IsSelfRelease;
}
inline UINT CD3DSubMesh::GetIndexCount()
{
	return m_IndexCount;
}
inline void CD3DSubMesh::SetIndexCount(UINT Count)
{
	m_IndexCount=Count;
}
inline UINT CD3DSubMesh::GetIndexStart()
{
	return m_StartIndex;
}
inline void CD3DSubMesh::SetIndexStart(UINT Start)
{
	m_StartIndex=Start;
}

inline BYTE * CD3DSubMesh::GetBackupIndexBuffer()
{
	return m_pBackupIndexBuffer;
}

inline CD3DSubMeshMaterial& CD3DSubMesh::GetMaterial()
{
	return m_Material;
}
inline CD3DBoundingBox& CD3DSubMesh::GetBoundingBox()
{
	return m_BoundingBox;
}
inline CD3DBoundingSphere& CD3DSubMesh::GetBoundingSphere()
{
	return m_BoundingSphere;
}

inline UINT64 CD3DSubMesh::GetProperty()
{
	return m_Property;
}
inline void CD3DSubMesh::SetProperty(UINT64 Property)
{
	m_Property=Property;
}
inline void CD3DSubMesh::AddProperty(UINT64 Property)
{
	m_Property|=Property;
}
inline void CD3DSubMesh::RemoveProperty(UINT64 Property)
{
	m_Property&=~Property;
}
inline bool CD3DSubMesh::CheckProperty(UINT64 Property)
{
	return (m_Property&Property)!=0;
}
inline UINT64 CD3DSubMesh::GetFlag()
{
	return m_Flag;
}
inline void CD3DSubMesh::SetFlag(UINT64 Flag)
{
	m_Flag=Flag;
}
inline bool CD3DSubMesh::IsTransparent()
{
	return (m_Property&SMF_IS_TRANSPARENT)!=0;
}
inline void CD3DSubMesh::SetTransparent(bool Enable)
{
	if(Enable)
		m_Property=m_Property|SMF_IS_TRANSPARENT;
	else
		m_Property=m_Property&(~SMF_IS_TRANSPARENT);
}
inline bool CD3DSubMesh::IsCulled()
{
	return (m_Flag&SMF_IS_CULLED)!=0;
}
inline void CD3DSubMesh::SetCulled(bool Enable)
{
	if(Enable)
		m_Flag=m_Flag|SMF_IS_CULLED;
	else
		m_Flag=m_Flag&(~SMF_IS_CULLED);
}

inline bool CD3DSubMesh::IsSelected()
{
	return (m_Flag&SMF_IS_SELECTED)!=0;
}

inline void CD3DSubMesh::SetSelected(bool Enable)
{
	if(Enable)
		m_Flag=m_Flag|SMF_IS_SELECTED;
	else
		m_Flag=m_Flag&(~SMF_IS_SELECTED);
}
inline bool CD3DSubMesh::IsVisible()
{
	return (m_Property&SMF_IS_VISIBLE)!=0;
}
inline void CD3DSubMesh::SetVisible(bool Enable)
{
	if(Enable)
		m_Property=m_Property|SMF_IS_VISIBLE;
	else
		m_Property=m_Property&(~SMF_IS_VISIBLE);
}

inline UINT CD3DSubMesh::GetRenderBufferUsed()
{
	return m_RenderBufferUsed;
}
inline void CD3DSubMesh::SetRenderBufferUsed(UINT Which)
{
	m_RenderBufferUsed=Which;
}
inline UINT CD3DSubMesh::GetOrginDataBufferUsed()
{
	return m_OrginDataBufferUsed;
}
inline void CD3DSubMesh::SetOrginDataBufferUsed(UINT Which)
{
	m_OrginDataBufferUsed=Which;
}
}