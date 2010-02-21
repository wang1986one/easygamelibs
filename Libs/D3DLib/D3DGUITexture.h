#pragma once

namespace D3DGUI{
class CD3DGUITexture :
	public CD3DTexture,public IUITexture
{
protected:
	DECLARE_CLASS_INFO(CD3DGUITexture)
public:
	CD3DGUITexture();
	CD3DGUITexture(CD3DTextureManager * pManager);
	virtual ~CD3DGUITexture(void);

	virtual CEasyString GetName()
	{
		return CD3DTexture::GetName();
	}
	virtual int GetWidth()
	{
		return CD3DTexture::GetWidth();
	}
	virtual int GetHeight()
	{
		return CD3DTexture::GetHeight();
	}
	virtual FLOAT X2U(int X)
	{
		return ((FLOAT)X)/(m_TextureSurfaceInfo.Width);
	}
	virtual FLOAT Y2V(int Y)
	{
		return ((FLOAT)Y)/(m_TextureSurfaceInfo.Height);
	}
	virtual int U2X(FLOAT U)
	{
		return (int)(U*m_TextureSurfaceInfo.Width);
	}
	virtual int V2X(FLOAT V)
	{
		return (int)(V*m_TextureSurfaceInfo.Height);
	}

	
	virtual void PickResource(CNameObjectSet * pObjectSet,UINT Param=0)
	{
		pObjectSet->Add((CD3DTexture *)this);
	}

	void AddUseRef()
	{
		CD3DTexture::AddUseRef();
	}

	virtual void Release()
	{
		CD3DTexture::Release();
	}

};

}