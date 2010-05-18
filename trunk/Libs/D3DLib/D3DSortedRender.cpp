/****************************************************************************/
/*                                                                          */
/*      文件名:    D3DSortedRender.cpp                                      */
/*      创建日期:  2010年02月09日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#include "StdAfx.h"

namespace D3DLib{

IMPLEMENT_CLASS_INFO(CD3DSortedRender,CD3DRender);

CD3DSortedRender::CD3DSortedRender(void):CD3DRender()
{
}

CD3DSortedRender::~CD3DSortedRender(void)
{
}


void CD3DSortedRender::Render()
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
	for(int l=0;l<MAX_LIGHT;l++)
	{
		if(m_Lights[l])
		{
			m_pDevice->GetD3DDevice()->LightEnable(l,true);
			m_Lights[l]->Apply(m_pDevice,l);
		}
		else
		{
			m_pDevice->GetD3DDevice()->LightEnable(l,false);
		}
	}			

	

	CD3DMatrix ViewProjMat=m_pCamera->GetViewMat()*m_pCamera->GetProjectMat();

	CD3DFrustum Frustum=CD3DFrustum::FromCombinedMatrix(ViewProjMat);
	
	
	//清空计数
	m_ObjectCount=0;
	m_SubMeshCount=0;
	m_FaceCount=0;
	m_VertexCount=0;

		
	//CSubMeshSortMap SubMeshQueue;

	m_TransparentSubMeshList.Empty();
	
		
	//先绘制不透明的

	for(UINT i=0;i<m_ObjectList.GetCount();i++)
	{
		CD3DObject * pObject=m_ObjectList[i];
		
		if(pObject->IsVisible())
		{
			
			//视锥裁减

			int BoxCut=0;

			CD3DBoundingBox * pBBox=pObject->GetBoundingBox();
			if(pBBox)
			{
				CD3DBoundingBox BBox=(*pBBox)*pObject->GetWorldMatrix();
				BoxCut=Frustum.BoxLocation(BBox);
				if(BoxCut==0)
				{
					pObject->SetCull(true);
					continue;				
				}
				pObject->SetCull(false);
			}
			else
			{
				pObject->SetCull(true);
				continue;	
			}
			
			m_ObjectCount++;
			
			pObject->PrepareRender(m_pDevice,NULL,NULL,m_Lights,m_pCamera);
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
							SORT_DATA SortData;
							SortData.pObj=pObject;
							SortData.pSubMesh=pSubMesh;
							SortData.pMaterial=pMaterial;
							//CD3DBoundingBox BBox=pSubMesh->GetBoundingBox();
							//BBox*=pObject->GetWorldMatrix();
							//BBox*=ViewProjMat;
							//SortData.ZMin=BBox.m_Min.z;
							//SortData.ZMax=BBox.m_Max.z;
							m_TransparentSubMeshList.Add(SortData);							
							continue;
						}				
						pObject->PrepareRender(m_pDevice,pSubMesh,pMaterial,m_Lights,m_pCamera);
						RenderSubMesh(pSubMesh,pMaterial->GetFX());
					}					
				}

			}
			
		}

	}

	//再绘制透明的
	
	for(UINT i=0;i<m_TransparentSubMeshList.GetCount();i++)
	{
		SORT_DATA& SortData=m_TransparentSubMeshList[i];
	
		CD3DObject * pObject=SortData.pObj;
			
		pObject->PrepareRender(m_pDevice,NULL,NULL,m_Lights,m_pCamera);
		int SubMeshCount=pObject->GetSubMeshCount();
		
		CD3DSubMesh * pSubMesh;		
		CD3DSubMeshMaterial * pMaterial;

		pSubMesh=SortData.pSubMesh;
		pMaterial=SortData.pMaterial;

		pObject->PrepareRender(m_pDevice,pSubMesh,pMaterial,m_Lights,m_pCamera);
		RenderSubMesh(pSubMesh,pMaterial->GetFX());
				
	}
	FUNCTION_END;
}

void CD3DSortedRender::Update(FLOAT Time)
{	
	FUNCTION_BEGIN;
	for(int i=0;i<(int)m_RootObjectList.GetCount();i++)
	{
		m_RootObjectList[i]->Update(Time);
	}
	FUNCTION_END;
}

void CD3DSortedRender::InsertSubMeshSorted(CEasyList<SORT_DATA>& SubMeshList,SORT_DATA& SortData)
{	
	SubMeshList.InsertAfter(SortData);
	//void * Pos=SubMeshList.GetHead();
	//void * OldPos=Pos;
	//while(Pos)
	//{		
	//	SORT_DATA& ExistSortData=SubMeshList.GetNextObject(Pos);
	//	if(ExistSortData.ZMax<SortData.ZMin)
	//		break;
	//	OldPos=Pos;
	//}
	//SubMeshList.InsertBefore(SortData,OldPos);
}

}