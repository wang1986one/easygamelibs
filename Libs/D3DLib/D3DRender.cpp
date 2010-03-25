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
	m_pVertexBuffer=NULL;
	m_VertexBuffSize=0;
	m_pIndexBuffer=NULL;
	m_IndexBuffSize=0;
	m_MaxLayer=0;
	m_Is16BitIndex=true;
	m_pCamera=NULL;
	m_ObjectCount=0;
	m_SubMeshCount=0;
	m_FaceCount=0;
	m_VertexCount=0;
	ZeroMemory(m_Lights,sizeof(m_Lights));
	

	m_ObjectList.Create(100,10);
	m_RootObjectList.Create(100,10);

}

CD3DRender::~CD3DRender(void)
{	
	Destory();	
}

bool CD3DRender::Create(CD3DDevice * pDevice,UINT VertexBuffSize,UINT IndexBuffSize,int MaxLayer,bool Is16BitIndex)
{
	m_pDevice=pDevice;
	m_VertexBuffSize=VertexBuffSize;
	m_IndexBuffSize=IndexBuffSize;
	m_MaxLayer=MaxLayer;
	m_Is16BitIndex=Is16BitIndex;	
	return Restore();	
}

void CD3DRender::Destory()
{
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pIndexBuffer);
	m_ObjectList.Clear();
	m_RootObjectList.Clear();
	SAFE_RELEASE(m_pCamera);
	for(int i=0;i<MAX_LIGHT;i++)
	{
		SAFE_RELEASE(m_Lights[i]);
	}
	CNameObject::Destory();
}

bool CD3DRender::Reset()
{	
	Destory();
	return true;
}

bool CD3DRender::Restore() 
{	
	HRESULT hr;
	LPDIRECT3DDEVICE9 pD3DDevice=m_pDevice->GetD3DDevice();
	if(pD3DDevice==NULL)
		return false;
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pIndexBuffer);
	if(m_VertexBuffSize)
	{		
		hr = pD3DDevice->CreateVertexBuffer( m_VertexBuffSize,
			D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC, 0,
			D3DPOOL_DEFAULT, &m_pVertexBuffer, NULL );
		if( FAILED(hr) )
		{
			return false;
		}   
	}
	if(m_IndexBuffSize)
	{
		hr = pD3DDevice->CreateIndexBuffer(m_IndexBuffSize,
			D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC,
			//( m_Is16BitIndex ? D3DFMT_INDEX16 : D3DFMT_INDEX32 ),
			D3DFMT_INDEX16,
			D3DPOOL_DEFAULT,
			&m_pIndexBuffer,
			NULL);
	}
    return true;
}

void CD3DRender::Release()
{
	if(this)
		delete this;
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
						pObject->PrepareRender(m_pDevice,pSubMesh,pMaterial,m_Lights,m_pCamera);
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
		pObject->PrepareRender(m_pDevice,NULL,NULL,m_Lights,m_pCamera);
		for(int i=0;i<SubMeshCount;i++)
		{
			CD3DSubMesh * pSubMesh;
			CD3DSubMeshMaterial * pMaterial;

			pSubMesh=pObject->GetSubMesh(i);
			pMaterial=pObject->GetSubMeshMaterial(i);
			if(pSubMesh)
			{
				pObject->PrepareRender(m_pDevice,pSubMesh,pMaterial,m_Lights,m_pCamera);
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

bool CD3DRender::AddObject(CD3DObject * pObj)
{
	if(pObj->GetParent()==NULL)
		AddRootObject(pObj);

	pObj->SetRender(this);
	if(pObj->CanRender())
	{
		for(UINT i=0;i<m_ObjectList.GetCount();i++)
		{
			if(m_ObjectList[i]==pObj)
				return false;
		}		
		m_ObjectList.Add(pObj);
	}
	
	return true;
}

void CD3DRender::AddObjectRecursive(CD3DObject * pObj)
{
	AddObject(pObj);
	for(UINT i=0;i<pObj->GetChildCount();i++)
	{
		AddObjectRecursive(pObj->GetChildByIndex(i));
	}
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




void CD3DRender::RenderSubMesh(CD3DSubMesh * pSubMesh,CD3DFX * pRenderFX)
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
	

	//填充顶点数据
	if(pSubMesh->GetDXVertexBuffer())
		m_pDevice->GetD3DDevice()->SetStreamSource( 0, pSubMesh->GetDXVertexBuffer(), 0, pSubMesh->GetVertexFormat().VertexSize );
	else
	{
		m_pDevice->GetD3DDevice()->SetStreamSource( 0, m_pVertexBuffer, 0, pSubMesh->GetVertexFormat().VertexSize );

		BYTE * pVertices;
		int DataSize;
		DataSize=pSubMesh->GetVertexFormat().VertexSize*pSubMesh->GetVertexCount();
		if(SUCCEEDED(m_pVertexBuffer->Lock( 0, DataSize, (void**)&pVertices, D3DLOCK_DISCARD )))
		{
			memcpy(pVertices,pSubMesh->GetVertexs(),DataSize);			
			m_pVertexBuffer->Unlock();		
		}
		else
			return;
	}

	//填充索引数据
	if(pSubMesh->GetDXIndexBuffer())
		m_pDevice->GetD3DDevice()->SetIndices(pSubMesh->GetDXIndexBuffer());
	else
	{
		if(m_pIndexBuffer)
		{
			m_pDevice->GetD3DDevice()->SetIndices(m_pIndexBuffer);

			BYTE * pIndexs;
			int DataSize=pSubMesh->GetVertexFormat().IndexSize*pSubMesh->GetIndexCount();
			if(SUCCEEDED(m_pIndexBuffer->Lock( 0, DataSize, (void**)&pIndexs, D3DLOCK_DISCARD )))
			{
				memcpy(pIndexs,pSubMesh->GetIndexs(),DataSize);
				m_pIndexBuffer->Unlock();		
			}
			else
				return;
		}
	}	

	
	
	
	HRESULT hr;
	//提交渲染
	//CD3DFX * pFX=pSubMesh->GetMaterial().pFX;
	if(pRenderFX)
	{		
		pRenderFX->UseActiveTechnique();
		int Pass=pRenderFX->Begin();
		for(int i=0;i<Pass;i++)
		{
			if(pRenderFX->BeginPass(i))
			{
				if(pSubMesh->GetIndexCount())
					hr=m_pDevice->GetD3DDevice()->DrawIndexedPrimitive((D3DPRIMITIVETYPE)pSubMesh->GetPrimitiveType(),0,0,pSubMesh->GetVertexCount(),pSubMesh->GetIndexStart(),pSubMesh->GetPrimitiveCount());
				else
					hr=m_pDevice->GetD3DDevice()->DrawPrimitive( (D3DPRIMITIVETYPE)pSubMesh->GetPrimitiveType(), pSubMesh->GetVertexStart(), pSubMesh->GetPrimitiveCount() );
				pRenderFX->EndPass(i);
			}
		}
		pRenderFX->End();
	}
	else
	{
		if(pSubMesh->GetIndexCount())
			m_pDevice->GetD3DDevice()->DrawIndexedPrimitive ( (D3DPRIMITIVETYPE)pSubMesh->GetPrimitiveType(),0, 0,pSubMesh->GetVertexCount(),pSubMesh->GetIndexStart(), pSubMesh->GetPrimitiveCount() );
		else
			m_pDevice->GetD3DDevice()->DrawPrimitive( (D3DPRIMITIVETYPE)pSubMesh->GetPrimitiveType(), pSubMesh->GetVertexStart(), pSubMesh->GetPrimitiveCount() );
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

void CD3DRender::SetLight(UINT Index,CD3DLight * pLight)
{
	if(Index<MAX_LIGHT)
	{
		SAFE_RELEASE(m_Lights[Index]);
		m_Lights[Index]=pLight;
		if(pLight)
			pLight->AddUseRef();
	}
}

bool CD3DRender::DeleteLight(CD3DLight * pLight)
{
	for(int i=0;i<MAX_LIGHT;i++)
	{
		if(m_Lights[i]==pLight)
		{
			SAFE_RELEASE(m_Lights[i]);
			return true;
		}
	}
	return false;
}

bool CD3DRender::DeleteLight(LPCTSTR LightName)
{
	for(int i=0;i<MAX_LIGHT;i++)
	{
		if(m_Lights[i])
		{
			if(strcmp(m_Lights[i]->GetName(),LightName)==0)
			{
				SAFE_RELEASE(m_Lights[i]);
				return true;
			}
		}
	}
	return false;
}

}
