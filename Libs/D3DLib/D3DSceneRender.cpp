/****************************************************************************/
/*                                                                          */
/*      文件名:    D3DSceneRender.cpp                                       */
/*      创建日期:  2010年07月19日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/

#include "StdAfx.h"

namespace D3DLib{

IMPLEMENT_CLASS_INFO(CD3DSceneRender,CD3DRender);

CD3DSceneRender::CD3DSceneRender(void)
{
	m_RenderMode=RENDER_MODE_NORMAL;
	m_TreeMode=TREE_MODE_QD;
	m_pSceneRoot=NULL;
	m_MaxTreeDepth=16;
	m_pDepthTexture=NULL;
	m_ShowNodeFrame=false;
}

CD3DSceneRender::~CD3DSceneRender(void)
{
	Destory();
}

bool CD3DSceneRender::Create(CD3DDevice * pDevice,UINT TreeMode,CD3DBoundingBox SceneBox,FLOAT MinSize)
{
	m_TreeMode=TreeMode;
	RebuildScene(SceneBox,MinSize);
	return CD3DRender::Create(pDevice);
}
void CD3DSceneRender::Destory()
{
	SAFE_RELEASE(m_pDepthTexture);
	if(m_pSceneRoot)
	{
		DeleteSceneNode(m_pSceneRoot);
		m_pSceneRoot=NULL;
	}
	CD3DRender::Destory();
}

//bool CD3DSceneRender::AddObject(CD3DObject * pObj,bool IsRecursive)
//{
//	return AddSceneObject(pObj,IsRecursive,true);
//}

bool CD3DSceneRender::DelObject(CD3DObject * pObj)
{
	if(CD3DRender::DelObject(pObj))
	{
		return true;
	}

	return DeleteSceneObject(pObj);
}

bool CD3DSceneRender::AddSceneObject(CD3DObject * pObject,bool IsRecursive,bool IsUpdateSceneTree)
{
	
	if((!pObject->IsKindOf(GET_CLASS_INFO(CD3DWOWADTModel)))&&
		(!pObject->IsKindOf(GET_CLASS_INFO(CD3DWOWWMOModel)))&&
		(!pObject->IsKindOf(GET_CLASS_INFO(CD3DWOWWMOGroupModel)))&&
		(!pObject->IsKindOf(GET_CLASS_INFO(CD3DWOWM2Model)))&&
		(!pObject->IsKindOf(GET_CLASS_INFO(CD3DScene))))
	{
		return AddNoSceneObject(pObject,IsRecursive);
	}

	pObject->Update(0);	
	CD3DBoundingBox BBox=(*(pObject->GetBoundingBox()))*pObject->GetWorldMatrix();
	int Relation=m_pSceneRoot->NodeBox.CheckRelation(BBox);
	if((!BBox.IsValid())||Relation==CD3DBoundingBox::RELATION_TYPE_OUT||Relation==CD3DBoundingBox::RELATION_TYPE_BE_INCLUDE)
	{
		return AddNoSceneObject(pObject,IsRecursive);
	}

	if(pObject->GetParent()==NULL)
		AddRootObject(pObject);

	pObject->SetRender(this);
	if(pObject->CanRender())
	{
		for(UINT i=0;i<m_SceneObjectList.GetCount();i++)
		{
			if(m_SceneObjectList[i]==pObject)
			{
				m_SceneObjectList.Delete(i);
				return false;
			}
		}

		m_SceneObjectList.Add(pObject);
		m_pSceneRoot->ObjectList.Add(pObject);
	}
	
	if(IsRecursive)
	{
		for(UINT i=0;i<pObject->GetChildCount();i++)
		{
			AddSceneObject(pObject->GetChildByIndex(i),true,false);
		}
	}

	if(IsUpdateSceneTree)
	{
		UpdateSceneTree();
	}
	return true;
}

bool CD3DSceneRender::AddNoSceneObject(CD3DObject * pObj,bool IsRecursive)
{
	return CD3DRender::AddObject(pObj,IsRecursive);
}

bool CD3DSceneRender::DeleteSceneObject(CD3DObject * pObject)
{
	for(UINT i=0;i<m_SceneObjectList.GetCount();i++)
	{
		if(m_SceneObjectList[i]==pObject)
		{
			m_SceneObjectList.Delete(i);
			return DeleteObject(m_pSceneRoot,pObject)!=0;
		}
	}
	return false;
}

void CD3DSceneRender::UpdateSceneTree()
{
	UpdateTree(m_pSceneRoot);
	UpdateChildCount(m_pSceneRoot);
}

void CD3DSceneRender::RebuildScene(CD3DBoundingBox SceneBox,FLOAT MinSize)
{
	//确定最终包围盒大小
	CEasyArray<CD3DObject *> SceneObjectList=m_SceneObjectList;
	for(UINT i=0;i<SceneObjectList.GetCount();i++)
	{
		CD3DBoundingBox BBox=(*(SceneObjectList[i]->GetBoundingBox()))*SceneObjectList[i]->GetWorldMatrix();
		if(BBox.IsValid())
			SceneBox.Merge(BBox);		
	}
	m_SceneObjectList.Clear();
	//包围盒标准化
	FLOAT XLen=SceneBox.m_Max.x-SceneBox.m_Min.x;
	FLOAT ZLen=SceneBox.m_Max.z-SceneBox.m_Min.z;
	if(XLen>ZLen)
	{
		SceneBox.m_Max.z+=(XLen-ZLen)/2;
		SceneBox.m_Min.z-=(XLen-ZLen)/2;
	}
	else
	{
		SceneBox.m_Max.x+=(ZLen-XLen)/2;
		SceneBox.m_Min.x-=(ZLen-XLen)/2;
	}
	XLen=SceneBox.m_Max.x-SceneBox.m_Min.x;
	if(m_TreeMode==TREE_MODE_OC)
	{
		FLOAT YLen=SceneBox.m_Max.y-SceneBox.m_Min.y;
		if(XLen>YLen)
		{
			SceneBox.m_Max.y+=(XLen-YLen);
		}
	}
	//决定最大切割层数
	m_MaxTreeDepth=0;
	while(XLen>MinSize)
	{
		m_MaxTreeDepth++;
		XLen=XLen/2;
	}
	if(m_pSceneRoot)
	{
		DeleteSceneNode(m_pSceneRoot);
		m_pSceneRoot=NULL;
	}
	m_pSceneRoot=new TREE_NODE;
	m_pSceneRoot->NodeBox=SceneBox;
	for(UINT i=0;i<SceneObjectList.GetCount();i++)
	{
		AddSceneObject(SceneObjectList[i],false,false);
	}
	UpdateSceneTree();
}


void CD3DSceneRender::Render()
{	
	FUNCTION_BEGIN;
	LPDIRECT3DDEVICE9 pD3DDevice=m_pDevice->GetD3DDevice();
	if(pD3DDevice==NULL)
		return ;	
	if(!m_pCamera)
		return;

	//设置摄像机

	m_pCamera->Apply(m_pDevice,D3DCAMERA_APPLY_ALL);


	//设置灯光
	for(UINT i=0;i<MAX_LIGHT;i++)
	{
		m_pDevice->GetD3DDevice()->LightEnable(i,false);
	}
	for(UINT i=0;i<m_LightList.GetCount();i++)
	{
		if(i<MAX_LIGHT)
		{
			m_pDevice->GetD3DDevice()->LightEnable(i,true);			
			m_LightList[i]->Apply(m_pDevice,i);
		}
	}			



	//CD3DMatrix ViewProjMat=m_pCamera->GetViewMat()*m_pCamera->GetProjectMat();

	//CD3DFrustum Frustum=CD3DFrustum::FromCombinedMatrix(ViewProjMat);
	CD3DFrustum Frustum=CD3DFrustum::FromMatrixs(m_pCamera->GetViewMat(),m_pCamera->GetProjectMat());


	//清空计数
	m_ObjectCount=0;
	m_SubMeshCount=0;
	m_FaceCount=0;
	m_VertexCount=0;

	m_TreeCheckCount=0;
	m_ObjectCheckCount=0;
	m_TreeCutCount=0;

	m_TransparentSubMeshList.Empty();

	m_RenderMode=RENDER_MODE_NORMAL;

	for(UINT i=0;i<m_SceneObjectList.GetCount();i++)
	{
		m_SceneObjectList[i]->RemoveFlag(CD3DObject::OBJECT_FLAG_PREPARE_RENDERED|CD3DObject::OBJECT_FLAG_RENDERED);
	}

	for(UINT i=0;i<m_ObjectList.GetCount();i++)
	{
		m_ObjectList[i]->RemoveFlag(CD3DObject::OBJECT_FLAG_PREPARE_RENDERED|CD3DObject::OBJECT_FLAG_RENDERED);
	}

	//渲染场景对象

	RenderScene(m_pSceneRoot,Frustum,true);

	//渲染非场景对象

	for(UINT i=0;i<m_ObjectList.GetCount();i++)
	{
		CD3DObject * pObject=m_ObjectList[i];

		if(!pObject->CheckFlag(CD3DObject::OBJECT_FLAG_PREPARE_RENDERED))
			RenderObject(pObject,Frustum);
	}

	//再绘制透明的

	for(UINT i=0;i<m_TransparentSubMeshList.GetCount();i++)
	{
		SORT_DATA& SortData=m_TransparentSubMeshList[i];

		CD3DObject * pObject=SortData.pObj;

		CD3DSubMesh * pSubMesh;		
		CD3DSubMeshMaterial * pMaterial;

		pSubMesh=SortData.pSubMesh;
		pMaterial=SortData.pMaterial;

		pObject->PrepareRender(m_pDevice,NULL,NULL,m_LightList,m_pCamera);
		pObject->PrepareRender(m_pDevice,pSubMesh,pMaterial,m_LightList,m_pCamera);
		RenderSubMesh(pSubMesh,pMaterial->GetFX());

	}

	FUNCTION_END;
}

void CD3DSceneRender::RenderDepthTexture()
{
	FUNCTION_BEGIN;
	LPDIRECT3DDEVICE9 pD3DDevice=m_pDevice->GetD3DDevice();
	if(pD3DDevice==NULL)
		return ;	
	if(!m_pCamera)
		return;

	//设置摄像机

	m_pCamera->Apply(m_pDevice,D3DCAMERA_APPLY_ALL);

	//CD3DMatrix ViewProjMat=m_pCamera->GetViewMat()*m_pCamera->GetProjectMat();

	//CD3DFrustum Frustum=CD3DFrustum::FromCombinedMatrix(ViewProjMat);

	CD3DFrustum Frustum=CD3DFrustum::FromMatrixs(m_pCamera->GetViewMat(),m_pCamera->GetProjectMat());

	

	m_RenderMode=RENDER_MODE_DEPTH;

	//渲染场景对象
	
	for(UINT i=0;i<m_SceneObjectList.GetCount();i++)
	{
		m_SceneObjectList[i]->RemoveFlag(CD3DObject::OBJECT_FLAG_PREPARE_RENDERED|CD3DObject::OBJECT_FLAG_RENDERED);
	}

	RenderScene(m_pSceneRoot,Frustum,true);

	
	FUNCTION_END;
}

bool CD3DSceneRender::CreateDepthTexture(UINT Width,UINT Height)
{
	m_pDepthTexture=new CD3DTexture(m_pDevice->GetTextureManager());
	if(m_pDepthTexture->CreateTexture(Width,Height,
		D3DFMT_R32F,D3DUSAGE_RENDERTARGET,D3DPOOL_DEFAULT))
	{
		return true;
	}
	SAFE_RELEASE(m_pDepthTexture);		
	return false;
}

void CD3DSceneRender::DeleteSceneNode(TREE_NODE * pNode)
{
	if(pNode->HaveChild)
	{
		for(UINT i=0;i<m_TreeMode;i++)
		{
			DeleteSceneNode(pNode->Childs[i]);
			pNode->Childs[i]=NULL;
		}
	}
	SAFE_RELEASE(pNode->pBoundingFrame);
	delete pNode;
}

void CD3DSceneRender::UpdateTree(TREE_NODE * pNode)
{
	if(pNode->ObjectList.GetCount())
	{		
		if((!pNode->HaveChild)&&CanBeCut(pNode))
		{
			CreateChilds(pNode);
		}

		if(pNode->HaveChild)
		{
			for(UINT i=0;i<pNode->ObjectList.GetCount();i++)
			{
				CD3DBoundingBox BBox=(*(pNode->ObjectList[i]->GetBoundingBox()))*pNode->ObjectList[i]->GetWorldMatrix();
				for(UINT j=0;j<m_TreeMode;j++)
				{
					int Relation=pNode->Childs[j]->NodeBox.CheckRelation(BBox);
					if(Relation!=CD3DBoundingBox::RELATION_TYPE_OUT)
					{
						pNode->Childs[j]->ObjectList.Add(pNode->ObjectList[i]);
					}
				}
			}
			pNode->ObjectList.Clear();
		}
	}
	if(pNode->HaveChild)
	{
		for(UINT i=0;i<m_TreeMode;i++)
		{
			UpdateTree(pNode->Childs[i]);
		}
	}
}

void CD3DSceneRender::CreateChilds(TREE_NODE * pNode)
{
	pNode->HaveChild=true;
	CD3DBoundingBox ChildBBoxs[OTC_MAX];
	CutBBox(pNode->NodeBox,ChildBBoxs);
	for(UINT i=0;i<m_TreeMode;i++)
	{
		pNode->Childs[i]=new TREE_NODE;
		pNode->Childs[i]->pParent=pNode;
		pNode->Childs[i]->Depth=pNode->Depth+1;		
		pNode->Childs[i]->NodeBox=ChildBBoxs[i];
		if(m_ShowNodeFrame)
		{
			pNode->Childs[i]->pBoundingFrame=new CD3DBoundingFrame();
			pNode->Childs[i]->pBoundingFrame->SetDevice(m_pDevice);
			pNode->Childs[i]->pBoundingFrame->CreateFromBBox(pNode->Childs[i]->NodeBox,0xFF0000FF);
			AddNoSceneObject(pNode->Childs[i]->pBoundingFrame);
		}
	}
}

void CD3DSceneRender::CutBBox(CD3DBoundingBox BBox,CD3DBoundingBox * pChildBBoxs)
{
	CD3DVector3 Center=(BBox.m_Max+BBox.m_Min)/2;
	if(m_TreeMode==QTC_MAX)
	{
		for(UINT i=0;i<QTC_MAX;i++)
		{
			switch(i)
			{
			case QTC_FRONT_LEFT:		
				pChildBBoxs[i].m_Max.SetValue(Center.x,BBox.m_Max.y,BBox.m_Max.z);
				pChildBBoxs[i].m_Min.SetValue(BBox.m_Min.x,BBox.m_Min.y,Center.z);
				break;
			case QTC_FRONT_RIGHT:
				pChildBBoxs[i].m_Max.SetValue(BBox.m_Max.x,BBox.m_Max.y,BBox.m_Max.z);
				pChildBBoxs[i].m_Min.SetValue(Center.x,BBox.m_Min.y,Center.z);
				break;
			case QTC_BACK_LEFT:
				pChildBBoxs[i].m_Max.SetValue(Center.x,BBox.m_Max.y,Center.z);
				pChildBBoxs[i].m_Min.SetValue(BBox.m_Min.x,BBox.m_Min.y,BBox.m_Min.z);			
				break;
			case QTC_BACK_RIGHT:
				pChildBBoxs[i].m_Max.SetValue(BBox.m_Max.x,BBox.m_Max.y,Center.z);
				pChildBBoxs[i].m_Min.SetValue(Center.x,BBox.m_Min.y,BBox.m_Min.z);
				break;			
			}
		}
	}
	else
	{
		
		for(UINT i=0;i<OTC_MAX;i++)
		{
			switch(i)
			{
			case OTC_FRONT_LEFT_BOTTOM:		
				pChildBBoxs[i].m_Max.SetValue(Center.x,Center.y,BBox.m_Max.z);
				pChildBBoxs[i].m_Min.SetValue(BBox.m_Min.x,BBox.m_Min.y,Center.z);
				break;
			case OTC_FRONT_RIGHT_BOTTOM:
				pChildBBoxs[i].m_Max.SetValue(BBox.m_Max.x,Center.y,BBox.m_Max.z);
				pChildBBoxs[i].m_Min.SetValue(Center.x,BBox.m_Min.y,Center.z);
				break;
			case OTC_BACK_LEFT_BOTTOM:
				pChildBBoxs[i].m_Max=Center;
				pChildBBoxs[i].m_Min.SetValue(BBox.m_Min.x,BBox.m_Min.y,BBox.m_Min.z);			
				break;
			case OTC_BACK_RIGHT_BOTTOM:
				pChildBBoxs[i].m_Max.SetValue(BBox.m_Max.x,Center.y,Center.z);
				pChildBBoxs[i].m_Min.SetValue(Center.x,BBox.m_Min.y,BBox.m_Min.z);
				break;
			case OTC_FRONT_LEFT_TOP:	
				pChildBBoxs[i].m_Max.SetValue(Center.x,BBox.m_Max.y,BBox.m_Max.z);
				pChildBBoxs[i].m_Min.SetValue(BBox.m_Min.x,Center.y,Center.z);
				break;
			case OTC_FRONT_RIGHT_TOP:
				pChildBBoxs[i].m_Max.SetValue(BBox.m_Max.x,BBox.m_Max.y,BBox.m_Max.z);
				pChildBBoxs[i].m_Min=Center;
				break;
			case OTC_BACK_LEFT_TOP:
				pChildBBoxs[i].m_Max.SetValue(Center.x,BBox.m_Max.y,Center.z);
				pChildBBoxs[i].m_Min.SetValue(BBox.m_Min.x,Center.y,BBox.m_Min.z);
				break;
			case OTC_BACK_RIGHT_TOP:
				pChildBBoxs[i].m_Max.SetValue(BBox.m_Max.x,BBox.m_Max.y,Center.z);
				pChildBBoxs[i].m_Min.SetValue(Center.x,Center.y,BBox.m_Min.z);
				break;
			}
		}
	}
}

int CD3DSceneRender::DeleteObject(TREE_NODE * pNode,CD3DObject * pObject)
{
	int DeleteCount=0;
	for(UINT i=0;i<pNode->ObjectList.GetCount();i++)
	{
		if(pNode->ObjectList[i]==pObject)
		{
			pNode->ObjectList.Delete(i);
			DeleteCount++;
			break;
		}
	}
	if(pNode->HaveChild)
	{
		for(UINT i=0;i<m_TreeMode;i++)
		{
			DeleteCount+=DeleteObject(pNode->Childs[i],pObject);
		}
	}
	return DeleteCount;
}

bool CD3DSceneRender::CanBeCut(TREE_NODE * pNode)
{
	if(pNode->ObjectList.GetCount()>10&&pNode->Depth<m_MaxTreeDepth)
	{
		CD3DBoundingBox ChildBBoxs[OTC_MAX];
		CutBBox(pNode->NodeBox,ChildBBoxs);

		UINT ChildCount=0;
		//尝试分割，如果分割后，所有有对象的子树的对象数和父树都一样的，判定不可分割,如果只有一个子树被分配到对象，且对象数较多，就继续分割
		for(UINT j=0;j<m_TreeMode;j++)
		{
			UINT ObjCount=0;
			for(UINT i=0;i<pNode->ObjectList.GetCount();i++)
			{
				CD3DBoundingBox BBox=(*(pNode->ObjectList[i]->GetBoundingBox()))*pNode->ObjectList[i]->GetWorldMatrix();

				int Relation=ChildBBoxs[j].CheckRelation(BBox);
				if(Relation!=CD3DBoundingBox::RELATION_TYPE_OUT)
				{
					ObjCount++;
				}
			}

			if(ObjCount!=0&&ObjCount!=pNode->ObjectList.GetCount())
			{
				return true;
			}		

			if(ObjCount)
			{
				ChildCount++;
			}

		}

		if(ChildCount==1&&pNode->ObjectList.GetCount()>50)
		{
			return true;
		}

		return false;		
		
	}
	return false;
}

void CD3DSceneRender::RenderObject(CD3DObject * pObject,const CD3DFrustum& Frustum)
{
	m_ObjectCheckCount++;
	pObject->AddFlag(CD3DObject::OBJECT_FLAG_PREPARE_RENDERED);	

	if(pObject->IsVisible())//&&(!pObject->CheckFlag(CD3DObject::OBJECT_FLAG_RENDERED)))
	{

		//渲染深度纹理时，只渲染ADT和WMO
		if(m_RenderMode==RENDER_MODE_DEPTH&&
			(!pObject->IsKindOfFast(GET_CLASS_INFO(CD3DWOWADTModel)))&&
			(!pObject->IsKindOfFast(GET_CLASS_INFO(CD3DWOWWMOModel)))&&
			(!pObject->IsKindOfFast(GET_CLASS_INFO(CD3DWOWWMOGroupModel))))
		{
			return;
		}
		//视锥裁减

		

		int BoxCut=0;

		CD3DBoundingBox * pBBox=pObject->GetBoundingBox();
		if(pBBox)
		{
			CD3DBoundingBox BBox=(*pBBox)*pObject->GetWorldMatrix();
			BoxCut=Frustum.BoxLocation(BBox);
			if(BoxCut==0)
			{
				return;				
			}
		}
		else
		{
			return;	
		}

		m_ObjectCount++;			
		

		pObject->AddFlag(CD3DObject::OBJECT_FLAG_RENDERED);

		if(m_RenderMode==RENDER_MODE_NORMAL&&pObject->IsKindOfFast(GET_CLASS_INFO(CD3DWOWWMOModel)))
		{
			((CD3DWOWWMOModel *)pObject)->DoPortalCull(m_pCamera);
		}
		

		pObject->PrepareRender(m_pDevice,NULL,NULL,m_LightList,m_pCamera);
		int SubMeshCount=pObject->GetSubMeshCount();
		for(int i=0;i<SubMeshCount;i++)
		{
			CD3DSubMesh * pSubMesh;
			CD3DSubMeshMaterial * pMaterial;

			pSubMesh=pObject->GetSubMesh(i);
			pMaterial=pObject->GetSubMeshMaterial(i);

			if(pSubMesh)
			{
				if(pSubMesh->IsVisible())
				{
					if(m_RenderMode==RENDER_MODE_DEPTH)
					{
						//渲染深度纹理时，对于ADT只渲染带有水面的部分
						if(pObject->IsKindOfFast(GET_CLASS_INFO(CD3DWOWADTModel))&&
							(!pSubMesh->CheckProperty(CD3DWOWADTModelResource::SMP_RENDER_TO_DEPTH)))
						{
							continue;
						}
					}
					
					if(BoxCut==1&&pObject->CanDoSubMeshViewCull())
					{
						//对于对象部分可见的，进行SubMesh级裁减
						CD3DBoundingBox BBox=pSubMesh->GetBoundingBox();
						BBox*=pObject->GetWorldMatrix();
						if(!Frustum.BoxLocation(BBox))
						{
							pSubMesh->SetCulled(true);
							continue;
						}
					}
					pSubMesh->SetCulled(false);
					if(pSubMesh->IsTransparent())
					{
						if(m_RenderMode==RENDER_MODE_NORMAL)
						{
							SORT_DATA SortData;
							SortData.pObj=pObject;
							SortData.pSubMesh=pSubMesh;
							SortData.pMaterial=pMaterial;							
							m_TransparentSubMeshList.Add(SortData);							
						}
						continue;
					}				
					pObject->PrepareRender(m_pDevice,pSubMesh,pMaterial,m_LightList,m_pCamera);
					if(m_RenderMode==RENDER_MODE_NORMAL)
					{
						RenderSubMesh(pSubMesh,pMaterial->GetFX());
					}
					else
					{
						RenderSubMesh(pSubMesh,pMaterial->GetFX(),"TechForDepthTexture");
					}
				}					
			}

		}

	}
}

void CD3DSceneRender::RenderScene(TREE_NODE * pNode,const CD3DFrustum& Frustum,bool NeedCheck)
{
	if(NeedCheck)
	{
		m_TreeCheckCount++;
		int BoxCut=Frustum.BoxLocation(pNode->NodeBox);
		switch(BoxCut)
		{
		case CD3DFrustum::LR_OUT:
			m_TreeCutCount++;
			return;
			break;
		case CD3DFrustum::LR_INTERSECT:
			if(pNode->HaveChild)
			{
				for(UINT i=0;i<m_TreeMode;i++)
				{
					RenderScene(pNode->Childs[i],Frustum,true);
				}
			}
			else
			{
				for(UINT i=0;i<pNode->ObjectList.GetCount();i++)
				{
					//RenderObject(pNode->ObjectList[i],Frustum);
					CD3DObject * pObject=pNode->ObjectList[i];
					if(!pObject->CheckFlag(CD3DObject::OBJECT_FLAG_PREPARE_RENDERED))
						RenderObject(pObject,Frustum);
				}
			}
			break;
		case CD3DFrustum::LR_INCLUDE:
			if(pNode->HaveChild)
			{
				for(UINT i=0;i<m_TreeMode;i++)
				{
					RenderScene(pNode->Childs[i],Frustum,false);
				}
			}
			else
			{
				for(UINT i=0;i<pNode->ObjectList.GetCount();i++)
				{
					//RenderObject(pNode->ObjectList[i],Frustum);
					for(UINT i=0;i<pNode->ObjectList.GetCount();i++)
					{
						CD3DObject * pObject=pNode->ObjectList[i];
						if(!pObject->CheckFlag(CD3DObject::OBJECT_FLAG_PREPARE_RENDERED))
							RenderObject(pObject,Frustum);
					}
				}
			}
			break;
		}
	}
	else
	{
		if(pNode->HaveChild)
		{
			for(UINT i=0;i<m_TreeMode;i++)
			{
				RenderScene(pNode->Childs[i],Frustum,false);
			}
		}
		else
		{
			for(UINT i=0;i<pNode->ObjectList.GetCount();i++)
			{
				//RenderObject(pNode->ObjectList[i],Frustum);
				for(UINT i=0;i<pNode->ObjectList.GetCount();i++)
				{
					CD3DObject * pObject=pNode->ObjectList[i];
					if(!pObject->CheckFlag(CD3DObject::OBJECT_FLAG_PREPARE_RENDERED))
						RenderObject(pObject,Frustum);
				}
			}
		}
	}
}

void CD3DSceneRender::UpdateChildCount(TREE_NODE * pNode)
{
	pNode->ChildCount=0;
	pNode->ObjectCount=pNode->ObjectList.GetCount();
	if(pNode->HaveChild)
	{
		for(UINT i=0;i<m_TreeMode;i++)
		{			
			pNode->ChildCount++;
			UpdateChildCount(pNode->Childs[i]);
			pNode->ChildCount+=pNode->Childs[i]->ChildCount;
			pNode->ObjectCount+=pNode->Childs[i]->ObjectCount;
		}
		
	}
}

}