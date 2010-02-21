#pragma once

namespace D3DLib{

#pragma comment(linker,"/include:?m_CD3DWOWWMOModelClassInfoRegister@CD3DWOWWMOModel@D3DLib@@1VCClassInfoRegister@@A")

class CD3DWOWWMOModel :
	public CD3DBaseStaticModel
{
protected:
	enum SST_MEMBER_ID
	{
		SST_D3DWWM_MODEL_RESOURCE=SST_D3DO_MAX,		
		SST_D3DWWM_CUR_DOODAD_SET,
		SST_D3DWWM_MAX=SST_D3DO_MAX+50,
	};


	CD3DWOWWMOModelResource *							m_pModelResource;
	CEasyArray<CD3DWOWM2Model *>						m_DoodadList;
	int													m_CurDoodadSet;

	DECLARE_FILE_CHANNEL_MANAGER
	DECLARE_CLASS_INFO(CD3DWOWWMOModel)
public:
	CD3DWOWWMOModel(void);
	~CD3DWOWWMOModel(void);

	virtual void Destory();

	virtual bool Reset();
	virtual bool Restore();

	bool LoadFromFile(LPCTSTR ModelFileName);
	bool LoadFromResource(CD3DWOWWMOModelResource * pModelResource);

	UINT GetDoodadSetCount();
	bool ShowDoodads(UINT SetIndex);
	void ClearDoodads();
	int GetCurDoodadSet();

public:
	virtual void PickResource(CNameObjectSet * pObjectSet,UINT Param=0);
	virtual bool ToSmartStruct(CSmartStruct& Packet,CUSOFile * pUSOFile,UINT Param=0);
	virtual bool FromSmartStruct(CSmartStruct& Packet,CUSOFile * pUSOFile,UINT Param=0);
	virtual UINT GetSmartStructSize(UINT Param=0);

	virtual void PrepareRender(CD3DDevice * pDevice,CD3DSubMesh * pSubMesh,CD3DSubMeshMaterial * pMaterial,CD3DLight ** pLight,CD3DCamera * pCamera);
	virtual int GetSubMeshCount();
	virtual CD3DSubMesh * GetSubMesh(int index);
	virtual CD3DSubMeshMaterial * GetSubMeshMaterial(int index);

	virtual CD3DBoundingBox * GetBoundingBox();
	virtual CD3DBoundingSphere * GetBoundingSphere();

protected:
	
};

inline int CD3DWOWWMOModel::GetCurDoodadSet()
{
	return m_CurDoodadSet;
}

}