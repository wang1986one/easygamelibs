/****************************************************************************/
/*                                                                          */
/*      文件名:    D3DRender.h                                              */
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

class CD3DCamera;
class CD3DLight;
class CD3DObject;


class CD3DRender :
	public CNameObject
{
protected:
	CD3DDevice *				m_pDevice;
	CD3DCamera *				m_pCamera;
	CEasyArray<CD3DLight *>		m_LightList;

	D3DCOLOR					m_FogColor;
	FLOAT						m_FogNear;
	FLOAT						m_FogFar;
	

	int							m_ObjectCount;
	int							m_SubMeshCount;
	int							m_FaceCount;
	int							m_VertexCount;

	CEasyArray<CD3DObject *>	m_ObjectList;
	CEasyArray<CD3DObject *>	m_RootObjectList;

	

	DECLARE_CLASS_INFO(CD3DRender)
public:
	CD3DRender();
	virtual ~CD3DRender(void);

	bool Create(CD3DDevice * pDevice);
	virtual void Destory();
	virtual bool Reset();
	virtual bool Restore();	
	
	virtual void Render();
	virtual void RenderDirectly(CD3DObject * pObject);
	virtual void Update(FLOAT Time);

	CD3DDevice * GetDevice();

	virtual bool AddObject(CD3DObject * pObj,bool IsRecursive=true);
	virtual bool AddRootObject(CD3DObject * pObj);
	virtual bool DelObject(CD3DObject * pObj);
	virtual bool DelRootObject(CD3DObject * pObj);

	bool MoveToTop(CD3DObject * pObj);	
	bool MoveToTop(CD3DObject * pObj,CD3DObject *pBefore);
	bool MoveToTop(CD3DObject ** ppObj,CD3DObject *pBefore,int ObjectCount);

	
	
	void SetCamera(CD3DCamera * pCamera);
	CD3DCamera * GetCamera();

	
	UINT GetLightCount();
	CD3DLight * GetLight(UINT Index);
	

	int GetRenderObjectCount();
	int	GetRenderSubMeshCount();
	int GetRenderFaceCount();
	int GetRenderVertexCount();

	virtual CD3DTexture * GetDepthTexture();

	D3DCOLOR GetFogColor();
	FLOAT GetFogNear();
	FLOAT GetFogFar();

	void SetFog(D3DCOLOR FogColor,FLOAT FogNear,FLOAT FogFar);


protected:

	void RenderSubMesh(CD3DSubMesh * pSubMesh,CD3DFX * pRenderFX,LPCTSTR RenderTech=NULL);	

	void AddLight(CD3DLight * pLight);
	bool DeleteLight(UINT Index);
	bool DeleteLight(CD3DLight * pLight);
	bool DeleteLight(LPCTSTR LightName);
	
};

inline CD3DDevice * CD3DRender::GetDevice()
{
	return m_pDevice;
}


inline CD3DCamera * CD3DRender::GetCamera()
{
	return m_pCamera;
}

inline UINT CD3DRender::GetLightCount()
{
	return m_LightList.GetCount();
}

inline CD3DLight * CD3DRender::GetLight(UINT Index)
{
	if(Index<m_LightList.GetCount())
	{
		return m_LightList[Index];
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

inline D3DCOLOR CD3DRender::GetFogColor()
{
	return m_FogColor;
}
inline FLOAT CD3DRender::GetFogNear()
{
	return m_FogNear;
}
inline FLOAT CD3DRender::GetFogFar()
{
	return m_FogFar;
}

inline void CD3DRender::SetFog(D3DCOLOR FogColor,FLOAT FogNear,FLOAT FogFar)
{
	m_FogColor=FogColor;
	m_FogNear=FogNear;
	m_FogFar=FogFar;
}

}