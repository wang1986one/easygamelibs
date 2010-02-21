#pragma once


namespace D3DLib{

class CD3DTextureManager;
class CD3DFXManager;
class CD3DFontManager;
class CD3DTexture;
class CD3DObjectResourceManager;
class CD3DObjectManager;

struct D3DDEVICE_PARAMS
{
	UINT					Adapter;
	D3DDEVTYPE				DeviceType;
	HWND					hFocusWindow;
	DWORD					BehaviorFlags;
	D3DPRESENT_PARAMETERS	PresentParams;
	D3DDEVICE_PARAMS()
	{
		Adapter=D3DADAPTER_DEFAULT;
		DeviceType=D3DDEVTYPE_HAL;
		hFocusWindow=NULL;
		BehaviorFlags=D3DCREATE_MIXED_VERTEXPROCESSING;
		ZeroMemory(&PresentParams,sizeof(PresentParams));
		PresentParams.Windowed = TRUE;	
		PresentParams.SwapEffect =D3DSWAPEFFECT_DISCARD;
		PresentParams.BackBufferFormat = D3DFMT_UNKNOWN;
		PresentParams.EnableAutoDepthStencil = TRUE;
		PresentParams.AutoDepthStencilFormat = D3DFMT_D16;
		PresentParams.PresentationInterval=D3DPRESENT_INTERVAL_IMMEDIATE;
	}

};

class CD3DDevice :
	public CNameObject
{
protected:
	LPDIRECT3D9				m_pD3D;
	LPDIRECT3DDEVICE9		m_pd3dDevice;
	LPDIRECT3DSURFACE9		m_pSavedRenderTarget;
	LPDIRECT3DSURFACE9		m_pCurRenderTarget;	
	D3DDEVICE_PARAMS		m_D3DParams;
	D3DCAPS9				m_D3DCaps;

	CD3DTextureManager *		m_pTextureManager;	
	CD3DFXManager *				m_pFXManager;
	CD3DFontManager	*			m_pFontManager;
	CD3DObjectResourceManager * m_pObjectResourceManager;	

	DECLARE_CLASS_INFO(CD3DDevice)
public:
	CD3DDevice();
	virtual ~CD3DDevice(void);

	bool Create(D3DDEVICE_PARAMS& Params);
	bool Reset();

	LPDIRECT3D9 GetD3D()
	{
		return m_pD3D;
	}

	LPDIRECT3DDEVICE9 GetD3DDevice()
	{return m_pd3dDevice;}

	HWND GetBindHwnd()
	{return m_D3DParams.hFocusWindow;}

	D3DCAPS9& GetDeviceCaps()
	{
		return m_D3DCaps;
	}

	virtual bool RestoreObject();	
	virtual bool ReleaseObject();
	
	
	inline CD3DTextureManager * GetTextureManager()
	{
		return m_pTextureManager;
	}

	inline CD3DFXManager * GetFXManager()
	{
		return m_pFXManager;
	}

	inline CD3DFontManager * GetFontManager()
	{
		return m_pFontManager;
	}
	inline CD3DObjectResourceManager * GetObjectResourceManager()
	{
		return m_pObjectResourceManager;
	}	

	bool Clear(D3DCOLOR ClearColor,DWORD Flag);

	bool StartRender();
	bool StartRender(D3DCOLOR ClearColor);
	void EndRender(bool WantPresent=true);

	bool StartRenderToTexture(CD3DTexture * pTexture);
	bool EndRenderToTexture();

	bool SetViewPort(RECT& Rect);

	bool IsCreated()
	{
		return m_pd3dDevice!=NULL;
	}

	bool CheckDeviceFormat(DWORD CheckFormat,DWORD Usage=0,DWORD RType=D3DRTYPE_TEXTURE);

};

}