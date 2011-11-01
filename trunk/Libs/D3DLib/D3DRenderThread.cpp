#include "StdAfx.h"


namespace D3DLib{


IMPLEMENT_CLASS_INFO(CD3DRenderThread,CEasyThread);

CD3DRenderThread::CD3DRenderThread(void)
{
	m_pDevice=NULL;

	m_FrameCount=0;
	m_FrameRate=0;

	m_RenderInstances.Resize(1);

}

CD3DRenderThread::~CD3DRenderThread(void)
{
}

BOOL CD3DRenderThread::OnStart()
{
	if(!CD3DDevice::IsUseMultiThreadRender())
	{
		PrintD3DLog(0,"未采用多线程模式创建设备,渲染线程无法启用");
		return FALSE;
	}
	m_FrameCountTimer.SaveTime();
	return m_pDevice!=NULL;
}
BOOL CD3DRenderThread::OnRun()
{
	CAutoLock Lock(m_EditLock);

	for(UINT i=0;i<m_RenderInstances.GetCount();i++)
	{
		if(m_RenderInstances[i].Enable)
		{
			DoRender(m_RenderInstances.GetObject(i));
		}
	}

	m_FrameCount++;
	if(m_FrameCountTimer.IsTimeOut(FRAME_COUNT_TIME))
	{
		m_FrameCountTimer.SaveTime();
		m_FrameRate=(float)(m_FrameCount*1000.0f)/FRAME_COUNT_TIME;
		m_FrameCount=0;

		//for(UINT j=0;j<m_RenderInstances.GetCount();j++)
		//{
		//	for(UINT i=0;i<m_RenderInstances[j].RenderList.GetCount();i++)
		//	{
		//		if(m_RenderInstances[j].RenderList[i]->IsKindOf(GET_CLASS_INFO(CD3DSceneRender)))
		//		{
		//			PrintSystemLog(0,"FrameRate=%g,TreeCheck=%u,TreeCut=%u,ObjectCheck=%u,DirectRenderCount=%u,BranchObjectCheckCount=%u,ObjectCutCount=%u",
		//				m_FrameRate,
		//				((CD3DSceneRender *)m_RenderInstances[j].RenderList[i])->GetTreeCheckCount(),
		//				((CD3DSceneRender *)m_RenderInstances[j].RenderList[i])->GetTreeCutCount(),
		//				((CD3DSceneRender *)m_RenderInstances[j].RenderList[i])->GetObjectCheckCount(),
		//				((CD3DSceneRender *)m_RenderInstances[j].RenderList[i])->GetDirectRenderCount(),
		//				((CD3DSceneRender *)m_RenderInstances[j].RenderList[i])->GetBranchObjectCheckCount(),
		//				((CD3DSceneRender *)m_RenderInstances[j].RenderList[i])->GetObjectCutCount());
		//		}

		//	}	
		//}
	}

	if(!m_RenderControlTimer.IsTimeOut(MINI_RENDER_TIME))
	{
		Sleep(1);
	}
	
	return TRUE;
}
void CD3DRenderThread::OnTerminate()
{
	
}

void CD3DRenderThread::DoRender(RENDER_INSTANCE * pRenderInstance)
{
	int ObjectCount=0;
	int SubMeshCount=0;
	int FaceCount=0;
	int VertexCount=0;

	m_RenderControlTimer.SaveTime();

	m_pDevice->GetTextureManager()->PrepareRenderData();

	if(pRenderInstance->pSwapChain)
	{
		m_pDevice->SetRenderTarget(0,pRenderInstance->pSwapChain);
	}
	m_pDevice->StartRender(pRenderInstance->ClearColor);
	for(UINT i=0;i<pRenderInstance->RenderList.GetCount();i++)
	{		

		if(pRenderInstance->RenderList[i]->IsKindOf(GET_CLASS_INFO(CD3DSceneRender)))
		{
			CD3DSceneRender * pSceneRender=(CD3DSceneRender *)pRenderInstance->RenderList[i];
			if(pSceneRender->GetDepthTexture())
			{
				pSceneRender->ClearDepthTexture();
				m_pDevice->SetRenderTarget(1,pSceneRender->GetDepthTexture(),0);
				//m_pDevice->Clear(pRenderInstance->ClearColor);
			}
		}
		pRenderInstance->RenderList[i]->Render();	

		if(pRenderInstance->RenderList[i]->IsKindOf(GET_CLASS_INFO(CD3DSceneRender)))
		{
			CD3DSceneRender * pSceneRender=(CD3DSceneRender *)pRenderInstance->RenderList[i];
			if(pSceneRender->GetDepthTexture())
			{
				m_pDevice->RestoreRenderTargetToDefault(1);
			}
		}

		ObjectCount+=pRenderInstance->RenderList[i]->GetRenderObjectCount();
		SubMeshCount+=pRenderInstance->RenderList[i]->GetRenderSubMeshCount();
		FaceCount+=pRenderInstance->RenderList[i]->GetRenderFaceCount();
		VertexCount+=pRenderInstance->RenderList[i]->GetRenderVertexCount();		
	}
	if(pRenderInstance->pSwapChain)
	{
		m_pDevice->EndRender(false);
		pRenderInstance->pSwapChain->Present();
		m_pDevice->RestoreRenderTargetToDefault(0);
	}
	else
	{
		m_pDevice->EndRender();
	}
	
	pRenderInstance->ObjectCount=ObjectCount;
	pRenderInstance->SubMeshCount=SubMeshCount;
	pRenderInstance->FaceCount=FaceCount;
	pRenderInstance->VertexCount=VertexCount;		
	
}

}