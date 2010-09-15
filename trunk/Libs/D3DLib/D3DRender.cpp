/****************************************************************************/
/*                                                                          */
/*      文件名:    D3DRender.cpp                                            */
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


IMPLEMENT_CLASS_INFO(CD3DRender,CNameObject);

CD3DRender::CD3DRender():CNameObject()
{
	m_pDevice=NULL;		
	m_pCamera=NULL;
	m_ObjectCount=0;
	m_SubMeshCount=0;
	m_FaceCount=0;
	m_VertexCount=0;

	m_FogColor=0xFFFFFFFF;
	m_FogNear=10000.0f;
	m_FogFar=10000.0f;

	

	m_ObjectList.Create(100,10);
	m_RootObjectList.Create(100,10);

}

CD3DRender::~CD3DRender(void)
{	
	Destory();	
}

bool CD3DRender::Create(CD3DDevice * pDevice)
{
	m_pDevice=pDevice;	
	return true;	
}

void CD3DRender::Destory()
{
	m_ObjectList.Clear();
	m_RootObjectList.Clear();
	SAFE_RELEASE(m_pCamera);	
	m_LightList.Clear();
	CNameObject::Destory();
}

bool CD3DRender::Reset()
{		
	for(UINT i=0;i<m_ObjectList.GetCount();i++)
	{
		m_ObjectList[i]->Reset();
	}
	return true;
}

bool CD3DRender::Restore() 
{		
	for(UINT i=0;i<m_ObjectList.GetCount();i++)
	{
		m_ObjectList[i]->Restore();
	}
    return true;
}


void CD3DRender::Render()
{	
	FUNCTION_BEGIN;
	LPDIRECT3DDEVICE9 pD3DDevice=m_pDevice->GetD3DDevice();
	if(pD3DDevice==NULL)
		return ;	

	//设置摄像机
	if(m_pCamera)
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

	//清空计数
	m_ObjectCount=0;
	m_SubMeshCount=0;
	m_FaceCount=0;
	m_VertexCount=0;

	bool IsCurDefaultLight=false;


	for(UINT i=0;i<m_ObjectList.GetCount();i++)
	{
		CD3DObject * pObject=m_ObjectList[i];
		if(pObject->IsVisible())
		{
			m_ObjectCount++;
			
			
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
						pObject->PrepareRender(m_pDevice,pSubMesh,pMaterial,m_LightList,m_pCamera);
						RenderSubMesh(pSubMesh,pMaterial->GetFX());
					}					
				}
					
			}
		}
		
	}	
	FUNCTION_END;
}

void CD3DRender::RenderDirectly(CD3DObject * pObject)
{
	LPDIRECT3DDEVICE9 pD3DDevice=m_pDevice->GetD3DDevice();
	if(pD3DDevice==NULL)
		return ;	

	if(pObject->IsVisible())
	{
		int SubMeshCount=pObject->GetSubMeshCount();
		pObject->PrepareRender(m_pDevice,NULL,NULL,m_LightList,m_pCamera);
		for(int i=0;i<SubMeshCount;i++)
		{
			CD3DSubMesh * pSubMesh;
			CD3DSubMeshMaterial * pMaterial;

			pSubMesh=pObject->GetSubMesh(i);
			pMaterial=pObject->GetSubMeshMaterial(i);
			if(pSubMesh)
			{
				pObject->PrepareRender(m_pDevice,pSubMesh,pMaterial,m_LightList,m_pCamera);
				RenderSubMesh(pSubMesh,pMaterial->GetFX());
			}

		}
	}

}

void CD3DRender::Update(FLOAT Time)
{	
	FUNCTION_BEGIN;
	for(int i=0;i<(int)m_RootObjectList.GetCount();i++)
	{
		m_RootObjectList[i]->Update(Time);
	}
	FUNCTION_END;
}

bool CD3DRender::AddObject(CD3DObject * pObj,bool IsRecursive)
{
	if(pObj->GetParent()==NULL)
		AddRootObject(pObj);

	pObj->SetRender(this);
	if(pObj->CanRender())
	{
		bool IsExist=false;
		for(UINT i=0;i<m_ObjectList.GetCount();i++)
		{
			if(m_ObjectList[i]==pObj)
			{
				IsExist=true;
				break;
			}
		}	
		if(!IsExist)
			m_ObjectList.Add(pObj);
	}
	else if(pObj->IsKindOf(GET_CLASS_INFO(CD3DLight)))
	{
		AddLight((CD3DLight *)pObj);
	}

	if(IsRecursive)
	{
		for(UINT i=0;i<pObj->GetChildCount();i++)
		{
			AddObject(pObj->GetChildByIndex(i));
		}
	}
	
	return true;
}


bool CD3DRender::AddRootObject(CD3DObject * pObj)
{
	for(size_t i=0;i<m_RootObjectList.GetCount();i++)
	{
		if(m_RootObjectList[i]==pObj)
			return false;
	}
	pObj->SetRender(this);
	m_RootObjectList.Add(pObj);
	return true;
}

bool CD3DRender::DelObject(CD3DObject * pObj)
{
	for(int i=(int)m_ObjectList.GetCount()-1;i>=0;i--)
	{
		if(m_ObjectList[i]==pObj)
		{
			m_ObjectList.Delete(i);
			if(pObj->IsKindOf(GET_CLASS_INFO(CD3DLight)))
			{
				DeleteLight((CD3DLight *)pObj);
			}
			if(pObj->GetParent()==NULL)
				DelRootObject(pObj);
			return true;
		}
	}
	return false;
}

bool CD3DRender::DelRootObject(CD3DObject * pObj)
{
	for(int i=(int)m_RootObjectList.GetCount()-1;i>=0;i--)
	{
		if(m_RootObjectList[i]==pObj)
		{
			m_RootObjectList.Delete(i);
			return true;
		}
	}
	return false;
}

bool CD3DRender::MoveToTop(CD3DObject * pObj)
{
	if(DelObject(pObj))
	{
		m_ObjectList.Add(pObj);
		return true;
	}
	return false;
}

bool CD3DRender::MoveToTop(CD3DObject * pObj,CD3DObject *pBefore)
{
	if(DelObject(pObj))
	{
		for(UINT i=0;i<m_ObjectList.GetCount();i++)
		{		
			if(m_ObjectList[i]==pBefore)
			{				
				m_ObjectList.Insert(i+1,pObj);
				return true;				
			}
		}	
		m_ObjectList.Add(pObj);
		return true;
	}	
	return false;
}

bool CD3DRender::MoveToTop(CD3DObject ** ppObj,CD3DObject *pBefore,int ObjectCount)
{
	for(int i=0;i<(int)m_ObjectList.GetCount();i++)
	{	
		
		if(m_ObjectList[i]==pBefore)
		{			
			for(int j=0;j<ObjectCount;j++)
			{				
				if(DelObject(ppObj[j]))
				{
					m_ObjectList.Insert(i+1,ppObj[j]);
				}				
			}
			return true;;
		}
	}	
	return false;
}

CD3DTexture * CD3DRender::GetDepthTexture()
{
	return NULL;
}


void CD3DRender::RenderSubMesh(CD3DSubMesh * pSubMesh,CD3DFX * pRenderFX,LPCTSTR RenderTech)
{	
	FUNCTION_BEGIN;
	m_SubMeshCount++;
	m_FaceCount+=pSubMesh->GetPrimitiveCount();
	m_VertexCount+=pSubMesh->GetVertexCount();

	if(pSubMesh->GetPrimitiveCount()<=0)
		return;


	//设置顶点结构定义
	if(pSubMesh->GetVertexFormat().pVertexDeclaration)
	{
		m_pDevice->GetD3DDevice()->SetVertexDeclaration(pSubMesh->GetVertexFormat().pVertexDeclaration);
	}
	else
	{
		m_pDevice->GetD3DDevice()->SetFVF(pSubMesh->GetVertexFormat().FVF);
	}

	D3DFORMAT IndexFormat=D3DFMT_INDEX16;
	if(pSubMesh->GetVertexFormat().IndexSize==sizeof(DWORD))
		IndexFormat=D3DFMT_INDEX32;
	

	//填充顶点数据
	if(pSubMesh->GetRenderBufferUsed()==CD3DSubMesh::BUFFER_USE_DX)
	{
		m_pDevice->GetD3DDevice()->SetStreamSource( 0, pSubMesh->GetDXVertexBuffer(), 0, pSubMesh->GetVertexFormat().VertexSize );
		if(pSubMesh->GetIndexCount())
			m_pDevice->GetD3DDevice()->SetIndices(pSubMesh->GetDXIndexBuffer());
	}
	
	
	HRESULT hr;
	//提交渲染	
	if(pRenderFX)
	{		
		if(RenderTech)
			pRenderFX->UseTechnique(RenderTech);
		else
			pRenderFX->UseActiveTechnique();
		int Pass=pRenderFX->Begin();
		for(int i=0;i<Pass;i++)
		{
			if(pRenderFX->BeginPass(i))
			{
				if(pSubMesh->GetIndexCount())
				{
					if(pSubMesh->GetRenderBufferUsed()==CD3DSubMesh::BUFFER_USE_DX)
					{
						hr=m_pDevice->GetD3DDevice()->DrawIndexedPrimitive((D3DPRIMITIVETYPE)pSubMesh->GetPrimitiveType(),
							0,0,pSubMesh->GetVertexCount(),pSubMesh->GetIndexStart(),pSubMesh->GetPrimitiveCount());
					}
					else
					{
						hr=m_pDevice->GetD3DDevice()->DrawIndexedPrimitiveUP((D3DPRIMITIVETYPE)pSubMesh->GetPrimitiveType(),
							0,pSubMesh->GetVertexCount(),pSubMesh->GetPrimitiveCount(),pSubMesh->GetIndexBuffer(),IndexFormat,
							pSubMesh->GetVertexBuffer(),pSubMesh->GetVertexFormat().VertexSize);
					}
				}
				else
				{
					if(pSubMesh->GetRenderBufferUsed()==CD3DSubMesh::BUFFER_USE_DX)
					{
						hr=m_pDevice->GetD3DDevice()->DrawPrimitive( (D3DPRIMITIVETYPE)pSubMesh->GetPrimitiveType(), 
							pSubMesh->GetVertexStart(), pSubMesh->GetPrimitiveCount() );
					}
					else
					{
						hr=m_pDevice->GetD3DDevice()->DrawPrimitiveUP( (D3DPRIMITIVETYPE)pSubMesh->GetPrimitiveType(), 
							pSubMesh->GetPrimitiveCount(), pSubMesh->GetVertexBuffer(),pSubMesh->GetVertexFormat().VertexSize );
					}
				}
				pRenderFX->EndPass(i);
			}
		}
		pRenderFX->End();
	}
	else
	{
		if(pSubMesh->GetIndexCount())
		{
			if(pSubMesh->GetRenderBufferUsed()==CD3DSubMesh::BUFFER_USE_DX)
			{
				hr=m_pDevice->GetD3DDevice()->DrawIndexedPrimitive((D3DPRIMITIVETYPE)pSubMesh->GetPrimitiveType(),
					0,0,pSubMesh->GetVertexCount(),pSubMesh->GetIndexStart(),pSubMesh->GetPrimitiveCount());
			}
			else
			{
				hr=m_pDevice->GetD3DDevice()->DrawIndexedPrimitiveUP((D3DPRIMITIVETYPE)pSubMesh->GetPrimitiveType(),
					0,pSubMesh->GetVertexCount(),pSubMesh->GetPrimitiveCount(),pSubMesh->GetIndexBuffer(),IndexFormat,
					pSubMesh->GetVertexBuffer(),pSubMesh->GetVertexFormat().VertexSize);
			}
		}
		else
		{
			if(pSubMesh->GetRenderBufferUsed()==CD3DSubMesh::BUFFER_USE_DX)
			{
				hr=m_pDevice->GetD3DDevice()->DrawPrimitive( (D3DPRIMITIVETYPE)pSubMesh->GetPrimitiveType(), 
					pSubMesh->GetVertexStart(), pSubMesh->GetPrimitiveCount() );
			}
			else
			{
				hr=m_pDevice->GetD3DDevice()->DrawPrimitiveUP( (D3DPRIMITIVETYPE)pSubMesh->GetPrimitiveType(), 
					pSubMesh->GetPrimitiveCount(), pSubMesh->GetVertexBuffer(),pSubMesh->GetVertexFormat().VertexSize );
			}
		}
	}
	FUNCTION_END;
}

void CD3DRender::SetCamera(CD3DCamera * pCamera)
{
	SAFE_RELEASE(m_pCamera);
	m_pCamera=pCamera;
	if(m_pCamera)
		m_pCamera->AddUseRef();
}

void CD3DRender::AddLight(CD3DLight * pLight)
{
	m_LightList.Add(pLight);	
}

bool CD3DRender::DeleteLight(UINT Index)
{
	if(Index<m_LightList.GetCount())
	{
		m_LightList.Delete(Index);
		return true;
	}
	return false;
}

bool CD3DRender::DeleteLight(CD3DLight * pLight)
{
	for(UINT i=0;i<m_LightList.GetCount();i++)
	{
		if(m_LightList[i]==pLight)
		{
			return DeleteLight(i);
		}
	}
	return false;
}

bool CD3DRender::DeleteLight(LPCTSTR LightName)
{
	for(UINT i=0;i<m_LightList.GetCount();i++)
	{
		if(strcmp(m_LightList[i]->GetName(),LightName)==0)
		{
			return DeleteLight(i);
		}
	}	
	return false;
}

}
