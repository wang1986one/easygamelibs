/****************************************************************************/
/*                                                                          */
/*      文件名:    D3DWOWM2RibbonEmitter.cpp                                */
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

IMPLEMENT_CLASS_INFO(CD3DWOWM2RibbonEmitter,CD3DObject);

CD3DWOWM2RibbonEmitter::CD3DWOWM2RibbonEmitter(void)
{
	m_pModelResource=NULL;
	m_EmitterIndex=0;
	m_MaxRibbonCount=0;
	m_pRibbonVertexBuffer=NULL;
	m_RibbonCount=0;
	m_StartTime=-1;
}

CD3DWOWM2RibbonEmitter::~CD3DWOWM2RibbonEmitter(void)
{
	Destory();
}

void CD3DWOWM2RibbonEmitter::Destory()
{
	m_pModelResource=NULL;
	m_EmitterIndex=0;
	m_MaxRibbonCount=0;
	m_RibbonCount=0;
	m_StartTime=-1;
	SAFE_DELETE_ARRAY(m_pRibbonVertexBuffer);
	m_SubMesh.Destory();
	CD3DObject::Destory();
}

bool CD3DWOWM2RibbonEmitter::Reset()
{
	return m_SubMesh.Reset();
}
bool CD3DWOWM2RibbonEmitter::Restore()
{
	return m_SubMesh.Restore();
}

int CD3DWOWM2RibbonEmitter::GetSubMeshCount()
{
	return 1;
}
CD3DSubMesh * CD3DWOWM2RibbonEmitter::GetSubMesh(int index)
{
	return &m_SubMesh;
}

CD3DBoundingBox * CD3DWOWM2RibbonEmitter::GetBoundingBox()
{	
	if(m_pModelResource)
	{			
		return m_pModelResource->GetBoundingBox();
	}
	return NULL;
}
CD3DBoundingSphere * CD3DWOWM2RibbonEmitter::GetBoundingSphere()
{
	if(m_pModelResource)
		return m_pModelResource->GetBoundingSphere();
	return NULL;
}

bool CD3DWOWM2RibbonEmitter::CanDoSubMeshViewCull()
{
	return false;
}

bool CD3DWOWM2RibbonEmitter::CloneFrom(CNameObject * pObject,UINT Param)
{
	if(!pObject->IsKindOf(GET_CLASS_INFO(CD3DWOWM2RibbonEmitter)))
		return false;

	if(!CD3DObject::CloneFrom(pObject,Param))
		return false;

	CD3DWOWM2RibbonEmitter * pSrcObject=(CD3DWOWM2RibbonEmitter *)pObject;

	SetDevice(pSrcObject->GetDevice());

	if(!Init(pSrcObject->m_pModelResource,pSrcObject->m_EmitterIndex))
		return false;

	

	m_RibbonCount=pSrcObject->m_RibbonCount;
	m_StartTime=pSrcObject->m_StartTime;
	m_RecentCreatePos=pSrcObject->m_RecentCreatePos;
	m_ClipLength=pSrcObject->m_ClipLength;	

	memcpy(m_pRibbonVertexBuffer,pSrcObject->m_pRibbonVertexBuffer,sizeof(RIBBON_PAIR)*m_RibbonCount);

	m_SubMesh.SetVertexCount(m_RibbonCount*2);		
	m_SubMesh.SetPrimitiveCount((m_RibbonCount-1)*2);

	return true;
}

void CD3DWOWM2RibbonEmitter::PrepareRender(CD3DDevice * pDevice,CD3DSubMesh * pSubMesh,CD3DSubMeshMaterial * pMaterial,CD3DLight ** pLight,CD3DCamera * pCamera)
{
	if(pSubMesh&&pMaterial)
	{	
		if(pMaterial->GetFX())
		{
			//设置灯光
			if(pLight[0])
			{			
				D3DLIGHT9 Light;
				pLight[0]->GetCurLight(Light);
				pMaterial->GetFX()->SetVector("LightDir",CD3DVector4(Light.Direction));
				pMaterial->GetFX()->SetColor("LightAmbient",Light.Ambient);
				pMaterial->GetFX()->SetColor("LightDiffuse",Light.Diffuse);
				pMaterial->GetFX()->SetColor("LightSpecular",Light.Specular);
			}
			//设置材质
			D3DMATERIAL9 * pD3DMaterial;
			if(pSubMesh->IsSelected())
				pD3DMaterial=&SELECTED_SUBMESH_MATERIAL;
			else
				pD3DMaterial=&(pMaterial->GetMaterial());
			pMaterial->GetFX()->SetColor("MaterialAmbient",pD3DMaterial->Ambient);
			pMaterial->GetFX()->SetColor("MaterialDiffuse",pD3DMaterial->Diffuse);
			pMaterial->GetFX()->SetColor("MaterialSpecular",pD3DMaterial->Specular);
			pMaterial->GetFX()->SetColor("MaterialEmissive",pD3DMaterial->Emissive);
			pMaterial->GetFX()->SetFloat("MaterialPower",pD3DMaterial->Power);

			//设置纹理
			pMaterial->GetFX()->SetTexture("TexLay0",pMaterial->GetTexture(0));

			//设置视投影矩阵
			pMaterial->GetFX()->SetMatrix("ViewMatrix",pCamera->GetViewMat());
			pMaterial->GetFX()->SetMatrix("ProjMatrix",pCamera->GetProjectMat());


		}	
	}
	else
	{

	}
}

void CD3DWOWM2RibbonEmitter::Update(FLOAT Time)
{
	FUNCTION_BEGIN;
	CD3DObject::Update(Time);

	//Time=Time/100;
	
	

	int CreateCount=0;
	

	CD3DWOWM2ModelResource::RIBBON_EMITTER_INFO * pRibbonEmitterInfo=m_pModelResource->GetRibbonEmitterInfo(m_EmitterIndex);
	if(pRibbonEmitterInfo)
	{
		if(m_StartTime<0)
		{
			m_StartTime=Time;		
			m_RibbonCount=0;
			m_RecentCreatePos=CD3DVector3(0,0,0)*GetWorldMatrix();
			m_ClipLength=(pRibbonEmitterInfo->Length/pRibbonEmitterInfo->Resolution)*GetWorldMatrix().GetScale().x;
			CD3DWOWM2ModelResource::RIBBON_PARAM Param;
			if(m_pModelResource->MakeRibbonParam(m_EmitterIndex,0,Param))
			{
				BuildRibbon(pRibbonEmitterInfo,&Param);
				CreateCount++;
			}
		}

		UINT CurTime=(UINT)((Time-m_StartTime)*1000);

		CD3DVector3 Pos(0,0,0);
		Pos=Pos*GetWorldMatrix();
		CD3DVector3 Len=Pos-m_RecentCreatePos;
		if(Len.Length()>=m_ClipLength)
		{			
			m_RecentCreatePos=Pos;
			CD3DWOWM2ModelResource::RIBBON_PARAM Param;
			if(m_pModelResource->MakeRibbonParam(m_EmitterIndex,CurTime,Param))
			{
				BuildRibbon(pRibbonEmitterInfo,&Param);
				CreateCount++;
			}
		}	
		
	}
	
	if(CreateCount)
	{
		m_SubMesh.SetVertexCount(m_RibbonCount*2);		
		m_SubMesh.SetPrimitiveCount((m_RibbonCount-1)*2);
	}

	

	//PrintSystemLog(0,"Particle=%u,%u",CreateCount,DeleteCount);
	FUNCTION_END;
}

bool CD3DWOWM2RibbonEmitter::Init(CD3DWOWM2ModelResource * pModelResource,UINT EmitterIndex)
{
	if(GetDevice()==NULL)
		return false;
	
	Destory();
	m_pModelResource=pModelResource;
	m_EmitterIndex=EmitterIndex;
	

	CD3DWOWM2ModelResource::RIBBON_EMITTER_INFO * pRibbonEmitterInfo=m_pModelResource->GetRibbonEmitterInfo(m_EmitterIndex);
	if(pRibbonEmitterInfo==NULL)
		return false;

	m_MaxRibbonCount=(UINT)pRibbonEmitterInfo->Resolution+1;

	if(m_MaxRibbonCount>MAX_RIBBON_COUNT)
		m_MaxRibbonCount=MAX_RIBBON_COUNT;	
	m_pRibbonVertexBuffer=new RIBBON_PAIR[m_MaxRibbonCount];
	

	


	m_SubMesh.GetVertexFormat().FVF=D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1;
	m_SubMesh.GetVertexFormat().VertexSize=sizeof(RIBBON_VERTEX);
	m_SubMesh.GetVertexFormat().IndexSize=0;
	m_SubMesh.SetPrimitiveType(D3DPT_TRIANGLESTRIP);
	m_SubMesh.SetVertexs((BYTE *)m_pRibbonVertexBuffer);
	
	for(UINT i=0;i<m_MaxRibbonCount;i++)
	{
		m_pRibbonVertexBuffer[i].Vertex[0].Tex.x=0.0f;
		m_pRibbonVertexBuffer[i].Vertex[0].Tex.y=0.0f;
		m_pRibbonVertexBuffer[i].Vertex[1].Tex.x=0.0f;
		m_pRibbonVertexBuffer[i].Vertex[1].Tex.y=1.0f;		
	}
	for(UINT i=0;i<pRibbonEmitterInfo->Textures.GetCount();i++)
	{
		m_SubMesh.GetMaterial().AddTexture(pRibbonEmitterInfo->Textures[i],0);
		if(pRibbonEmitterInfo->Textures[i])
			pRibbonEmitterInfo->Textures[i]->AddUseRef();
	}
	

	m_SubMesh.GetMaterial().SetFX(pRibbonEmitterInfo->pFX);
	if(pRibbonEmitterInfo->pFX)
		pRibbonEmitterInfo->pFX->AddUseRef();
	
	m_SubMesh.SetTransparent(pRibbonEmitterInfo->IsTransparent);

	m_RibbonCount=0;
	return true;
}

void CD3DWOWM2RibbonEmitter::BuildRibbon(CD3DWOWM2ModelResource::RIBBON_EMITTER_INFO * pRibbonEmitterInfo,CD3DWOWM2ModelResource::RIBBON_PARAM * pParam)
{	
	
	
	if(m_RibbonCount>=m_MaxRibbonCount)//||RibbonLen>=m_ClipLength*pRibbonEmitterInfo->Resolution)
	{
		for(UINT i=0;i<m_RibbonCount-1;i++)
		{
			m_pRibbonVertexBuffer[i].Vertex[0]=m_pRibbonVertexBuffer[i+1].Vertex[0];
			m_pRibbonVertexBuffer[i].Vertex[1]=m_pRibbonVertexBuffer[i+1].Vertex[1];
		}
		
		m_pRibbonVertexBuffer[m_RibbonCount-1].Vertex[0].Pos=CD3DVector3(pParam->Above,0,0)*GetWorldMatrix();
		m_pRibbonVertexBuffer[m_RibbonCount-1].Vertex[0].Color=pParam->Color;
		m_pRibbonVertexBuffer[m_RibbonCount-1].Vertex[1].Pos=CD3DVector3(-pParam->Below,0,0)*GetWorldMatrix();
		m_pRibbonVertexBuffer[m_RibbonCount-1].Vertex[1].Color=pParam->Color;	
	}
	else
	{
		m_pRibbonVertexBuffer[m_RibbonCount].Vertex[0].Pos=CD3DVector3(pParam->Above,0,0)*GetWorldMatrix();
		m_pRibbonVertexBuffer[m_RibbonCount].Vertex[0].Color=pParam->Color;
		m_pRibbonVertexBuffer[m_RibbonCount].Vertex[1].Pos=CD3DVector3(-pParam->Below,0,0)*GetWorldMatrix();
		m_pRibbonVertexBuffer[m_RibbonCount].Vertex[1].Color=pParam->Color;	
		m_RibbonCount++;

		
	}

	m_pRibbonVertexBuffer[0].Vertex[0].Tex.x=0.0f;
	m_pRibbonVertexBuffer[0].Vertex[1].Tex.x=0.0f;

	for(UINT i=1;i<m_RibbonCount;i++)
	{
		m_pRibbonVertexBuffer[i].Vertex[0].Tex.x=i*1.0f/(m_RibbonCount-1);
		m_pRibbonVertexBuffer[i].Vertex[1].Tex.x=i*1.0f/(m_RibbonCount-1);
	}
	
}

}