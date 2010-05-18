/****************************************************************************/
/*                                                                          */
/*      文件名:    D3DWOWM2BillBoardParticleEmitter.cpp                     */
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


IMPLEMENT_CLASS_INFO(CD3DWOWM2BillBoardParticleEmitter,CD3DObject);

CD3DWOWM2BillBoardParticleEmitter::CD3DWOWM2BillBoardParticleEmitter(void)
{
	m_pModelResource=NULL;
	m_EmitterIndex=0;
	m_MaxParticleCount=0;	
	m_pParticleVertexBuffer=NULL;
	m_pParticleIndexBuffer=NULL;
	m_ParticleCount=0;
	m_StartTime=-1;
	m_RecentUpdateTime=0;
	m_EmitterRate=0;
	m_EnbaleGravityTransform=true;
	m_NoTrail=false;
}

CD3DWOWM2BillBoardParticleEmitter::~CD3DWOWM2BillBoardParticleEmitter(void)
{
	Destory();
}


void CD3DWOWM2BillBoardParticleEmitter::Destory()
{
	m_pModelResource=NULL;
	m_EmitterIndex=0;
	m_MaxParticleCount=0;
	m_MaxParticleCount=0;	
	m_ParticleCount=0;
	m_StartTime=-1;
	m_RecentUpdateTime=0;
	m_EmitterRate=0;
	m_EnbaleGravityTransform=true;
	m_NoTrail=false;
	SAFE_DELETE_ARRAY(m_pParticleVertexBuffer);
	SAFE_DELETE_ARRAY(m_pParticleIndexBuffer);
	m_SubMesh.Destory();
	CD3DObject::Destory();
}

bool CD3DWOWM2BillBoardParticleEmitter::Reset()
{
	return m_SubMesh.Reset();
}
bool CD3DWOWM2BillBoardParticleEmitter::Restore()
{
	return m_SubMesh.Restore();
}

int CD3DWOWM2BillBoardParticleEmitter::GetSubMeshCount()
{
	return 1;
}
CD3DSubMesh * CD3DWOWM2BillBoardParticleEmitter::GetSubMesh(int index)
{
	return &m_SubMesh;
}

CD3DBoundingBox * CD3DWOWM2BillBoardParticleEmitter::GetBoundingBox()
{	
	if(m_pModelResource)
	{			
		return m_pModelResource->GetBoundingBox();
	}
	return NULL;
}
CD3DBoundingSphere * CD3DWOWM2BillBoardParticleEmitter::GetBoundingSphere()
{
	if(m_pModelResource)
		return m_pModelResource->GetBoundingSphere();
	return NULL;
}

bool CD3DWOWM2BillBoardParticleEmitter::CanDoSubMeshViewCull()
{
	return false;
}

bool CD3DWOWM2BillBoardParticleEmitter::CloneFrom(CNameObject * pObject,UINT Param)
{
	if(!pObject->IsKindOf(GET_CLASS_INFO(CD3DWOWM2BillBoardParticleEmitter)))
		return false;

	if(!CD3DObject::CloneFrom(pObject,Param))
		return false;

	CD3DWOWM2BillBoardParticleEmitter * pSrcObject=(CD3DWOWM2BillBoardParticleEmitter *)pObject;

	SetDevice(pSrcObject->GetDevice());

	if(!Init(pSrcObject->m_pModelResource,pSrcObject->m_EmitterIndex,pSrcObject->m_MaxParticleCount))
		return false;

	

		
	m_ParticleCount=pSrcObject->m_ParticleCount;
	m_StartTime=pSrcObject->m_StartTime;
	m_RecentUpdateTime=pSrcObject->m_RecentUpdateTime;
	m_EmitterRate=pSrcObject->m_EmitterRate;	
	
	memcpy(m_pParticleVertexBuffer,pSrcObject->m_pParticleVertexBuffer,sizeof(PARTICLE_RECT)*m_ParticleCount);

	m_SubMesh.SetVertexCount(m_ParticleCount*4);
	m_SubMesh.SetIndexCount(m_ParticleCount*6);
	m_SubMesh.SetPrimitiveCount(m_ParticleCount*2);

	return true;
}

void CD3DWOWM2BillBoardParticleEmitter::PrepareRender(CD3DDevice * pDevice,CD3DSubMesh * pSubMesh,CD3DSubMeshMaterial * pMaterial,CD3DLight ** pLight,CD3DCamera * pCamera)
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

			if(m_NoTrail)
			{
				pMaterial->GetFX()->SetMatrix("WorldMatrix",GetWorldMatrix());
				pMaterial->GetFX()->SetMatrix("WorldRotation",GetWorldMatrix().GetRotation());
				pMaterial->GetFX()->SetVector("WorldScale",GetWorldMatrix().GetScale());
				pMaterial->GetFX()->SetVector("WorldTranslation",GetWorldMatrix().GetTranslation());			
			}
			else
			{
				CD3DMatrix Mat;				
				CD3DVector3 Vec;

				Mat.SetIdentity();
				pMaterial->GetFX()->SetMatrix("WorldMatrix",Mat);			
				pMaterial->GetFX()->SetMatrix("WorldRotation",Mat);
				Vec=1.0f;
				pMaterial->GetFX()->SetVector("WorldScale",Vec);
				Vec=0.0f;
				pMaterial->GetFX()->SetVector("WorldTranslation",Vec);			
			}
			
			//设置视投影矩阵
			pMaterial->GetFX()->SetMatrix("ViewMatrix",pCamera->GetViewMat());
			pMaterial->GetFX()->SetMatrix("ProjMatrix",pCamera->GetProjectMat());

			pMaterial->GetFX()->SetBool("TransformGravity",m_EnbaleGravityTransform);
			
			
		}	
	}
	else
	{

	}
}


void CD3DWOWM2BillBoardParticleEmitter::Update(FLOAT Time)
{
	FUNCTION_BEGIN;
	CD3DObject::Update(Time);	

	//Time=Time/100;

	if(m_StartTime<0)
	{
		m_StartTime=Time;
		m_RecentUpdateTime=Time;
	}
	
	UINT CurTime=(UINT)((Time-m_StartTime)*1000);
	FLOAT TimeSpan=Time-m_RecentUpdateTime;
	m_RecentUpdateTime=Time;

	

	int CreateCount=0;
	int DeleteCount=0;

	CD3DWOWM2ModelResource::PARTICLE_EMITTER_INFO * pParticleEmitterInfo=m_pModelResource->GetParticleEmitterInfo(m_EmitterIndex);
	if(pParticleEmitterInfo)
	{
		CD3DWOWM2ModelResource::PARTICLE_PARAM Param;
		if(m_pModelResource->MakeParticleParam(m_EmitterIndex,CurTime,Param))
		{
			m_EmitterRate+=Param.EmissionRate*TimeSpan;
			while(m_EmitterRate>=1.0f)
			{
				m_EmitterRate-=1.0f;
				if(m_ParticleCount<m_MaxParticleCount)
				{
					BuildParticle(pParticleEmitterInfo,&Param);
					CreateCount++;
				}
				
			}
		}
	}
	for(int i=(int)m_ParticleCount-1;i>=0;i--)
	{
		for(int j=0;j<4;j++)
		{
			m_pParticleVertexBuffer[i].Vertex[j].Info.Life+=TimeSpan;
		}
		if(m_pParticleVertexBuffer[i].Vertex[0].Info.Life>=m_pParticleVertexBuffer[i].Vertex[0].Info.LifeSpan)
		{
			if(DeleteCount>1)
			{
				int err=1;
			}
			memmove(m_pParticleVertexBuffer+i,m_pParticleVertexBuffer+i+1,sizeof(PARTICLE_RECT)*(m_ParticleCount-i-1));
			DeleteCount++;
			m_ParticleCount--;			
		}
	}

	if(CreateCount||DeleteCount)
	{
		m_SubMesh.SetVertexCount(m_ParticleCount*4);
		m_SubMesh.SetIndexCount(m_ParticleCount*6);
		m_SubMesh.SetPrimitiveCount(m_ParticleCount*2);
	}

	//PrintSystemLog(0,"Particle=%u,%u",CreateCount,DeleteCount);
	FUNCTION_END;
}


bool CD3DWOWM2BillBoardParticleEmitter::Init(CD3DWOWM2ModelResource * pModelResource,UINT EmitterIndex,UINT MaxParticleCount)
{
	if(GetDevice()==NULL)
		return false;
	


	Destory();
	m_pModelResource=pModelResource;
	m_EmitterIndex=EmitterIndex;
	m_MaxParticleCount=MaxParticleCount;

	CD3DWOWM2ModelResource::PARTICLE_EMITTER_INFO * pParticleEmitterInfo=m_pModelResource->GetParticleEmitterInfo(m_EmitterIndex);
	if(pParticleEmitterInfo==NULL)
		return false;

	if(m_MaxParticleCount==0)
	{
		FLOAT MaxEmissionRate=0.0f;
		for(UINT i=0;i<pParticleEmitterInfo->EmissionRate.Animations[0].Keys.GetCount();i++)
		{
			if(pParticleEmitterInfo->EmissionRate.Animations[0].Keys[i]>MaxEmissionRate)
			{
				MaxEmissionRate=pParticleEmitterInfo->EmissionRate.Animations[0].Keys[i];
			}
		}
		FLOAT MaxLifeSpan=0.0f;
		for(UINT i=0;i<pParticleEmitterInfo->LifeSpan.Animations[0].Keys.GetCount();i++)
		{
			if(pParticleEmitterInfo->LifeSpan.Animations[0].Keys[i]>MaxLifeSpan)
			{
				MaxLifeSpan=pParticleEmitterInfo->LifeSpan.Animations[0].Keys[i];
			}
		}
		m_MaxParticleCount=(UINT)(MaxEmissionRate*MaxLifeSpan*2.0f);
	}
	
	if(m_MaxParticleCount>MAX_PARTICLE_COUNT)
		m_MaxParticleCount=MAX_PARTICLE_COUNT;	
	if(m_MaxParticleCount<MIN_PARTICLE_COUNT)
		m_MaxParticleCount=MIN_PARTICLE_COUNT;
	m_pParticleVertexBuffer=new PARTICLE_RECT[m_MaxParticleCount];
	m_pParticleIndexBuffer=new WORD[m_MaxParticleCount*6];

	D3DVERTEXELEMENT9 ParticleVertexElements[] = 
	{
		{0, 0,  D3DDECLTYPE_FLOAT4,   D3DDECLMETHOD_DEFAULT,		D3DDECLUSAGE_POSITION,	0},
		{0, 16, D3DDECLTYPE_FLOAT4,   D3DDECLMETHOD_DEFAULT,		D3DDECLUSAGE_NORMAL,	0},
		{0, 32, D3DDECLTYPE_FLOAT4,   D3DDECLMETHOD_DEFAULT,		D3DDECLUSAGE_COLOR,		0},
		{0, 48, D3DDECLTYPE_FLOAT4,   D3DDECLMETHOD_DEFAULT,		D3DDECLUSAGE_COLOR,		1},		
		{0, 64, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT,		D3DDECLUSAGE_TEXCOORD,	0},
		{0, 68, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT,		D3DDECLUSAGE_TEXCOORD,	1},
		{0, 72, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT,		D3DDECLUSAGE_TEXCOORD,	2},
		{0, 76, D3DDECLTYPE_FLOAT4,   D3DDECLMETHOD_DEFAULT,		D3DDECLUSAGE_TEXCOORD,	3},
		{0, 92,	D3DDECLTYPE_FLOAT4,   D3DDECLMETHOD_DEFAULT,		D3DDECLUSAGE_TEXCOORD,	4},
		{0, 108, D3DDECLTYPE_FLOAT4,   D3DDECLMETHOD_DEFAULT,		D3DDECLUSAGE_TEXCOORD,	5},
		{0, 124, D3DDECLTYPE_FLOAT4,   D3DDECLMETHOD_DEFAULT,		D3DDECLUSAGE_TEXCOORD,	6},
		{0, 140, D3DDECLTYPE_FLOAT4,   D3DDECLMETHOD_DEFAULT,		D3DDECLUSAGE_TEXCOORD,	7},
		D3DDECL_END()
	};


	m_SubMesh.DeclareVertexFormat(GetDevice(),ParticleVertexElements,sizeof(PARTICLE_VERTEX),sizeof(WORD));
	m_SubMesh.SetPrimitiveType(D3DPT_TRIANGLELIST);
	m_SubMesh.SetVertexs((BYTE *)m_pParticleVertexBuffer);
	m_SubMesh.SetIndexs((BYTE *)m_pParticleIndexBuffer);
	for(UINT i=0;i<m_MaxParticleCount;i++)
	{
		m_pParticleVertexBuffer[i].Vertex[0].Tex.x=0.0f;
		m_pParticleVertexBuffer[i].Vertex[0].Tex.y=0.0f;
		m_pParticleVertexBuffer[i].Vertex[1].Tex.x=1.0f;
		m_pParticleVertexBuffer[i].Vertex[1].Tex.y=0.0f;
		m_pParticleVertexBuffer[i].Vertex[2].Tex.x=0.0f;
		m_pParticleVertexBuffer[i].Vertex[2].Tex.y=1.0f;
		m_pParticleVertexBuffer[i].Vertex[3].Tex.x=1.0f;
		m_pParticleVertexBuffer[i].Vertex[3].Tex.y=1.0f;
		m_pParticleIndexBuffer[i*6]=i*4;
		m_pParticleIndexBuffer[i*6+1]=i*4+1;
		m_pParticleIndexBuffer[i*6+2]=i*4+2;
		m_pParticleIndexBuffer[i*6+3]=i*4+2;
		m_pParticleIndexBuffer[i*6+4]=i*4+1;
		m_pParticleIndexBuffer[i*6+5]=i*4+3;
	}
	m_SubMesh.GetMaterial().AddTexture(pParticleEmitterInfo->pTexture,0);
	if(pParticleEmitterInfo->pTexture)
		pParticleEmitterInfo->pTexture->AddUseRef();
	m_SubMesh.GetMaterial().SetFX(pParticleEmitterInfo->pFX);
	if(pParticleEmitterInfo->pFX)
		pParticleEmitterInfo->pFX->AddUseRef();

	if(pParticleEmitterInfo->BlendingType!=EBT_OPACITY&&pParticleEmitterInfo->BlendingType!=EBT_ALPHA_TEST)
	{
		m_SubMesh.SetTransparent(true);
	}

	if(pParticleEmitterInfo->Flags&PEF_NO_GRAVITY_TRANSFORM)
	{
		m_EnbaleGravityTransform=false;
	}
	else
	{
		m_EnbaleGravityTransform=true;
	}

	m_NoTrail=((pParticleEmitterInfo->Flags&PEF_NO_TRAIL)!=0);


	m_ParticleCount=0;
	return true;
}

void CD3DWOWM2BillBoardParticleEmitter::BuildParticle(CD3DWOWM2ModelResource::PARTICLE_EMITTER_INFO * pParticleEmitterInfo,CD3DWOWM2ModelResource::PARTICLE_PARAM * pParam)
{
	CD3DVector3 Dir(0.0f,1.0f,0.0f);
	FLOAT RotationX=0;
	FLOAT RotationZ=0;

	if(pParticleEmitterInfo->Flags&PEDTT_DIR_HORIZONTAL_TRANS)
	{
		RotationX=GetRandf(0.0f,pParam->HorizontalRange)-pParam->HorizontalRange/2;
	}

	if(pParticleEmitterInfo->Flags&PEF_DIR_VERTICAL_TRANS)
	{
		RotationZ=GetRandf(0.0f,pParam->VerticalRange)-pParam->VerticalRange/2;
	}

	if(RotationX!=0.0f||RotationZ!=0.0f)
	{
		Dir=Dir*CD3DMatrix::FromRotationX(RotationX)*CD3DMatrix::FromRotationZ(RotationZ);	
		Dir.Normalize();
	}

	CD3DVector3 StartPos;

	if(pParticleEmitterInfo->EmitterType==EMITTER_PLANE)
	{
		StartPos.x=GetRandf(-pParam->EmissionAreaLength/2,pParam->EmissionAreaLength/2);
		StartPos.y=0;
		StartPos.z=GetRandf(-pParam->EmissionAreaWidth/2,pParam->EmissionAreaWidth/2);
	}
	else
	{
		FLOAT S=GetRandf(0.0f,1.0f);
		FLOAT Radius1=pParam->EmissionAreaWidth*S;
		FLOAT Radius2=pParam->EmissionAreaLength*S;		
		FLOAT Angle=GetRandf(0.0f,D3DX_PI*2);

		StartPos.y=GetRandf(-Radius2,Radius2);
		
		if(Radius2>0)
		{
			Radius1=sqrt((1.0f-(StartPos.y*StartPos.y)/(Radius2*Radius2))*(Radius1*Radius1));
		}		
		StartPos.x=Radius1*cos(Angle);
		StartPos.z=Radius1*sin(Angle);
	}

	m_pParticleVertexBuffer[m_ParticleCount].Vertex[0].Info.Position=StartPos;
	m_pParticleVertexBuffer[m_ParticleCount].Vertex[0].Info.Life=0.0f;

	m_pParticleVertexBuffer[m_ParticleCount].Vertex[0].Info.Direction=Dir;
	m_pParticleVertexBuffer[m_ParticleCount].Vertex[0].Info.LifeSpan=pParam->LifeSpan;

	m_pParticleVertexBuffer[m_ParticleCount].Vertex[0].Info.Speed=pParam->Speed;
	m_pParticleVertexBuffer[m_ParticleCount].Vertex[0].Info.SpeedVariation=pParam->SpeedVariation;
	m_pParticleVertexBuffer[m_ParticleCount].Vertex[0].Info.Gravity=pParam->Gravity;
	m_pParticleVertexBuffer[m_ParticleCount].Vertex[0].Info.Decelerate=pParam->Decelerate;

	m_pParticleVertexBuffer[m_ParticleCount].Vertex[0].Info.Slowdown=pParticleEmitterInfo->Slowdown;
	m_pParticleVertexBuffer[m_ParticleCount].Vertex[0].Info.ParticleRotation=
		GetRandf(-pParticleEmitterInfo->Rotation*D3DX_PI,pParticleEmitterInfo->Rotation*D3DX_PI);

	m_pParticleVertexBuffer[m_ParticleCount].Vertex[0].Info.ColorLifeMid=pParticleEmitterInfo->ParticleColor.TimeStamps[1];
	m_pParticleVertexBuffer[m_ParticleCount].Vertex[0].Info.OpacityLifeMid=pParticleEmitterInfo->ParticleOpacity.TimeStamps[1];

	m_pParticleVertexBuffer[m_ParticleCount].Vertex[0].Info.SizeLifeMid=pParticleEmitterInfo->ParticleSizes.TimeStamps[1];

	m_pParticleVertexBuffer[m_ParticleCount].Vertex[0].Info.ColorStart=
		D3DCOLOR_RGBA(
		(DWORD)pParticleEmitterInfo->ParticleColor.Keys[0].x,
		(DWORD)pParticleEmitterInfo->ParticleColor.Keys[0].y,
		(DWORD)pParticleEmitterInfo->ParticleColor.Keys[0].z,
		(DWORD)(pParticleEmitterInfo->ParticleOpacity.Keys[0]*255)
		);
	m_pParticleVertexBuffer[m_ParticleCount].Vertex[0].Info.ColorMid=
		D3DCOLOR_RGBA(
		(DWORD)pParticleEmitterInfo->ParticleColor.Keys[1].x,
		(DWORD)pParticleEmitterInfo->ParticleColor.Keys[1].y,
		(DWORD)pParticleEmitterInfo->ParticleColor.Keys[1].z,
		(DWORD)(pParticleEmitterInfo->ParticleOpacity.Keys[1]*255)
		);
	m_pParticleVertexBuffer[m_ParticleCount].Vertex[0].Info.ColorEnd=
		D3DCOLOR_RGBA(
		(DWORD)pParticleEmitterInfo->ParticleColor.Keys[2].x,
		(DWORD)pParticleEmitterInfo->ParticleColor.Keys[2].y,
		(DWORD)pParticleEmitterInfo->ParticleColor.Keys[2].z,
		(DWORD)(pParticleEmitterInfo->ParticleOpacity.Keys[2]*255)
		);

	m_pParticleVertexBuffer[m_ParticleCount].Vertex[0].Info.SizeStart=pParticleEmitterInfo->ParticleSizes.Keys[0];
	m_pParticleVertexBuffer[m_ParticleCount].Vertex[0].Info.SizeMid=pParticleEmitterInfo->ParticleSizes.Keys[1];
	m_pParticleVertexBuffer[m_ParticleCount].Vertex[0].Info.SizeEnd=pParticleEmitterInfo->ParticleSizes.Keys[2];

	m_pParticleVertexBuffer[m_ParticleCount].Vertex[0].Info.TexColRow=pParticleEmitterInfo->TextureRows*100.0f+pParticleEmitterInfo->TextureCols;

	if(m_NoTrail)
	{
		m_pParticleVertexBuffer[m_ParticleCount].Vertex[0].Info.Translation=0.0f;
		m_pParticleVertexBuffer[m_ParticleCount].Vertex[0].Info.Rotation.SetIdentity();
		m_pParticleVertexBuffer[m_ParticleCount].Vertex[0].Info.Scaling=1.0f;
	}
	else
	{
		m_pParticleVertexBuffer[m_ParticleCount].Vertex[0].Info.Translation=GetWorldMatrix().GetTranslation();
		m_pParticleVertexBuffer[m_ParticleCount].Vertex[0].Info.Rotation=GetWorldMatrix().GetRotationOnQuaternion();
		m_pParticleVertexBuffer[m_ParticleCount].Vertex[0].Info.Scaling=GetWorldMatrix().GetScale();
	}
	



	m_pParticleVertexBuffer[m_ParticleCount].Vertex[1].Info=m_pParticleVertexBuffer[m_ParticleCount].Vertex[0].Info;
	m_pParticleVertexBuffer[m_ParticleCount].Vertex[2].Info=m_pParticleVertexBuffer[m_ParticleCount].Vertex[0].Info;
	m_pParticleVertexBuffer[m_ParticleCount].Vertex[3].Info=m_pParticleVertexBuffer[m_ParticleCount].Vertex[0].Info;

	m_ParticleCount++;


}

}



