#include "StdAfx.h"

namespace D3DLib{

IMPLEMENT_CLASS_INFO(CD3DLight,CD3DObject);

CD3DLight::CD3DLight(void):CD3DObject()
{
	const D3DCOLORVALUE WhiteColor={1.0f,1.0f,1.0f,1.0f};	
	const D3DCOLORVALUE GrayColor={0.5f,0.5f,0.5f,1.0f};	
	const D3DCOLORVALUE BlackColor={0.0f,0.0f,0.0f,1.0f};
	m_LightData.Type=D3DLIGHT_DIRECTIONAL;
	m_LightData.Ambient=WhiteColor;
	m_LightData.Diffuse=WhiteColor;
	m_LightData.Specular=WhiteColor;
	m_LightData.Position.x=0.0f;
	m_LightData.Position.y=0.0f;
	m_LightData.Position.z=0.0f;
	m_LightData.Direction.x=1.0f;
	m_LightData.Direction.y=1.0f;
	m_LightData.Direction.z=1.0f;
	m_LightData.Range=sqrt(FLT_MAX);
	m_LightData.Falloff=1.0f;
	m_LightData.Attenuation0=0.0f;
	m_LightData.Attenuation1=1.0f;
	m_LightData.Attenuation2=0.0f;
	m_LightData.Theta=PI/8;
	m_LightData.Phi=PI/6;	
}

CD3DLight::CD3DLight(const CD3DLight& Light)
{
	m_LightData=Light.m_LightData;
	m_LocalMatrix=Light.m_LocalMatrix;
	m_WorldMatrix=Light.m_WorldMatrix;
}

CD3DLight::CD3DLight(const D3DLIGHT9& Light)
{
	m_LightData=Light;
}

CD3DLight::~CD3DLight(void)
{
}

void CD3DLight::GetCurLight(D3DLIGHT9& Light)
{
	Light=m_LightData;
	Light.Position=CD3DVector3(Light.Position)+GetWorldMatrix().GetTranslation();
	Light.Direction=CD3DVector3(Light.Direction)*GetWorldMatrix().GetRotation();
}

void CD3DLight::Apply(CD3DDevice * pDevice,int Index)
{
	if(pDevice)
	{
		D3DLIGHT9 Light;
		GetCurLight(Light);
		pDevice->GetD3DDevice()->SetLight(Index,&Light);
	}
}

}