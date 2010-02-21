#pragma once

namespace D3DLib{

#pragma comment(linker,"/include:?m_CD3DWOWADTModelClassInfoRegister@CD3DWOWADTModel@D3DLib@@1VCClassInfoRegister@@A")

class CD3DWOWADTModel :
	public CD3DBaseStaticModel
{
protected:
	enum SST_MEMBER_ID
	{
		SST_D3DWAM_MODEL_RESOURCE=SST_D3DO_MAX,				
		SST_D3DWAM_MAX=SST_D3DO_MAX+50,
	};

	CD3DWOWADTModelResource *							m_pModelResource;
	CEasyArray<CD3DWOWM2Model *>						m_M2ObjectList;
	CEasyArray<CD3DWOWWMOModel *>						m_WMOObjectList;

	DECLARE_FILE_CHANNEL_MANAGER
	DECLARE_CLASS_INFO(CD3DWOWADTModel)
public:
	CD3DWOWADTModel(void);
	~CD3DWOWADTModel(void);

	virtual void Destory();

	virtual bool Reset();
	virtual bool Restore();

	bool LoadFromFile(LPCTSTR ModelFileName);
public:
	virtual void PickResource(CNameObjectSet * pObjectSet,UINT Param=0);
	virtual bool ToSmartStruct(CSmartStruct& Packet,CUSOFile * pUSOFile,UINT Param=0);
	virtual bool FromSmartStruct(CSmartStruct& Packet,CUSOFile * pUSOFile,UINT Param=0);
	virtual UINT GetSmartStructSize(UINT Param=0);
public:
	virtual void PrepareRender(CD3DDevice * pDevice,CD3DSubMesh * pSubMesh,CD3DSubMeshMaterial * pMaterial,CD3DLight ** pLight,CD3DCamera * pCamera);
	virtual int GetSubMeshCount();
	virtual CD3DSubMesh * GetSubMesh(int index);
	virtual CD3DSubMeshMaterial * GetSubMeshMaterial(int index);

	virtual CD3DBoundingBox * GetBoundingBox();
	virtual CD3DBoundingSphere * GetBoundingSphere();

protected:
	bool LoadObjects();
};

}