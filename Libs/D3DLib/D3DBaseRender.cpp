/****************************************************************************/
/*                                                                          */
/*      文件名:    D3DBaseRender.cpp                                        */
/*      创建日期:  2011年01月14日				                            */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#include "StdAfx.h"


namespace D3DLib{


IMPLEMENT_CLASS_INFO(CD3DBaseRender,CNameObject);

CD3DBaseRender::CD3DBaseRender(void)
{
	m_pDevice=NULL;

	m_PrepareRenderDataFailCount=0;

	m_ObjectCount=0;
	m_SubMeshCount=0;
	m_FaceCount=0;
	m_VertexCount=0;
}

CD3DBaseRender::~CD3DBaseRender(void)
{
}

bool CD3DBaseRender::Reset()
{
	return true;
}
bool CD3DBaseRender::Restore()
{
	return true;
}


void CD3DBaseRender::Render()
{

}

void CD3DBaseRender::Update(FLOAT Time)
{
	for(int i=0;i<(int)m_RootObjectList.GetCount();i++)
	{
		m_RootObjectList[i]->Update(Time);
	}

	if(CD3DDevice::IsUseMultiThreadRender())
	{		
		CAutoLockEx Lock;
		if(m_PrepareRenderDataFailCount<MAX_PREPARE_RENDER_DATA_FAIL_COUNT)
		{
			if(Lock.TryLock(m_RenderLock))
			{
				for(int i=0;i<(int)m_RootObjectList.GetCount();i++)
				{
					PrepareRenderData(m_RootObjectList[i]);
				}
				m_PrepareRenderDataFailCount=0;
			}
			else
			{
				m_PrepareRenderDataFailCount++;
			}
		}
		else
		{
			Lock.Lock(m_RenderLock);
			for(int i=0;i<(int)m_RootObjectList.GetCount();i++)
			{
				PrepareRenderData(m_RootObjectList[i]);
			}
			m_PrepareRenderDataFailCount=0;
		}
		
	}
}


bool CD3DBaseRender::AddObject(CD3DObject * pObj,bool IsRecursive)
{
	return false;
}
bool CD3DBaseRender::AddRootObject(CD3DObject * pObj)
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
bool CD3DBaseRender::DelObject(CD3DObject * pObj)
{
	return false;
}
bool CD3DBaseRender::DelRootObject(CD3DObject * pObj)
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

void CD3DBaseRender::RenderSubMesh(CD3DSubMesh * pSubMesh,CD3DFX * pRenderFX,LPCTSTR RenderTech)
{	
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
							pSubMesh->GetVertexStart(),pSubMesh->GetVertexStart(),pSubMesh->GetVertexCount(),
							pSubMesh->GetIndexStart(),pSubMesh->GetPrimitiveCount());
					}
					else
					{
						hr=m_pDevice->GetD3DDevice()->DrawIndexedPrimitiveUP((D3DPRIMITIVETYPE)pSubMesh->GetPrimitiveType(),
							pSubMesh->GetVertexStart(),
							pSubMesh->GetVertexStart()+pSubMesh->GetVertexCount(),
							pSubMesh->GetPrimitiveCount(),
							pSubMesh->GetIndexBufferR()+pSubMesh->GetVertexFormat().IndexSize*pSubMesh->GetIndexStart(),
							IndexFormat,
							pSubMesh->GetVertexBufferR(),
							pSubMesh->GetVertexFormat().VertexSize);
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
							pSubMesh->GetPrimitiveCount(), 
							pSubMesh->GetVertexBufferR()+pSubMesh->GetVertexFormat().VertexSize*pSubMesh->GetVertexStart(),
							pSubMesh->GetVertexFormat().VertexSize );
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
					pSubMesh->GetVertexStart(),pSubMesh->GetVertexStart(),pSubMesh->GetVertexCount(),
					pSubMesh->GetIndexStart(),pSubMesh->GetPrimitiveCount());
			}
			else
			{
				hr=m_pDevice->GetD3DDevice()->DrawIndexedPrimitiveUP((D3DPRIMITIVETYPE)pSubMesh->GetPrimitiveType(),
					pSubMesh->GetVertexStart(),
					pSubMesh->GetVertexStart()+pSubMesh->GetVertexCount(),
					pSubMesh->GetPrimitiveCount(),
					pSubMesh->GetIndexBufferR()+pSubMesh->GetVertexFormat().IndexSize*pSubMesh->GetIndexStart(),
					IndexFormat,
					pSubMesh->GetVertexBufferR(),
					pSubMesh->GetVertexFormat().VertexSize);
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
					pSubMesh->GetPrimitiveCount(), 
					pSubMesh->GetVertexBufferR()+pSubMesh->GetVertexFormat().VertexSize*pSubMesh->GetVertexStart(),
					pSubMesh->GetVertexFormat().VertexSize );
			}
		}
	}
}


void CD3DBaseRender::PrepareRenderData(CD3DObject * pObject)
{
	{
		CAutoLock Lock(pObject->GetRenderLock());		

		pObject->OnPrepareRenderData();
	}
	for(UINT i=0;i<pObject->GetChildCount();i++)
	{
		PrepareRenderData(pObject->GetChildByIndex(i));
	}
}

}