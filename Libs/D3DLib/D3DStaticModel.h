#pragma once

namespace D3DLib{

class CD3DBoundingFrame;

#pragma comment(linker,"/include:?m_CD3DStaticModelClassInfoRegister@CD3DStaticModel@D3DLib@@1VCClassInfoRegister@@A")

class CD3DStaticModel :
	public CD3DBaseStaticModel
{
public:
	enum PROPERTY_VALUES:UINT64
	{
		PV_IS_GROUND=1,
	};
protected:
	//struct STORAGE_STRUCT:public CD3DObject::STORAGE_STRUCT
	//{		
	//	int				ModelResourceID;
	//};
	enum SST_MEMBER_ID
	{
		SST_D3DSM_MODEL_RESOURCE=SST_D3DO_MAX,
		SST_D3DSM_PROPERTY,
		SST_D3DSM_MAX=SST_D3DO_MAX+50,
	};

	CD3DStaticModelResource*	m_pResource;
	//CD3DBoundingBox				m_WholeBoundingBox;
	//CD3DBoundingSphere			m_WholeBoundingSphere;
	CD3DBoundingFrame *			m_pBoundingFrame;
	UINT64						m_Property;

	DECLARE_CLASS_INFO(CD3DStaticModel)
	
public:
	CD3DStaticModel(void);
	virtual ~CD3DStaticModel(void);

	virtual void Destory();

	virtual bool Reset();
	virtual bool Restore();

	bool LoadFromXFile(LPCTSTR FileName);
	bool LoadFromMDL(LPCTSTR FileName);
	bool LoadFromSMDL(LPCTSTR FileName);
	
	//void SetFx(CD3DFX * pFX);
public:
	//virtual void PrepareRender(CD3DDevice * pDevice,CD3DSubMesh * pSubMesh,CD3DSubMeshMaterial * pMaterial,CD3DLight ** pLight,CD3DCamera * pCamera);
	virtual int GetSubMeshCount();
	virtual CD3DSubMesh * GetSubMesh(int index);

	virtual CD3DBoundingBox * GetBoundingBox();
	virtual CD3DBoundingSphere * GetBoundingSphere();
	//virtual CD3DBoundingBox * GetWholeBoundingBox();
	//virtual CD3DBoundingSphere * GetWholeBoundingSphere();

	virtual bool RayIntersect(const CD3DVector3& Point,const CD3DVector3& Dir,CD3DVector3& IntersectPoint,FLOAT& Distance,bool TestOnly=true);
	virtual bool GetHeightByXZ(FLOAT x,FLOAT z,FLOAT& y);

	virtual void Update(FLOAT Time);

	virtual void ShowBoundingFrame(int Operator);

	CD3DStaticModelResource * GetStaticModelResource();

	void SetProperty(UINT64 Property);
	UINT64 GetProperty();

	virtual void PickResource(CNameObjectSet * pObjectSet,UINT Param=0);
	virtual bool CloneFrom(CNameObject * pObject,UINT Param=0);

	virtual bool ToSmartStruct(CSmartStruct& Packet,CUSOFile * pUSOFile,UINT Param=0);
	virtual bool FromSmartStruct(CSmartStruct& Packet,CUSOFile * pUSOFile,UINT Param=0);
	virtual UINT GetSmartStructSize(UINT Param=0);
protected:
	void CreateDummy();

	//virtual CNameObject::STORAGE_STRUCT * USOCreateHead(UINT Param=0);
	//virtual int USOWriteHead(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,UINT Param=0);	

	//virtual int USOReadHead(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,UINT Param=0);
	
};

inline CD3DStaticModelResource * CD3DStaticModel::GetStaticModelResource()
{
	return m_pResource;
}

inline void CD3DStaticModel::SetProperty(UINT64 Property)
{
	m_Property=Property;
}
inline UINT64 CD3DStaticModel::GetProperty()
{
	return m_Property;
}

}