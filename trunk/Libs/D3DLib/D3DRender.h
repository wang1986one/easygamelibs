#pragma once

namespace D3DLib{

class CD3DCamera;
class CD3DLight;
class CD3DObject;


class CD3DRender :
	public CNameObject
{
protected:
	CD3DDevice *			m_pDevice;
	LPDIRECT3DVERTEXBUFFER9 m_pVertexBuffer;
	UINT					m_VertexBuffSize;
	LPDIRECT3DINDEXBUFFER9	m_pIndexBuffer;
	UINT					m_IndexBuffSize;
	int						m_MaxLayer;
	bool					m_Is16BitIndex;
	CD3DCamera *			m_pCamera;
	CD3DLight *				m_Lights[MAX_LIGHT];
	

	int						m_ObjectCount;
	int						m_SubMeshCount;
	int						m_FaceCount;
	int						m_VertexCount;

	CEasyArray<CD3DObject *>	m_ObjectList;
	CEasyArray<CD3DObject *>	m_RootObjectList;

	

	DECLARE_CLASS_INFO(CD3DRender)
public:
	CD3DRender();
	virtual ~CD3DRender(void);

	bool Create(CD3DDevice * pDevice,UINT VertexBuffSize,UINT IndexBuffSize,int MaxLayer,bool Is16BitIndex=true);
	virtual void Destory();
	virtual bool Reset();
	virtual bool Restore();	
	virtual void Release();	
	
	virtual void Render();
	virtual void RenderDirectly(CD3DObject * pObject);
	virtual void Update(FLOAT Time);

	CD3DDevice * GetDevice();

	virtual bool AddObject(CD3DObject * pObj);
	virtual void AddObjectRecursive(CD3DObject * pObj);
	virtual bool AddRootObject(CD3DObject * pObj);
	virtual bool DelObject(CD3DObject * pObj);
	virtual bool DelRootObject(CD3DObject * pObj);

	bool MoveToTop(CD3DObject * pObj);	
	bool MoveToTop(CD3DObject * pObj,CD3DObject *pBefore);
	bool MoveToTop(CD3DObject ** ppObj,CD3DObject *pBefore,int ObjectCount);

	
	
	void SetCamera(CD3DCamera * pCamera);
	CD3DCamera * GetCamera();

	void SetLight(UINT Index,CD3DLight * pLight);
	CD3DLight * GetLight(UINT Index);
	bool DeleteLight(CD3DLight * pLight);
	bool DeleteLight(LPCTSTR LightName);

	int GetRenderObjectCount();
	int	GetRenderSubMeshCount();
	int GetRenderFaceCount();
	int GetRenderVertexCount();


protected:

	void RenderSubMesh(CD3DSubMesh * pSubMesh,CD3DFX * pRenderFX);	
	
};

inline CD3DDevice * CD3DRender::GetDevice()
{
	return m_pDevice;
}


inline CD3DCamera * CD3DRender::GetCamera()
{
	return m_pCamera;
}



inline CD3DLight * CD3DRender::GetLight(UINT Index)
{
	if(Index<MAX_LIGHT)
	{
		return m_Lights[Index];
	}
	return NULL;
}

inline int CD3DRender::GetRenderObjectCount()
{
	return m_ObjectCount;
}
inline int CD3DRender::GetRenderSubMeshCount()
{
	return m_SubMeshCount;
}
inline int CD3DRender::GetRenderFaceCount()
{
	return m_FaceCount;
}
inline int CD3DRender::GetRenderVertexCount()
{
	return m_VertexCount;
}



}