/****************************************************************************/
/*                                                                          */
/*      文件名:    D3DLight.h                                               */
/*      创建日期:  2009年09月11日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once

namespace D3DLib{

class CD3DLight :
	public CD3DObject
{
protected:
	D3DLIGHT9	m_LightData;

	DECLARE_CLASS_INFO(CD3DLight)
public:
	CD3DLight(void);
	CD3DLight(const CD3DLight& Light);
	CD3DLight(const D3DLIGHT9& Light);

	virtual ~CD3DLight(void);

	bool CanRender();

	void SetLight(const D3DLIGHT9& Light);
	D3DLIGHT9& GetLight();	
	void GetCurLight(D3DLIGHT9& Light);

	void SetDirect(FLOAT x,FLOAT y,FLOAT z);
	void SetPosition(FLOAT x,FLOAT y,FLOAT z);
	void SetAmbient(FLOAT r,FLOAT g,FLOAT b,FLOAT a);
	void SetDiffuse(FLOAT r,FLOAT g,FLOAT b,FLOAT a);
	void SetSpecular(FLOAT r,FLOAT g,FLOAT b,FLOAT a);	

	void Apply(CD3DDevice * pDevice,int Index);
	virtual void Update(FLOAT Time);
};

inline bool CD3DLight::CanRender()
{
	return false;
}




inline void CD3DLight::SetAmbient(FLOAT r,FLOAT g,FLOAT b,FLOAT a)
{
	m_LightData.Ambient.r=r;
	m_LightData.Ambient.g=g;
	m_LightData.Ambient.b=b;
	m_LightData.Ambient.a=a;
}

inline void CD3DLight::SetDiffuse(FLOAT r,FLOAT g,FLOAT b,FLOAT a)
{
	m_LightData.Diffuse.r=r;
	m_LightData.Diffuse.g=g;
	m_LightData.Diffuse.b=b;
	m_LightData.Diffuse.a=a;
}

inline void CD3DLight::SetSpecular(FLOAT r,FLOAT g,FLOAT b,FLOAT a)
{
	m_LightData.Specular.r=r;
	m_LightData.Specular.g=g;
	m_LightData.Specular.b=b;
	m_LightData.Specular.a=a;
}
}