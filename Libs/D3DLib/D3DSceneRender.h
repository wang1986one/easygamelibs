/****************************************************************************/
/*                                                                          */
/*      文件名:    D3DSceneRender.h                                         */
/*      创建日期:  2010年07月19日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once

namespace D3DLib{

class CD3DBoundingFrame;
class CD3DWOWWMOModel;

class CD3DSceneRender :
	public CD3DBaseRender
{
public:
	enum RENDER_MODE
	{
		RENDER_MODE_NORMAL,
		RENDER_MODE_DEPTH,
	};

	enum OCTREE_CHILD
	{
		OTC_FRONT_LEFT_BOTTOM,
		OTC_FRONT_RIGHT_BOTTOM,
		OTC_BACK_LEFT_BOTTOM,
		OTC_BACK_RIGHT_BOTTOM,
		OTC_FRONT_LEFT_TOP,
		OTC_FRONT_RIGHT_TOP,
		OTC_BACK_LEFT_TOP,
		OTC_BACK_RIGHT_TOP,
		OTC_MAX,
	};	

	enum QDTREE_CHILD
	{
		QTC_FRONT_LEFT,
		QTC_FRONT_RIGHT,
		QTC_BACK_LEFT,
		QTC_BACK_RIGHT,		
		QTC_MAX,
	};

	enum eMINI_CUT_COUNT
	{
		MINI_CUT_COUNT=2,
	};

	enum RENDER_OBJECT_TYPE
	{
		RBT_OBJECT,
		RBT_SUBMESH,
	};
	
	struct RENDER_BATCH_INFO
	{			
		CD3DObject *			pObj;
		CD3DSubMesh *			pSubMesh;
		CD3DSubMeshMaterial *	pMaterial;
	};

	struct RENDER_OBJECT_INFO
	{
		BYTE					ObjectType;
		CD3DBoundingBox			WorldBBox;
		CD3DObject *			pObject;
		CD3DSubMesh *			pSubMesh;
		CD3DSubMeshMaterial *	pMaterial;
		bool					IsPreRenderd;
		bool					IsRenderd;
	};
	
	struct TREE_NODE
	{
		CEasyArray<RENDER_OBJECT_INFO *>	ObjectList;
		TREE_NODE *							Childs[OTC_MAX];
		TREE_NODE *							pParent;
		UINT								Depth;
		CD3DBoundingBox						NodeBox;
		bool								HaveChild;
		UINT								ChildCount;
		UINT								ObjectCount;
		CD3DBoundingFrame *					pBoundingFrame;
		bool								IsRendered;
		bool								IsModified;
		bool								IsFixed;
		TREE_NODE()
		{
			ZeroMemory(Childs,sizeof(Childs));
			pParent=NULL;
			Depth=0;
			HaveChild=false;
			ChildCount=0;
			ObjectCount=0;
			pBoundingFrame=NULL;
			IsRendered=false;
			IsModified=true;
			IsFixed=false;
		}
	};	

	struct BBOX_RANGE_INFO
	{
		float	Weight;
		UINT	MinCount;
		UINT	MaxCount;
		float	Min;
		float	Max;
	};

	
protected:

	CD3DCamera *							m_pCamera;
	CD3DFX *								m_pSharedParamFX;
	CEasyArray<CD3DLight *>					m_LightList;

	D3DCOLOR								m_FogColor;
	FLOAT									m_FogNear;
	FLOAT									m_FogFar;	

	CEasyArray<CD3DObject *>				m_ObjectList;
	CEasyArray<CD3DWOWWMOModel *>			m_WMOObjectList;

	UINT									m_TreeMode;
	TREE_NODE *								m_pSceneRoot;
	TREE_NODE *								m_pSelectedSceneTree;
	CEasyArray<RENDER_OBJECT_INFO *>		m_SceneRenderUnitList;
	CEasyArray<CD3DObject *>				m_SceneObjectList;
	UINT									m_MaxTreeDepth;

	CEasyArray<RENDER_BATCH_INFO>			m_RenderBatchList;
	CEasyArray<RENDER_BATCH_INFO>			m_TransparentSubMeshList;
	CD3DTexture *							m_pDepthTexture;

	UINT									m_TreeCheckCount;
	UINT									m_ObjectCheckCount;
	UINT									m_DirectRenderCount;
	UINT									m_BranchObjectCheckCount;
	UINT									m_TreeCutCount;
	UINT									m_ObjectCutCount;
	UINT									m_HeightTestCount;

	bool									m_ShowNodeFrame;

	float									m_SizeCullSize;

	DECLARE_CLASS_INFO(CD3DSceneRender)
public:
public:
	enum TREE_MODE
	{
		TREE_MODE_QD=QTC_MAX,
		TREE_MODE_OC=OTC_MAX,
	};


	CD3DSceneRender(void);
	~CD3DSceneRender(void);

	bool Create(CD3DDevice * pDevice,UINT TreeMode,CD3DBoundingBox SceneBox,int StartDepth);
	virtual void Destory();


	virtual bool AddObject(CD3DObject * pObj,bool IsRecursive=true);	
	virtual bool DelObject(CD3DObject * pObj);
	
	bool AddSceneObject(CD3DObject * pObject,bool IsRecursive=true,bool IsUpdateSceneTree=true);
	

	void UpdateSceneTree();

	virtual void SetCamera(CD3DCamera * pCamera);
	virtual CD3DCamera * GetCamera();

	virtual UINT GetLightCount();
	virtual CD3DLight * GetLight(UINT Index);

	D3DCOLOR GetFogColor();
	FLOAT GetFogNear();
	FLOAT GetFogFar();

	void SetFog(D3DCOLOR FogColor,FLOAT FogNear,FLOAT FogFar);
	

	virtual void Render();

	bool CreateDepthTexture();

	virtual CD3DTexture * GetDepthTexture();

	void SetSizeCullSize(float MiniSize);

	UINT GetTreeCheckCount();
	UINT GetObjectCheckCount();
	UINT GetDirectRenderCount();
	UINT GetBranchObjectCheckCount();
	UINT GetTreeCutCount();
	UINT GetObjectCutCount();

	void ShowNodeFrame(bool IsShow);
	void ShowNodeFrame(TREE_NODE * pTree,bool IsShow);

	TREE_NODE * GetSceneRoot();
	void SetSelectTree(TREE_NODE * pTree);
	UINT GetSceneTreeMode();

	void HideCutObject(TREE_NODE * pTree);

	void FindTreeByObject(TREE_NODE * pRoot,CD3DObject * pObject,CEasyArray<TREE_NODE *>& TreeList);
	TREE_NODE * FindTreeByBBox(TREE_NODE * pRoot,const CD3DBoundingBox& BBox);

	bool GetHeightByXZ(const CD3DVector3 Pos,FLOAT MinHeight,FLOAT MaxHeight,FLOAT& Height,FLOAT& WaterHeight);

protected:

	void BuildSceneTree(TREE_NODE * pRoot,int StartDepth);

	bool AddWMOObject(CD3DWOWWMOModel * pWMOObject);
	bool DelWMOObject(CD3DWOWWMOModel * pWMOObject);

	void AddLight(CD3DLight * pLight);
	bool DeleteLight(UINT Index);
	bool DeleteLight(CD3DLight * pLight);
	bool DeleteLight(LPCTSTR LightName);
	
	bool DeleteSceneObject(CD3DObject * pObject);
	int DeleteObject(TREE_NODE * pNode,RENDER_OBJECT_INFO * pObjectInfo);
	void DeleteSceneNode(TREE_NODE * pNode);
	bool AddObjectToTree(TREE_NODE * pTree,CD3DObject * pObject,bool BeWalkTree);
	void AddObjectToTree(TREE_NODE * pTree,RENDER_OBJECT_INFO * pObjectInfo);
	void UpdateTree(TREE_NODE * pNode);
	void CreateChilds(TREE_NODE * pNode,CD3DVector3 CutCenter);
	void CutBBox(TREE_NODE * pNode,CD3DVector3 CutCenter,CD3DBoundingBox * ChildBBoxs);
	
	bool CanBeCut(TREE_NODE * pNode,CD3DVector3 CutCenter);
	void RenderObject(CD3DObject * pObject,const CD3DFrustum& Frustum,bool NeedFrustumCheck);
	void RenderSubMesh(RENDER_OBJECT_INFO * pObjectInfo,const CD3DFrustum& Frustum,bool NeedFrustumCheck);
	void RenderScene(TREE_NODE * pNode,const CD3DFrustum& Frustum,bool NeedCheck);
	void DoBatchRender();

	void ClearTreeRenderFlag(TREE_NODE * pNode);

	int CheckBBoxRelation(const CD3DBoundingBox& BBox1,const CD3DBoundingBox& BBox2);
	CD3DVector3 GetBoxCutCenter(TREE_NODE * pNode);
	bool HeightTest(TREE_NODE * pRoot,const CD3DVector3 Pos,FLOAT MinHeight,FLOAT MaxHeight,FLOAT& Height,FLOAT& WaterHeight);
	
	static int SubMeshCompare(const void * s1,const void * s2);

	void RenderSubMeshEx(CD3DSubMesh * pSubMesh);
};

inline CD3DCamera * CD3DSceneRender::GetCamera()
{
	return m_pCamera;
}

inline UINT CD3DSceneRender::GetLightCount()
{
	return m_LightList.GetCount();
}

inline CD3DLight * CD3DSceneRender::GetLight(UINT Index)
{
	if(Index<m_LightList.GetCount())
	{
		return m_LightList[Index];
	}
	return NULL;
}


inline D3DCOLOR CD3DSceneRender::GetFogColor()
{
	return m_FogColor;
}
inline FLOAT CD3DSceneRender::GetFogNear()
{
	return m_FogNear;
}
inline FLOAT CD3DSceneRender::GetFogFar()
{
	return m_FogFar;
}

inline void CD3DSceneRender::SetFog(D3DCOLOR FogColor,FLOAT FogNear,FLOAT FogFar)
{
	m_FogColor=FogColor;
	m_FogNear=FogNear;
	m_FogFar=FogFar;
}

inline CD3DTexture * CD3DSceneRender::GetDepthTexture()
{
	return m_pDepthTexture;
}

inline void CD3DSceneRender::SetSizeCullSize(float MiniSize)
{
	m_SizeCullSize=MiniSize;
}

inline UINT CD3DSceneRender::GetTreeCheckCount()
{
	return m_TreeCheckCount;
}
inline UINT CD3DSceneRender::GetObjectCheckCount()
{
	return m_ObjectCheckCount;
}
inline UINT CD3DSceneRender::GetDirectRenderCount()
{
	return m_DirectRenderCount;
}
inline UINT CD3DSceneRender::GetBranchObjectCheckCount()
{
	return m_BranchObjectCheckCount;
}
inline UINT CD3DSceneRender::GetTreeCutCount()
{
	return m_TreeCutCount;
}
inline UINT CD3DSceneRender::GetObjectCutCount()
{
	return m_ObjectCutCount;
}

inline void CD3DSceneRender::ShowNodeFrame(bool IsShow)
{
	m_ShowNodeFrame=IsShow;
}

inline CD3DSceneRender::TREE_NODE * CD3DSceneRender::GetSceneRoot()
{
	return m_pSceneRoot;
}

inline UINT CD3DSceneRender::GetSceneTreeMode()
{
	return m_TreeMode;
}

}