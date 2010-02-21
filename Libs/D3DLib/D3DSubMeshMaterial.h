#pragma once
namespace D3DLib{

class CD3DDevice;
class CD3DTexture;
class CD3DFX;

class CD3DSubMeshMaterial :
	public CNameObject
{
protected:
	//struct STORAGE_STRUCT:public CNameObject::STORAGE_STRUCT
	//{		
	//	D3DMATERIAL9		Material;
	//	int					TextureCount;
	//	int					TextureIndex[MAX_TEXTURE_LAYER];
	//	UINT64				TextureProperty[MAX_TEXTURE_LAYER];
	//	int					FXIndex;
	//};
	enum SST_MEMBER_ID
	{
		SST_D3DSMM_MATERIAL=SST_NO_MAX,
		SST_D3DSMM_TEXTURE,
		SST_D3DSMM_FX,
		SST_D3DSMM_MAX=SST_NO_MAX+50,
	};
	enum SST_TEXTURE
	{
		SST_TEX_TEXTURE=1,
		SST_TEX_PROPERTY,
	};
	struct MATERIAL_TEXTURE
	{
		CD3DTexture*	pTexture;
		UINT64			Property;
		CD3DMatrix		UVTransform;
	};
	D3DMATERIAL9				m_Material;
	D3DCOLORVALUE				m_GlobalColor;
	vector<MATERIAL_TEXTURE>	m_TextureList;	
	CD3DFX*						m_pFX;


	DECLARE_CLASS_INFO(CD3DSubMeshMaterial)
public:
	CD3DSubMeshMaterial(void);
	~CD3DSubMeshMaterial(void);

	virtual void Destory();

	bool Reset();
	bool Restore();

	void SetMaterial(D3DMATERIAL9& Material);
	D3DMATERIAL9& GetMaterial();	

	void SetGlobalColor(D3DCOLORVALUE& Color);
	D3DCOLORVALUE& GetGlobalColor();

	void SetFX(CD3DFX * pFX);
	CD3DFX * GetFX();

	void AddTexture(CD3DTexture * pTexture,UINT64 Property);
	bool SetTexture(UINT Layer,CD3DTexture * pTexture);
	bool SetTextureProperty(UINT Layer,UINT64 Property);
	bool SetTextureUVTransform(UINT Layer,CD3DMatrix& TransformMat);
	CD3DTexture * GetTexture(UINT Layer);
	UINT64 GetTextureProperty(UINT Layer);
	CD3DMatrix& GetTextureUVTransform(UINT Layer);
	UINT GetTextureLayerCount();
	void ClearAllTexture();


	virtual bool ToSmartStruct(CSmartStruct& Packet,CUSOFile * pUSOFile,UINT Param=0);
	virtual bool FromSmartStruct(CSmartStruct& Packet,CUSOFile * pUSOFile,UINT Param=0);
	virtual UINT GetSmartStructSize(UINT Param=0);
protected:
	

	//virtual CNameObject::STORAGE_STRUCT * USOCreateHead(UINT Param=0);
	//virtual int USOWriteHead(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,UINT Param=0);	

	//virtual int USOReadHead(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,UINT Param=0);
	
};


inline void CD3DSubMeshMaterial::SetMaterial(D3DMATERIAL9& Material)
{
	m_Material=Material;
}

inline D3DMATERIAL9& CD3DSubMeshMaterial::GetMaterial()
{
	return m_Material;
}

inline void CD3DSubMeshMaterial::SetGlobalColor(D3DCOLORVALUE& Color)
{
	m_GlobalColor=Color;
}
inline D3DCOLORVALUE& CD3DSubMeshMaterial::GetGlobalColor()
{
	return m_GlobalColor;
}


inline CD3DFX * CD3DSubMeshMaterial::GetFX()
{
	return m_pFX;
}

inline void CD3DSubMeshMaterial::AddTexture(CD3DTexture * pTexture,UINT64 Property)
{
	MATERIAL_TEXTURE Tex;
	Tex.pTexture=pTexture;
	Tex.Property=Property;
	Tex.UVTransform.SetIdentity();
	m_TextureList.push_back(Tex);
}

inline bool CD3DSubMeshMaterial::SetTextureProperty(UINT Layer,UINT64 Property)
{
	if(Layer<m_TextureList.size())
	{
		m_TextureList[Layer].Property=Property;
		return true;
	}
	return false;
}

inline bool CD3DSubMeshMaterial::SetTextureUVTransform(UINT Layer,CD3DMatrix& TransformMat)
{
	if(Layer<m_TextureList.size())
	{
		m_TextureList[Layer].UVTransform=TransformMat;
		return true;
	}
	return false;
}

inline CD3DTexture * CD3DSubMeshMaterial::GetTexture(UINT Layer)
{
	if(Layer<m_TextureList.size())
	{
		return m_TextureList[Layer].pTexture;
	}
	return NULL;
}

inline UINT64 CD3DSubMeshMaterial::GetTextureProperty(UINT Layer)
{
	if(Layer<m_TextureList.size())
	{
		return m_TextureList[Layer].Property;
	}
	return 0;
}

inline CD3DMatrix& CD3DSubMeshMaterial::GetTextureUVTransform(UINT Layer)
{
	return m_TextureList[Layer].UVTransform;
}

inline UINT CD3DSubMeshMaterial::GetTextureLayerCount()
{
	return (UINT)m_TextureList.size();
}

}