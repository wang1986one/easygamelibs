#pragma once


namespace D3DLib{

class CD3DDevice;
class CD3DTexture;
class CD3DFX;
class CD3DRender;
class CD3DMatrix;
class CD3DLight;
class CD3DCamera;

enum D3DOBJ_BOUNDING_FRAME_OPT
{
	DBFO_HIDE,
	DBFO_SHOW,
	DBFO_RELEASE,
};



class CD3DObject :
	public CTreeObject
{	
protected:
	//struct STORAGE_STRUCT:public CNameObject::STORAGE_STRUCT
	//{
	//	CD3DMatrix		LocalMatrix;
	//	bool			IsVisible;		
	//	int				ChildCount;
	//};

	enum SST_MEMBER_ID
	{
		SST_D3DO_LOCAL_MATIRX=SST_TO_MAX,
		SST_D3DO_IS_VISIBLE,		
		SST_D3DO_MAX=SST_TO_MAX+50,
	};

	CD3DDevice *				m_pD3DDevice;
	CD3DRender *				m_pRender;		
	int							m_Layer;
	bool						m_IsVisible;
	
	CD3DMatrix					m_LocalMatrix;
	CD3DMatrix					m_WorldMatrix;	
	
	bool						m_IsCulled;	

	DECLARE_CLASS_INFO(CD3DObject)

public:	

	static D3DMATERIAL9		SELECTED_SUBMESH_MATERIAL;


	CD3DObject();
	virtual ~CD3DObject(void);	

	virtual void Destory();
	
	virtual bool Reset();
	virtual bool Restore();

	virtual void SetDevice(CD3DDevice * pDevice);
	CD3DDevice * GetDevice();

	virtual void SetRender(CD3DRender * pRender);
	CD3DRender * GetRender();

	virtual bool CanRender();


	virtual void SetParent(CTreeObject* pParent);
	CD3DObject* GetParent();
	virtual CD3DObject * GetChildByIndex(int Index);
	virtual CD3DObject * GetChild(LPCTSTR ObjName,bool NoCase=false);
	virtual CD3DObject * GetChild(UINT ObjID);
	virtual CD3DObject * GetChildByStorageID(UINT StorageID);
	virtual CD3DObject * GetChildRecursive(LPCTSTR ObjName,bool NoCase=false);
	virtual CD3DObject * GetChildRecursive(UINT ObjID);
	virtual CD3DObject * GetChildByStorageIDRecursive(UINT StorageID);
	
	

	CD3DMatrix& GetLocalMatrix();
	void SetLocalMatrix(CD3DMatrix& Mat);
	CD3DMatrix& GetWorldMatrix();
	CD3DMatrix GetWorldMatrixDirect();

	void TransForm(const CD3DMatrix& Mat);

	virtual int GetSubMeshCount();
	virtual CD3DSubMesh * GetSubMesh(int index);
	virtual CD3DSubMeshMaterial * GetSubMeshMaterial(int index);

	virtual CD3DBoundingBox * GetBoundingBox();
	virtual CD3DBoundingSphere * GetBoundingSphere();
	//virtual CD3DBoundingBox * GetWholeBoundingBox();
	//virtual CD3DBoundingSphere * GetWholeBoundingSphere();

	virtual CD3DLight * GetLight(int Index);
	virtual CD3DLight ** GetLights();

	virtual void PrepareRender(CD3DDevice * pDevice,CD3DSubMesh * pSubMesh,CD3DSubMeshMaterial * pMaterial,CD3DLight ** pLight,CD3DCamera * pCamera);
	

	virtual void Update(FLOAT Time);
	
	inline int GetLayer();
	inline void SetLayer(int Layer);

	virtual void SetVisible(bool IsVisible);
	virtual void SetVisibleRecursive(bool IsVisible);
	virtual bool IsVisible();

	virtual bool RayIntersect(const CD3DVector3& Point,const CD3DVector3& Dir,CD3DVector3& IntersectPoint,FLOAT& Distance,bool TestOnly=true);
	virtual bool GetHeightByXZ(FLOAT x,FLOAT z,FLOAT& y);

	void SetCull(bool IsCulled);
	bool IsCulled();

	virtual void ShowBoundingFrame(int Operator);
	virtual void UpdateBoundingFrame();
	
	virtual bool CloneFrom(CNameObject * pObject,UINT Param=0);
	virtual void PickResource(CNameObjectSet * pObjectSet,UINT Param=0);

	virtual bool ToSmartStruct(CSmartStruct& Packet,CUSOFile * pUSOFile,UINT Param=0);
	virtual bool FromSmartStruct(CSmartStruct& Packet,CUSOFile * pUSOFile,UINT Param=0);
	virtual UINT GetSmartStructSize(UINT Param=0);

protected:
	

	//virtual CNameObject::STORAGE_STRUCT * USOCreateHead(UINT Param=0);
	//virtual int USOWriteHead(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,UINT Param=0);	
	//virtual bool USOWriteChild(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,UINT Param=0);	

	//virtual int USOReadHead(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,UINT Param=0);	
	//virtual bool USOReadChild(CNameObject::STORAGE_STRUCT * pHead,CUSOFile * pUSOFile,UINT Param=0);
	
	
	
};

inline void CD3DObject::SetDevice(CD3DDevice * pDevice)
{
	m_pD3DDevice=pDevice;
}

inline CD3DDevice * CD3DObject::GetDevice()
{
	return m_pD3DDevice;
}

inline CD3DRender * CD3DObject::GetRender()
{
	return m_pRender;
}

inline bool CD3DObject::CanRender()
{
	return true;
}

inline CD3DObject* CD3DObject::GetParent()
{
	return (CD3DObject*)CTreeObject::GetParent();
}	
inline  CD3DObject * CD3DObject::GetChildByIndex(int Index)
{
	return (CD3DObject*)CTreeObject::GetChildByIndex(Index);
}
inline  CD3DObject * CD3DObject::GetChild(LPCTSTR ObjName,bool NoCase)
{
	return (CD3DObject*)CTreeObject::GetChild(ObjName,NoCase);
}
inline  CD3DObject * CD3DObject::GetChild(UINT ObjID)
{
	return (CD3DObject*)CTreeObject::GetChild(ObjID);
}
inline CD3DObject * CD3DObject::GetChildByStorageID(UINT StorageID)
{
	return (CD3DObject*)CTreeObject::GetChildByStorageID(StorageID);
}
inline  CD3DObject * CD3DObject::GetChildRecursive(LPCTSTR ObjName,bool NoCase)
{
	return (CD3DObject*)CTreeObject::GetChildRecursive(ObjName,NoCase);
}
inline  CD3DObject * CD3DObject::GetChildRecursive(UINT ObjID)
{
	return (CD3DObject*)CTreeObject::GetChildRecursive(ObjID);
}
inline CD3DObject * CD3DObject::GetChildByStorageIDRecursive(UINT StorageID)
{
	return (CD3DObject*)CTreeObject::GetChildByStorageIDRecursive(StorageID);
}

inline CD3DMatrix& CD3DObject::GetLocalMatrix()
{	
	return m_LocalMatrix;
}
inline void CD3DObject::SetLocalMatrix(CD3DMatrix& Mat)
{
	m_LocalMatrix=Mat;
}
inline CD3DMatrix& CD3DObject::GetWorldMatrix()
{
	return m_WorldMatrix;
}



inline void CD3DObject::TransForm(const CD3DMatrix& Mat)
{
	m_LocalMatrix=m_LocalMatrix*Mat;
}

inline int CD3DObject::GetLayer()
{
	return m_Layer;
}
inline void CD3DObject::SetLayer(int Layer)
{
	m_Layer=Layer;
}

inline void CD3DObject::SetCull(bool IsCulled)
{
	m_IsCulled=IsCulled;
}

inline bool CD3DObject::IsCulled()
{
	return m_IsCulled;
}



}