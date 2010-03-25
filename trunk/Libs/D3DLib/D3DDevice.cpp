/****************************************************************************/
/*                                                                          */
/*      文件名:    D3DDevice.cpp                                            */
/*      创建日期:  2009年09月11日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#include "StdAfx.h"





namespace D3DLib{

IMPLEMENT_CLASS_INFO(CD3DDevice,CNameObject);

CD3DDevice::CD3DDevice():CNameObject()
{
	m_pD3D=NULL;
	m_pd3dDevice=NULL;
	m_pSavedRenderTarget=NULL;
	m_pCurRenderTarget=NULL;
	m_pTextureManager=NULL;
	m_pFXManager=NULL;
	m_pFontManager=NULL;
	m_pObjectResourceManager=NULL;	
	ZeroMemory( &m_D3DParams, sizeof(m_D3DParams));
	ZeroMemory(&m_D3DCaps,sizeof(m_D3DCaps));
}

CD3DDevice::~CD3DDevice(void)
{
	ReleaseObject();
}

bool CD3DDevice::Create(D3DDEVICE_PARAMS& Params)
{
	ReleaseObject();
	
	m_D3DParams=Params;

	if(m_D3DParams.PresentParams.hDeviceWindow==NULL)
		m_D3DParams.PresentParams.hDeviceWindow=m_D3DParams.hFocusWindow;
	

    if( NULL == ( m_pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
        return false;     
	

    if( FAILED( m_pD3D->CreateDevice(m_D3DParams.Adapter,m_D3DParams.DeviceType,m_D3DParams.hFocusWindow,m_D3DParams.BehaviorFlags,&m_D3DParams.PresentParams,&m_pd3dDevice)))
    {
        return false;
    }

	//m_pd3dDevice->SetRenderState(D3DRS_FILLMODE,D3DFILL_WIREFRAME );

	m_pd3dDevice->GetDeviceCaps(&m_D3DCaps);

	m_pTextureManager=new CD3DTextureManager(this,MAX_TEXTURE_STORAGE);	
	m_pFXManager=new CD3DFXManager(this,MAX_FX_STORAGE);
	m_pFontManager=new CD3DFontManager(this,MAX_FONT_STORAGE);
	m_pObjectResourceManager=new CD3DObjectResourceManager(this,MAX_FONT_STORAGE);
	return true;
}

bool CD3DDevice::Reset()
{
	if(m_D3DParams.PresentParams.Windowed)
	{
		 m_D3DParams.PresentParams.BackBufferFormat = D3DFMT_UNKNOWN;
		 m_D3DParams.PresentParams.BackBufferWidth = 0;
		 m_D3DParams.PresentParams.BackBufferHeight = 0;
	}	
	HRESULT hr=m_pd3dDevice->Reset(&m_D3DParams.PresentParams);
	return SUCCEEDED(hr);
}

bool CD3DDevice::RestoreObject()
{
	return true;
}

bool CD3DDevice::ReleaseObject()
{	
	SAFE_RELEASE(m_pObjectResourceManager);
	SAFE_RELEASE(m_pTextureManager);	
	SAFE_RELEASE(m_pFontManager);		
	SAFE_RELEASE(m_pFXManager);	
	SAFE_RELEASE(m_pSavedRenderTarget);
	SAFE_RELEASE(m_pCurRenderTarget);
	SAFE_RELEASE(m_pd3dDevice);
	SAFE_RELEASE(m_pD3D);
	
	return true;
}

bool CD3DDevice::Clear(D3DCOLOR ClearColor,DWORD Flag)
{
	if(m_pd3dDevice)
	{
		if(SUCCEEDED(m_pd3dDevice->Clear( 0, NULL, Flag,
			ClearColor, 1.0f, 0 )))
		{
			return true;
		}
	}
	return false;

}

bool CD3DDevice::StartRender()
{
	if(m_pd3dDevice)
	{		
		if(SUCCEEDED(m_pd3dDevice->BeginScene()))
			return true;
	}
	return false;
}

bool CD3DDevice::StartRender(D3DCOLOR ClearColor)
{
	if(m_pd3dDevice)
	{
		m_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,
                        ClearColor, 1.0f, 0 );
		if(SUCCEEDED(m_pd3dDevice->BeginScene()))
			return true;
	}
	return false;
}

void CD3DDevice::EndRender(bool WantPresent)
{
	if(m_pd3dDevice)
	{
		m_pd3dDevice->EndScene();
		if(WantPresent)
			m_pd3dDevice->Present( NULL, NULL, NULL, NULL );
	}
}


bool CD3DDevice::StartRenderToTexture(CD3DTexture * pTexture)
{
	if(pTexture==NULL)
		return false;

	if(FAILED(m_pd3dDevice->BeginScene()))
		return false;

	if(FAILED(m_pd3dDevice->GetRenderTarget(0, &m_pSavedRenderTarget)))
	{
		m_pSavedRenderTarget=NULL;
		return false;
	}
	LPDIRECT3DTEXTURE9 pTargetTexture = pTexture->GetD3DTexture();
	if(pTargetTexture == NULL)
	{
		SAFE_RELEASE(m_pSavedRenderTarget);
		return false;
	}


	if(FAILED(pTargetTexture->GetSurfaceLevel(0, &m_pCurRenderTarget)))
	{
		m_pd3dDevice->SetRenderTarget(0, m_pSavedRenderTarget);
		
		SAFE_RELEASE(m_pSavedRenderTarget);
		return false;
	}
			
	if(FAILED(m_pd3dDevice->SetRenderTarget(0, m_pCurRenderTarget)))
	{
		m_pd3dDevice->SetRenderTarget(0, m_pSavedRenderTarget);
					
		SAFE_RELEASE(m_pSavedRenderTarget);
		SAFE_RELEASE(m_pCurRenderTarget);
		return false;
	}
		

	
	return true;
}

bool CD3DDevice::EndRenderToTexture()
{
	if(m_pSavedRenderTarget)
	{
		m_pd3dDevice->SetRenderTarget(0, m_pSavedRenderTarget);
		SAFE_RELEASE(m_pSavedRenderTarget);
	}

	if (m_pCurRenderTarget)
	{		
		SAFE_RELEASE(m_pCurRenderTarget);
	}

	m_pd3dDevice->EndScene();

	return true;
}


bool CD3DDevice::SetViewPort(RECT& Rect)
{
	if(m_pd3dDevice)
	{
		D3DVIEWPORT9 vp;
		vp.X      = Rect.left;
		vp.Y      = Rect.top;
		vp.Width  = Rect.right-Rect.left;
		vp.Height = Rect.bottom-Rect.top;
		vp.MinZ   = 0.0f;
		vp.MaxZ   = 1.0f;


		if(SUCCEEDED(m_pd3dDevice->SetViewport(&vp)))
			return true;			
	}	
	return false;
}

bool CD3DDevice::CheckDeviceFormat(DWORD CheckFormat,DWORD Usage,DWORD RType)
{
	return SUCCEEDED(m_pD3D->CheckDeviceFormat(m_D3DParams.Adapter,m_D3DParams.DeviceType,
		m_D3DParams.PresentParams.BackBufferFormat,
		Usage,(D3DRESOURCETYPE)RType,(D3DFORMAT)CheckFormat));
}

}