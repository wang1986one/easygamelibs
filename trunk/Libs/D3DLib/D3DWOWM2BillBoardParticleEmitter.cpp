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
	m_pEmitterInfo=NULL;
	m_MaxParticleCount=0;	
	m_pParticleVertexBuffer=NULL;
	m_pParticleIndexBuffer=NULL;
	m_ParticleCount=0;
	m_StartTime=-1;
	m_RecentUpdateTime=0;
	m_EmitterRate=0;
	m_EnbaleGravityTransform=true;
	m_NoTrail=false;
	m_TextureTileRotation=0;
	
}

CD3DWOWM2BillBoardParticleEmitter::~CD3DWOWM2BillBoardParticleEmitter(void)
{
	Destory();
}


void CD3DWOWM2BillBoardParticleEmitter::Destory()
{
	m_pModelResource=NULL;
	m_EmitterIndex=0;
	m_pEmitterInfo=NULL;
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
CD3DSubMesh * CD3DWOWM2BillBoardParticleEmitter::GetSubMesh(UINT index)
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

void CD3DWOWM2BillBoardParticleEmitter::PrepareRender(CD3DDevice * pDevice,CD3DSubMesh * pSubMesh,CD3DSubMeshMaterial * pMaterial,CEasyArray<CD3DLight *>& LightList,CD3DCamera * pCamera)
{
	if(pSubMesh&&pMaterial)
	{	

		if(pMaterial->GetFX())
		{
			//设置灯光
			if(LightList.GetCount())
			{		
				D3DLIGHT9	Light;
				char		szParamName[32];
				pMaterial->GetFX()->SetInt("LightCount",LightList.GetCount());
				for(UINT i=0;i<LightList.GetCount();i++)
				{
					LightList[i]->GetCurLight(Light);
					sprintf_s(szParamName,32,"LightType[%d]",i);
					pMaterial->GetFX()->SetInt(szParamName,Light.Type);
					sprintf_s(szParamName,32,"LightPos[%d]",i);
					pMaterial->GetFX()->SetVector(szParamName,CD3DVector3(Light.Position));
					sprintf_s(szParamName,32,"LightDir[%d]",i);
					pMaterial->GetFX()->SetVector(szParamName,CD3DVector3(Light.Direction));
					sprintf_s(szParamName,32,"LightAmbient[%d]",i);
					pMaterial->GetFX()->SetColor(szParamName,Light.Ambient);
					sprintf_s(szParamName,32,"LightDiffuse[%d]",i);
					pMaterial->GetFX()->SetColor(szParamName,Light.Diffuse);
					sprintf_s(szParamName,32,"LightSpecular[%d]",i);
					pMaterial->GetFX()->SetColor(szParamName,Light.Specular);
					sprintf_s(szParamName,32,"LightRange[%d]",i);
					pMaterial->GetFX()->SetFloat(szParamName,Light.Range);
					sprintf_s(szParamName,32,"LightAtn0[%d]",i);
					pMaterial->GetFX()->SetFloat(szParamName,Light.Attenuation0);
					sprintf_s(szParamName,32,"LightAtn1[%d]",i);
					pMaterial->GetFX()->SetFloat(szParamName,Light.Attenuation1);
					sprintf_s(szParamName,32,"LightAtn2[%d]",i);
					pMaterial->GetFX()->SetFloat(szParamName,Light.Attenuation2);
					//sprintf_s(szParamName,32,"LightFalloff[%d]",i);
					//pMaterial->GetFX()->SetFloat(szParamName,Light.Falloff);
					//sprintf_s(szParamName,32,"LightTheta[%d]",i);
					//pMaterial->GetFX()->SetFloat(szParamName,Light.Theta);
					//sprintf_s(szParamName,32,"LightPhi[%d]",i);
					//pMaterial->GetFX()->SetFloat(szParamName,Light.Phi);

				}

			}
			//设置雾
			pMaterial->GetFX()->SetColor("FogColor",GetRender()->GetFogColor());
			pMaterial->GetFX()->SetFloat("FogNear",GetRender()->GetFogNear());
			pMaterial->GetFX()->SetFloat("FogFar",GetRender()->GetFogFar());

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
			pMaterial->GetFX()->SetInt("TextureTileRotation",m_TextureTileRotation);
			
			
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

	if(IsVisible())
	{		
		if(m_pEmitterInfo)
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
						BuildParticle(m_pEmitterInfo,&Param);
						CreateCount++;
					}

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

	m_pEmitterInfo=m_pModelResource->GetParticleEmitterInfo(m_EmitterIndex);
	if(m_pEmitterInfo==NULL)
		return false;

	if(m_MaxParticleCount==0)
	{
		FLOAT MaxEmissionRate=0.0f;
		for(UINT i=0;i<m_pEmitterInfo->EmissionRate.Animations[0].Keys.GetCount();i++)
		{
			if(m_pEmitterInfo->EmissionRate.Animations[0].Keys[i]>MaxEmissionRate)
			{
				MaxEmissionRate=m_pEmitterInfo->EmissionRate.Animations[0].Keys[i];
			}
		}
		FLOAT MaxLifeSpan=0.0f;
		for(UINT i=0;i<m_pEmitterInfo->LifeSpan.Animations[0].Keys.GetCount();i++)
		{
			if(m_pEmitterInfo->LifeSpan.Animations[0].Keys[i]>MaxLifeSpan)
			{
				MaxLifeSpan=m_pEmitterInfo->LifeSpan.Animations[0].Keys[i];
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
	m_SubMesh.SetVertices((BYTE *)m_pParticleVertexBuffer);
	m_SubMesh.SetIndices((BYTE *)m_pParticleIndexBuffer);
	m_SubMesh.SetRenderBufferUsed(CD3DSubMesh::BUFFER_USE_CUSTOM);
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

	D3DCOLORVALUE WhiteColor={1.0f,1.0f,1.0f,1.0f};
	D3DCOLORVALUE GrayColor={0.8f,0.8f,0.8f,1.0f};
	D3DCOLORVALUE BlackColor={0.0f,0.0f,0.0f,1.0f};

	m_SubMesh.GetMaterial().GetMaterial().Ambient=WhiteColor;
	m_SubMesh.GetMaterial().GetMaterial().Diffuse=WhiteColor;
	m_SubMesh.GetMaterial().GetMaterial().Specular=WhiteColor;
	m_SubMesh.GetMaterial().GetMaterial().Emissive=BlackColor;
	m_SubMesh.GetMaterial().GetMaterial().Power=40.0f;


	m_SubMesh.GetMaterial().AddTexture(m_pEmitterInfo->pTexture,0);
	if(m_pEmitterInfo->pTexture)
		m_pEmitterInfo->pTexture->AddUseRef();
	m_SubMesh.GetMaterial().SetFX(m_pEmitterInfo->pFX);
	if(m_pEmitterInfo->pFX)
		m_pEmitterInfo->pFX->AddUseRef();

	if(m_pEmitterInfo->BlendingType!=EBT_OPACITY&&m_pEmitterInfo->BlendingType!=EBT_ALPHA_TEST)
	{
		m_SubMesh.SetTransparent(true);
	}

	if(m_pEmitterInfo->Flags&PEF_GRAVITY_TRANSFORM)
	{
		m_EnbaleGravityTransform=true;
	}
	else
	{
		m_EnbaleGravityTransform=false;
	}

	m_NoTrail=((m_pEmitterInfo->Flags&PEF_NO_TRAIL)!=0);
	m_TextureTileRotation=m_pEmitterInfo->TextureTileRotation;

	m_ParticleCount=0;
	return true;
}

void CD3DWOWM2BillBoardParticleEmitter::BuildParticle(CD3DWOWM2ModelResource::PARTICLE_EMITTER_INFO * pParticleEmitterInfo,CD3DWOWM2ModelResource::PARTICLE_PARAM * pParam)
{
	CD3DVector3 Dir(0.0f,1.0f,0.0f);
	FLOAT RotationX=0;
	FLOAT RotationZ=0;

	

	CD3DVector3 StartPos;

	if(pParticleEmitterInfo->EmitterType==EMITTER_PLANE)
	{
		if((pParticleEmitterInfo->Flags&PED_DIR_NO_HORIZONTAL_TRANS)==0)
		{
			RotationX=GetRandf(0.0f,pParam->HorizontalRange)-pParam->HorizontalRange/2;
		}

		//if(pParticleEmitterInfo->Flags&PEF_DIR_VERTICAL_TRANS)
		{
			RotationZ=GetRandf(0.0f,pParam->VerticalRange)-pParam->VerticalRange/2;
		}

		if(RotationX!=0.0f||RotationZ!=0.0f)
		{
			Dir=Dir*CD3DMatrix::FromRotationX(RotationX)*CD3DMatrix::FromRotationZ(RotationZ);	
			Dir.Normalize();
		}

		StartPos.x=GetRandf(-pParam->EmissionAreaLength/2,pParam->EmissionAreaLength/2);
		StartPos.y=0;
		StartPos.z=GetRandf(-pParam->EmissionAreaWidth/2,pParam->EmissionAreaWidth/2);
	}
	else
	{
		RotationX=GetRandf(0.0f,pParam->HorizontalRange)-pParam->HorizontalRange/2;
		RotationZ=GetRandf(0.0f,pParam->VerticalRange)-pParam->VerticalRange/2;
		Dir=Dir*CD3DMatrix::FromRotationX(RotationX)*CD3DMatrix::FromRotationZ(RotationZ);	
		Dir.Normalize();

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
	if(pParticleEmitterInfo->Flags&PEF_HAVE_SPEED_VARIATION)
	{
		m_pParticleVertexBuffer[m_ParticleCount].Vertex[0].Info.SpeedVariation=pParam->SpeedVariation;
	}
	else
	{
		m_pParticleVertexBuffer[m_ParticleCount].Vertex[0].Info.SpeedVariation=0;
	}	
	m_pParticleVertexBuffer[m_ParticleCount].Vertex[0].Info.Gravity=pParam->Gravity;
	m_pParticleVertexBuffer[m_ParticleCount].Vertex[0].Info.Decelerate=pParam->Decelerate;

	m_pParticleVertexBuffer[m_ParticleCount].Vertex[0].Info.Slowdown=pParticleEmitterInfo->Slowdown;
	m_pParticleVertexBuffer[m_ParticleCount].Vertex[0].Info.ParticleRotation=
		GetRandf(-pParticleEmitterInfo->Rotation*D3DX_PI,pParticleEmitterInfo->Rotation*D3DX_PI);

	//m_pParticleVertexBuffer[m_ParticleCount].Vertex[0].Info.ColorLifeMid=pParticleEmitterInfo->ParticleColor.TimeStamps[1];
	//m_pParticleVertexBuffer[m_ParticleCount].Vertex[0].Info.OpacityLifeMid=pParticleEmitterInfo->ParticleOpacity.TimeStamps[1];

//	m_pParticleVertexBuffer[m_ParticleCount].Vertex[0].Info.SizeLifeMid=pParticleEmitterInfo->ParticleSizes.TimeStamps[1];

	BuildColor(pParticleEmitterInfo->ParticleColor,pParticleEmitterInfo->ParticleOpacity,
		m_pParticleVertexBuffer[m_ParticleCount].Vertex[0].Info.ColorStart,
		m_pParticleVertexBuffer[m_ParticleCount].Vertex[0].Info.ColorMid,
		m_pParticleVertexBuffer[m_ParticleCount].Vertex[0].Info.ColorEnd,
		m_pParticleVertexBuffer[m_ParticleCount].Vertex[0].Info.ColorLifeMid,
		m_pParticleVertexBuffer[m_ParticleCount].Vertex[0].Info.OpacityLifeMid);
	//m_pParticleVertexBuffer[m_ParticleCount].Vertex[0].Info.ColorStart=
	//	D3DCOLOR_RGBA(
	//	(DWORD)pParticleEmitterInfo->ParticleColor.Keys[0].x,
	//	(DWORD)pParticleEmitterInfo->ParticleColor.Keys[0].y,
	//	(DWORD)pParticleEmitterInfo->ParticleColor.Keys[0].z,
	//	(DWORD)(pParticleEmitterInfo->ParticleOpacity.Keys[0]*255)
	//	);
	//m_pParticleVertexBuffer[m_ParticleCount].Vertex[0].Info.ColorMid=
	//	D3DCOLOR_RGBA(
	//	(DWORD)pParticleEmitterInfo->ParticleColor.Keys[1].x,
	//	(DWORD)pParticleEmitterInfo->ParticleColor.Keys[1].y,
	//	(DWORD)pParticleEmitterInfo->ParticleColor.Keys[1].z,
	//	(DWORD)(pParticleEmitterInfo->ParticleOpacity.Keys[1]*255)
	//	);
	//m_pParticleVertexBuffer[m_ParticleCount].Vertex[0].Info.ColorEnd=
	//	D3DCOLOR_RGBA(
	//	(DWORD)pParticleEmitterInfo->ParticleColor.Keys[2].x,
	//	(DWORD)pParticleEmitterInfo->ParticleColor.Keys[2].y,
	//	(DWORD)pParticleEmitterInfo->ParticleColor.Keys[2].z,
	//	(DWORD)(pParticleEmitterInfo->ParticleOpacity.Keys[2]*255)
	//	);

	BuildSize(pParticleEmitterInfo->ParticleSizes,
		m_pParticleVertexBuffer[m_ParticleCount].Vertex[0].Info.SizeStart,
		m_pParticleVertexBuffer[m_ParticleCount].Vertex[0].Info.SizeMid,
		m_pParticleVertexBuffer[m_ParticleCount].Vertex[0].Info.SizeEnd,
		m_pParticleVertexBuffer[m_ParticleCount].Vertex[0].Info.SizeLifeMid);

	//m_pParticleVertexBuffer[m_ParticleCount].Vertex[0].Info.SizeStart=pParticleEmitterInfo->ParticleSizes.Keys[0];
	//m_pParticleVertexBuffer[m_ParticleCount].Vertex[0].Info.SizeMid=pParticleEmitterInfo->ParticleSizes.Keys[1];
	//m_pParticleVertexBuffer[m_ParticleCount].Vertex[0].Info.SizeEnd=pParticleEmitterInfo->ParticleSizes.Keys[2];

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

void CD3DWOWM2BillBoardParticleEmitter::BuildColor(CD3DWOWM2ModelResource::FAKE_ANIMATION_FRAME<CD3DVector3>& ColorAni,CD3DWOWM2ModelResource::FAKE_ANIMATION_FRAME<FLOAT>& OpacityAni,D3DCOLOR& StartColor,D3DCOLOR& MidColor,D3DCOLOR& EndColor,FLOAT& ColorMidTime,FLOAT& OpacityMidTime)
{
	UINT RedStart=0,RedMid=0,RedEnd=0;
	UINT GreenStart=0,GreenMid=0,GreenEnd=0;
	UINT BlueStart=0,BlueMid=0,BlueEnd=0;
	UINT AlphaStart=255,AlphaMid=255,AlphaEnd=255;
	ColorMidTime=0;
	OpacityMidTime=0;
	if(ColorAni.Keys.GetCount()==1)
	{
		RedStart=RedMid=RedEnd=(UINT)ColorAni.Keys[0].x;
		GreenStart=GreenMid=GreenEnd=(UINT)ColorAni.Keys[0].y;
		BlueStart=BlueMid=BlueEnd=(UINT)ColorAni.Keys[0].z;
	}
	else if(ColorAni.Keys.GetCount()==2)
	{
		RedStart=(UINT)ColorAni.Keys[0].x;
		RedEnd=(UINT)ColorAni.Keys[1].x;
		RedMid=(RedStart+RedEnd)/2;

		GreenStart=(UINT)ColorAni.Keys[0].y;
		GreenEnd=(UINT)ColorAni.Keys[1].y;
		GreenMid=(GreenStart+GreenEnd)/2;

		BlueStart=(UINT)ColorAni.Keys[0].z;
		BlueEnd=(UINT)ColorAni.Keys[1].z;
		BlueMid=(BlueStart+BlueEnd)/2;

		ColorMidTime=(ColorAni.TimeStamps[0]+ColorAni.TimeStamps[1])/2;
	}
	else if(ColorAni.Keys.GetCount()>=3)
	{
		RedStart=(UINT)ColorAni.Keys[0].x;
		RedMid=(UINT)ColorAni.Keys[1].x;
		RedEnd=(UINT)ColorAni.Keys[2].x;

		GreenStart=(UINT)ColorAni.Keys[0].y;
		GreenMid=(UINT)ColorAni.Keys[1].y;
		GreenEnd=(UINT)ColorAni.Keys[2].y;

		BlueStart=(UINT)ColorAni.Keys[0].z;
		BlueMid=(UINT)ColorAni.Keys[1].z;
		BlueEnd=(UINT)ColorAni.Keys[2].z;	

		ColorMidTime=ColorAni.TimeStamps[1];
	}

	if(OpacityAni.Keys.GetCount()==1)
	{
		AlphaStart=AlphaMid=AlphaEnd=(UINT)(OpacityAni.Keys[0]*255);
	}
	else if(OpacityAni.Keys.GetCount()==2)
	{
		AlphaStart=(UINT)(OpacityAni.Keys[0]*255);		
		AlphaEnd=(UINT)(OpacityAni.Keys[1]*255);
		AlphaMid=(AlphaStart+AlphaEnd)/2;

		OpacityMidTime=(OpacityAni.TimeStamps[0]+OpacityAni.TimeStamps[1])/2;
	}
	else if(OpacityAni.Keys.GetCount()>=3)
	{
		AlphaStart=(UINT)(OpacityAni.Keys[0]*255);
		AlphaMid=(UINT)(OpacityAni.Keys[1]*255);
		AlphaEnd=(UINT)(OpacityAni.Keys[2]*255);

		OpacityMidTime=OpacityAni.TimeStamps[1];
	}

	StartColor=D3DCOLOR_RGBA(RedStart,GreenStart,BlueStart,AlphaStart);
	MidColor=D3DCOLOR_RGBA(RedMid,GreenMid,BlueMid,AlphaMid);
	EndColor=D3DCOLOR_RGBA(RedEnd,GreenEnd,BlueEnd,AlphaEnd);
}

void CD3DWOWM2BillBoardParticleEmitter::BuildSize(CD3DWOWM2ModelResource::FAKE_ANIMATION_FRAME<CD3DVector2>& SizeAni,CD3DVector2& StartSize,CD3DVector2& MidSize,CD3DVector2& EndSize,FLOAT& MidTime)
{
	if(SizeAni.Keys.GetCount()==1)
	{
		MidTime=0;
		StartSize=SizeAni.Keys[0];
		MidSize=SizeAni.Keys[0];
		EndSize=SizeAni.Keys[0];
	}
	else if(SizeAni.Keys.GetCount()==2)
	{
		MidTime=(SizeAni.TimeStamps[0]+SizeAni.TimeStamps[1])/2;
		StartSize=SizeAni.Keys[0];		
		EndSize=SizeAni.Keys[1];
		MidSize=(StartSize+EndSize)/2;
	}
	else if(SizeAni.Keys.GetCount()>=3)
	{
		MidTime=SizeAni.TimeStamps[1];
		StartSize=SizeAni.Keys[0];
		MidSize=SizeAni.Keys[1];
		EndSize=SizeAni.Keys[2];
	}
	else
	{
		MidTime=0;
		StartSize=0;
		MidSize=0;
		EndSize=0;
	}
}

}



