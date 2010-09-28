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
class CD3DSceneRender :
	public CD3DRender
{
protected:
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
	
	struct TREE_NODE
	{
		CEasyArray<CD3DObject *>	ObjectList;
		TREE_NODE *					Childs[OTC_MAX];
		TREE_NODE *					pParent;
		BYTE						Depth;
		CD3DBoundingBox				NodeBox;
		bool						HaveChild;
		UINT						ChildCount;
		UINT						ObjectCount;
		CD3DBoundingFrame *			pBoundingFrame;
		TREE_NODE()
		{
			ZeroMemory(Childs,sizeof(Childs));
			pParent=NULL;
			Depth=0;
			HaveChild=false;
			ChildCount=0;
			ObjectCount=0;
			pBoundingFrame=NULL;
		}
	};	

	struct SORT_DATA
	{			
		FLOAT			ZMin;
		FLOAT			ZMax;
		CD3DObject *	pObj;
		CD3DSubMesh *	pSubMesh;
		CD3DSubMeshMaterial * pMaterial;
	};	

	UINT									m_RenderMode;
	UINT									m_TreeMode;
	TREE_NODE *								m_pSceneRoot;
	CEasyArray<CD3DObject *>				m_SceneObjectList;
	UINT									m_MaxTreeDepth;

	CEasyArray<SORT_DATA>					m_TransparentSubMeshList;
	CD3DTexture *							m_pDepthTexture;

	UINT									m_TreeCheckCount;
	UINT									m_ObjectCheckCount;
	UINT									m_TreeCutCount;

	bool									m_ShowNodeFrame;

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

	bool Create(CD3DDevice * pDevice,UINT TreeMode,CD3DBoundingBox SceneBox,FLOAT MinSize);
	virtual void Destory();

	//virtual bool AddObject(CD3DObject * pObj,bool IsRecursive=true);
	virtual bool DelObject(CD3DObject * pObj);
	bool AddSceneObject(CD3DObject * pObject,bool IsRecursive=true,bool IsUpdateSceneTree=true);
	bool AddNoSceneObject(CD3DObject * pObj,bool IsRecursive=true);
	bool DeleteSceneObject(CD3DObject * pObject);
	void UpdateSceneTree();

	void RebuildScene(CD3DBoundingBox SceneBox,FLOAT MinSize);

	virtual void Render();

	void RenderDepthTexture();

	bool CreateDepthTexture(UINT Width,UINT Height);

	virtual CD3DTexture * GetDepthTexture();

protected:
	void DeleteSceneNode(TREE_NODE * pNode);
	void UpdateTree(TREE_NODE * pNode);
	void CreateChilds(TREE_NODE * pNode);
	void CutBBox(CD3DBoundingBox BBox,CD3DBoundingBox * ChildBBoxs);
	int DeleteObject(TREE_NODE * pNode,CD3DObject * pObject);
	bool CanBeCut(TREE_NODE * pNode);
	void RenderObject(CD3DObject * pObject,const CD3DFrustum& Frustum);
	void RenderScene(TREE_NODE * pNode,const CD3DFrustum& Frustum,bool NeedCheck);
	void UpdateChildCount(TREE_NODE * pNode);
	
};

inline CD3DTexture * CD3DSceneRender::GetDepthTexture()
{
	return m_pDepthTexture;
}

}